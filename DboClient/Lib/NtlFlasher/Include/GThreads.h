/**********************************************************************

Filename    :   GThreads.h
Content     :   Contains thread-related (safe) functionality
Created     :   May 5, 2003
Authors     :   Michael Antonov, Andrew Reisse

Copyright   :   (c) 2003 Scaleform Corp. All Rights Reserved.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GTHREADS_H
#define INC_GTHREADS_H

#include "GTypes.h"
#include "GAtomic.h"
#include "GRefCount.h"
#include "GTLTypes.h"

#ifdef GFC_OS_WII
#include <setjmp.h>
#endif

// Defines the infinite wait delay timeout
#define GFC_WAIT_INFINITE       0xFFFFFFFF


// ****** Declared classes

// To be defined in the project configuration options
#ifndef GFC_NO_THREADSUPPORT

// Declared with thread support only:
class   GWaitable;
class   GAcquireInterface;
class   GMutex;
class   GWaitCondition;
class   GEvent;
class   GSemaphore;

#endif


// The rest of the classes are only defined when thread support is enabled.
#ifndef GFC_NO_THREADSUPPORT


// ***** Waitable and Acquisition Interface

/*
    GAcquireInterface provides a two-stage acquisition interface for a resource. This interface is used
    primarily to acquire multiple objects that can be shared among threads. For this reason,
    GAcquireInterface is often used together with GWaitable. Whenever a waitable object changes state
    indicating that a shared resource is potentially available, this abstract acquisition interface is used to 
    acquire the resource in a resource-independent manner.
              
    In the first stage of acquisition TryAcquire is called to see if the resource can be obtained. If it succeeds, 
    the resource is considered to be acquired, and TryAcquireCommit is called to confirm the ownership. If the
    resource is, however, not needed (because, for example, acquisition of another object in a multi-object wait
    has failed) TryAcquireCancel can be called. 
*/


class   GAcquireInterface
{
public:
    GEXPORT virtual ~GAcquireInterface() { }
    
    // Call this function to test if an object can be acquired
    // Like TryAcquire() and TryAcquireCancel() pair, except does not actually try to acquire the object
    // This function is called from a different thread then the acquisition waiting thread, it is used to check
    // for potential availability of resource during Wait/Acquire functions
    GEXPORT virtual bool    CanAcquire();

    // These functions implement a two-stage acquiring protocol used by the AcquireMultiple functions.
    // First, TryAcquire is called to capture resource. If it succeeded, 
    // TryAcquireCommit is called to permanently obtain the resource.
    // TryAcquireCancel can be called instead of TryAcquireCommit
    // in order to release the resource, and leave it untouched.
    // These functions are only called from the waiter thread by the AcquireMultiple functions.
    
    // Try to acquire the resource, return 1 if succeeded.
    GEXPORT virtual bool    TryAcquire();
    // Complete resource acquisition. Should not fail in general.
    GEXPORT virtual bool    TryAcquireCommit();
    // Cancel resource acquisition. Should not fail in general.
    GEXPORT virtual bool    TryAcquireCancel();

    // Static function to acquire multiple objects simultaneously
    GEXPORT static  bool    AcquireMultipleObjects(GWaitable** waitList, UInt waitCount, 
                                                   UInt delay = GFC_WAIT_INFINITE);
    // Static function to acquire one of multiple objects in a list
    // Returns the index of the object acquired, -1 for fail (wait expired)
    GEXPORT static  SInt    AcquireOneOfMultipleObjects(GWaitable** waitList, UInt waitCount, 
                                                        UInt delay = GFC_WAIT_INFINITE);
};



// Static acquire interface that doesn't do anything
// All acquire calls on this interface always succeed.

class   GDefaultAcquireInterface : public GAcquireInterface
{
public:
    GEXPORT virtual ~GDefaultAcquireInterface() { }

    // Does nothing

    GEXPORT static GDefaultAcquireInterface* GetDefaultAcquireInterface();
};



/*
    GWaitable interface is used for thread-shared objects that need to be waited on. A waitable object
    has an associated 'Signalled' state such that an object is signaled when its associated resource is available,
    and non-signaled when it is not available. Some objects, such as a Semaphore, can have multiple internal signaled
    states; in this case the object is considered to be signaled if ANY amount of a resource is available, and
    non-signaled if NO resource is available.

    Waiting on a GWaitable object is achieved with help of handlers installed by the waiter threads. Whenever the
    objects state changes from non-signaled to signaled, or from one internal gradation of signaled to another,
    wait handlers are called (handlers should not get called when the object changes its state from signaled to
    non-signaled). When called, wait handlers can be used to wake up a waiter thread, so that it can compete for the
    acquisition of the object's associated resource. The actual acquisition of the resource can happen either through
    object-specific methods for with help of an associated GAcquireInterface.
*/

class   GWaitable : public GRefCountBase<GWaitable>
{
public:

    // Wait handler type
    typedef void (*WaitHandler)(Handle pdata);    

protected:
    class   HandlerStruct 
    {
    public: 
        WaitHandler Handler;
        Handle      pUserData;

        // Default constructor - only used in garray.
        HandlerStruct()
        {
            Handler   = 0;
            pUserData = 0;
        }

        HandlerStruct(WaitHandler h, Handle pd)
        {
            Handler     = h;
            pUserData   = pd;
        }
        HandlerStruct(const HandlerStruct &src)
        {
            Handler     = src.Handler;
            pUserData   = src.pUserData;
        }

        HandlerStruct & operator = (const HandlerStruct &src)
        {
            Handler     = src.Handler;
            pUserData   = src.pUserData;
            return *this;
        }

        bool operator == (const HandlerStruct &src)
        {
            return (Handler == src.Handler) && (pUserData==src.pUserData);
        }
    };

    // Handler array and lock are allocated in a separate ref-counted object
    // so that their lifetime can be extended past the waitable object in
    // case other threads are still executing wait handlers. HandlerArray is
    // only allocated if multiWait was passed in a contsructor; otherwise
    // it is null.
    struct HandlerArray : public GNewOverrideBase
    {
        GAtomicInt<SInt32>          RefCount;
        GTL::garray<HandlerStruct>  Handlers;
        GLock                       HandlersLock;

        HandlerArray() { RefCount.Value = 1; }

        void	AddRef()    { RefCount++; }
        // Decrement ref count. This needs to be thread-safe, since
        // a different thread could have also decremented the ref count.
        GEXPORT void  Release();

        // Calls all of the installed handlers during a lock.
        GEXPORT void  CallWaitHandlers();        
    };

    // A pointer to handler array, only allocated if 'enabled' flag was passed
    // in a constructor. Such setup is done to save memory and improve performance
    // of non-multiwait GMutex. Unfortunately, lazy initialization can not
    // be used for pHandlers, since access of this variable must be done before
    // system mutex Unlock, and thus can occur before AddHandlers in multi-object wait. 
    HandlerArray*      pHandlers;

public:

    // Enable flag must be passed to enable WaitHandlers; if not specified the
    // wait handlers and multi-object wait functionality is not available.
    GWaitable(bool enable);
    // Expose RefCount implementation, so that we can override it if necessary - such as in GThread.
    GWaitable(bool enable, GRefCountImpl *pnormalImpl, GRefCountImpl *pnullImpl=0);
    
    virtual ~GWaitable();

    // Invoke the associated wait handlers; only safe if we know GWaitable could not
    // have died in response to it being signaled (otherwise GetCallableHandlers should be used).
    GEXPORT void            CallWaitHandlers();

    // A handle used to access wait handlers after waitable has been signaled
    // and thus can not be accessed directly (other threads can kill the object).    
    class  CallableHandlers
    {
        friend class GWaitable;
        GPtr<HandlerArray>  pArray;        
    public:
        CallableHandlers() { }                
        // Calls handlers in a handle.
        inline void         CallWaitHandlers() { if (pArray) pArray->CallWaitHandlers(); }
    };
    
    inline  void            GetCallableHandlers(CallableHandlers *ph) { if (pHandlers) ph->pArray = pHandlers; }
    

    // Register a handler to be notified when the wait is finished.
    // The wait may be called on a different thread and is 
    // usually responsible for waking the waiting thread.
    GEXPORT bool            AddWaitHandler(WaitHandler handler, Handle pdata);
    GEXPORT bool            RemoveWaitHandler(WaitHandler handler, Handle pdata);

    // Wait for this object to become signaled
    GEXPORT bool            Wait(UInt delay = GFC_WAIT_INFINITE);   
    // Acquires the current object based on associated acquisition interface
    GEXPORT bool            Acquire(UInt delay = GFC_WAIT_INFINITE);

    // Returns 1 if the object is currently signaled, i.e. the associated
    // data is available and no wait is required. Note that this state can change
    // asynchronously based on behavior of other threads.
    GEXPORT virtual bool    IsSignaled() const;

    // Obtain the acquisition interface
    GEXPORT virtual GAcquireInterface*  GetAcquireInterface();
};



// ***** Mutex Class

// GMutex class represents a Mutex??a synchronization object that provides access 
// serialization between different threads, allowing one thread mutually exclusive access 
// to a resource.

class GMutexImpl;

class GMutex : public GWaitable, public GAcquireInterface
{
    friend class GWaitConditionImpl;    
    friend class GMutexImpl;

    // Internal mutex structures
    GMutexImpl  *pImpl; 

public:
        // Constructor/destructor
    GEXPORT GMutex(bool recursive = 1, bool multiWait = 0);
    GEXPORT ~GMutex();

    // Locking functions
    GEXPORT void            Lock();
    GEXPORT bool            TryLock();
    GEXPORT void            Unlock();

    // Returns 1 if the mutes is currently locked by another thread
    // Returns 0 if the mutex is not locked by another thread, and can therefore be acquired. 
    GEXPORT bool            IsLockedByAnotherThread();

    // **  Waitable overrides
    // A mutex is only signaled if it is not locked by ANYONE, so IsSignalled() will
    // return 0 even if it is locked by US. This means that we cannot call Wait() on
    // a mutex that is locked by us; it will never return. However, we can call Acquire() instead.
    GEXPORT virtual bool    IsSignaled() const;
    GEXPORT virtual GAcquireInterface* GetAcquireInterface();

    // ** Acquire Interface implementation
    GEXPORT virtual bool    CanAcquire();
    GEXPORT virtual bool    TryAcquire();   
    GEXPORT virtual bool    TryAcquireCommit();
    GEXPORT virtual bool    TryAcquireCancel();

    // Locker class
    // Used for automatic locking of a mutex
    class Locker
    {
    public:
        GMutex *pMutex;
        Locker(GMutex *pmutex)
            { pMutex = pmutex; pMutex->Lock(); }
        ~Locker()
            { pMutex->Unlock(); }
    };
};



// ***** GWaitCondition

/*
    GWaitCondition is a synchronization primitive that can be used to implement what is known as a monitor.
    Dependent threads wait on a wait condition by calling Wait(), and get woken up by other threads that
    call Notify() or NotifyAll().

    The unique feature of this class is that it provides an atomic way of first releasing a GMutex, and then 
    starting a wait on a wait condition. If both the mutex and the wait condition are associated with the same
    resource, this ensures that any condition checked for while the mutex was locked does not change before
    the wait on the condition is actually initiated.
*/

class GWaitConditionImpl;

class GWaitCondition
{
    friend class GWaitConditionImpl;
    // Internal implementation structure
    GWaitConditionImpl *pImpl;

public:
    // Constructor/destructor
    GEXPORT GWaitCondition();
    GEXPORT ~GWaitCondition();

    // Release mutex and wait for condition. The mutex is re-aqured after the wait.
    GEXPORT bool    Wait(GMutex *pmutex, UInt delay = GFC_WAIT_INFINITE);

    // Notify a condition, releasing at one object waiting
    GEXPORT void    Notify();
    // Notify a condition, releasing all objects waiting
    GEXPORT void    NotifyAll();
};



// ***** GEvent

class GEvent : public GWaitable, public GAcquireInterface
{
    // Event state, its mutex and the wait condition
    volatile bool   State;
    volatile bool   Temporary;  
    mutable GMutex  StateMutex;
    GWaitCondition  StateWaitCondition;

public:
    // Constructor/destructor
    GEXPORT GEvent(bool setInitially = 0, bool multiWait = 0);
    GEXPORT ~GEvent();

    // Wait on an event condition until it is set
    GEXPORT bool    Wait(UInt delay = GFC_WAIT_INFINITE);   
    
    // Set an event, releasing objects waiting on it
    GEXPORT void    SetEvent();
    // Reset an event, un-signaling it
    GEXPORT void    ResetEvent();
    // Set and then reset an event once a waiter is released.
    // If threads are already waiting, they will be notified and released
    // If threads are not waiting, the event is set until the first thread comes in
    GEXPORT void    PulseEvent();
    
    // Signaled override, an event is signaled once it is set
    GEXPORT virtual bool    IsSignaled() const;

    GEXPORT virtual GAcquireInterface*  GetAcquireInterface();

    // Acquire interface implementation 
    GEXPORT virtual bool    CanAcquire();
    GEXPORT virtual bool    TryAcquire();
    GEXPORT virtual bool    TryAcquireCommit();
    GEXPORT virtual bool    TryAcquireCancel();
};



// ***** GSemaphore

// The GSemaphore class represents a Semaphore??a synchronization object 
// that allows a limited number of threads in one or more processes to access
// a resource. A GSemaphore object maintains a count of the number of 
// threads currently accessing a specified resource. The GSemaphore class is 
// primarily used to serialize thread execution, similar to GMutex, 
// however, unlike a mutex, a semaphore can be accessed by more than one thread
// at a time.

class GSemaphore : public GWaitable, public GAcquireInterface
{   
protected:

    // Event state, its mutex and the wait condition
    SInt            MaxValue;
    volatile SInt   Value;
    mutable GMutex  ValueMutex;
    GWaitCondition  ValueWaitCondition;

public:
    // Constructor/destructor
    GEXPORT GSemaphore(SInt maxValue = 1, bool multiWait = 0);
    GEXPORT ~GSemaphore();

    
    // Get current value and max
    SInt            GetMaxValue() const;
    SInt            GetValue() const;
    SInt            GetAvailable() const;

    
    // *** Actions

    // Obtains multiple value of a semaphore
    // Returns 0 if query failed (count > max value error or timeout)
    GEXPORT bool            ObtainSemaphore(SInt count = 1, UInt delay = GFC_WAIT_INFINITE);
    // Release semaphore values
    // Returns success code
    GEXPORT bool            ReleaseSemaphore(SInt count = 1);


    // *** Operators

    // Postfix increment/decrement, return value before operation
    GEXPORT SInt            operator ++ (SInt);
    GEXPORT SInt            operator -- (SInt);
    // Postfix increment/decrement, return value before operation
    GEXPORT SInt            operator += (SInt count);
    GEXPORT SInt            operator -= (SInt count);


    // *** Waitable objects

    // Create a semaphore acquisition object that would increment a semaphore by a user-defined count
    // This object can be passed to AcquireMultipleObjects functions, 
    // and will acquire several values form a semaphore
    // This object must be released before the semaphore
    GEXPORT GWaitable*          CreateWaitableIncrement(SInt count);

    // Acquire interface implementation
    // Default
    GEXPORT virtual bool                CanAcquire();
    GEXPORT virtual bool                TryAcquire();
    GEXPORT virtual bool                TryAcquireCommit();
    GEXPORT virtual bool                TryAcquireCancel();

    // GWaitable implementation
    GEXPORT virtual bool                IsSignaled() const; 
    GEXPORT virtual GAcquireInterface*  GetAcquireInterface();


    // Locker class, used for automatic acquisition of a semaphore
    class Locker
    {
    public:     
        GSemaphore *pSemaphore;
        SInt        Count;
        Locker(GSemaphore *psemaphore, SInt count = 1)
            { pSemaphore = psemaphore; Count = count; 
              pSemaphore->ObtainSemaphore(count); }
        ~Locker()
            { pSemaphore->ReleaseSemaphore(Count); }
    };
};


// ***** GThread class


// *** Thread flags

// Indicates that the thread is has been started, i.e. Start method has been called, and threads
// OnExit() method has not yet been called/returned.
#define GFC_THREAD_STARTED              0x01
// This flag is set once the thread has ran, and finished.
#define GFC_THREAD_FINISHED             0x02
// This flag is set temporarily if this thread was started suspended. It is used internally.
#define GFC_THREAD_START_SUSPENDED      0x08
// This flag is used to ask a thread to exit. Message driven threads will usually check this flag
// and finish once it is set.
#define GFC_THREAD_EXIT                 0x10


class GThread : public GWaitable, public GAcquireInterface 
{ // NOTE: GWaitable must be the first base since it implements GRefCountImpl.    
#if defined(GFC_OS_WIN32) || defined(GFC_OS_XBOX) || defined(GFC_OS_XBOX360)
    friend unsigned WINAPI GThread_Win32StartFn(void *pthread);

#elif defined(GFC_OS_WII)
    friend void *GThread_PthreadStartFn(void * phandle);

#else
    friend void *GThread_PthreadStartFn(void * phandle);

    static int            InitAttr;
    static pthread_attr_t Attr;
#endif

protected:    
    // Thread state flags
    GAtomicInt<UInt32>  ThreadFlags;
    GAtomicInt<SInt32>  SuspendCount;
    UPInt               StackSize;

#if defined(GFC_OS_WIN32) || defined(GFC_OS_XBOX) || defined(GFC_OS_XBOX360)
    Handle              ThreadHandle;

    // System-specific cleanup function called from destructor
    void                CleanupSystemThread();

#elif defined(GFC_OS_WII)
    OSThread            ThreadHandle;
    void*               Stack;
    jmp_buf             ExitJmp;

#else
    pthread_t           ThreadHandle;
#endif

    // Exit code of the thread, as returned by Run.
    SInt                ExitCode;

    // Internal run function.
    SInt                    PRun();    
    // Finishes the thread and releases internal reference to it.
    void                    FinishAndRelease();
 
    // Protected copy constructor
    GThread(const GThread &source) : GWaitable(1) { GUNUSED(source); }

public:

    // *** Callback functions, can be used instead of overriding Run

    // Run function prototypes.    
    // Thread function and user handle passed to it, executed by the default
    // GThread::Run implementation if not null.
    typedef SInt (*ThreadFn)(GThread *pthread, Handle h);
    
    // Thread ThreadFunction1 is executed if not 0, otherwise ThreadFunction2 is tried
    ThreadFn    ThreadFunction;    
    // User handle passes to a thread
    Handle      UserHandle;

    // Thread state to start a thread with
    enum ThreadState
    {
        NotRunning  = 0,
        Running     = 1,
        Suspended   = 2
    };


    // *** Constructors

    // A default constructor always creates a thread in NotRunning state, because
    // the derived class has not yet been initialized. The derived class can call Start explicitly.
    explicit GEXPORT GThread(UPInt stackSize = 128 * 1024);
    // Constructors that initialize the thread with a pointer to function.
    // An option to start a thread is available, but it should not be used if classes are derived from GThread.
    GEXPORT          GThread(ThreadFn threadFunction, Handle userHandle = 0, UPInt stackSize = 128 * 1024,
                             ThreadState initialState = NotRunning);

    // Destructor.
    GEXPORT virtual ~GThread();

    // Waits for all GThreads to finish; should be called only from the root
    // application thread. Once this function returns, we know that all other
    // thread's references to GThread object have been released.
    GEXPORT static  void FinishAllThreads();


    // *** Overridable Run function for thread processing

    // - returning from this method will end the execution of the thread
    // - return value is usually 0 for success 
    GEXPORT virtual SInt    Run();
    // Called after return/exit function
    GEXPORT virtual void    OnExit();


    // *** Thread management

    // Starts the thread if its not already running
    // - internally sets up the threading and calls Run()
    // - initial state can either be Running or Suspended, NotRunning will just fail and do nothing
    // - returns the exit code
    GEXPORT virtual bool    Start(ThreadState initialState = Running);

    // Quits with an exit code
    GEXPORT virtual void    Exit(SInt exitCode=0);

    // Suspend the thread until resumed
    // Returns 1 for success, 0 for failure.
    GEXPORT         bool    Suspend();
    // Resumes currently suspended thread
    // Returns 1 for success, 0 for failure.
    GEXPORT         bool    Resume();

    // Static function to return a pointer to the current thread
    //GEXPORT static GThread* GetThread();


    // *** Thread status query functions

    GEXPORT bool            GetExitFlag() const;
    GEXPORT void            SetExitFlag(bool exitFlag);

    // Determines whether the thread was running and is now finished
    GEXPORT bool            IsFinished() const;
    // Determines if the thread is currently suspended
    GEXPORT bool            IsSuspended() const;
    // Returns current thread state
    GEXPORT ThreadState     GetThreadState() const;


    // Returns the thread exit code. Exit code is initialized to 0,
    // and set to the return value if Run function after the thread is finished.
    GINLINE SInt            GetExitCode() const { return ExitCode; }
    // Returns an OS handle 
#if defined(GFC_OS_WIN32) || defined(GFC_OS_XBOX) || defined(GFC_OS_XBOX360)
    GINLINE Handle          GetOSHandle() const { return ThreadHandle; }

#elif defined(GFC_OS_WII)
    GINLINE OSThread *      GetOSHandle() const { return const_cast<OSThread*>(&ThreadHandle); }

#else
    GINLINE pthread_t       GetOSHandle() const { return ThreadHandle; }
#endif

    // *** Sleep

    // Sleep secs seconds
    GEXPORT virtual bool    Sleep(UInt secs);
    // Sleep msecs milliseconds
    GEXPORT virtual bool    MSleep(UInt msecs);


    // *** GWaitable interface implementation

    // A thread is signaled once it has finished. GWaitable interface
    // can be used to wait for a thread to finish.
    virtual bool   IsSignaled() const {  return (ThreadFlags & GFC_THREAD_FINISHED) != 0; }

    // A thread can be acquired if it has finished.
    // However, the acquisition of a thread does not require any release logic.
    virtual GAcquireInterface*  GetAcquireInterface() { return this; }

    // *** GAcquire interface implementation    
    virtual bool    CanAcquire() { return IsSignaled(); }
    virtual bool    TryAcquire() { return IsSignaled(); }
    // Default implementation (that just succeeds) is fine here
    //GEXPORT   virtual bool    TryAcquireCommit();
    //GEXPORT   virtual bool    TryAcquireCancel();
};


#endif // GFC_NO_THREADSUPPORT

#endif // INC_GTHREADS_H

//**********************************************************************
//
// Copyright (c) 2002
// Thomas Young, PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

// disable warning 'symbol truncated in debugging information'
#pragma warning(disable : 4786)

// disable warning 'return type for 'nList_Internal::const_iterator<class cShape *>::operator ->' is 'class cShape **const  ' (ie; not a UDT or reference to a UDT.  Will produce errors if applied using infix notation)'
#pragma warning(disable : 4284)

// disable warning 'obsolete declaration style: please use 'cMeshWithTrackedTransform<tMesh>::~cMeshWithTrackedTransform' instead'
#pragma warning(disable : 4812)

// disable warning 'strcpy' was declared depreciated
#pragma warning(disable : 4996)

//static inline long AsL(size_t value)
//{
//    return static_cast<long>(value);
//}
template <class T> long
SizeL(const T& container)
{
    return static_cast<long>(container.size());
}

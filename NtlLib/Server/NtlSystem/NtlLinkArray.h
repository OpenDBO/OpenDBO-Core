//***********************************************************************************
//
//	File		:	NtlLinkArray.h
//
//	Begin		:	2005-12-09
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	연결된 배열 구조
//
//***********************************************************************************

#ifndef __NTLLINKARRAY_H__
#define __NTLLINKARRAY_H__

#include "NtlDebug.h"


//-----------------------------------------------------------------------------------
// Static Link Array
//-----------------------------------------------------------------------------------
template<class TYPE, unsigned int MAX_SIZE = 3, unsigned int EXTRA_SIZE = 0>
class CNtlLinkArray
{
public:
	typedef unsigned int POSITION;

protected:

	struct sBUCKET
	{
		POSITION		prev;
		POSITION		next;
		bool			empty;
		TYPE			data;

		bool			IsEmpty() const { return empty; }
		bool			SetEmpty(const bool flag) { empty = flag; }
		POSITION		GetPrev() const { return prev; }
		POSITION		GetNext() const { return next; }
		POSITION		SetPrev(const POSITION pos) { prev = pos; }
		POSITION		SetNext(const POSITION pos) { next = pos; }
		void			SetData(const TYPE& t) { data = t; }
		TYPE&			GetData() { return data; }
	};


public:

	CNtlLinkArray()
		:m_end( MAX_SIZE + EXTRA_SIZE ) { Precreate(); }

	virtual ~CNtlLinkArray() {}


private:

	void				Precreate();


public:


	unsigned int		GetSize() { return m_size; }

	unsigned int		GetMaxSize() { return m_maxSize; }

	unsigned int		GetCapacity() { return MAX_SIZE; }

	unsigned int		GetFreeSize() { return MAX_SIZE - m_size; }

	bool				IsEmpty() { return m_begin == m_end; }



	POSITION			Begin();

	POSITION			rBegin();

	POSITION			Prev(POSITION pos);

	POSITION			Next(POSITION pos);

	POSITION			End() { return m_end; }



	TYPE *				Alloc(POSITION & pos);

	POSITION			Free(TYPE * t);


	POSITION			Insert(TYPE& t);

	POSITION			Remove(POSITION pos);

	void				Clear();

	TYPE&				GetAt(POSITION pos);

	TYPE*				GetPtr(POSITION pos);

	void				SetAt(POSITION pos, TYPE& t);

	POSITION			GetPosition(TYPE* t);


protected:

	unsigned int		m_size;	// 현재 사용중인 개수

	unsigned int		m_maxSize;	// 최대 사용한 개수

	POSITION			m_end; // 배열의 끝값 (end는 배열끝 + 1의 위치)

	POSITION			m_begin; // 사용하는 첫번째 위치값

	POSITION			m_rBegin; // 사용하는 첫번째 위치값

	POSITION			m_emptyHead; // 비어있는 맨 처음 위치

	POSITION			m_emptyTail; // 비어있는 마지막 위치(erase된 원소는 이뒤에 삽입된다)

	sBUCKET				m_array[MAX_SIZE + EXTRA_SIZE]; // 실제 배열

};


template<class TYPE, unsigned int MAX_SIZE, unsigned int EXTRA_SIZE>
inline void CNtlLinkArray<TYPE, MAX_SIZE, EXTRA_SIZE>::Clear()
{
	NTL_ASSERT( m_end > 3 );

//	memset( m_array, 0x00, sizeof(sBUCKET) * End() );

	for( POSITION pos = 0; pos != m_end; pos++ )
	{
		m_array[ pos ].empty = true;
		m_array[ pos ].prev = pos - 1;
		m_array[ pos ].next = pos + 1;
	}

	m_array[ 0 ].prev = m_array[ m_end - 1 ].next = m_end;

	m_size = 0;
	m_maxSize = 0;
	m_begin = m_rBegin = m_end;

	m_emptyHead	= 0;
	m_emptyTail	= m_end - 1;
}


template<class TYPE, unsigned int MAX_SIZE, unsigned int EXTRA_SIZE>
inline void CNtlLinkArray<TYPE, MAX_SIZE, EXTRA_SIZE>::Precreate()
{
	NTL_ASSERT( m_end > 3 );

//	memset( m_array, 0x00, sizeof(sBUCKET) * End() );

	for( POSITION pos = 0; pos != m_end; pos++ )
	{
		m_array[ pos ].empty = true;
		m_array[ pos ].prev = pos - 1;
		m_array[ pos ].next = pos + 1;
	}

	m_array[ 0 ].prev = m_array[ m_end - 1 ].next = m_end;

	m_size = 0;
	m_maxSize = 0;
	m_begin = m_rBegin = m_end;

	m_emptyHead	= 0;
	m_emptyTail	= m_end - 1;
}


template<class TYPE, unsigned int MAX_SIZE, unsigned int EXTRA_SIZE>
inline typename CNtlLinkArray<TYPE, MAX_SIZE, EXTRA_SIZE>::POSITION CNtlLinkArray<TYPE, MAX_SIZE, EXTRA_SIZE>::Begin()
{
	return m_begin;
}


template<class TYPE, unsigned int MAX_SIZE, unsigned int EXTRA_SIZE>
inline typename CNtlLinkArray<TYPE, MAX_SIZE, EXTRA_SIZE>::POSITION CNtlLinkArray<TYPE, MAX_SIZE, EXTRA_SIZE>::rBegin()
{
	return m_rBegin;
}


template<class TYPE, unsigned int MAX_SIZE, unsigned int EXTRA_SIZE>
inline typename CNtlLinkArray<TYPE, MAX_SIZE, EXTRA_SIZE>::POSITION CNtlLinkArray<TYPE, MAX_SIZE, EXTRA_SIZE>::Prev(POSITION pos)
{
	NTL_ASSERT( pos < m_end );
	NTL_ASSERT( !IsEmpty() );

	return m_array[ pos ].prev;
}


template<class TYPE, unsigned int MAX_SIZE, unsigned int EXTRA_SIZE>
inline typename CNtlLinkArray<TYPE, MAX_SIZE, EXTRA_SIZE>::POSITION CNtlLinkArray<TYPE, MAX_SIZE, EXTRA_SIZE>::Next(POSITION pos)
{
	NTL_ASSERT( pos < m_end );
	NTL_ASSERT( !IsEmpty() );

	return m_array[ pos ].next;
}


template<class TYPE, unsigned int MAX_SIZE, unsigned int EXTRA_SIZE>
inline TYPE* CNtlLinkArray<TYPE, MAX_SIZE, EXTRA_SIZE>::Alloc(POSITION& pos)
{
	if( m_emptyTail == m_end || !GetFreeSize() )
		return NULL;

	pos = m_emptyHead;

	m_emptyHead = m_array[ pos ].next;

	if( m_emptyHead != m_end )
		m_array[ m_emptyHead ].prev = m_end;
	else
		m_emptyTail = m_end;

	m_array[ pos ].next = m_begin; 

	if( m_array[ pos ].next != m_end )
		m_array[ m_array[ pos ].next ].prev = pos;	


	m_begin = pos;
	if( m_rBegin == m_end )
		m_rBegin = pos;

	++m_size;

	if( m_size > m_maxSize )
	{
		m_maxSize = m_size;
	}

	m_array[ pos ].empty = false;

	return &m_array[ pos ].data;
}


template<class TYPE, unsigned int MAX_SIZE, unsigned int EXTRA_SIZE>
inline typename CNtlLinkArray<TYPE, MAX_SIZE, EXTRA_SIZE>::POSITION CNtlLinkArray<TYPE, MAX_SIZE, EXTRA_SIZE>::Free(TYPE* p)
{
	POSITION pos = GetPosition( p );

	return Remove( pos );
}


template<class TYPE, unsigned int MAX_SIZE, unsigned int EXTRA_SIZE>
inline typename CNtlLinkArray<TYPE, MAX_SIZE, EXTRA_SIZE>::POSITION CNtlLinkArray<TYPE, MAX_SIZE, EXTRA_SIZE>::Insert(TYPE& t)
{
	POSITION pos = m_end;

	TYPE * p = Alloc( pos );	
	if( p )
	{
		*p = t;
	}

	return pos;
}


template<class TYPE, unsigned int MAX_SIZE, unsigned int EXTRA_SIZE>
inline typename CNtlLinkArray<TYPE, MAX_SIZE, EXTRA_SIZE>::POSITION CNtlLinkArray<TYPE, MAX_SIZE, EXTRA_SIZE>::Remove(POSITION pos)
{
	if( pos >= m_end || IsEmpty() )
		return m_end;

	POSITION ret = Next( pos );

	if( m_array[ pos ].next != m_end )
		m_array[ m_array[ pos ].next ].prev = m_array[ pos ].prev;


	if( m_array[ pos ].prev != m_end )
		m_array[ m_array[ pos ].prev ].next = m_array[ pos ].next;
	else
		m_begin = m_array[ pos ].next;


	if( m_rBegin == pos )
		m_rBegin = m_array[ pos ].prev;


	m_array[ pos ].prev = m_emptyTail;
	m_array[ pos ].next = m_end;


	if( m_array[ pos ].prev != m_end )
		m_array[ m_array[ pos ].prev ].next = pos;
	else
		m_emptyHead = pos;


	m_emptyTail = pos;

	--m_size;

	m_array[ pos ].empty = true;

	return ret;
}


template<class TYPE, unsigned int MAX_SIZE, unsigned int EXTRA_SIZE>
inline typename CNtlLinkArray<TYPE, MAX_SIZE, EXTRA_SIZE>::POSITION CNtlLinkArray<TYPE, MAX_SIZE, EXTRA_SIZE>::GetPosition(TYPE* t)
{
	NTL_ASSERT( t >= (TYPE*) &m_array[ 0 ]);
	NTL_ASSERT( t < (TYPE*) &m_array[ m_end ] );

	sBUCKET * pBucket = (sBUCKET*) ( size_t(t) - ( sizeof(sBUCKET) - sizeof(TYPE) ) );

	return (POSITION) ( size_t(pBucket) - size_t(m_array) ) / sizeof(sBUCKET);
}


template<class TYPE, unsigned int MAX_SIZE, unsigned int EXTRA_SIZE>
inline TYPE* CNtlLinkArray<TYPE, MAX_SIZE, EXTRA_SIZE>::GetPtr(POSITION pos)
{
	if( pos >= End() || m_array[ pos ].empty )
		return NULL;

	return &(m_array[ pos ].data);
}


template<class TYPE, unsigned int MAX_SIZE, unsigned int EXTRA_SIZE>
inline TYPE& CNtlLinkArray<TYPE, MAX_SIZE, EXTRA_SIZE>::GetAt(POSITION pos)
{
	NTL_ASSERT( pos < End() );
	NTL_ASSERT( !m_array[ pos ].empty );

	return *&(m_array[ pos ].data);
}


template<class TYPE, unsigned int MAX_SIZE, unsigned int EXTRA_SIZE>
inline void CNtlLinkArray<TYPE, MAX_SIZE, EXTRA_SIZE>::SetAt(POSITION pos, TYPE& t)
{
	NTL_ASSERT( pos < End() );
	NTL_ASSERT( m_array[ pos ].empty );

	m_array[ pos ].data = t;
}


//-----------------------------------------------------------------------------------
// Link Array2
//-----------------------------------------------------------------------------------
template<class TYPE>
class CNtlLinkArray_Static
{
public:
	typedef unsigned int POSITION;

protected:

	struct sBUCKET
	{
		POSITION		prev;
		POSITION		next;
		bool			empty;
		TYPE			data;

		bool			IsEmpty() const { return empty; }
		bool			SetEmpty(const bool flag) { empty = flag; }
		POSITION		GetPrev() const { return prev; }
		POSITION		GetNext() const { return next; }
		POSITION		SetPrev(const POSITION pos) { prev = pos; }
		POSITION		SetNext(const POSITION pos) { next = pos; }
		void			SetData(const TYPE& t) { data = t; }
		TYPE&			GetData() { return data; }
	};


public:

	CNtlLinkArray_Static() { Init(); }

	virtual ~CNtlLinkArray_Static() { Destroy(); }


	bool				Create(int iMaxSize, int iExtraSize);

	void				Destroy();


private:

	void				Init();

	void				Precreate();


public:


	unsigned int		GetSize() { return m_size; }

	unsigned int		GetMaxSize() { return m_maxSize; }

	unsigned int		GetCapacity() { return MAX_SIZE; }

	unsigned int		GetFreeSize() { return MAX_SIZE - m_size; }

	bool				IsEmpty() { return m_begin == m_end; }



	POSITION			Begin();

	POSITION			rBegin();

	POSITION			Prev(POSITION pos);

	POSITION			Next(POSITION pos);

	POSITION			End() { return m_end; }

	POSITION			EmptyHead() { return m_emptyHead; }



	TYPE *				Alloc(POSITION & pos);

	POSITION			Free(TYPE * t);


	POSITION			Insert(TYPE& t);

	POSITION			Remove(POSITION pos);

	void				Clear();

	TYPE&				GetAt(POSITION pos);

	TYPE*				GetPtr(POSITION pos);

	void				SetAt(POSITION pos, TYPE& t);

	POSITION			GetPosition(TYPE* t);


protected:

	unsigned int		m_size;	// 현재 사용중인 개수

	unsigned int		m_maxSize;	// 최대 사용한 개수

	POSITION			m_end; // 배열의 끝값 (end는 배열끝 + 1의 위치)

	POSITION			m_begin; // 사용하는 첫번째 위치값

	POSITION			m_rBegin; // 사용하는 맨 마지막 위치값

	POSITION			m_emptyHead; // 비어있는 맨 처음 위치

	POSITION			m_emptyTail; // 비어있는 마지막 위치(erase된 원소는 이뒤에 삽입된다)

	sBUCKET*			m_array; // 실제 배열

	int					MAX_SIZE;

	int					EXTRA_SIZE;

};

template<class TYPE>
inline bool CNtlLinkArray_Static<TYPE>::Create(int iMaxSize, int iExtraSize)
{
	NTL_ASSERT( NULL == m_array );


	MAX_SIZE = iMaxSize;

	EXTRA_SIZE = iExtraSize;

	m_end = MAX_SIZE + EXTRA_SIZE;


	NTL_ASSERT( m_end > 3 );

	m_array = new sBUCKET[m_end];
	if (NULL == m_array)
		return false;

	Precreate();

	return true;
}


template<class TYPE>
inline void CNtlLinkArray_Static<TYPE>::Destroy()
{
	SAFE_DELETE_ARRAY( m_array );
}


template<class TYPE>
inline void CNtlLinkArray_Static<TYPE>::Init()
{
	m_array = NULL;
	m_begin = m_rBegin = m_end = m_size = m_maxSize = m_emptyHead = m_emptyTail = MAX_SIZE = EXTRA_SIZE = 0;
}


template<class TYPE>
inline void CNtlLinkArray_Static<TYPE>::Clear()
{
	NTL_ASSERT( NULL != m_array );

//	memset( m_array, 0x00, sizeof(sBUCKET) * End() );

	for( POSITION pos = 0; pos != m_end; pos++ )
	{
		m_array[ pos ].empty = true;
		m_array[ pos ].prev = pos - 1;
		m_array[ pos ].next = pos + 1;
	}

	m_array[ 0 ].prev = m_array[ m_end - 1 ].next = m_end;

	m_size = 0;
	m_maxSize = 0;
	m_begin	= m_rBegin = m_end;

	m_emptyHead	= 0;
	m_emptyTail	= m_end - 1;
}


template<class TYPE>
inline void CNtlLinkArray_Static<TYPE>::Precreate()
{
	NTL_ASSERT( NULL != m_array );

//	memset( m_array, 0x00, sizeof(sBUCKET) * End() );

	for( POSITION pos = 0; pos != m_end; pos++ )
	{
		m_array[ pos ].empty = true;
		m_array[ pos ].prev = pos - 1;
		m_array[ pos ].next = pos + 1;
	}

	m_array[ 0 ].prev = m_array[ m_end - 1 ].next = m_end;

	m_size = 0;
	m_begin = m_rBegin = m_end;

	m_emptyHead	= 0;
	m_emptyTail	= m_end - 1;
}


template<class TYPE>
inline typename CNtlLinkArray_Static<TYPE>::POSITION CNtlLinkArray_Static<TYPE>::Begin()
{
	NTL_ASSERT( NULL != m_array );

	return m_begin;
}

template<class TYPE>
inline typename CNtlLinkArray_Static<TYPE>::POSITION CNtlLinkArray_Static<TYPE>::rBegin()
{
	NTL_ASSERT( NULL != m_array );

	return m_rBegin;
}



template<class TYPE>
inline typename CNtlLinkArray_Static<TYPE>::POSITION CNtlLinkArray_Static<TYPE>::Prev(POSITION pos)
{
	NTL_ASSERT( NULL != m_array );
	NTL_ASSERT( pos < m_end );

	return m_array[ pos ].prev;
}


template<class TYPE>
inline typename CNtlLinkArray_Static<TYPE>::POSITION CNtlLinkArray_Static<TYPE>::Next(POSITION pos)
{
	NTL_ASSERT( NULL != m_array );
	NTL_ASSERT( pos < m_end );

	return m_array[ pos ].next;
}


template<class TYPE>
inline TYPE* CNtlLinkArray_Static<TYPE>::Alloc(POSITION& pos)
{
	NTL_ASSERT( NULL != m_array );

	if( m_emptyTail == m_end || !GetFreeSize() )
		return NULL;

	pos = m_emptyHead;

	m_emptyHead = m_array[ pos ].next;

	if( m_emptyHead != m_end )
		m_array[ m_emptyHead ].prev = m_end;
	else
		m_emptyTail = m_end;

	m_array[ pos ].next = m_begin; 

	if( m_array[ pos ].next != m_end )
		m_array[ m_array[ pos ].next ].prev = pos;	


	m_begin = pos;
	if( m_rBegin == m_end )
		m_rBegin = pos;


	++m_size;

	if( m_size > m_maxSize )
	{
		m_maxSize = m_size;
	}

	m_array[ pos ].empty = false;

	return &m_array[ pos ].data;
}


template<class TYPE>
inline typename CNtlLinkArray_Static<TYPE>::POSITION CNtlLinkArray_Static<TYPE>::Free(TYPE* p)
{
	NTL_ASSERT( NULL != m_array );

	POSITION pos = GetPosition( p );

	return Remove( pos );
}


template<class TYPE>
inline typename CNtlLinkArray_Static<TYPE>::POSITION CNtlLinkArray_Static<TYPE>::Insert(TYPE& t)
{
	NTL_ASSERT( NULL != m_array );

	POSITION pos = m_end;

	TYPE * p = Alloc( pos );	
	if( p )
	{
		*p = t;
	}

	return pos;
}


template<class TYPE>
inline typename CNtlLinkArray_Static<TYPE>::POSITION CNtlLinkArray_Static<TYPE>::Remove(POSITION pos)
{
	NTL_ASSERT( NULL != m_array );

	if( pos >= m_end || IsEmpty() )
		return m_end;

	POSITION ret = Next( pos );

	if( m_array[ pos ].next != m_end )
		m_array[ m_array[ pos ].next ].prev = m_array[ pos ].prev;


	if( m_array[ pos ].prev != m_end )
		m_array[ m_array[ pos ].prev ].next = m_array[ pos ].next;
	else
		m_begin = m_array[ pos ].next;


	if( m_rBegin == pos )
		m_rBegin = m_array[ pos ].prev;


	m_array[ pos ].prev = m_emptyTail;
	m_array[ pos ].next = m_end;


	if( m_array[ pos ].prev != m_end )
		m_array[ m_array[ pos ].prev ].next = pos;
	else
		m_emptyHead = pos;


	m_emptyTail = pos;

	--m_size;

	m_array[ pos ].empty = true;

	return ret;
}


template<class TYPE>
inline typename CNtlLinkArray_Static<TYPE>::POSITION CNtlLinkArray_Static<TYPE>::GetPosition(TYPE* t)
{
	NTL_ASSERT( NULL != m_array );
	NTL_ASSERT( t >= (TYPE*) &m_array[ 0 ]);
	NTL_ASSERT( t < (TYPE*) &m_array[ m_end ] );

	sBUCKET * pBucket = (sBUCKET*) ( size_t(t) - ( sizeof(sBUCKET) - sizeof(TYPE) ) );

	return (POSITION) ( size_t(pBucket) - size_t(m_array) ) / sizeof(sBUCKET);
}


template<class TYPE>
inline TYPE* CNtlLinkArray_Static<TYPE>::GetPtr(POSITION pos)
{
	NTL_ASSERT( NULL != m_array );

	if( pos >= End() || m_array[ pos ].empty )
		return NULL;

	return &(m_array[ pos ].data);
}


template<class TYPE>
inline TYPE& CNtlLinkArray_Static<TYPE>::GetAt(POSITION pos)
{
	NTL_ASSERT( NULL != m_array );
	NTL_ASSERT( pos < End() );
	NTL_ASSERT( !m_array[ pos ].empty );

	return *&(m_array[ pos ].data);
}


template<class TYPE>
inline void CNtlLinkArray_Static<TYPE>::SetAt(POSITION pos, TYPE& t)
{
	NTL_ASSERT( NULL != m_array );
	NTL_ASSERT( pos < End() );
	NTL_ASSERT( m_array[ pos ].empty );

	m_array[ pos ].data = t;
}




//-----------------------------------------------------------------------------------
// Link Array3
//-----------------------------------------------------------------------------------
//  [5/11/2006 zeroera] : 추가필요 : header, body 분리형 array
/*
template<class TYPE>
class CNtlLinkArray3
{
public:
typedef unsigned int POSITION;

protected:

struct sHEAD
{
POSITION		prev;
POSITION		next;
};

struct sINFO
{
bool			bUsed;
};

protected:

sHEAD				aHead[];
sINFO				aInfo[];
TYPE				aData[];
};
*/


#endif // __NTLLINKARRAY_H__



//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef LOG_INCLUDED
#define LOG_INCLUDED

#ifdef LOGGING_ON

void Log(const char* string);
void Log(unsigned long value);

#else

inline void
Log(const char* string)
{
}
inline void
Log(unsigned long value)
{
}

#endif

#endif // include guard


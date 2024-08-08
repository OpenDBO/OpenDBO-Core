//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

class iXMLHandler;

bool ParseXML(const char* buffer, unsigned long buffer_size, iXMLHandler& handler);
bool ParseTokenisedXML(const char* buffer, unsigned long buffer_size, iXMLHandler& handler);

bool ParseXML(const char* format, const char* buffer, unsigned long buffer_size, iXMLHandler& handler);

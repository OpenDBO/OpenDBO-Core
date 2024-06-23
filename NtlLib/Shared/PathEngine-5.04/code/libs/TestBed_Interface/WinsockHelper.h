//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

namespace nWinsockHelper
{
    class cTCPConnection;
    class cTCPServer;

    void InitialiseWinsock();

    cTCPServer* CreateTCPServer(unsigned short port);
    cTCPConnection* ConnectToTCPServer(const char* ip_address, unsigned short port);

// cTCPServer 'methods'
    void Destroy(cTCPServer*);
    cTCPConnection* AcceptConnection(cTCPServer*);

// cTCPConnection 'methods'
    void Destroy(cTCPConnection*);
    bool ClosedByOtherEnd(cTCPConnection*);
    void Send(cTCPConnection*, const char* data_pointer, unsigned long data_size);
    int Receive(cTCPConnection*, char* buffer, int buffer_size);
};


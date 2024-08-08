//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "WinsockHelper.h"
#include "common/interface/Error.h"
#include <winsock.h>
#include <stdio.h>
#include <ctype.h>


#define	WSVERS MAKEWORD(1, 1)

namespace nWinsockHelper
{

static bool g_initialised=false;

class cTCPServer
{
public:
    SOCKET s;
    cTCPServer(SOCKET s) :
      s(s)
    {
    }
    ~cTCPServer();
};
class cTCPConnection
{
public:
    SOCKET s;
    bool closed;
    cTCPConnection(SOCKET s) :
    s(s),
    closed(false)
    {
    }
    ~cTCPConnection();
};

void InitialiseWinsock()
{
    if(g_initialised)
        return;

    WSADATA wsadata;
    if(WSAStartup(WSVERS, &wsadata))
    {
        Error("NonFatal","WSAStartup call failed");
        return;
    }
    if(wsadata.wVersion != WSVERS)
    {
        WSACleanup( );
        Error("NonFatal","WSAStartup dll version too low");
        return;
    }

    g_initialised=true;
}

cTCPServer* CreateTCPServer(unsigned short port)
{
    struct sockaddr_in local;
    SOCKET listen_socket;
    const char* attributes[3];
    char buffer[50];

    local.sin_family = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY;
    local.sin_port = htons(port);

    listen_socket = socket(AF_INET, SOCK_STREAM,0); // TCP socket
    if (listen_socket == INVALID_SOCKET)
    {
        attributes[0]="winsock_error_code";
        sprintf(buffer,"%d",WSAGetLastError());
        attributes[1]=buffer;
        attributes[2]=0;
        Error("NonFatal","cWinsockHelper::createTCPServer() - socket() failed. Returning 0.",attributes);
        return 0;
    }

    {
        unsigned long pointed_to=1;
        if(ioctlsocket(listen_socket, FIONBIO, &pointed_to)!=0)
        {
            closesocket(listen_socket);
            Error("NonFatal","cWinsockHelper::createTCPServer() - ioctlsocket() failed. Returning 0.",attributes);
            return 0;
        }
    }

    if(bind(listen_socket,(struct sockaddr*)&local,sizeof(local) ) == SOCKET_ERROR)
    {
        closesocket(listen_socket);
        attributes[0]="winsock_error_code";
        sprintf(buffer,"%d",WSAGetLastError());
        attributes[1]=buffer;
        attributes[2]=0;
        Error("NonFatal","cWinsockHelper::createTCPServer() - bind() failed. Returning 0.",attributes);
        return 0;
    }

    if (listen(listen_socket,SOMAXCONN) == SOCKET_ERROR)
    {
        closesocket(listen_socket);
        attributes[0]="winsock_error_code";
        sprintf(buffer,"%d",WSAGetLastError());
        attributes[1]=buffer;
        attributes[2]=0;
        Error("NonFatal","cWinsockHelper::createTCPServer() - listen() failed. Returning 0.",attributes);
        return 0;
    }

    return new cTCPServer(listen_socket);
}

cTCPConnection* ConnectToTCPServer(const char* ip_address, unsigned short port)
{
    struct hostent *hp;
    unsigned int addr;
    struct sockaddr_in server;
    SOCKET  conn_socket;
    const char* attributes[3];
    char buffer[50];


    // Attempt to detect if we should call gethostbyname() or
    // gethostbyaddr()

    if (isalpha(ip_address[0]))
    {   /* server address is a name */
        hp = gethostbyname(ip_address);
    }
    else  
    { /* Convert nnn.nnn address to a usable one */
        addr = inet_addr(ip_address);
        hp = gethostbyaddr((char *)&addr,4,AF_INET);
    }
    if (hp == NULL ) 
    {
        Error("NonFatal","cWinsockHelper::connectToTCPServer - could not resolve address. Returning 0.");
        return 0;
    }

    //
    // Copy the resolved information into the sockaddr_in structure
    //
    memset(&server,0,sizeof(server));
    memcpy(&(server.sin_addr),hp->h_addr,hp->h_length);
    server.sin_family = hp->h_addrtype;
    server.sin_port = htons(port);

    conn_socket = socket(AF_INET,SOCK_STREAM,0); /* Open a socket */
    if(conn_socket==INVALID_SOCKET)
    {
        attributes[0]="winsock_error_code";
        sprintf(buffer,"%d",WSAGetLastError());
        attributes[1]=buffer;
        attributes[2]=0;
        Error("NonFatal","cWinsockHelper::connectToTCPServer() - socket() failed. Returning 0.",attributes);
        return 0;
    }

    if (connect(conn_socket,(struct sockaddr*)&server,sizeof(server))
        == SOCKET_ERROR) 
    {
        int code=WSAGetLastError();
        closesocket(conn_socket);
        attributes[0]="winsock_error_code";
        sprintf(buffer,"%d",code);
        attributes[1]=buffer;
        attributes[2]=0;
        Error("NonFatal","cWinsockHelper::connectToTCPServer() - connect() failed. Returning 0.",attributes);
        return 0;
    }

    {
        unsigned long pointed_to=1;
        if(ioctlsocket(conn_socket, FIONBIO, &pointed_to)!=0)
        {
            closesocket(conn_socket);
            Error("NonFatal","cWinsockHelper::connectToTCPServer() - ioctlsocket() failed. Returning 0.",attributes);
            return 0;
        }
    }

    return new cTCPConnection(conn_socket);
}

void Destroy(cTCPServer* server)
{
    delete server;
}
cTCPServer::~cTCPServer()
{
//.... use shutdown to disconnect gracefully
    closesocket(s);
}
cTCPConnection* AcceptConnection(cTCPServer* server)
{
    SOCKET msgsock;
    struct sockaddr_in from;
    int fromlen;
    const char* attributes[3];
    char buffer[50];

    fromlen = sizeof(from);
    msgsock = accept(server->s,(struct sockaddr*)&from, &fromlen);
    if (msgsock == INVALID_SOCKET)
    {
        int code = WSAGetLastError();
        if(code!=WSAEWOULDBLOCK)
        {
            attributes[0]="winsock_error_code";
            sprintf(buffer,"%d",code);
            attributes[1]=buffer;
            attributes[2]=0;
            Error("NonFatal","cWinsockHelper::acceptConnection() - accept() failed. Returning 0.",attributes);
        }
        return 0;
    }

    {
        unsigned long pointed_to=1;
        if(ioctlsocket(msgsock, FIONBIO, &pointed_to)!=0)
        {
            closesocket(msgsock);
            Error("NonFatal","cWinsockHelper::acceptConnection() - ioctlsocket() failed. Returning 0.",attributes);
            return 0;
        }
    }

    return new cTCPConnection(msgsock);
}

void Destroy(cTCPConnection* connection)
{
    delete connection;
}
cTCPConnection::~cTCPConnection()
{
    closesocket(s);
}
bool ClosedByOtherEnd(cTCPConnection* connection)
{
    return connection->closed;
}
void Send(cTCPConnection* connection, const char* data_pointer, unsigned long data_size)
{
    int result;
    const char* attributes[3];
    char buffer[50];

    while(data_size>0)
    {
        result = send(connection->s,data_pointer,data_size,0 );
//..... check here for connection closed
        if(result==SOCKET_ERROR)
        {
            int code = WSAGetLastError();
            attributes[0]="winsock_error_code";
            sprintf(buffer,"%d",code);
            attributes[1]=buffer;
            attributes[2]=0;
            Error("NonFatal","cWinsockHelper::sendMessage() - send() failed.",attributes);
            return;
        }
        data_pointer+=result;
        data_size-=result;
    }
}
int Receive(cTCPConnection* connection, char* buffer, int buffer_size)
{
    int result;
    const char* attributes[3];

    result = recv(connection->s,buffer,buffer_size,0 );
//..... check here for connection closed
    if(result==SOCKET_ERROR)
    {
        int code = WSAGetLastError();
        if(code!=WSAEWOULDBLOCK)
        {
            attributes[0]="winsock_error_code";
            char buffer[50];
            sprintf(buffer,"%d",code);
            attributes[1]=buffer;
            attributes[2]=0;
            Error("NonFatal","cWinsockHelper::receiveMessage() - recv() failed. Returning 0.",attributes);
        }
        return 0;
    }
    return result;
}

}


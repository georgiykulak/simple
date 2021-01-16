#include <cstdio>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main()
{
    // #1 Create socket
    int masterSock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
    
    // Initialize socket
    struct sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons( 1234 );
    sockAddr.sin_addr.s_addr = htonl( INADDR_ANY );
    
    // #2 Bind socket
    bind( masterSock
        ,   reinterpret_cast< struct sockaddr * >( &sockAddr )
        ,   sizeof( sockAddr )
    );

    // #3 Listen socket
    listen( masterSock, SOMAXCONN );

    // #4 Start conversation between server and clients
    while ( true )
    {
        // Create slave socket
        int slaveSocket = accept( masterSock, nullptr, 0 );
        
        int inBuffer[ 5 ];
        // Recieve message
        recv( slaveSocket, inBuffer, 5, MSG_NOSIGNAL );
        // Terminate work with slave socket
        shutdown( slaveSocket, SHUT_RDWR );
        close( slaveSocket );
        
        printf( "%s\n", inBuffer );
    }

    return 0;
}
#include <cstdio>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main()
{
    // #1 Create socket
    int justSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
    
    // Initialize socket
    struct sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons( 1234 );
    sockAddr.sin_addr.s_addr = htonl( INADDR_LOOPBACK );

    // #2 Connect to server
    connect( justSocket
        ,   reinterpret_cast< struct sockaddr * >( &sockAddr )
        ,   sizeof( sockAddr )
    );

    // #3 Send messages
    char outBuffer[] = "Hell";
    send( justSocket, outBuffer, 5, MSG_NOSIGNAL );

    // Terminate work with socket
    shutdown( justSocket, SHUT_RDWR );
    close( justSocket );

    std::cout << outBuffer << std::endl;

    return 0;
}
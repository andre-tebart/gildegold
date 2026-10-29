/* 0x424510, logs the Winsock error err as a fatal error (err_Log level 1) with the error's English explanation, or "Unknown winsock error <n>" for a code the switch does not know */
void net_Error(int err)
{
    char buf[256];

    switch (err) {
    case WSANOTINITIALISED:
#line 49
        err_Log(__FILE__, __LINE__, "WSANOTINITIALISED", 1);
        break;
    case WSAENETDOWN:
#line 51
        err_Log(__FILE__, __LINE__, "WSAENETDOWN: The network subsystem has failed!", 1);
        break;
    case WSAEFAULT:
#line 53
        err_Log(__FILE__, __LINE__, "WSAEFAULT: The buf parameter is not completely contained in a valid part of the user address space", 1);
        break;
    case WSAENOTCONN:
#line 55
        err_Log(__FILE__, __LINE__, "WSAENOTCONN: The socket is not connected.", 1);
        break;
    case WSAEINTR:
#line 57
        err_Log(__FILE__, __LINE__, "WSAEINTR: The (blocking) call was canceled through WSACancelBlockingCall.", 1);
        break;
    case WSAEINPROGRESS:
#line 59
        err_Log(__FILE__, __LINE__, "WSAEINPROGRESS: A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.", 1);
        break;
    case WSAENETRESET:
#line 61
        err_Log(__FILE__, __LINE__, "WSAENETRESET: The connection has been broken due to the keep-alive activity detecting a failure while the operation was in progress.", 1);
        break;
    case WSAENOTSOCK:
#line 63
        err_Log(__FILE__, __LINE__, "WSAENOTSOCK: The descriptor is not a socket.", 1);
        break;
    case WSAEOPNOTSUPP:
#line 65
        err_Log(__FILE__, __LINE__, "WSAEOPNOTSUPP: MSG_OOB was specified, but the socket is not stream-style such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only send operations.", 1);
        break;
    case WSAESHUTDOWN:
#line 67
        err_Log(__FILE__, __LINE__, "WSAESHUTDOWN: The socket has been shut down; it is not possible to receive on a socket after shutdown has been invoked with how set to SD_RECEIVE or SD_BOTH.", 1);
        break;
    case WSAEWOULDBLOCK:
#line 69
        err_Log(__FILE__, __LINE__, "WSAEWOULDBLOCK: The socket is marked as nonblocking and the receive operation would block.", 1);
        break;
    case WSAEMSGSIZE:
#line 71
        err_Log(__FILE__, __LINE__, "WSAEMSGSIZE: The message was too large to fit into the specified buffer and was truncated.", 1);
        break;
    case WSAEINVAL:
#line 73
        err_Log(__FILE__, __LINE__, "WSAEINVAL: The socket has not been bound with bind, or an unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled or (for byte stream sockets only) len was zero or negative.", 1);
        break;
    case WSAECONNABORTED:
#line 75
        err_Log(__FILE__, __LINE__, "WSAECONNABORTED: The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable.", 1);
        break;
    case WSAETIMEDOUT:
#line 77
        err_Log(__FILE__, __LINE__, "WSAETIMEDOUT: The connection has been dropped because of a network failure or because the peer system failed to respond.", 1);
        break;
    case WSAECONNRESET:
#line 79
        err_Log(__FILE__, __LINE__, "WSAECONNRESET: The virtual circuit was reset by the remote side executing a hard or abortive close. The application should close the socket as it is no longer usable. On a UPD-datagram socket this error would indicate that a previous send operation resulted in an ICMP >Port Unreachable< message.", 1);
        break;
    default:
        sprintf(buf, "Unknown winsock error %u!!!", err);
#line 81
        err_Log(__FILE__, __LINE__, buf, 1);
        break;
    }
}

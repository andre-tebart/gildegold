/* 0x401e70, sends len bytes of buf to the broadcast address on the port udp_Init configured; does nothing while no socket is open */

void udp_Send(char *buf, int len)
{
    if (udp_Socket != INVALID_SOCKET)
        sendto(udp_Socket, buf, len, 0, (struct sockaddr *)&udp_Bcast, sizeof(udp_Bcast));
}

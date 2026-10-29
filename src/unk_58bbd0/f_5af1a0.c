/* 0x5af1a0, a forwarder of 0x5af1d0 with the mode 1 (16 B) */

int ai_OpenTransport(int param_1)
{
    return ai_TransportWindow(param_1, 1);
}

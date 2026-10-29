/* 0x5af1b0, a forwarder of 0x5af1d0 with the mode 2 (16 B) */

int ai_OpenTransportSell(int param_1)
{
    return ai_TransportWindow(param_1, 2);
}

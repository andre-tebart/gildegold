/* 0x52fe00, the three network shutdown steps */

void sv_PumpNet(void)
{
    cm_FlushSendQueue();
    cm_ReceiveMessages();
    cm_ExecCommands();
}

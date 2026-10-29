/* 0x5950e0, the trade listener (143 B): 0x52e190, the pump 0x5309a0 (this, 0x67fff); the contacts FERNEINKAUF
   (0x1442028, 0x10) -> 0x5af1c0 and VERKAUF (0x1442048, 0x10) -> 0x5af1b0 when they are the pending 0x6f22e4;
   0x5912c0 every frame */

void ai_Unk5950e0(int param_1)
{
    int r;
    int buy;
    int sell;

    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_Unk5950e0, 0x67fff);
    while (r != 0) {
        buy = sv_RegisterNamePlate("FERNEINKAUF", U_I((unsigned)&full_data_1442028), 0x10);
        sell = sv_RegisterNamePlate("VERKAUF", U_I((unsigned)&full_data_1442048), 0x10);
        ai_TradeContactChecks();
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == buy)
                ai_OpenTransportBuy(param_1);
            else if (U_I((unsigned)&ai_ClickedObject) == sell)
                ai_OpenTransportSell(param_1);
        }
        r = sv_RunFrame(ai_Unk5950e0, 0x67fff);
    }
}

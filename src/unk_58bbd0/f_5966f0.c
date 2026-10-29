/* 0x5966f0, the arrest / fight / denunciation listener (199 B): 0x52e190, the pump 0x5309a0 (this, 0x67fff);
   FESTGEBEN (0x143ff58, 0x16) -> 0x5b5e80 (param_1, param_2); KAMPF (0x143fc78, 0xc) -> 0x5b3130 (param_1,
   param_2, 3); ANSCHWAERZEN (0x143ff38, 0xc) -> 0x5b5010 (param_1) + 0x52d7a0 */

int ai_Unk5b5e80(); int ai_Unk5b5010(); void sv_FitForm(int f); void sv_ButtonRow(int f, int y, int *out, int n, ...);
int ai_AmountRowStatus(); int ai_AmountRow(); short * gm_FindChar(int id); int sv_CheckMoney();
int cm_SendExSetHE(); /* @original: the callers here push the key as an int and read the return (byte-matched ai_Unk5ae870), against cm.h's void (unsigned char, char *) view */ void cm_SendRelations(int a, int b, int c, int d); void cm_SendMoneyDelta(int a, int b); void d2_SetObjEnabled();

void ai_Unk5966f0(int param_1, int param_2)
{
    int r;
    int fest;
    int kampf;
    int ansch;

    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_Unk5966f0, 0x67fff);
    while (r != 0) {
        fest = sv_RegisterNamePlate("contact_FESTGEBEN", U_I((unsigned)&full_data_143ff58), 0x16);
        kampf = sv_RegisterNamePlate("contact_KAMPF", U_I((unsigned)&full_data_143fc78), 0xc);
        ansch = sv_RegisterNamePlate("contact_ANSCHWAERZEN", U_I((unsigned)&full_data_143ff38), 0xc);
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == fest) {
                ai_Unk5b5e80(param_1, param_2);
            } else if (U_I((unsigned)&ai_ClickedObject) == kampf) {
                ai_Unk5b3130(param_1, param_2, 3);
            } else if (U_I((unsigned)&ai_ClickedObject) == ansch) {
                ai_Unk5b5010(param_1);
                sv_CameraFollow();
            }
        }
        r = sv_RunFrame(ai_Unk5966f0, 0x67fff);
    }
}

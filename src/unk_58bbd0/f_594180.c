/* 0x594180, opens the burglar-guild break-in window (63 B): 0x520fe0, 0x525220; the record 0x5b2180 (4, 0,
   0x1440c10, the callback 0x593f40, 0); then 0x525440 and 0x52ae40 */

void ai_Unk594180(int param_1)
{
    if (sv_CheckCharacterFlag(param_1) == 0) {
        sv_OpenPlayerBar(param_1, 0);
        ai_Unk5b2180(4, 0, U_I((unsigned)&full_data_1440c10), (void *)ai_Unk593f40, 0);
        sv_ClosePlayerBar();
        sv_ClearSelection();
    }
}

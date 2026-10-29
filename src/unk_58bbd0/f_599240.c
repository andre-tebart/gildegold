/* 0x599240, opens the regulars' table (124 B): the table record 0x4ffe90 (param_2 + 0x14, 1, 0, 0x12d) and the
   table building 0x53e400 (2, 0, 0x2a, 3, param_1 + 1); when the player's word 0x770b80 + player * 0x21c is the
   record's +0x27 the plain table 0x598b80 opens, else the joinable one 0x598dc0; then 0x52d7a0 */


void ai_Unk599240(int param_1, int param_2)
{
    int rec;
    int b;

    rec = gm_Query(*(int *)(param_2 + 0x14), 1, 0, 0x12d);
    if (rec != 0) {
        b = he_FindEvent(2, 0, 0x2a, 3, U_I(param_1 + 1));
        if (b != 0) {
            if (*(unsigned short *)((unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c) ==
                *(unsigned short *)(param_1 + 0x27)) {
                ai_Unk598b80(param_1, rec);
                sv_CameraFollow();
                return;
            }
            ai_Unk598dc0(param_1, rec);
        }
    }
    sv_CameraFollow();
}

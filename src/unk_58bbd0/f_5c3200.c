/* 0x5c3200, the master letter contact (448 B): the character param_1 (its kind byte 0 via 0x7e3f30 +
   kind * 0x24d), the player word 0x6f25b8 against param_1 + 0x27; the mode: 0x40 for the kinds 5 / 9,
   0x30 with a character (0x50a810), 0x20 without (the profession byte 0x223 of the kind record against
   the six 0x504ad0 / 0x5044b0 slots, the match gives the own character 0x770b80), 0x50 when the carrier
   0x53e400 (2, 0, 7, 3) chain ends. The windows: 0x10 / 0x20 -> 0x5c27b0 (param_1, the mode), 0x30 ->
   0x5c2b90 (param_1, the character), 0x40 -> 0x5c2730 (param_1), 0x50 -> 0x5c26a0 (param_1); 0x52d7a0 */


void ai_MasterLetterWindow(unsigned char *param_1)
{
    char *kind;
    unsigned int id;
    int carrier;
    int mode;
    int v;
    int n;
    int i;
    char *ch;

    if (param_1 == 0 || U_W((unsigned)&ai_PlayerId) != *(unsigned short *)(param_1 + 0x27))
        return;
    id = U_W((unsigned)&ai_PlayerId);
    kind = (char *)(U_I((unsigned)&ai_BTypes) + *param_1 * 0x24d);
    ch = (char *)gm_FindCharacterByMaster(param_1);
    if (*kind == 5 || *kind == 9) {
        mode = 0x40;
    } else if (ch == 0) {
        mode = 0x20;
        v = gm_TitleGroup(*(unsigned char *)(kind + 0x223));
        n = gm_Unk5044b0(U_W((unsigned)&ai_PlayerId), v);
        if (n != 0) {
            for (i = 0; i < 6; i++) {   /* ROUGH: the loop index local */
                if (n == *(unsigned char *)(kind + 0x223 + i)) {
                    mode = 0x10;
                    ch = (char *)(unsigned)&ai_Chars + id * 0x21c;
                    break;
                }
            }
        }
    } else {
        mode = 0x30;
    }
    carrier = he_FindEvent(2, 0, 7, 3, U_I(param_1 + 1));
    for (;;) {
        if (carrier == 0) {
            break;
        }
        if (*(char *)(carrier + 0xac) == 0) {
            mode = 0x50;
            break;
        }
        carrier = he_NextEvent();
    }
    switch (mode) {
    case 0x10:
    case 0x20:
        ai_Unk5c27b0((char *)param_1, mode);
        sv_CameraFollow();
        return;
    case 0x30:
        ai_Unk5c2b90((char *)param_1, (short *)ch);
        sv_CameraFollow();
        return;
    case 0x40:
        ai_Unk5c2730();
        sv_CameraFollow();
        return;
    case 0x50:
        ai_Unk5c26a0();
        break;
    }
    sv_CameraFollow();
}

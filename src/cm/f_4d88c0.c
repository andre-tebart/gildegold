/* 0x4d88c0, the fight loop: starts one of two fight tracks, then runs until the fight ends (ai_Unk6f0cf4 / ai_Unk6f0cf8): the pause
   toggle (key 0x39 = space) with the character animations and the "pause" text, the frame of the UI, and every 0x15e timer calls
   the characters' fight-AI (0x4d2f80) and the fighters' commands (0x4d76a0). Ends with the end-of-fight code 0x4d6b70. */

void ai_Unk4d88c0(char *a)
{
    unsigned int t;
    int i, r;
    char *o;

    t = tb_Calls - 0x15e;
    r = util_RandomU(100);
    ai_Unk4d0a30();
    ai_Unk4d6a50(a);
    ai_Unk6f0d4c = 0;
    if (r < 0x32)
        ai_Unk6f0d4c = msx_PlayAmbient("Athmos\\Kampfathmo_1.mp3", 0xb);
    else
        ai_Unk6f0d4c = msx_PlayAmbient("Athmos\\Kampfathmo_2.mp3", 0xb);
    ai_Unk6f0ce4 = 0;
    while (sv_RunFrame(ai_Unk4d88c0, 0xfd6c6) != 0 && ai_Unk6f0cf8 == 0) {
        if ((ai_GameFlags & 4) == 0 && kb_Last == 0x39) {
            ai_Unk6f0ce4 = ai_Unk6f0ce4 == 0;
            for (i = 0; i < 32; i++) {
                o = ai_Unk7603a0V[i];
                if (o != 0 && *(char **)(*(char **)(o + 0x184) + 0x88) == ai_Unk5fb5b0)
                    ch_PauseAnim(*(char **)(o + 0x184), ai_Unk6f0ce4);
            }
            sv_SetStatusText(&ch_EmptyName);
        }
        if (ai_Unk6f0ce4 != 0)
            sv_SetStatusText(ai_Unk143e674);
        else
            ai_Unk4d6970(a);
        if (ai_Unk6f0cf4 == 0 && ai_Unk6f0cf8 == 0 && ai_Unk6f0ce4 == 0) {
            if (tb_Calls > t + 0x1a4) {
                for (i = 0; i < (unsigned char)ai_Unk6f0d04[0x30]; i++) {
                    if (*(int *)(ai_Unk6f0d04 + 0x34 + i * 4) != -1) {
                        o = gm_FindChar(*(int *)(ai_Unk6f0d04 + 0x34 + i * 4));
                        if (o != 0 && o[2] != 6 && o[2] != 7)
                            ai_Unk4d2f80((int)o);
                    }
                }
                t = tb_Calls;
            }
            if (*(int *)(ai_Unk6f0d74 + 4) == ai_Unk60cb5c) {
                ai_Unk4d3b30(ai_Unk6f0d04);
                for (i = 0; i < (unsigned char)ai_Unk6f0d04[0x30]; i++) {
                    if (*(int *)(ai_Unk6f0d04 + 0x34 + i * 4) != -1)
                        ai_Unk4d76a0((int *)ai_Unk92cae0[i]);
                }
            }
        }
        if (ai_Unk6f0cf4 != 0 || ai_Unk6f0cf8 != 0)
            sv_PopRequest = 1;
    }
    ai_Unk4d6b70();
}

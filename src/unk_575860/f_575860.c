/* 0x575860, the end-of-game tick (1984 B, ROUGH SKELETON): validates the end-of-game gates and the state record,
   then runs the state machine on *(int *)0x5eec60 - 1 (12 states, jump table 0x575fe8, the states 1 .. 0xc). The
   case bodies are not written yet: their helper calls, in address order, are 0x476f80, 0x577c60, 0x57b8a0,
   0x577d90, 0x5778b0, 0x576040 (the state transition), the state record writes, 0x53dcb0 (the end-of-game
   message), 0x577a00, 0x577a90, 0x577b60 (the clock writers 0x6333cc / 0x6333e0), 0x4741a0, 0x577e50,
   0x475e40, 0x5780e0, 0x577b90, 0x52d7a0, 0x52e330, 0x426f20, 0x427290, 0x4c2760; the exit is -4 (0xfffffffc)
   on a failed gate. Ends at 0x575fe2 (a shared return), the jump table 0x575fe8 .. 0x576017, nop fill to 0x576020. */

int ai_Unk575860(void)
{
    char *st;
    char *per;

    if (!U_I((unsigned)&full_data_6f265c))
        return -4;
    st = S;
    if (!st)
        return -4;
    if (!*(int *)(st + 0x18))
        return -4;
    per = *(char **)(st + 0x1c);
    if (!*(int *)(per + 0x74))
        return -4;
    /* 0x57589d .. 0x575915: the first-run / pause handling: the state 3 and the gate pairs 0x6310e4 / 0x75e8c0
       (0x476f80), the state record [st] = [st+4] when the progress word changed, [st+0x30] == 1 and [st] != 9
       resets the flag and calls 0x577c60 / 0x57b8a0(2) */
    if (U_I((unsigned)&full_data_6f262c) == *(int *)(per + 0x74)) {
        if (U_I(st) != 2 && !U_I((unsigned)&d3t_CamMoving) && !U_I((unsigned)&sv_PanelFlag)) {
            d2_SetPointerSprite(0, 0);
            *(int *)(S) = 3;
        }
    }
    st = S;
    if (U_I((unsigned)&full_data_6f262c) != *(int *)(*(char **)(st + 0x1c) + 0x74)) {
        *(int *)st = *(int *)(st + 4);
        st = S;
    }
    if (*(int *)(st + 0x30) == 1 && U_I(st) != 9) {
        *(int *)(st + 0x30) = 0;
        ai_Unk577c60();
        ai_Unk57b8a0(2);
    }
    if (!U_I((unsigned)&full_data_6f2660))
        goto end;
    ai_Unk577d90();
    if (U_I((unsigned)&d3t_CamMoving) || U_I((unsigned)&sv_PanelFlag))
        goto end;
    st = S;
    switch (U_I(st) - 1) {
    case 0:                     /* state 1, 0x57595a: show the main-intro text and start the step chain */
        if (d3t_CamMove != 0)
            break;
        if (ai_Unk5778b0() != 0)
            return -4;
        U_I(S + 0x38) = 0;
        U_I(S + 0x3c) = 0;
        U_I(S) = 2;
        st = S;
        per = *(char **)(st + 0x18);
        if (*(int *)(per + 8) == 1 || *(int *)(per + 8) == 2) {
            U_I(S + 4) = 4;
            U_I(S + 0x2c) = 0;
        } else {
            U_I(S + 4) = 0xb;
            U_I(S + 0x2c) = 1;
        }
        if (*(int *)(per + 8) == 2)
            U_I(S + 0x3c) = 1;
        if (*(int *)(per + 0x38))
            ai_Unk576040(2, 0, 0, 0);
        U_I(S + 0x50) = 0;
        U_I(S + 0x58) = *(int *)(per + 0x64) != 0;
        U_I(S + 0x54) = 0;
        U_I(S + 0x5c) = 0;
        U_I(S + 0x60) = -1;
        if (*(int *)(per + 0x34))
            ((void (*)(int))*(int *)(per + 0x34))(0);
        he_Unk53dcb0(*(int **)(*(char **)(S + 0x1c) + 0x74));
        d2_SetPointerSprite(0, 0);
        break;
    case 1:                     /* state 2, 0x575a69: the intro is answered, close the window and continue */
        if (ai_Unk577a00() != 1)
            break;
        U_I(S) = U_I(S + 4);
        U_I(S + 8) = U_I((unsigned)&ai_Unk6333cc);
        U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
        U_I(S + 0x10) = 0;
        he_Unk53dcb0(0);
        break;
    case 2:                     /* state 3, 0x575ab5: the same without the clocks */
        if (ai_Unk577a00() != 1)
            break;
        U_I(S) = U_I(S + 4);
        he_Unk53dcb0(0);
        break;
    case 3:                     /* state 4, 0x575adc: the timed first text window */
        st = S;
        per = *(char **)(st + 0x18);
        if (*(int *)(per + 0x40) == 3) {
            ai_Unk576040(0, 0, 0, 0);
            break;
        }
        if (U_I((unsigned)&ai_Unk6333cc) > U_I(st + 8) + *(int *)(per + 0x2c)) {
            U_I(st + 8) = U_I((unsigned)&ai_Unk6333cc);
            if (U_I(st + 0x28) == 0 && *(int *)(per + 0x14) != 0) {
                U_I(st + 0x28) = 1;
                ai_Unk577a90();
            }
        } else if (U_I((unsigned)&ai_Unk6333cc) > U_I(st + 8) + 0x238) {
            if (U_I(st + 0x28) != 1)
                break;
            U_I(st + 8) = U_I((unsigned)&ai_Unk6333cc);
            U_I(st + 0x28) = 0;
            ai_Unk577b60();
        }
        if (U_I(st + 0x28) == 1)
            d2_RaiseViewWindows(U_I(st + 0x24));
        break;
    case 4:                     /* state 5, 0x575b82: drop the intro window and open the first step */
        U_I(S + 8) = U_I((unsigned)&ai_Unk6333cc);
        U_I(S + 0x28) = 0;
        ai_Unk577b60();
        U_I(S + 0x3c) = 2;
        st = S;
        per = *(char **)(st + 0x18);
        if (*(int *)(per + 0x50) == 0) {
            U_I(S) = 7;
            U_I(S + 4) = 7;
            break;
        }
        U_I(S + 0x40) = 0;
        U_I(S) = 6;
        U_I(S + 4) = 6;
        ai_Unk577e50();
        break;
    case 5:                     /* state 6, 0x575bdc: one step of the chapter chain */
        st = S;
        per = *(char **)(st + 0x18);
        U_I((unsigned)&tb_Paused) = U_I((unsigned)&ai_Unk6333cc) >= *(int *)(*(int *)(per + 0x58) + U_I(st + 0x40) * 0x10) + U_I(st + 0xc);
        d2_SetObjEnabled(U_I(st + 0x4c), U_I((unsigned)&tb_Paused));
        if (ai_Unk5780e0() != 1)
            break;
        U_I(S + 0x40)++;
        U_I(S + 8) = U_I((unsigned)&ai_Unk6333cc);
        st = S;
        per = *(char **)(st + 0x18);
        if (U_I(st + 0x40) < *(int *)(per + 0x50)) {
            U_I(st + 0xc) = U_I((unsigned)&ai_Unk6333cc);
            ai_Unk577e50();
            break;
        }
        U_I(S) = 7;
        U_I(S + 4) = 7;
        break;
    case 6:                     /* state 7, 0x575c74: the chapter is done, pick the next state */
        U_I(S + 0x3c) = 3;
        U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
        U_I(S + 0x50) = 0;
        ai_Unk577e50();
        U_I((unsigned)&tb_Paused) = 0;
        st = S;
        per = *(char **)(st + 0x18);
        if (*(int *)(per + 0x4c) == 0) {
            U_I(S) = 9;
            U_I(S + 4) = 9;
        } else {
            U_I(S) = 0xa;
            U_I(S + 4) = 0xa;
            U_I(S + 0x2c) = *(int *)(per + 0x24) == 0;
        }
        break;
    case 7:                     /* state 8, 0x575fe2: nothing */
        break;
    case 8:                     /* state 9, 0x575cec: the second text window and the run update */
        if (U_I(S + 0x28) == 1) {
            U_I(S + 8) = U_I((unsigned)&ai_Unk6333cc);
            U_I(S + 0x28) = 0;
            ai_Unk577b60();
        }
        st = S;
        per = *(char **)(st + 0x18);
        if (U_I(st + 0x30) == 0 && *(int *)(per + 0x1c) != 0) {
            U_I(st + 0x30) = 1;
            ai_Unk577b90();
            ai_Unk57b8a0(0);
        }
        if (U_I(S + 0x30) == 1)
            d2_RaiseViewWindows(U_I(S + 0x34));
        ai_Unk57b8a0(1);
        break;
    case 9:                     /* state 10, 0x575d57: the chapter's timed tail */
        if (U_I(S + 0x28) == 1) {
            U_I(S + 8) = U_I((unsigned)&ai_Unk6333cc);
            U_I(S + 0x28) = 0;
            ai_Unk577b60();
        }
        st = S;
        per = *(char **)(st + 0x18);
        if (U_I((unsigned)&ai_Unk6333cc) > U_I(st + 0xc) + *(int *)(per + 0x44)) {
            sv_CameraFollow();
            U_I((unsigned)&sv_PopRequest) = U_I((unsigned)&sv_FrameDepth) - U_I((unsigned)&sv_FrameBase);
            U_I(S) = 1;
            U_I(S + 4) = 1;
            U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
        }
        st = S;
        if (U_I(st + 0x2c) != 0)
            break;
        per = *(char **)(st + 0x18);
        if (*(int *)(per + 0x24) == 0)
            break;
        sv_SetStatusText(txt_Ptrs[*(int *)(per + 0x24)]);
        if (*(int *)(per + 0x28) != 0) {
            if (U_I(S + 0x14)) {
                if (sb_IsPlaying((struct SBSAMPLE *)U_I(S + 0x14)))
                    sb_Stop((struct SBSAMPLE *)U_I(S + 0x14), 0);
                U_I(S + 0x14) = 0;
            }
            U_I(S + 0x14) = snd_PlayVoice(0xfffffff9, 0, (char *)U_I(per + 0x28), -1);
        }
        U_I(S + 0x2c) = 1;
        ai_Unk5778b0();
        break;
    case 10:                    /* state 11, 0x575e68: the same with the second window's timing */
        if (U_I(S + 0x28) == 1) {
            U_I(S + 8) = U_I((unsigned)&ai_Unk6333cc);
            U_I(S + 0x28) = 0;
            ai_Unk577b60();
        }
        st = S;
        per = *(char **)(st + 0x18);
        if (U_I((unsigned)&ai_Unk6333cc) > U_I(st + 0xc) + *(int *)(per + 0x30)) {
            sv_CameraFollow();
            U_I((unsigned)&sv_PopRequest) = U_I((unsigned)&sv_FrameDepth) - U_I((unsigned)&sv_FrameBase);
            U_I(S) = 0xc;
            U_I(S + 4) = 0xc;
            U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
        }
        st = S;
        if (U_I(st + 0x2c) != 0)
            break;
        per = *(char **)(st + 0x18);
        if (*(int *)(per + 0x24) == 0)
            break;
        sv_SetStatusText(txt_Ptrs[*(int *)(per + 0x24)]);
        if (*(int *)(per + 0x28) != 0) {
            if (U_I(S + 0x14)) {
                if (sb_IsPlaying((struct SBSAMPLE *)U_I(S + 0x14)))
                    sb_Stop((struct SBSAMPLE *)U_I(S + 0x14), 0);
                U_I(S + 0x14) = 0;
            }
            U_I(S + 0x14) = snd_PlayVoice(0xfffffff9, 0, (char *)U_I(per + 0x28), -1);
        }
        U_I(S + 0x2c) = 1;
        ai_Unk5778b0();
        break;
    case 11:                    /* state 12, 0x575f80: next chapter record */
        if (U_I(S + 0x28) == 1) {
            U_I(S + 8) = U_I((unsigned)&ai_Unk6333cc);
            U_I(S + 0x28) = 0;
            ai_Unk577b60();
        }
        st = S;
        per = *(char **)(st + 0x18);
        if (*(int *)(per + 0x34))
            ((void (*)(int))*(int *)(per + 0x34))(1);
        st = S;
        per = *(char **)(st + 0x18);
        *(char **)(st + 0x18) = *(char **)(per + 0x70);
        U_I(S) = 1;
        U_I(S + 4) = 1;
        sv_SetStatusText((char *)(unsigned)&ch_EmptyName);
        break;
    }
end:
    return 0;
}

/* 0x410ac0: button state machine (press timing, hold, double click) and event queue @deep behaviour draft, one code hunk left (7 differing instructions): the queue scan's tail. Ours sinks i=31 into the loop preheader, so the guard load dm_Q[31].valid takes eax (mov eax,ds:X; test eax,eax; jne) and the constant follows the branch; the original materialises i (mov eax,0x1f) between the load and its test, so the load takes ecx (mov ecx,ds:X; mov eax,0x1f; test ecx,ecx; jne). The 1-byte fill nop is an alignment artifact of the 1-byte-shorter body. Meaning is right (hunks read against the disassembly 410e58..410ec2). spell.py: no menu rule for either hunk. Last-bytes leads tried, each one edit + one build, all reverted as WORSE: (a) for-init form 'for (i = 31, q = &dm_Q[31].valid; *q == 0;)' with the && dm_Q[31].valid == 0 guard dropped -> 458 bytes / 180 asm lines; (b) comma form '(i = 31, q = &dm_Q[31].valid, *q == 0)' inside the guard -> identical 458 / 180; (c) 'changed && (i = 31, dm_Q[31].valid == 0)' with q initialised in the body -> 458 / 176, the same collapse: every form that materialises i before the guard test merges the loop head. The cost is the register-priority shift the note measured with c2prio (2026-10-23): the const#0 web lifts from -58 to -55 past _di_PressX (-57), the zero takes ebp and PressX loses its promotion -> allocation/scheduling work (match-deep). Readability done: Q4's 4 raw volatile-short casts replaced by the DIMOUSEST anonymous-union volatile view (dm_S.vx / dm_S.vy) with identical bytes; Q6 already described. */

void di_UpdateButtons(void)
{
    int i;
    int *q;      /* walks the valid flags of dm_Q downwards (stride 19 ints = one DIMOUSEST) */
    int now = tb_Calls;

    if (di_Disabled)
        return;
    if (di_LDownTime != 0
        && (now - di_LDownTime > 15 || dm_S.x < di_PressX - 3 || dm_S.x > di_PressX + 3
            || dm_S.y > di_PressY + 3 || dm_S.y < di_PressY - 3)) {
        dm_S.lhold = 1;
        di_LDownTime = 0;
        di_LClickTime = -1;
    }
    if (dm_S.lup == 0 || di_GuiClick == 0) {
        if (dm_S.ldown != 0 && di_LDownTime == 0) {
            di_LDownTime = now;
            di_PressX = dm_S.vx;
            di_PressY = dm_S.vy;
        }
        if (di_LClickTime != -1 && now - di_LClickTime > 40) {
            di_LDownTime = 0;
            di_LClickTime = -1;
        }
        if (dm_S.lup != 0) {
            if (di_LClickTime == -1 || now - di_LClickTime >= 40) {
                di_LDownTime = 0;
                di_LClickTime = now;
                if (dm_S.lhold != 0) {
                    dm_S.lup = 0;
                    di_LClickTime = -1;
                }
                dm_S.lhold = 0;
            } else {
                dm_S.lextra = 1;
                dm_S.lup = 0;
                di_LClickTime = -1;
                di_LDownTime = 0;
            }
        }
    } else {
        di_LDownTime = 0;
        di_GuiClick = 0;
        dm_S.ldown = 0;
        dm_S.lup = 0;
        di_LClickTime = -1;
        dm_S.lhold = 0;
    }
    if (dm_S.rdown != 0 && di_RDownTime == 0) {
        di_RDownTime = now;
        di_PressX = dm_S.vx;
        di_PressY = dm_S.vy;
    }
    if (di_RDownTime != 0) {
        if (dm_S.rhold == 0 && dm_S.rdown != 0) {
            if (now - di_RDownTime > 15) {
                di_RDownTime = 0;
                dm_S.rhold = 1;
            }
            if (dm_S.x < di_PressX - 3 || dm_S.x > di_PressX + 3 || dm_S.y > di_PressY + 3
                || dm_S.y < di_PressY - 3) {
                di_RDownTime = 0;
                dm_S.rhold = 1;
            }
        }
    }
    if (dm_S.rup != 0) {
        if (dm_S.rhold != 0) {
            dm_S.rup = 0;
            dm_S.rhold = 0;
            dm_S.ldown = 0;
            dm_S.lhold = 0;
        }
        di_RDownTime = 0;
    }
    if (dm_S.mdown != 0 && di_MDownTime == 0) {
        di_MDownTime = now;
        di_PressX = dm_S.x;
        di_PressY = dm_S.y;
    }
    if (di_MDownTime != 0) {
        if (dm_S.mhold == 0 && dm_S.mdown != 0) {
            if (now - di_MDownTime > 15) {
                dm_S.mhold = 1;
                di_MDownTime = 0;
            }
            if (dm_S.x < di_PressX - 3 || dm_S.x > di_PressX + 3 || dm_S.y > di_PressY + 3
                || dm_S.y < di_PressY - 3) {
                di_MDownTime = 0;
                dm_S.mhold = 1;
            }
        }
    }
    if (dm_S.mup != 0) {
        dm_S.mup = 0;
        if (dm_S.mhold != 0)
            dm_S.mhold = 0;
        di_MDownTime = 0;
    }
    if ((dm_S.lup != dm_P.lup || dm_S.ldown != dm_P.ldown || dm_S.lhold != dm_P.lhold
         || dm_S.lextra != dm_P.lextra || dm_S.rup != dm_P.rup || dm_S.rdown != dm_P.rdown
         || dm_S.rhold != dm_P.rhold || dm_S.mup != dm_P.mup || dm_S.mdown != dm_P.mdown
         || dm_S.mhold != dm_P.mhold || dm_S.wup != dm_P.wup || dm_S.wdown != dm_P.wdown)
        && dm_Q[31].valid == 0) {
        /* find the last used queue slot, append the state behind it */
        i = 31;
        q = &dm_Q[31].valid;
        do {
            q -= 19;
            i--;
            if ((int)q < (int)&dm_Q[0].valid) {   /* signed compare, needed for VC6's code */
                i = 0;
                goto put;
            }
        } while (*q == 0);
        if (i < 31) {
put:
            i++;
            dm_Q[i] = dm_S;
            dm_Q[i].valid = 1;
        }
    }
    dm_P = dm_S;
}

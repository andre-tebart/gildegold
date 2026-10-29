/* 0x53e300, the next record that matches the filter of 0x53e400 (type 0x625708, owner 0x62570c, player 0x625710, argument +0x10 0x625714) after the cursor 0x7585e0; 0 at the end */

char *he_NextEvent(void)
{
    int ok = 0;
    char *last = 0;
    char *cur;

    if (U_I((unsigned)&ai_Unk6f261c) < 0 || U_I((unsigned)&ai_Unk6f261c) > 0x3ff)
        err_Message("he_fn_Handlung(): invalid Handlungen_LAST");
    else
        last = (char *)((unsigned)&he_Events + U_I((unsigned)&ai_Unk6f261c) * 0x14c);
    if (U_I(((unsigned)&he_QuickList + 0xaaa0)) == 0 || (char *)U_I(((unsigned)&he_QuickList + 0xaaa0)) == last || last < (char *)U_I(((unsigned)&he_QuickList + 0xaaa0)))
        return 0;
    if (*(char *)(unsigned)&full_data_6f2624 == 0)
        U_I(((unsigned)&he_QuickList + 0xaaa0)) += 0x14c;
    else
        *(char *)(unsigned)&full_data_6f2624 = 0;
    do {
        cur = (char *)U_I(((unsigned)&he_QuickList + 0xaaa0));
        if (*cur == 0) {
L_chk:
            if (ok)
                return (char *)U_I(((unsigned)&he_QuickList + 0xaaa0));
        } else {
            if (U_I((unsigned)&full_data_625708) == -1) {
L_a:
                if (U_I((unsigned)&full_data_62570c) != -1) {
                    if (*(int *)(cur + 4) != U_I((unsigned)&full_data_62570c))
                        goto L_no;
                    ok = 1;
                }
                if (U_I((unsigned)&full_data_625710) != -1) {
                    if (*(short *)(cur + 8) != U_I((unsigned)&full_data_625710))
                        goto L_no;
                    ok = 1;
                }
                if (U_I((unsigned)&full_data_625714) == -1)
                    goto L_chk;
                if (*(int *)(cur + 0x10) == U_I((unsigned)&full_data_625714))
                    return (char *)U_I(((unsigned)&he_QuickList + 0xaaa0));
            } else if (*cur == (char)U_I((unsigned)&full_data_625708)) {
                ok = 1;
                goto L_a;
            }
L_no:
            ok = 0;
        }
        if (last <= (char *)U_I(((unsigned)&he_QuickList + 0xaaa0))) {
            U_I(((unsigned)&he_QuickList + 0xaaa0)) = 0;
            return 0;
        }
        U_I(((unsigned)&he_QuickList + 0xaaa0)) += 0x14c;
    } while (1);
}

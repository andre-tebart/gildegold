/* 0x52bbc0, moves the selected characters (the table 0x770cec, stride 0x21c, the selected flag byte at +0x1c of the entry) into the group (object kind 'C') of the first one's object: the first selected character fixes
   the object (+0x10) and the owner (+0); the entries that belong to the current group 0x760458 are taken out (0x4ffe90 (+0x5d, 1, 0, 0x146) / 0x524260 / 0x4c5230 / 0x4c52e0), the others are collected (up to four ids)
   into a message ('C', +8 the player record, +0x36 = 2, +0x58 / +0x5c the two group handles, +0x70 the four ids) that 0x4c4b70 sends; the group slot list (+0xc4, 4 ints) loses the moved id; the selected flags are cleared
   (ROUGH) */

void sv_Unk52bbc0(void)
{
    char *obj, *o;
    int *rec, *out;
    int state, owner, cur, n, done, k, g, id;
    unsigned char m[0xf8];

    obj = 0;
    state = 0;
    owner = 0;
    n = 0;
    done = 0;
    cur = SV_I((unsigned)&full_data_760458);
    m[4] = 0;
    out = (int *)(m + 0x70);
    for (rec = (int *)(unsigned)&ai_Unk770cec; !done && (int)rec < ((unsigned)((unsigned)ai_Unk770cec + 0x65296) + 0x16a); rec += 0x87) {
        if ((char)rec[7] == 0)
            continue;
        id = rec[-0x5a];
        if (state == 0) {
            o = (char *)rec[4];
            if (o != 0 && rec[0] != 0) {
                owner = rec[0];
                state = 0x43;
                obj = o;
                if (*o == 'C') {
                    if (owner == cur) {
                        g = gm_Query(*(int *)(cur + 0x5d), 1, 0, 0x146);
                        for (k = 0; k < 4; k++) {
                            if (*(int *)(o + 0xc4 + k * 4) == id) {
                                *(int *)(o + 0xc4 + k * 4) = -1;
                                break;
                            }
                        }
                        sv_MoveToGroup(SV_I((unsigned)&full_data_760458), 0, (short)rec[-0x5b], 0);
                        ai_Unk4c5230(id);
                        ai_Unk4c52e0(id, *(int *)(SV_I((unsigned)&full_data_760458) + 1), *(int *)(g + 2), 0, 0, 0);
                        cur = SV_I((unsigned)&full_data_760458);
                    } else {
                        m[4] = 'C';
                        m[0x36] = 2;
                        *(int *)(m + 8) = SV_I((unsigned)&ai_CharInts + SV_U((unsigned)&ai_PlayerId) * 0x21c);
                        *(int *)(m + 0x58) = *(int *)(cur + 1);
                        *(int *)(m + 0x5c) = *(int *)(owner + 1);
                        *out++ = id;
                        n++;
                        for (k = n; k < 4; k++)
                            *out++ = -1;
                        out = (int *)(m + 0x70) + n;
                        for (k = 0; k < 4; k++) {
                            if (*(int *)(o + 0xc4 + k * 4) == id) {
                                *(int *)(o + 0xc4 + k * 4) = -1;
                                cur = SV_I((unsigned)&full_data_760458);
                                break;
                            }
                        }
                    }
                }
            }
        } else if (state == 0x43 && rec[0] == owner && (char *)rec[4] == obj) {
            if (rec[0] == cur) {
                g = gm_Query(*(int *)(cur + 0x5d), 1, 0, 0x146);
                for (k = 0; k < 4; k++) {
                    if (*(int *)(obj + 0xc4 + k * 4) == id) {
                        *(int *)(obj + 0xc4 + k * 4) = -1;
                        break;
                    }
                }
                sv_MoveToGroup(SV_I((unsigned)&full_data_760458), 0, (short)rec[-0x5b], 0);
                ai_Unk4c5230(id);
                ai_Unk4c52e0(id, *(int *)(SV_I((unsigned)&full_data_760458) + 1), *(int *)(g + 2), 0, 0, 0);
                cur = SV_I((unsigned)&full_data_760458);
            } else {
                *out++ = id;
                n++;
                for (k = 0; k < 4; k++) {
                    if (*(int *)(obj + 0xc4 + k * 4) == id) {
                        *(int *)(obj + 0xc4 + k * 4) = -1;
                        cur = SV_I((unsigned)&full_data_760458);
                        break;
                    }
                }
                if (n > 3)
                    done = 1;
            }
        }
        *(char *)(rec + 7) = 0;
    }
    if (obj != 0)
        cm_SendExSetHE(*(int *)(obj + 0x70), obj);
    if (m[4] != 0)
        cm_SendLongMessage(m);
}

/* 0x4fa700, the wage figures of the character id for the report: out[0] = the byte +0x166, out[1] = +0x169, out[2] = the kind 0x23 .. 0x27 by the highest of the flag bits 0x200 .. 0x2000 of the int +0x1cc, out[4] / [8] / [0xc] = the three amounts (0x4e6f80 of the kinds, 0 when the character is no active person); flag != 0 sends the amounts to the player (and for kind 3 a random extra, 0x4c5f60). Returns 1 when amounts were computed. ROUGH */

int gm_CharacterWages(unsigned short id, int flag, unsigned char *out)  /* the callers pass a 16-bit id and the original call site loads it without an extension (0x4f5a80: mov cx,[esi]) */
{
    char *c;
    unsigned f;
    int res;
    int tmp[3];
    int r[9];
    unsigned char b;

    res = 0;
    c = ai_Chars[id];
    out[0] = c[0x166];
    out[1] = c[0x169];
    f = *(unsigned *)(c + 0x1cc);
    if (f & 0x1000)
        b = 0x26;
    else if (f & 0x2000)
        b = 0x27;
    else if (f & 0x800)
        b = 0x25;
    else if (f & 0x400)
        b = 0x24;
    else
        b = (f & 0x200) ? 0x23 : 0;
    out[2] = b;
    *(int *)(out + 4) = 0;
    *(int *)(out + 8) = 0;
    *(int *)(out + 0xc) = 0;
    if (*(short *)c != -1 && (unsigned char)c[2] < 10 && c[8] != 0 && (c[0x166] != 0 || c[0x169] != 0 || (f & 0xe00) != 0)) {
        gv_GetTableRecord(0xf, r);
        *(int *)(out + 8) = (int)amt_GetKind(c[0x169], tmp);
        *(int *)(out + 4) = (int)amt_GetKind(c[0x166], tmp);
        *(int *)(out + 0xc) = (int)amt_GetKind(out[2], tmp);
        res = 1;
        if (flag != 0) {
            char *bl = (char *)gm_FirstBuilding(1);
            char *o;
            if (bl == 0)
                return 0;
            o = (char *)gm_Query(*(int *)(bl + 0x5d), 2, 6, 0, 0x115);
            if (o == 0)
                return 0;
            if (*(int *)(out + 4) != 0) {
                cm_SendMoveGoodsUnchecked(*(int *)(c + 4), -1, 0, *(int *)(out + 4));
                ai_Unk6f1b98++;
            }
            if (*(int *)(out + 8) != 0) {
                cm_SendMoveGoodsUnchecked(*(int *)(c + 4), -1, 0, *(int *)(out + 8));
                ai_Unk6f1b98++;
            }
            if (*(int *)(out + 0xc) != 0) {
                cm_SendMoveGoodsUnchecked(*(int *)(c + 4), -1, 0, *(int *)(out + 0xc));
                ai_Unk6f1b98++;
            }
            if (c[2] == 3) {
                unsigned short rr = util_RandomU((tmp[2] & 0xff) + 2);
                cm_SendMoveGoodsUnchecked(*(int *)(c + 4), -1, 0, (rr + (tmp[2] & 0xff)) * 0x1900);
                cm_SendMoneyDelta(*(int *)(c + 4), r[2] & 0xff);
                ai_Unk6f1b98 += 2;
            }
        }
    }
    return res;
}

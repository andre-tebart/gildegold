/* 0x4f30a0, the text of a record with the type byte (+0x1c) 2 .. 8: types 2 / 3 / 4 / 8 pick the text numbers 0x140a / 0x140f / 0x1414 / 0x1428 (+ 1 for mode < 2); anything else the fixed text; 1 when a text was built.
   +4 the int value (shown by address), +0x12 the character id (its short id or 0xfffe). */

#define R4(r, o) (*(int *)((r).b + (o)))

int unk_Unk4f30a0(char *out, UNKREC r, int mode)
{
    int m;
    unsigned short *p;
    unsigned short id;

    m = mode < 2;
    p = (unsigned short *)gm_FindChar(R4(r, 0x12));
    if (p == 0)
        id = 0xfffe;
    else
        id = *p;
    switch ((unsigned char)r.b[0x1c]) {
    case 2:
        break;
    case 3:
        txt_Format(out, m + 0x140f, r.b + 4);
        return 1;
    case 4:
        txt_Format(out, m + 0x1414, r.b + 4, id);
        return 1;
    default:
        strcpy(out, "Hm, das ist doch nicht verboten");
        return 0;
    case 8:
        txt_Format(out, m + 0x1428, r.b + 4);
        return 1;
    }
    if (R4(r, 4) < 0x75) {
        txt_Format(out, m + 0x140a, r.b + 4, R4(r, 0x1d) + 0x13f9);
        return 1;
    }
    txt_Format(out, m + 0x140a, r.b + 4, R4(r, 0x1d) + 0x13fb);
    return 1;
}

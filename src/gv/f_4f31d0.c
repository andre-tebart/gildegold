/* 0x4f31d0, the text of a record with the type byte 0xe / 0x10 (texts 0x1446 / 0x1450, + 1 for mode < 2); anything else the fixed text; 1 when a text was built. */

int unk_Unk4f31d0(char *out, UNKREC r, int mode)
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
    if ((unsigned char)r.b[0x1c] != 0xe) {
        if ((unsigned char)r.b[0x1c] != 0x10) {
            strcpy(out, "Hm, das ist doch nicht verboten");
            return 0;
        }
        txt_Format(out, m + 0x1450, id, r.b + 4);
        return 1;
    }
    txt_Format(out, m + 0x1446, r.b + 4, id);
    return 1;
}

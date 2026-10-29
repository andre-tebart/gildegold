/* 0x4f3270, the text of a record with the type byte 0x11 .. 0x1c (texts 0x1455 + 5 * n, + 1 for mode < 2; the object name (0x4fea50 of the id at +0x21, default "Geb\xe4ude") goes in as "%1G" when found); returns 0 always. ROUGH */

int unk_Unk4f3270(char *out, UNKREC r, int mode)
{
    char name[0x80] = "Geb\xe4ude";
    int m, obj, k;
    unsigned short *p;
    unsigned id;

    m = mode < 2;
    gm_FindById(&obj, 0, 0, R4(r, 0x21));
    if (obj != 0)
        txt_Format(name, "%1G", obj);
    p = (unsigned short *)gm_FindChar(R4(r, 0x12));
    if (p == 0)
        id = 0xfffe;
    else
        id = *p;
    switch ((unsigned char)r.b[0x1c]) {
    case 0x11:
        txt_Format(out, m + 0x1455, r.b + 4);
        return 0;
    case 0x12:
        txt_Format(out, m + 0x145a, r.b + 4);
        return 0;
    case 0x13:
        txt_Format(out, m + 0x145f, id, r.b + 4);
        return 0;
    case 0x14:
        txt_Format(out, m + 0x1464, r.b + 4, id);
        return 0;
    case 0x15:
        txt_Format(out, m + 0x1469, r.b + 4, id);
        return 0;
    case 0x16:
        txt_Format(out, m + 0x146e, id, r.b + 4);
        return 0;
    case 0x17:
        txt_Format(out, m + 0x1473, r.b + 4, id);
        return 0;
    case 0x18:
        k = ls_GetLanguage();
        if (k < 0)
            return 0;
        if (k < 2)
            txt_Format(out, m + 0x1478, r.b + 4, id, name);
        else if (k == 2)
            txt_Format(out, m + 0x1478, r.b + 4, name, id);
        return 0;
    case 0x19:
        txt_Format(out, m + 0x147d, r.b + 4, id);
        return 0;
    case 0x1a:
        txt_Format(out, m + 0x1482, r.b + 4, id);
        return 0;
    case 0x1b:
        txt_Format(out, m + 0x1487, r.b + 4);
        return 0;
    case 0x1c:
        txt_Format(out, m + 0x148c, r.b + 4, id);
        return 0;
    }
    return 0;
}

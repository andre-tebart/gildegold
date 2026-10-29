/* 0x410560, keyboard scan code of a character with the shift / ctrl / alt modifiers; the numpad scan codes map to the main row ('0'..'9', '-' 0x2d, '+' 0x2b), 0 when no key matches */

unsigned char kb_FindKey(int shift, int ctrl, int alt, char code)
{
    unsigned int v = code;
    int i;

    if (v == 0x52)
        return 0x30;
    if (v == 0x4f)
        return 0x31;
    if (v == 0x50)
        return 0x32;
    if (v == 0x51)
        return 0x33;
    if (v == 0x4b)
        return 0x34;
    if (v == 0x4c)
        return 0x35;
    if (v == 0x4d)
        return 0x36;
    if (v == 0x47)
        return 0x37;
    if (v == 0x48)
        return 0x38;
    if (v == 0x49)
        return 0x39;
    if (v == 0x4a)
        return 0x2d;
    if (v == 0x4e)
        return 0x2b;
    if (shift)
        v |= 0x100;
    if (ctrl)
        v |= 0x200;
    if (alt)
        v |= 0x400;
    for (i = 0; i < 256; i++) {
        if (kb_Map[i] == v)
            return i;
    }
    return 0;
}

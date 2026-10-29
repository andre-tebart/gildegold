/* 0x4f3580, picks the text builder by the type byte (+0x1c) of the record: below 9 0x4f30a0, below 0x11 0x4f31d0, below 0x1d 0x4f3270; 0 otherwise */

int unk_Unk4f3580(char *out, UNKREC r, int mode)
{
    if ((unsigned char)r.b[0x1c] < 9)
        return unk_Unk4f30a0(out, r, mode);
    if ((unsigned char)r.b[0x1c] < 0x11)
        return unk_Unk4f31d0(out, r, mode);
    if ((unsigned char)r.b[0x1c] < 0x1d)
        return unk_Unk4f3270(out, r, mode);
    return 0;
}

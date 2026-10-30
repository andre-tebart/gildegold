/* 0x4f1170, the message 0x4c6130 about the object o (a character +0x27, not the player's): graveyard 0x4f2e30 (0x1b, 1, id, -1, object id) first; fields v at +8, byte b at +0x10, x at +0x11, y at +0x15; -1 for no character,
   the player's own object or b -1 */

int unk_Unk4f1170(char *o, int v, int b, int x, int y)
{
    unsigned short ch;
    char m[0x19];

    ch = *(unsigned short *)(o + 0x27);
    if (ch != 0xffff && ch != *(unsigned short *)ai_PlayerCharC && b != -1) {
        *(int *)(m + 8) = *(int *)(o + 1);
        m[0x10] = (char)b;
        *(int *)(m + 0x11) = x;
        *(int *)(m + 0xc) = v;
        *(int *)(m + 0x15) = y;
        unk_Unk4f2e30(0x1b, 1, *(int *)(ai_Chars[ch] + 4), -1, *(int *)(m + 8));
        return ((int (*)(void *))ai_Unk4c6130)(m);
    }
    return -1;
}

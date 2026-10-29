/* 0x528290, a callback over the objects of a model (name o): for the one named "sp_WIMPEL" (the banner) sets its colour (0x446f30 (o, o +0x1cc, 0x66 + the byte +0x54 of the character record (0x770bd4) of the owner, 1))
   when the owner (+0x27) is valid; returns 1 */

char sv_Unk528290(char *o, char *b)
{
    if (_stricmp(o, "sp_WIMPEL") == 0) {
        if (*(unsigned short *)(b + 0x27) != 0xffff)
            d3if_SelectTextureSet(o, *(int *)(o + 0x1cc), (char)(*(char *)(ai_Chars[*(unsigned short *)(b + 0x27)] + 0x54) + 'f'), 1);
    }
    return 1;
}

/* 0x44e6f0, text-format line handler: stores the quoted first bitmap name of the current material
   (file extension stripped) and checks that bitmap */

void agi_Bitmap0(void *unused, AGICTX *c)
{
    char *s = AGI_TOK("\"");
    char *dot = strrchr(s, '.');

    if (dot)
        *dot = 0;
    strcpy(AGI_M(c).name[0], s);
    agi_CheckBitmap(AGI_M(c).name[0], 1);
}

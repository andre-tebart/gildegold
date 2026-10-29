/* 0x44e7d0, text-format line handler: stores the quoted third bitmap name of the current material
   (file extension stripped) and checks that bitmap */

void agi_Bitmap2(void *unused, AGICTX *c)
{
    char *s = AGI_TOK("\"");
    char *dot = strrchr(s, '.');

    if (dot)
        *dot = 0;
    strcpy(AGI_M(c).name[2], s);
    agi_CheckBitmap(AGI_M(c).name[2], 1);
}

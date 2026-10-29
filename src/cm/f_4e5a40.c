/* 0x4e5a40, the five bytes +0x80 .. +0x84 of a person as a tab separated line */

void ai_Unk4e5a40(char *c, char *buf)
{
    sprintf(buf, "%i\t%i\t%i\t%i\t%i\t", (unsigned char)c[0x80], (unsigned char)c[0x81], (unsigned char)c[0x82], (unsigned char)c[0x83], (unsigned char)c[0x84]);
}

/* 0x4e5980, the debug dump of the floats +0x10 .. +0x20 (the 6th "%5.2f" gets the int +0x24 as the original does), the word +0x28 and the bit fields of +0x2c */

void ai_Unk4e5980(char *c, char *buf)
{
    unsigned int v;

    sprintf(buf, "%5.2f\t %5.2f\t %5.2f\t %5.2f\t %5.2f\t %5.2f\t %i\t", *(float *)(c + 0x10), *(float *)(c + 0x14), *(float *)(c + 0x18), *(float *)(c + 0x1c),
            *(float *)(c + 0x20), *(int *)(c + 0x24), *(short *)(c + 0x28));
    v = *(unsigned int *)(c + 0x2c);
    sprintf(buf + strlen(buf), "%i\t %i\t %i\t %i\t %i\t %i\t %i\t %i\t %i\t", v & 0xf, (v >> 4) & 0xf, (v >> 8) & 0xf, (v >> 0xc) & 3, (v >> 0xe) & 7, (v >> 0x11) & 7,
            (v >> 0x14) & 7, (v >> 0x17) & 3, (v >> 0x19) & 0xf);
}

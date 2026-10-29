/* 0x4fda10, moves the character c into the building b (+0x170) when b (+0x65 its occupancy) has room (0x4f8890 capacity): the old building loses one */

void unk_Unk4fda10(char *c, char *b)
{
    unsigned short cap[4];
    char *old;

    ord_GetKindRecord(*(unsigned char *)b, (int *)cap);
    if ((unsigned char)b[0x65] < cap[0]) {
        old = *(char **)(c + 0x170);
        if (old != 0 && (unsigned char)old[0x65] > 0)
            old[0x65]--;
        *(char **)(c + 0x170) = b;
        b[0x65]++;
    }
}

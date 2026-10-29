/* 0x5012b0, the first free record of the building table (0x100 records of 0xa9 bytes), 0 when it is full */

char *gm_FindFreeBuilding(void)
{
    char *end, *p;

    end = ai_Blds + 0xa900;
    p = ai_Blds;
    if (p < end) {
        do {
            if (*p == 0)
                return p;
            p += 0xa9;
        } while (p < end);
    }
    return 0;
}

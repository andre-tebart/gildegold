/* 0x4eaf50, the research time of the recipe g for the character c, in tenths of minutes (0x3840 = the 1440 minutes of a day times 10): one to two days by the
   recipe's ATTR0 (the float at +4, 0 .. 1: 14400 * (2 - ATTR0)), reduced by up to half by the share 0 (the alchemist's skill) */

int ahm_ResearchTime(char *c, AHMGRID *g)
{
    int a, b;

    a = (int)((1.0f - g->rcp.a) * -14400.0f);
    b = 0x3840 - a;
    return b - (int)(ahm_Share(c, 0) * b * 0.5f);
}

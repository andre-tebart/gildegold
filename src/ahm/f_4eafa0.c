/* 0x4eafa0, the research time in minutes (ahm_ResearchTime / 10): the amount the research event 0x95 (0x56d2d0) counts down by the elapsed minutes (0x4fe280) */

int ahm_ResearchMinutes(char *c, AHMGRID *g)
{
    return (int)(ahm_ResearchTime(c, g) * 0.1f);
}

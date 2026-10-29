/* 0x522ec0, remembers the research list of the building o for the panel (sv_ResearchClass): the category 3 (0x501d80,
   the research buildings) sets it to 1, the category 5 takes the class number of the building kind (gm_BuildingClass) */

void sv_SetResearchClass(unsigned char *o)
{
    int k;
    int v;

    k = gm_BuildingClass(*o);
    if (k == 3) {
        sv_ResearchClass = 1;
        return;
    }
    if (k == 5) {
        if (gm_ResearchClass(*o, &v) != 0)
            sv_ResearchClass = v;
    }
}

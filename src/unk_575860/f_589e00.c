/* 0x589e00, fills slot i of the table 0x6f5ec0 from the building b (79 B): the building's byte +0 is its kind, its
   id at +1 -> the slot's +4; the main object of the building (0x501b50, else 0x501ce0; none for the kind 0x1d)
   gives its id at +2 -> the slot's +8, -1 without an object */

int gm_BuildingMainObject(unsigned char *b); void gm_GetGebaeudeHauptraum(unsigned char *b);

void ai_Unk589e00(int i, char *b)
{
    char *o;
    int off;

    o = 0;
    if (*(unsigned char *)b != 0x1d) {
        o = (char *)gm_BuildingMainObject(b);
        if (o == 0)
            o = (char *)((void *(*)(unsigned char *))gm_GetGebaeudeHauptraum)(b);
    }
    off = i * 0xc;
    *(int *)((unsigned)&full_data_6f5ec4 + off) = *(int *)(b + 1);
    if (o != 0)
        *(int *)((unsigned)&full_data_6f5ec8 + off) = *(int *)(o + 2);
    else
        *(int *)((unsigned)&full_data_6f5ec8 + off) = -1;
}

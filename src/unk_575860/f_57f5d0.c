/* 0x57f5d0, copies three vectors of the cannon object (55 B): 0x458810 (sc + 0x8c, 0x5ebb74, sc + 0x24),
   (0x5ebb94, sc + 0x44) and (0x5ebb84, sc + 0x34) */

void d3_DirToWorld();
int gm_FindById();

void ai_Unk57f5d0(char *sc)
{
    d3_DirToWorld(*(int *)(sc + 0x8c), (char *)(unsigned)&ai_Unk5ebb74, sc + 0x24);
    d3_DirToWorld(*(int *)(sc + 0x8c), (char *)(unsigned)&d3_YAxis, sc + 0x44);
    d3_DirToWorld(*(int *)(sc + 0x8c), (char *)((unsigned)&ai_Unk5ebb74 + 0x10), sc + 0x34);
}

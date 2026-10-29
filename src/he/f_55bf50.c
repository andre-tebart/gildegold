/* 0x55bf50, a Ghidra miss: the allocation handler of the event 0x21 (ROUGH): the object 0x502020 (+0xac); the first run is random(10) + 10 units ahead; 0x4c4a60 (object + 1, 0x5a, 2, 0x200, 0) */

float gm_Liking();
void cm_SendOwnership(int a, int b, int c);
void ai_Unk4c59b0(int a, unsigned char b);
int ai_Unk5b92e0();

void he_Unk55bf50(int ev)
{
    int o;

    o = gm_FindBuildingById(*(int *)(ev + 0xac));
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, (unsigned short)((int (*)(int, int))util_Random)(10, 0) + 10);
    cm_SendSetField(*(int *)(o + 1), 0x5a, 2, 0x200, 0);
}

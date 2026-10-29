/* 0x54d790, a Ghidra miss: the init of the level-up event: the due clock = the start clock (+0x44), +0xb4 = random(6) */

unsigned short hmt_RecordCount(void);
unsigned short hmt_RandomRecord(short k);
unsigned short hmt_RandomTextOffset(unsigned short k);
struct G11REC *hmt_GetRecord(unsigned short k);
void d2_CentreViewWindows(int view);

void he_Unk54d790(int ev)
{
    *(HETIME *)(ev + 0x52) = *(HETIME *)(ev + 0x44);
    *(int *)(ev + 0xb4) = (unsigned short)util_Random(6);
}

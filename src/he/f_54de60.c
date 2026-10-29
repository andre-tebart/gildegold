/* 0x54de60, a Ghidra miss: the init of the brawl event: the due clock now, +0xac = +0xb0 = 0, posted (state 0) */

float unk_Unk4f9e00(int *o);
int ord_GetKindRecord();
void cm_SendBoss(int a, int b);
float gm_AddHealth(unsigned id, int d);

void he_Unk54de60(int ev)
{
    if ((*(unsigned char *)(ev + 0x78) & 4) == 0) {
        *(HETIME *)(ev + 0x52) = ai_Time;
        *(int *)(ev + 0xac) = 0;
        *(int *)(ev + 0xb0) = 0;
        *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    }
}

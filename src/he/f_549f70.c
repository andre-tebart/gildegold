/* 0x549f70, a Ghidra miss: the start of the plague event ("Pest"): unless a running event of the type 0x59 exists the clock is copied from the start (+0x44), the state 5 is set and the event is posted (0x4c4c60 (5, ev)); else it is posted as failed */

int sv_Unk524770();
void cm_SendKillCharacter(int a, int b);
int ai_Unk5a9240();
float gm_Unk505440();
float util_RandomF2();

void he_Unk549f70(int ev)
{
    int e;

    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    e = (int)he_FindEvent(1, 0, 0x59);
    while (e != 0) {
        if (e != ev) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        e = (int)he_NextEvent();
    }
    *(int *)(ev + 0x70) = 5;
    *(HETIME *)(ev + 0x52) = *(HETIME *)(ev + 0x44);
    *(int *)(ev + 0x84) = -1;
    cm_SendExSetHE(5, ev);
}

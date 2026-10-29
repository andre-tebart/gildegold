/* 0x546100, a Ghidra miss (disassembled): the start handler of the event of a figure pair (+0xac): when the target (0x501190 kind 1) is gone the event ends, else it is posted again at once (ROUGH) */

int unk_Unk4f2e30();
int unk_Unk4f4080();
void gm_Shuffled(unsigned char n, int *out);
void ai_Unk4c4f10(int a, int b, int c);
int ai_Unk4b44e0();
void cm_SendMoneyDelta(int a, int b);

void he_Unk546100(int ev)
{
    if ((*(unsigned char *)(ev + 0x78) & 2) != 0) {
        if (gm_BuildQuery(1, 1, *(int *)(ev + 0xac)) == 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 0, 1);
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 0, 1);
        *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    }
}

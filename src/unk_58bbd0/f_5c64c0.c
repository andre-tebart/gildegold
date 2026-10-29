/* 0x5c64c0, the candidate list of a type 9 search (100 B): 0x4a5ef0 (param_1, 9, 0xc0540a - the +9
   byte, 0.0f, 100.0f, ids); the ids become the character dwords 0x770b84 + id * 0x87 into param_3 */

struct AIPER;
int gm_Unk505e60(unsigned id, int again); int ord_DispatchOrder();
unsigned int ai_Unk4a5ef0(struct AIPER *p, unsigned int n, unsigned int fl, float lo, float hi, unsigned short *out);

void ai_Unk5c64c0(int param_1, int *param_2, int param_3)
{
    unsigned short ids[10];
    int i;
    int n;

    n = ai_Unk4a5ef0((struct AIPER *)param_1, 9, ((unsigned)&ai_Unka69140 + 0x19c2ca) - (*(char *)(param_1 + 9) != 0), 0.0f, 100.0f, ids);
    *param_2 = n;
    for (i = 0; i < *param_2; i++)
        *(int *)(param_3 + i * 4) = U_I((unsigned)&ai_CharInts + ids[i] * 0x87);
}

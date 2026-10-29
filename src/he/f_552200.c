/* 0x552200, the allocation handler of the 0x552270 event: state 0, due now, minute word 9 + random 0x42ec00(3), +0xb0 = 0 (no answer yet), +0x58 = random 0x42ec00(4) * 15 */

int cm_SendLongMessage(char *msg);
float gm_GetSkill(char *c, int skill);
int gm_GetWorth(unsigned short id);
float gm_CharacterHealth();
float gm_AddHealth(unsigned id, int d);
void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d);

void he_Unk552200(int ev)
{
    int r;

    *(int *)(ev + 0x70) = 0;
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(unsigned short *)(ev + 0x56) = (unsigned short)(util_Random(3) + 9);
    r = (unsigned short)util_Random(4);
    *(int *)(ev + 0xb0) = 0;
    *(int *)(ev + 0x58) = r * 15;
}

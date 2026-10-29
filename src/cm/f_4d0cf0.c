/* 0x4d0cf0, the reward of the quest rec (+0x80 .. +0xbc the 16 ids already given, +0x110 the kind a): per kind one or
   two of the slots ai_KindBChars[] are taken, their characters get the property 0x156 (kind 1; 0x158 / 0x160 for 2 / 3)
   and the field block 0x83 + id is sent with the value 0xa8 / 0xd2. Rough draft. */

#define G19_GRANT(K, B)                                                                                 \
    i = ai_Unk4d0cd0((int *)(rec + 0x80));                                                              \
    *(int *)(rec + 0x80 + i * 4) = *(int *)(ai_KindBChars[i] + 4);                                       \
    ai_Unk4cdb20(ai_KindBChars[i]);                                                                      \
    cm_SendExSellObject(*(int *)(ai_KindBChars[i] + 4), -1, K, 1, ai_OwnPlayer, 0);                             \
    gm_AddHealth(*(unsigned short *)ai_KindBChars[i], -*(int *)(ai_KindBChars[i] + 0x24));                \
    buf[0] = B;                                                                                         \
    cm_BeginFieldBlock(ai_KindBChars[i], *(int *)(ai_KindBChars[i] + 4));                                       \
    cm_AddFieldDelta(1, 1, (unsigned short)(*(unsigned short *)&ai_KindBChars[i] - (unsigned short)cm_FieldObject + 0x83), buf)

void ai_Unk4d0cf0(char *rec, unsigned char a)
{
    unsigned char buf[0x10];
    int i;

    if (a != 0)
        *(int *)(rec + 0x110) = a;
    if (a == 1) {
        G19_GRANT(0x156, 0xa8);
        cm_SendFieldDeltas();
        G19_GRANT(0x156, 0xa8);
        cm_SendFieldDeltas();
    } else if (a == 2 || a == 3) {
        G19_GRANT(0x158, 0xd2);
        cm_SendFieldDeltas();
        G19_GRANT(0x160, 0xd2);
        cm_SendFieldDeltas();
    }
}

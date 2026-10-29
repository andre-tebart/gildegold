/* 0x504ec0, the skill value (0 .. 1) of the character c for the skill number 0 .. 4 (-1 when there is none): the base byte (+0x80 + skill) / 252, plus the bonuses of the character's items (the group (2, 2) records 0x53e400: float +0xb0 when the first skill byte (+0xac) is the skill, +0xb4 for the second (+0xad); halved when 0x5ba6b0 (c, 0x2c4) is 1), plus 0x5ba6b0 (c, 0x15a) / 6 for the skill 3, minus a penalty from the bit fields of the word +0x2c (a jump table over the skills: fields at bits 4 / 8 / 12 / 17 / 20 / 25 times 0.0333 / 0.0555 / 0.1667), plus 0.5 when the skill's flag (+0x1cd: bit 2 / 3 / 4 for the skills 2 / 3 / 4) is set; clamped to 0 .. 1 and scaled by 0.5 (0.75) when the character is no ruler (0x56f3b0) and 0x5ba6b0 (c, 0x2c4) is 0 (else). ROUGH */

int ai_Unk617850[] = { 0, 0, 1, 1, 1109393408, 16, 2, 1097859072, 16, 3, 1101004800, 16, 4, 1092616192, 4, 5, 0, 8, 6, 1090519040, 8, 7, 1084227584, 8, 8, 1077936128, 4, 9, 1094713344, 16 };  /* 0x617850, records of 3 ints ( */
int amt_GetKind(int k, struct AMTKIND *out);                     /* 0x4e6f80 */
float gm_AddHealth(unsigned id, int d);                   /* 0x4fca40 */
void gm_ClockToDate(struct GCLOCK *c, unsigned char *out);                     /* 0x4fe2c0 */

int ai_InventoryHas(void *ch, int id);                     /* 0x5ba6b0 */
int he_Unk56f3b0();                     /* 0x56f3b0 */

float gm_GetSkill(char *c, int skill)
{
    float s, acc, r;
    unsigned f;
    char *it;
    int v;

    r = 0.0f;
    if (c == 0 || skill >= 5)
        return -1.0f;
    s = (unsigned char)c[0x80 + skill] * 0.003968254f;
    if (skill != 1) {
        acc = 0.0f;
        it = (char *)he_FindEvent(2, 2, *(unsigned short *)c, 0, 0x82);
        while (it != 0) {
            if ((unsigned char)it[0xac] == skill)
                acc = acc + *(float *)(it + 0xb0);
            else if ((unsigned char)it[0xad] == skill)
                acc = acc + *(float *)(it + 0xb4);
            it = (char *)he_NextEvent();
        }
        v = ai_InventoryHas(c, 0x2c4);
        if (v == 1)
            acc = acc * 0.5f;
        s = acc + s;
    }
    if (skill == 3) {
        v = ai_InventoryHas(c, 0x15a);
        s = v * 0.1666667f + s;
    } else if (skill > 4)
        goto after;
    f = *(unsigned *)(c + 0x2c);
    switch (skill) {
    case 0:
        r = ((f >> 20) & 7) * 0.0555f + ((f >> 4) & 0xf) * 0.03333333432674408;
        break;
    case 1:
        r = ((f >> 8) & 0xf) * 0.03333333432674408 + ((f >> 20) & 7) * 0.0555f;
        break;
    case 2:
        r = ((f >> 17) & 7) * 0.055500001654028894 + ((f >> 25) & 0xf) * 0.03333333432674408;
        break;
    case 3:
        r = ((f >> 8) & 0xf) * 0.055500001654028894 + ((f >> 25) & 0xf) * 0.03333333432674408;
        break;
    case 4:
        r = ((f >> 12) & 3) * 0.1666667f + ((f >> 4) & 0xf) * 0.03333333432674408;
        break;
    }
after:
    s = s - r;
    if (skill == 4 ? (*(unsigned *)(c + 0x1cc) & 0x400) != 0 : skill == 3 ? (*(unsigned *)(c + 0x1cc) & 0x200) != 0 : skill == 2 ? (*(unsigned *)(c + 0x1cc) & 0x800) != 0 : 0)
        s = s + 0.5f;
    if (1.0f > s) {
        if (s <= 0.0f)
            s = 0.0f;
    } else
        s = 1.0f;
    if (he_Unk56f3b0(c) == 0) {
        v = ai_InventoryHas(c, 0x2c4);
        if (v == 0)
            return s * 0.5f;
        return s * 0.75f;
    }
    return s;
}

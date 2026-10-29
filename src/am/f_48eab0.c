/* 0x48eab0, execute dispatcher of 0x48e470: by the way to pay in s1->unk10 */

int ai_Unk48eab0(AIPX *p, AIARGX *s1, AIARGX *s2)
{
    switch (s1->unk10) {
    case 0:
        return ai_Unk48eb50(p, s1, s2) ? 0x41 : 0;
    case 1:
        return ai_Unk48ec90(p, s1, s2) ? 0x41 : 0;
    case 2:
        return ai_Unk48edc0(p, s1, s2) ? 0x41 : 0;
    case 3:
        return ai_Unk48ef10(p, s1, s2) ? 0x41 : 0;
    }
    return 0;
}

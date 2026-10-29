/* 0x4945c0, the rank (0..7) of the score: 4..5 -> 1, 6..7 -> 2 .. 14..16 -> 6, more -> 7; one less
   for a character with flag 0x22e */

int ai_Unk4945c0(AIPLAN *q)
{
    int n, r;
    float fl;
    AIC4 *c;

    n = (int)(q->unk04 - 1.0f);
    fl = (float)(q->unk04 - floor(q->unk04));
    c = AI_C4(q->c18);
    if (fl >= 0.5f)
        n++;
    switch (n < 0 ? 0 : n) {
    case 0:
    case 1:
    case 2:
    case 3:
        return 0;
    case 4:
    case 5:
        r = 1;
        break;
    case 6:
    case 7:
        r = 2;
        break;
    case 8:
    case 9:
        r = 3;
        break;
    case 10:
    case 11:
        r = 4;
        break;
    case 12:
    case 13:
        r = 5;
        break;
    case 14:
    case 15:
    case 16:
        r = 6;
        break;
    default:
        r = 7;
    }
    if (ai_InventoryHas(c, 0x22e))
        r--;
    return r;
}

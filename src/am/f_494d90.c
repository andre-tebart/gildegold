/* 0x494d90, whether a character takes a risk: closeness plus a roll plus a bonus from two flags
   (0xd: -0.25, 0xa: +0.1), the flag that applied goes to *out */

int ai_Unk494d90(AIC4 *a, AIC4 *b, int *out)
{
    float sum, f;
    int k;
    int q[0x88];

    sum = gm_Mix(a, b, 3) + util_RandomF();
    f = 0.0f;
    k = 0x3b;
    memset(q, 0, sizeof(q));
    q[0] = 1;
    if (ord_DispatchOrder(0xd, b, q) & 2) {
        f = -0.25f;
        k = 0xd;
    } else if (ord_DispatchOrder(10, b, q) & 2) {
        f = 0.1f;
        k = 10;
    }
    if (out)
        *out = k;
    if (sum + f > 1.0f)
        return 1;
    return 0;
}

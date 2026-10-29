/* 0x499520, decide: action 0x16 when 0x4f81c0 rates kind 2 so */

int ai_Unk499520(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    int buf[0x88];

    if (a)
        return 0;
    memset(buf, 0, sizeof(buf));
    buf[0] = 1;
    return ord_DispatchOrder(2, p, buf) == 2 ? 0x16 : 0;
}

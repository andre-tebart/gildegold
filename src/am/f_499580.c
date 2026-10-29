/* 0x499580, execute of 0x499520 */

int ai_Unk499580(AIPER *p, AIARG *s1, AIARG *s2)
{
    int buf[0x88];

    memset(buf, 0, sizeof(buf));
    buf[0] = 1;
    return ord_DispatchOrder(2, p, buf) == 2 ? 0x16 : 0;
}

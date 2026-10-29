/* 0x4f81c0, the order dispatcher: the handler number id (below 0x3a, else 0x40) of the table 0x611768 (0x4f4e20 .. 0x4f8140, one per order) runs on the character c (0x40 without one, 1 for a character whose kind +2 is 10 or more) and the message m */

int ord_DispatchOrder(int id, char *c, int *m)
{
    if (id >= 0x3a)
        return 0x40;
    if (c == 0)
        return 0x40;
    if ((unsigned char)c[2] >= 10)
        return 1;
    return ord_OrderHandlers[id](c, m);
}

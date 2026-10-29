/* 0x472f50, moves an object up to the end of its run in the draw order list */

void d2_RaiseObject(int obj)
{
    int i, first;

    first = -1;
    for (i = 0x2ff; i > 0; i--) {
        if (d2_DrawOrder[i] == &d2_Objs[obj]) {
            if (first != -1) {
                d2_DrawOrder[first] = d2_DrawOrder[i];
                memmove(&d2_DrawOrder[i], &d2_DrawOrder[i + 1], 0xbf8 - i * 4);
            }
            return;
        }
        if (d2_DrawOrder[i] != 0 && first == -1)
            first = i + 1;
    }
}

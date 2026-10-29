/* 0x5a7bc0, the map scroll by keys / drag (272 B): the scroll of the screen record (+0x258 / +0x248) follows the
   drag delta (0x6310b0 / 0x6310b4 against 0x1467380 / 0x1467382, never past the bounds 0x5f3238 / 0x5f323c) and
   the arrow keys 0x1468288 (up) / 0x1468290 (down) / 0x146828b (left) / 0x146828d (right) by 10, clamped to
   0 .. 0x6f4ee8 - 0x200 / 0x6f4ee4 - 0x168; returns whether the scroll changed */

int ai_Unk5a7bc0(int rec)
{
    int x;
    int y;
    int changed = 0;

    x = *(int *)(rec + 0x258);
    y = *(int *)(rec + 0x248);
    if (*(int *)((unsigned)&di_MinX) < *(int *)((unsigned)&dm_L)) {
        if (*(int *)((unsigned)&di_MaxX) <= *(int *)((unsigned)&dm_L))
            x += 10;
    } else {
        x -= 10;
    }
    if (*(int *)((unsigned)&di_MinY) < *(int *)(((unsigned)&dm_L + 0x2))) {
        if (*(int *)((unsigned)&di_MaxY) <= *(int *)(((unsigned)&dm_L + 0x2)))
            y += 10;
    } else {
        y -= 10;
    }
    if (*(unsigned char *)(((unsigned)&kb_Down + 0xc8)) == 0) {
        if (*(unsigned char *)(((unsigned)&kb_Down + 0xd0)) == 0) {
            if (*(unsigned char *)(((unsigned)&kb_Down + 0xcb)) == 0) {
                if (*(unsigned char *)(((unsigned)&kb_Down + 0xcd)) != 0)
                    x += 10;
            } else {
                x -= 10;
            }
        } else {
            y += 10;
        }
    } else {
        y -= 10;
    }
    if (x < 0)
        x = 0;
    if (x > *(int *)((unsigned)&ai_MapSize) - 0x200)
        x = *(int *)((unsigned)&ai_MapSize) - 0x200;
    if (y < 0)
        y = 0;
    if (y > *(int *)((unsigned)&full_data_6f4ee4) - 0x168)
        y = *(int *)((unsigned)&full_data_6f4ee4) - 0x168;
    if (*(int *)(rec + 0x258) != x || *(int *)(rec + 0x248) != y) {
        *(int *)(rec + 0x258) = x;
        *(int *)(rec + 0x248) = y;
        changed = 1;
    }
    return changed;
}

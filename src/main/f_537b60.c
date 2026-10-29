/* 0x537b60, a Ghidra miss (inside the span of 0x537af0): the loading progress callback (kind, cur, total): kind 0 sets the bar to 10 + cur * 70 / (2 * total) percent, kind 1 to 45 + cur * 70 / (2 * total) (only
   when it differs from the last value 0x6f2610) */

void main_Unk537b60(int kind, int cur, int total)
{
    int v;

    if (kind == 0) {
        main_SetLoadProgress(0, cur * 70 / (total * 2) + 10);
    } else if (kind == 1) {
        v = cur * 70 / (total * 2) + 0x2d;
        if (v != U_I(((unsigned)&cm_RequestedSaveName + 0x40))) {
            U_I(((unsigned)&cm_RequestedSaveName + 0x40)) = v;
            main_SetLoadProgress(0, v);
        }
    }
}

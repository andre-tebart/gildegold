/* 0x4ad510, in election years 1..3 the holder of the office object (hmt_ElectionObjTypes) is cleared @deep code hunk 40 at instruction 10 is register allocation only: the original loads hmt_ElectionObjTypes[m-1] into eax and pushes it, ours into edx and pushes edx; the other 10 differing instructions are the same eax<->edx<->ecx swaps (12 asmdiff lines, all register renames, no instruction added or changed) */

void hmt_ClearOffices(void)
{
    int m;
    HMTOBJ o;

    if (ai_TimeT.year >= 4) {
        m = ai_TimeT.year % 4;
        if (m != 0) {
            if (amt_GetFirstOffice(hmt_ElectionObjTypes[m - 1], &o) && gm_FindChar(o.id) != 0)
                amt_SendOfficeMessage(o.obj, 0, 0, 1, 0xff);
        }
    }
}

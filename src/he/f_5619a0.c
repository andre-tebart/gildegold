/* 0x5619a0, a Ghidra miss: the allocation handler of the event 0x3f: an older event of the type with the same +0xac makes this one -1 (free on its first run); the first run is now + 1 */

void he_Unk5619a0(int ev)
{
    int o;

    for (o = (int)he_FindEvent(1, 0, 0x3f); o != 0; o = (int)he_NextEvent()) {
        if (o != ev && *(int *)(o + 0xac) == *(int *)(ev + 0xac)) {
            *(int *)(ev + 0x70) = -1;
            break;
        }
    }
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0, 1);
}

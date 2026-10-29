/* 0x4e2d90, 0x429e90 of what 0x4284a0 finds */

int ai_Unk4e2d90(int a)
{
    int r;

    if (ai_Unk6f0d8c == 0) {
        r = (char *)evt_FindById(a);
        if (r != 0)
            return evt_Stop((EVTSCRIPT *)r);
    }
}

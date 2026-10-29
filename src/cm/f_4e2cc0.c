/* 0x4e2cc0, like 0x4e2c70, but ends with the callback cb(arg) and 1 as soon as 0x14673a0 is set */

int ai_Unk4e2cc0(int a, int (*cb)(int), int arg)
{
    if (ai_Unk6f0d8c != 0)
        return 0;
    while (sv_RunFrame(0, ai_Unk60d360 | 0x8000) != 0) {
        if (evt_FindById(a) == 0)
            return 0;
        if (dm_L.rup != 0) {
            if (cb != 0)
                return cb(arg);
            return 1;
        }
    }
    return 0;
}

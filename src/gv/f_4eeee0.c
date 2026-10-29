/* 0x4eeee0, sets the callback (+0x168) and its argument (+0x16c) of the slot of the id; without a callback the default one (0x4ed3f0) with the slot's record 0x6f0e48 + 0x38 * n */

void unk_Unk4eeee0(int id, void *cb, void *arg)
{
    char *e;

    if (id != -1) {
        e = unk_Unk4eedd0(id);
        if (e != 0) {
            if (cb != 0) {
                *(void **)(e + 0x168) = cb;
                *(void **)(e + 0x16c) = arg;
                return;
            }
            *(void **)(e + 0x168) = unk_Unk4ed3f0;
            *(char **)(e + 0x16c) = ai_Unk6f0e48 + ((e - gv_Effects) / 0x170) * 0x38;
        }
    }
}

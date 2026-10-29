/* 0x4e2bf0, loads the named thing (0x4287b0, then 0x4299a0) and returns its +0x100; 0 in the replay mode */

int ai_Unk4e2bf0(char *name)
{
    char *p;

    if (ai_Unk6f0d8c == 0) {
        p = (char *)evt_Load(name);
        if (p != 0) {
            evt_RunScript((EVTSCRIPT *)p);
            return *(int *)(p + 0x100);
        }
    }
    return 0;
}

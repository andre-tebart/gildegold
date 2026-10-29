/* 0x4e2c30, the same with a second argument for 0x429c40 */

int ai_Unk4e2c30(char *name, int arg)
{
    char *p;

    if (ai_Unk6f0d8c == 0) {
        p = (char *)evt_Load(name);
        if (p != 0) {
            evt_RunCommand((EVTSCRIPT *)p, arg);
            return *(int *)(p + 0x100);
        }
    }
    return 0;
}

/* 0x52b490, creates the "sp_<id>" object of a character record a (+4 id, +0x184 the character) from the template b and links it back (+0xc94 / +0x28) */

int sv_Unk52b490(char *a, int b)
{
    int o;
    char buf[0x80];

    sprintf(buf, "sp_%i", *(int *)(a + 4));
    o = (int)evt_Load(b);
    if (o != 0) {
        evt_RunScriptArgs((EVTSCRIPT *)o, 1, buf);
        *(int *)(o + 0xc94) = *(int *)(a + 0x184);
        *(int *)(*(int *)(a + 0x184) + 0x28) = *(int *)(o + 0x100);
    }
    return o;
}

/* 0x405670 */

int ch_DeclareAction(int id, CHACTFUNC func, char unk, char *name, int nargs)
{
    if (id > 63)
        return 0;
    if (ch_ActDefs[id].func) {
        err_Message("ch_DeclareAction:Function already declared!");
        return 0;
    }
    ch_ActDefs[id].func = func;
    ch_ActDefs[id].unk04 = unk;
    ch_ActDefs[id].nargs = nargs;
    strcpy(ch_ActDefs[id].name, name);
    return 1;
}

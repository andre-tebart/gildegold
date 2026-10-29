/* 0x428530 */

EVTSCRIPT *evt_LoadScript(char *path)
{
    char msg[512];
    void *f;
    int i, len;
    EVTSCRIPT *sc;
    char *tmp, *out;

    f = vfs_Tfopen(path, "rb", vfs_Root);
    if (!f) {
        sprintf(msg, "evt_LoadScript: Could not open t_vfs_file: %s!", path);
        err_Message(msg);
        return 0;
    }
    for (i = 0; i < 128; i++)
        if (!evt_Scripts[i].name[0])
            break;
    if (i >= 128) {
        sprintf(msg, "evt_LoadScript: Could not load script %s, because too many scripts are currently in memory!", path);
        vfs_fclose(f);
        return 0;
    }
    evt_Loaded = i;
    sc = &evt_Scripts[i];
    tmp = mem_Alloc(64001, "ect_LoadScript:tmp");
    out = mem_Alloc(64001, "ect_LoadScript:output");
    memset(out, 0, 64001);
    memset(tmp, 0, 64001);
    vfs_fread(tmp, 64000, 1, f);
    vfs_fclose(f);
    len = evt_Preprocess(tmp, out, evt_Lines);
    for (sc->nlines = 0; sc->nlines < 0x800; sc->nlines++)
        if (evt_Lines[sc->nlines] == -1)
            break;
    sc->lines = mem_Alloc(sc->nlines * 4, "evt:debuginfo");
    memcpy(sc->lines, evt_Lines, sc->nlines * 4);
    sc->source = mem_Alloc(len + 1, "evt_LoadScript:sc->SourceCode");
    sc->sourcelen = len + 1;
    sc->nvars = 0;
    sc->nfuncs = 0;
    sc->frame = 0;
    strcpy(sc->source, out);
    strcpy(sc->name, path + strlen(evt_Path));
    memset(sc->frames, 0, sizeof(sc->frames));
    if (tmp)
        mem_Free(tmp);
    if (out)
        mem_Free(out);
    sc->sp = sc->stack = mem_Alloc(0x400, "evt:stack");
    sc->scene = d3_CurrentScene;
    sc->group = evt_NoGroup;
    sc->waitfn = 0;
    sc->call = 0;
    sc->id = evt_NextId++;
    return sc;
}

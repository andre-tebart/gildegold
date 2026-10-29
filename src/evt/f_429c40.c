/* 0x429c40 */

void evt_RunCommand(EVTSCRIPT *sc, char *cmd)
{
    char sep[] = " ";
    int args[16];
    char buf[256];
    char words[16][0x40];
    int n = 0;
    int i;
    char *p;

    strcpy(buf, cmd);
    for (p = strtok(buf, sep); p; p = strtok(0, sep))
        strcpy(words[n++], p);
    for (i = 0; i < n; i++)
        if (isdigit(words[i][0]))
            args[i] = atoi(words[i]);
        else
            args[i] = (int)words[i];
    switch (n) {
    case 0:
        evt_RunScript(sc);
        return;
    case 1:
        evt_RunScriptArgs(sc, 1, args[0]);
        return;
    case 2:
        evt_RunScriptArgs(sc, 2, args[0], args[1]);
        return;
    case 3:
        evt_RunScriptArgs(sc, 3, args[0], args[1], args[2]);
        return;
    case 4:
        evt_RunScriptArgs(sc, 4, args[0], args[1], args[2], args[3]);
        return;
    case 5:
        evt_RunScriptArgs(sc, 5, args[0], args[1], args[2], args[3], args[4]);
        return;
    case 6:
        evt_RunScriptArgs(sc, 6, args[0], args[1], args[2], args[3], args[4], args[5]);
        return;
    case 7:
        evt_RunScriptArgs(sc, 7, args[0], args[1], args[2], args[3], args[4], args[5], args[6]);
        return;
    }
}

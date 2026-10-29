/* 0x4273d0 */
void evt_Error(EVTSCRIPT *sc, char *pos, char *msg)
{
    char buf[512];
    int line = 0;

    if (!sc) {
        sc = evt_Cur;
        pos = sc->pc;
    }
    if (evt_Cur) {
        if (sc->source) {
            for (line = 0; line < sc->nlines; line++)
                if (sc->source + sc->lines[line] > pos)
                    break;
            if (line) {
                sprintf(buf, "Line %i:%s", line, msg);
                goto log;
            }
        }
        sprintf(buf, "%s", msg);
log:
        strncpy(evt_Log[evt_LogPos].text, buf, 0x7f);
        evt_Log[evt_LogPos].line = line;
        if (++evt_LogPos > 30)
            evt_LogPos = 0;
        sprintf(buf, "\"%s\" %s in Line: %i", evt_Cur->name, msg, line);
    } else {
        sprintf(buf, "%s", msg);
        strncpy(evt_Log[evt_LogPos].text, buf, 0x7f);
        evt_Log[evt_LogPos].line = 0;
        if (++evt_LogPos > 30)
            evt_LogPos = 0;
        sprintf(buf, "%s in Line: %i", msg, 0);
    }
}

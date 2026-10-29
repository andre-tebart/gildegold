/* 0x424370, starts a fade-in (mode 1) or fade-out (mode 2) on a playing track: snapshots the stream
   position (t->pos is read back from the stream) and the current volume as the fade start. No return
   value; no-op for a NULL or idle track or mode 0, callers hold msx_Lock */

void msx_SetFade(MSXTRACK *t, int mode)
{
    if (t && t->playing && mode) {
        sfx_GetStreamPos(t->stream, &t->pos);
        t->fadepos = t->pos;
        t->fadevol = t->curvol;
        t->fademode = mode;
    }
}

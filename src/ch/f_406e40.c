/* 0x406e40, plays an animation action once and removes it when it fails */

/* ---- 0x406e40..0x408690 (worker ch4) ---- */

void ch_ActAnimOnce(CHACTION *a)
{
    if (ch_ActAnim(a) == -1)
        ch_RemoveAction(a);
}

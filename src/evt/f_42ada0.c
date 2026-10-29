/* 0x42ada0 @deep B: hunks code 8, order 14, fill 2. Read asmdiff: the original keeps -1 in edi (or edi,-1; mov [evt_LastId],edi; cmp eax,edi; ours uses immediates) and caches evt_Depth in edx, reloading it after every call; the pop tail is duplicated with a cross-jump. Using the -1 through the scene variable changed nothing (reverted). goto-pop / chained-assignment variants are recorded in notes/units/evt.md. */

void evt_Step(EVTSCRIPT *sc)
{
    int scene = -1;

    evt_LastId = -1;
    if (evt_Depth < 32)
        evt_Stack[evt_Depth++] = evt_Cur;
    evt_Cur = sc;
    if (sc->group == -1 || sc->group == evt_NoGroup || sc->ownScene || (sc->flags & 2) || !sc->pc) {
        if (sc->character && sc->character->scriptGroup != sc->group && sc->character->group != sc->group) {
            if (evt_Depth > 0) {
                evt_Cur = evt_Stack[evt_Depth - 1];
                goto pop;
            }
            return;
        }
        if (sc->ownScene || (sc->flags & 2)) {
            scene = d3_CurrentScene;
            d3if_SelectScene(sc->scene, 1);
        }
        if (!sc->waitfn || (evt_ResumeCall(sc), !sc->waitfn)) {
            do
                evt_Statement(sc);
            while (sc->mode == 2 && (sc->flags & 1));
        }
        if (scene != -1)
            d3if_SelectScene(scene, 1);
    }
    if (evt_Depth > 0) {
        evt_Cur = evt_Stack[evt_Depth - 1];
pop:
        evt_Depth--;
        evt_Stack[evt_Depth] = 0;
    }
}

/* 0x4b3a90, restarts the running mission of m's owner now */

void ai_RestartMission(G13MIS *m)
{
    G13MIS *e = ai_FindOwnerMission(m);

    if (e != 0) {
        TMX(*e) = NOWX;
        e->unk18 = 0;
        e->unk1c = 0;
        e->unk20 = 0;
    }
}

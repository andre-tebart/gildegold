/* 0x4b3b70, the first mission type of kind (after `after` if given) @deep B3 draft, 6 differing instructions: the original materialises the loop end &ai_MissionTypes[47] into ecx once (mov ecx,X) and compares the entry check and the loop test against it; ours compares the immediate twice (cmp eax,X). Same instructions otherwise (9 hunks incl. fill, 27/96 bytes). Leads tried, one build each: the cached local (G13TYPE *last = &ai_MissionTypes[47]; two uses - the compiler still folds it to the immediate, 6), the exclusive end &ai_MissionTypes[48] with p < end (10, worse), the direct form (6, kept). spell.py: no menu rule matched. Readability done this session: ai_FindMissionType -> ai_FindMissionType (its own comment: the first mission type of kind after the previous one, ai_MissionTypes -> ai_MissionTypes (the 48-entry mission type table, header comment), so Q1/Q2 hold; the remaining hunk is register allocation of a constant address. */

G13TYPE *ai_FindMissionType(int kind, G13TYPE *after)
{
    G13TYPE *p;
    int i;

    if (after == 0) {
        for (i = 0; i < 48; i++) {
            if (ai_MissionTypes[i].kind == kind)
                return &ai_MissionTypes[i];
        }
        return 0;
    }
    if (after >= &ai_MissionTypes[47])
        return 0;
    for (p = after + 1; p <= &ai_MissionTypes[47]; p++) {
        if (p->kind == kind)
            return p;
    }
    return 0;
}

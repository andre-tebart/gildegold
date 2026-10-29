/* 0x4888c0, frees the characters of the jobs of one kind whose entries are done */

void ai_FreeJobCharacters(AICHAR *ch, int n)
{
    AICHAR *c = (AICHAR *)ch;
    AIJOB *job;
    int i, found;

    for (job = he_FindEvent(2, 0, n, 3, c->bld->id); job; job = he_NextEvent()) {
        found = 0;
        if (n == 4) {
            for (i = 0; i < ai_NEntries; i++) {
                if (ai_Entries[i].item == job->u.w.lo && !(ai_Entries[i].flags & 8))
                    found = 1;
            }
        } else if (n == 8) {
            for (i = 0; i < ai_NEntries; i++) {
                if (ai_Entries[i].item == job->u.w.lo && !(ai_Entries[i].flags & 0x10))
                    found = 1;
            }
        } else if (n == 3) {
            for (i = 0; i < ai_NEntries; i++) {
                if (ai_Entries[i].item == job->u.w.lo && !(ai_Entries[i].flags & 8))
                    found = 1;
            }
        }
        if (found) {
            for (i = 0; i < 768; i++) {
                if (ai_Chars[i].id != 0xffff && ai_Chars[i].job == job)
                    sv_MoveToGroup(c->bld, 0, ai_Chars[i].id, 0);
            }
        }
    }
}

/* 0x58fb60, checks every placement candidate (62 B): ai_FindBauplaetze (prefix 0) fills the 0x100-entry
   candidate list, then ai_CheckBuildable runs on each non-null entry */

void ai_CheckBuildSites(void)
{
    int list[0x100];
    int n;
    int i;

    n = ai_FindBauplaetze(0, list);
    for (i = 0; i < n; i++) {
        if (list[i] != 0)
            ai_CheckBuildable(list[i]);
    }
}

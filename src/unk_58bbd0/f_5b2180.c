/* 0x5b2180, the run callback of the map window (944 B, ROUGH): the request record param_2 (0x14 dwords) is
   copied into the window record (cleared first, at +0xc), param_1 is the record's flags (+4), the name param_3
   (+0x5c, or the text 0x631090); 0x5a78a0 opens the map window "Misc\Landkarte2" and 0x5a6cb0 rebuilds it; the
   pump 0x5309a0 (this, 0x677c7) starts the loop: the click id 0x5fe710 is matched against the ids at +0x26f0 ..,
   the flag 0x14673a0 sets 0x6f22a0, 0x6ed9dc feeds 0x62c26c, 0x5a7cd0 is the per-frame step and param_4 (the
   +4 flag) drives the drag / key branch. ROUGH: only the prologue and the pump loop are drafted, the click
   dispatch is left as a comment. The record is 0x27ac bytes in the original's 0x27b0-byte frame (0x5a78a0
   stores the background tile at +0x27a8); a smaller local let that store run into the caller's frame */

int ai_Unk5b2180(int param_1, int param_2, char *param_3, int param_4, int param_5)
{
    struct {
        int view;               /* +0x000 */
        int flags;              /* +0x004 */
        int screen;             /* +0x008 */
        int req[0x14];          /* +0x00c, the request record */
        char name[0x68];        /* +0x05c, the title */
        char pad[0x27a8 - 0xc4];
        int tile;               /* +0x27a8, the saved background tile */
    } map;

    memset(map.req, 0, sizeof(map.req));
    if (param_2 != 0)
        memcpy(map.req, (void *)param_2, sizeof(map.req));
    map.flags = param_1;
    if (param_3 != 0)
        strcpy(map.name, param_3);
    else
        map.name[0] = *(char *)(unsigned)&ch_EmptyName;
    ai_Unk5a78a0((int *)&map);
    ai_Unk5a6cb0((int *)&map);
    if (sv_RunFrame(ai_Unk5b2180, 0x677c7) == 0)
        return 0;
    while (sv_RunFrame(ai_Unk5b2180, 0x677c7) != 0) {
        /* ROUGH: the click dispatch 0x5fe710, the flag 0x14673a0 / 0x1467398, 0x6ed9dc and 0x5a7cd0 */
        ai_Unk5a7cd0(&map.flags);
    }
    return 1;
}

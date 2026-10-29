/* 0x511700, the scene object callback of the town scene (0x511790 hooks every scene object with it): a
   "gb_" building object (kind byte 2 at +0x4b) gets the flag 4 in +0x212 (bit 8 cleared; the bits 4 and 8
   for the building type 0x1f), objects with "_TOR_" in the name are left alone; returns 1
   @deep behaviour: 47 code hunks, meaning equal - the whole original 0x511700..0x51178d was read. Meaning fixes: the return type is char (the original sets only al at 0x511763 / 0x511789, it leaves eax dirty; int gave 50 code hunks / 89 differing bytes, char 47 / 68), Q4 through SCENEOBJ.proto (0x48) instead of a raw cast, Q3 through INIT_PROTO_MASK. Left (allocation / layout): the two flag blocks are swapped - the original keeps the strstr("_TOR_") arm inline at 0x511741 and jumps to the building arm at 0x511767, ours emits the building arm inline and jumps to the strstr arm; the p test's branch polarity (je vs jne) follows. Register allocation inside the arms: the original compares *p in memory (cmp BYTE [eax],0x1f) and keeps the flag in cl with al = 1 set before the store, ours loads *p into cl and the flag into al and sets al after. Spellings tried, all identical code: the early 'if (strncmp != 0) return 1' shape, the 'if (o[4b] != 2 || (p = ...) == 0)' inverted shape, and the if/else shape with both arms returning. */


char init_TownBuildingHook(char *o)
{
    SCENEOBJ *so;
    char *p;

    so = (SCENEOBJ *)o;
    if (strncmp(o, "gb_", 3) != 0)
        return 1;
    if (o[0x4b] != 2 || (p = ai_BTypes + (so->proto & INIT_PROTO_MASK) * 0x24d) == 0) {
        if (strstr(o, "_TOR_") != 0)
            return 1;
        o[0x212] = (o[0x212] & 0xf7) | 4;
        return 1;
    }
    if (*p != 0x1f) {
        o[0x212] = (o[0x212] & 0xf7) | 4;
        return 1;
    }
    o[0x212] = o[0x212] | 0xc;
    return 1;
}

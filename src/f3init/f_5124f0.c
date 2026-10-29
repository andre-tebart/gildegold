/* 0x5124f0, loads the shopping script of the building b ("%slocations\%s\Einkauf_%s.esc" in the engine's script folder) for its owner's window (the entry of 0x7603a0 whose character is 0x50a810 of b) and starts it with the window's character; the handle goes to init_ShopScript. ROUGH @deep behaviour: 212 code hunks / 305 of 400 bytes. Q1 named from the function's own format string ("%slocations\%s\Einkauf_%s.esc" = the shopping script of the building's owner window), Q3 through the new INIT_WINDOWS_END (0x760420, one past the 0x20-entry window table 0x7603a0), Q7 with prototypes for vfs_FindFile / gm_FindCharacterByMaster, init_ShopScript (was init_ShopScript: the handle of the running shopping script, -1 = none, written here and stopped by 0x5136b0's evt_FindById / evt_Stop). Left for match-deep: (1) the window record w (0x7603a0 entry, compared against gm_FindCharacterByMaster(b) and used at +4, +8 and +0x184) and the character record fields +0x28 / +0x30 / +0x100 / +0xc94 are not read yet - the draft's w+0x184 dereference, the *(int *)(character record + 0x30) compare against *(int *)(o + 2) and the sprintf(s2, "sp_%i", w + 4) need the disassembly read to check the meaning (the table entry compared with an int id vs used as a pointer is the suspicious spot), so Q4 (14 raw accesses) needs that typed view first; (2) Q5 ROUGH until then; (3) Q2: vfs_FindFile (0x42f8f0, the path lookup sv.c declares K&R, no name anywhere), ai_Unk4c2f30 (0x4c2f30, the 4th evt_RunScriptArgs argument - a function address as callback, its unit f3snd.c has no name for it), gm_FindCharacterByMaster (0x50a810, f3gm.c's 'first kind 6/7/1/2 character with a job whose master is m', no name), ai_Unk7603a0 (0x7603a0, the 0x20 window table - no canonical name; sv.c 0x52b2a0 fills it). */


void init_LoadShopScript(unsigned char *b, unsigned short *o)
{
    int c, w, i, nm, r, h;
    int *q;
    char buf[0x104];
    char s1[4], s2[0x30];
    char *nmp;

    w = 0;
    c = 0;
    i = gm_FindCharacterByMaster(b);
    q = ai_Unk7603a0;
    do {
        if (w != 0)
            goto have;
        c = *q;
        if (c != 0 && c == i)
            w = c;
        q++;
    } while ((int)q < INIT_WINDOWS_END);
    if (w != 0) {
have:
        if (1 < *(unsigned char *)(w + 8) && (o == 0 || *(int *)(*(int *)(w + 0x184) + 0x30) == *(int *)(o + 2))) {
            if (gm_IsHomeType(b) == 0) {
                nmp = ai_Goods + (unsigned)*o * 0x41;
                if (nmp == 0)
                    nmp = 0;
            } else
                nmp = ai_BTypes + (unsigned)*b * 0x24d;
            sprintf(buf, "%slocations\\%s\\Einkauf_%s.esc", evt_Path, nmp + 1, nmp + 1);
            r = vfs_FindFile(buf, vfs_Root, s1);
            if (r != 0) {
                sprintf(s2, "sp_%i", *(int *)(w + 4));
                h = evt_LoadScript(buf);
                if (h != 0) {
                    i = *(int *)(*(int *)(w + 0x184) + 0x28);
                    if (i != -1 && (i = (int)evt_FindById(i)) != 0)
                        evt_Stop((EVTSCRIPT *)i);
                    evt_RunScriptArgs((EVTSCRIPT *)h, 2, *(int *)(w + 0x184), ai_Unk4c2f30);
                    *(int *)(h + 0xc94) = *(int *)(w + 0x184);
                    *(int *)(*(int *)(w + 0x184) + 0x28) = *(int *)(h + 0x100);
                    init_ShopScript = *(int *)(h + 0x100);
                    return;
                }
                init_ShopScript = -1;
            }
        }
    }
}

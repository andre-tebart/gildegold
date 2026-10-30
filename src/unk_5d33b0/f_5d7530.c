/* 0x5d7530, saves a network game (0x237 B): saveName is the file name under Gamedata/network, name the
   save name recorded in the header. sv_FrameMask gets 0x20000 and the 8 acknowledgement slots cm_SaveAcks
   are cleared; only in a network game (ai_GameFlags bit 2): the other players are told the status text
   ai_SaveWaitText (sv_BroadcastText), the player count (kind 6 / 7 records with an id) goes into
   ai_NumPlayers, name into ls_SavePath, ls_SaveSlot takes the requested slot cm_RequestedSaveSlot, the
   standard city screen is shown, then a .SAV file is created with the save header (flag 5) and read back
   to hash the header's 0x78 bytes (util_Hash). The hash and the local player's netId (ai_Chars) go into
   the 0x7c-byte system message 0x10, which the peers acknowledge by filling cm_SaveAcks; the function
   pumps the network until as many slots are filled as there are players, then writes the .SRV (flag 6)
   and .SAV (flag 5) saves (ls_SaveGame) and clears the status line. Void; outside a network game it only
   clears the slots.
   @deep one code hunk, 1 instruction: the original stores ai_NumPlayers = 0 before the record walk (mov BYTE ds:0x62489c,dl with dl=0 at 0x5d7589) and the count again after it; our VC6 removes the first store as dead. Leads tried (one edit + build each): separate init (count = 0; ai_NumPlayers = count;), chained init (ai_NumPlayers = count = 0), for-loop vs do-while record walk, volatile declaration of ai_NumPlayers (keeps the store but turns the wait-loop compare into mov dl/and edx,0xff instead of the original's xor edx,edx/mov dl), wait-loop spellings n < ai_NumPlayers / (int) cast / + 0 / reversed compare / int temp. Read all hunks: the rest is instruction-equal (register renames, and the scheduling of rec[0]/rec[1], which matches with rec[0] written first); only a fill tail (6 nop bytes) remains. Readability done (Q1 name, Q2 named - AICHARVIEW for ai.h's AICHAR because f3snd.h's char ai_Chars[] view blocks ai.h, Q3/Q4/Q5/Q6/Q7 clean). Left is the dead-store codegen: a declaration/view that keeps the store without changing the wait loop, or an inlined-helper form - match-deep's toolbox */

void ai_SaveNetworkGame(char *saveName, char *name)
{
    char path[MAX_PATH];
    char hdr[0xfc];
    int rec[0x1f];
    VFSFILE *h;
    unsigned int hash = 0;
    unsigned char count;
    unsigned char *p;
    AICHARVIEW *c;
    int i, n;

    sv_FrameMaskU |= 0x20000;
    for (i = 0; i < 8; i++)
        cm_SaveAcks[i][0] = -1;
    if ((ai_GameFlags & 4) == 0)
        return;
    sv_BroadcastText(sv_FrameMaskU, ai_SaveWaitText);
    count = 0;
    ai_NumPlayers = count;
    p = (unsigned char *)ai_CharsC + 2;
    do {
        c = (AICHARVIEW *)(p - 2);
        if (c->id != 0xffff && (c->kind == 6 || c->kind == 7))
            count++;
        p += 0x21c;
    } while ((int)p < (int)gm_PlayerRecordBytes);
    ai_NumPlayers = count;
    strcpy(ls_SavePath, name);
    ls_SaveSlot = cm_RequestedSaveSlot;
    ai_LoadScreenImage("Gamedata/cities/standard.scr");
    sprintf(path, "Gamedata/network/%s.SAV", saveName);
    h = vfs_Tfopen(path, "wb", vfs_Root);
    if (h != 0) {
        ls_WriteSaveHeader(h, 5);
        vfs_fclose(h);
    }
    h = vfs_Tfopen(path, "rb", vfs_Root);
    if (h != 0) {
        ls_ReadSaveHeader(h, hdr);
        hash = util_Hash((unsigned char *)hdr, 0x78);
        vfs_fclose(h);
    }
    memset(rec, 0, sizeof(rec));
    rec[0] = ((AICHARVIEW *)ai_CharsC)[ai_PlayerId].netId;
    rec[1] = hash;
    cm_SendExSysMessage(0x10, rec);
    if (ai_NumPlayers > 0) {
        do {
            n = 0;
            for (i = 0; i < 8; i++)
                if (cm_SaveAcks[i][0] != -1)
                    n++;
            sv_PumpNet();
        } while (n < ai_NumPlayers);
    }
    sv_BroadcastText(sv_FrameMaskU, ai_SaveWaitText);
    sprintf(path, "Gamedata/network/%s.SRV", saveName);
    ls_SaveGame(path, name, 6);
    sprintf(path, "Gamedata/network/%s.SAV", saveName);
    ls_SaveGame(path, name, 5);
    sv_SetStatusText(ch_EmptyName);
}

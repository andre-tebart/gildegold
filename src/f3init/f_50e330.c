/* 0x50e330, creates the n opponents of a new game (the count is the player slots minus the human players, the
   list holds m free object prototypes; its caller 0x511400 (at 0x511547) clears the prototypes the existing characters already
   use): per opponent a prototype prm is taken from the list (an entry of 0 is skipped, the walk starts at a
   random index, steps by 1 or 3 and clears the entry it uses), the opponent's character record is created as
   message 0xb kind 5 (0x4c4650, money 0x10), its difficulty k is ai_Opponents[i] (0 .. 4) and it is paid with the
   difficulty's price tab[k] (500 / 750 / 1000 / 20000 / 40000) through the player factor (0x4c4880), plus a
   random 5000 .. 9999 when 0x6f2440 is set. After the group came through, the five skill bytes at +0x80 of every
   created record are moved by the difficulty's value tab[k + 5] (-31 / -21 / 0 / +42 / +84, the upper clamp
   through a float compare against 252.0f, the lower one through the byte guard) as single field values
   (0x4c3d90) of the message 0x17 block; the difficulty 2 opponents are left alone and every opponent's block is
   sent on its own.
   @deep behaviour draft, 108 differing instructions in 162 code hunks, every hunk read against tools/disasm.py 0x50e330 0x270: the meaning is now equal (the skip walk with its own counter and break, the hoisted step = sw != 0 ? 1 : 3, the two-sided clamp 252.0f / 0, the four send groups). Left is allocation: the original has m in ebp and prm in the stack slot [esp+0x10] (stored and reloaded), ours the reverse, and the tab[] / ids[] arrays sit 0x20 apart from the original's frame slots (swapping the declarations moves nothing); the ids / tab / __ftol temp slots are all shifted by that. Readability: Q1 init_CreateOpponents, Q3 / Q4 / Q5 / Q6 done (INIT_CHAR view, ai_Opponents, the clamp constants); Q2 left on ai_Unk6f2440 alone - its only two readers are this function (the extra random 5000 .. 9999 payment) and main.c's new-game block (0x511c70, the starter buildings of the first kind 6 character); no string, no third reader, so the name has no evidence. */


void init_CreateOpponents(int n, int m, int *list)
{
    int i, j, k, v, id, cnt, idx, step;
    unsigned short r;
    short sw;
    int tab[10];
    int ids[8];
    int prm;
    INIT_CHAR *pc;
    unsigned char b;

    i = 0;
    prm = 0;
    r = util_Random(m);
    idx = r;
    sw = util_Random(2);
    step = sw != 0 ? 1 : 3;
    tab[0] = 500;
    tab[1] = 0x2ee;
    tab[2] = 1000;
    tab[3] = 20000;
    tab[4] = 40000;
    tab[5] = -31;
    tab[6] = -21;
    tab[7] = 0;
    tab[8] = 0x2a;
    tab[9] = 0x54;
    cm_BeginSendGroup();
    if (0 < n) {
        do {
            if (m != 0) {
                cnt = m;
                do {
                    if (list[idx] != 0) {
                        prm = list[idx];
                        list[idx] = 0;
                        break;
                    }
                    idx = (idx + step) % m;
                } while (--cnt != 0);
            }
            id = ((int (*)(int, int, int, int, int, int, int, int))cm_SendCreateCharacter)(5, -1, -1, 0x10, -1, prm, 0, 2);
            k = ai_Opponents[i];
            ids[i] = id;
            v = gm_PlayerFactor(tab[k], ai_OwnPlayer);
            cm_SendMoveGoods(-2, -1, ai_OwnPlayer, v);
            if (ai_Unk6f2440 != 0) {
                r = util_Random(5000);
                v = gm_PlayerFactor((r & 0xffff) + 5000, ai_OwnPlayer);
                cm_SendMoveGoods(-2, -1, ai_OwnPlayer, v);
            }
            i++;
        } while (i < n);
    }
    cm_EndSendGroup();
    k = cm_SendGroupState();
    while (k == 0) {
        cm_FlushSendQueue();
        cm_ReceiveMessages();
        cm_ExecCommands();
        k = cm_SendGroupState();
    }
    cm_BeginSendGroup();
    for (i = 0; i < n; i++) {
        pc = (INIT_CHAR *)cm_GetSlotValue(ids[i]);
        if (ai_Opponents[i] != 2) {
            cm_BeginFieldBlock((char *)pc, pc->netId);
            for (j = 0; j < 5; j++) {
                v = pc->skillBytes[j] + tab[ai_Opponents[i] + 5];
                v = (int)(252.0f < v ? 252.0f : v);
                b = v < 0 ? 0 : v;
                cm_AddFieldValue(1, 1, (j - cm_FieldObjectI) + 0x80 + (int)pc, &b);
            }
            cm_SendFieldValues();
        }
    }
    cm_EndSendGroup();
    k = cm_SendGroupState();
    while (k == 0) {
        cm_FlushSendQueue();
        cm_ReceiveMessages();
        cm_ExecCommands();
        k = cm_SendGroupState();
    }
}

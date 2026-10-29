/* 0x4b0640, the STRAFE_HINRICHTUNG text command (callback 0x601b50[10], name 0x601070[10])
   "-KANONE" / "-GALGEN" / "-KOEPFEN" / "-SHAUFEN": starts the execution (kind 3, 0, 2, 4) of the character args[k]
   in 24 hours at 10:00, with the judge, ai_JudgeCatholic / ai_JudgeProtestant and up to 12 councillors
   @deep B: 153 code hunks, 189 vs 187 instructions, frame 0x158 vs 0x16c (20 B): the nested blocks name[4][8] / kinds[4] / list[16] get other slots in ours, so every [esp+N] after the prologue is shifted (first hunk 4b0640: sub esp,0x158 vs 0x16c, args at [esp+0x160]/[esp+0x15c] vs [esp+0x174]/[esp+0x170]); the constants, callees and statements were read against the original (the KANONE/GALGEN/KOEPFEN/SHAUFEN table and kinds 3/0/2/4, the kind list, msg.type 5 with unk14 -1 / unk26 0 / unk7d 1 and the judge + councillor netIds, the call to 0x4c4fa0) and match. Block-scope slot reuse -> match-deep's slotmap/vargen work. Q2 blocked: ai_Unk4c4fa0 (0x4c4fa0, cm.c) has no evidenced name in its unit (the note calls it only 'the vote / execution message' sender); G12XMSG.unk0c / unk14 / unk26 / unk7d are written here only, no reader in any source names them. Q4 blocked: the TMX/NOWX macro (the AITIMEX view on AITIME regresses ai_Unk483a60 - see ai_CommandFire's reason). Q1 done (callback 0x601b50[10], name 0x601070[10] STRAFE_HINRICHTUNG), Q3/Q5/Q6/Q7 pass. */

#pragma pack(push, 1)
typedef struct G12XMSG {                /* 0x100 bytes, given to 0x4c4fa0 (a vote / an execution) */
    int unk00;                          /* 0x00 */
    int unk04;                          /* 0x04 */
    unsigned char type;                 /* 0x08 */
    char pad09[3];
    int unk0c;                          /* 0x0c, the player's character */
    int unk10;                          /* 0x10 */
    int unk14;                          /* 0x14 */
    AITIME time;                        /* 0x18 */
    unsigned char unk26;                /* 0x26 */
    char pad27[9];
    unsigned char n;                    /* 0x30, entries of ids */
    char pad31[3];
    int ids[16];                        /* 0x34 */
    char pad74[8];
    unsigned char kind;                 /* 0x7c */
    unsigned char unk7d;                /* 0x7d */
    char pad7e[0x82];
} G12XMSG;
#pragma pack(pop)

void ai_Unk4c4fa0(void *p);                                                   /* 0x4c4fa0 */

int ai_CommandExecution(G12ARG *args, int k, char *fmt)
{
    G12XMSG msg;
    AICHAR *c;
    int kinds[4];
    int i, n, id;
    unsigned char kind;

    c = gm_FindChar(args[k].id);
    if (c == 0 || !ai_IsLivingCharacter(c->id))
        return 0;
    {
        char name[4][8] = { "KANONE", "GALGEN", "KOEPFEN", "SHAUFEN" };

        n = 4;
        kinds[3] = 4;
        kinds[0] = 3;
        kinds[1] = 0;
        kinds[2] = 2;
        if (*fmt != '-')
            return 0;
        fmt++;
        for (i = 0; i < 4; i++) {
            if (memcmp(fmt, name[i], strlen(name[i])) == 0)
                break;
        }
        if (i >= 4)
            return 0;
    }
    kind = (unsigned char)kinds[i];
    {
        int list[16];

        msg.unk14 = -1;
        list[1] = -1;
        msg.type = 5;
        msg.unk26 = 0;
        msg.kind = kind;
        msg.unk7d = 1;
        msg.unk0c = ai_Chars[ai_PlayerId].netId;
        list[0] = c->netId;
        list[2] = ai_JudgeCatholic->netId;
        list[3] = ai_JudgeProtestant->netId;
        for (i = 0; i < 0x300; i++) {
            id = ai_Chars[i].netId;
            if (id != c->netId && ai_IsLivingNonPlayer((unsigned short)i)) {
                if (G12_B(i, 0x166) != 0)
                    list[1] = id;
            } else if (n < 16 && ai_IsLivingHuman((unsigned short)i)) {
                list[n++] = id;
            }
        }
        if (list[1] == -1)
            list[1] = list[n-- - 1];
        if (n > 0)
            memcpy(msg.ids, list, n * 4);
    }
    for (i = n; i < 16; i++)
        msg.ids[i] = -1;
    msg.n = (unsigned char)n;
    TMX(msg) = NOWX;
    gm_AddTime(&msg.time, 0x18, 0, 0);
    msg.unk26 = 0x22;
    msg.time.hour = 10;
    ai_Unk4c4fa0(&msg);
    return 1;
}

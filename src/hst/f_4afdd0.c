/* 0x4afdd0, the FEST text command (callback 0x601b50[0], name 0x601070[0]) "-n" (n > 0): sends message 0x53
   for the character ai_PlayerChar, today 12:00
   @deep B: 38 code hunks / 23 differing instructions of 54 vs 53 - one scheduling region, meaning read and equal (type 0x53, to = ((AICHAR *)ai_PlayerChar)->netId, TMX(msg) = NOWX, fromId -1, flags 1, byte 0 at 0x58, hour 12, min 0, cm_SendLongMessage): the original hoists ai_Time's first dword above the msg zeroing loop and stores the TMX copy a,c,b,d with the '&msg' push before fromId/flags/car/hour/min, ours stores b,c,a,d and pushes after car/min. Tried, one build each: source order (TMX before msg.to moves the hoist but stores a,b,d,c, 29 differing - reverted), msg.car as int (no change), typed netId (byte-neutral). spell: width hint at 0x4afdf6 is the hoisted dword read of the TMX slot. Needs vargen perm/mask rounds (store order + hoisted load). Q4 left: (1) the TMX/NOWX macro (am.h deref cast) - a typed view needs an AITIMEX member on AITIME in ai.h; both spellings tried (struct AITIME{union{...;AITIMEX x;};} and union AITIME{AITIMEX x; struct{...};}) regress ai_Unk483a60 in ai.c from byte to the [esp+0x10]/[esp+0x18] slot swap (the same regression the ai_StartBuildingWork note records for an AIMSG char view) - reverted, shared-header decision; (2) *(char *)&msg.car = 0: the byte at AIMSG+0x58 has no evidenced name (G6MSG/G8MSG call it unk58, a Q2 placeholder; every other sender writes the int car there), so a typed view would invent a name. Q1 fixed by the rename (callback 0x601b50[0], name 0x601070[0] FEST), Q2/Q3/Q5/Q6/Q7 pass. */

int ai_CommandFest(int a, int b, char *fmt)
{
    AIMSG msg;

    if (*fmt != '-')
        return 0;
    if (atoi(fmt + 1) > 0) {
        memset(&msg, 0, sizeof(msg));
        msg.type = 0x53;
        msg.to = ((AICHAR *)ai_PlayerChar)->netId;
        TMX(msg) = NOWX;
        msg.fromId = -1;
        msg.flags = 1;
        *(char *)&msg.car = 0;
        msg.time.hour = 12;
        msg.time.min = 0;
        cm_SendLongMessage(&msg);
    }
    return 1;
}

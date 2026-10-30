/* 0x50e0d0, the start of a new game: the first 16 character records are opened in the game - every occupied
   record of the kinds 0 .. 4 gets a start purse (the field INIT_FIELD_MONEY: rand() & 0xffff + 10 through the
   player factor) as a field block of its own. Then the two judges are created (kind 0xc, money 0x12) in one
   send group and fetched back through their send slots as ai_JudgeCatholic / ai_JudgeProtestant, whose
   religion field INIT_FIELD_RELIGION is set to 0 / 1 in a second group; eight characters of the kind 0
   (money rand(8) + 0x10, the 6th argument rand(3) + 4) follow in a third and eight of the kind 0xb (the 7th
   argument 0x13, each one also announced as a trade message from object -2) in a fourth; every group is
   waited for by pumping the queue. The message senders leave their send slot in eax where cm.h's prototypes
   say void, so the calls are made through 8-argument int function pointers. No caller in the exe.
   @deep behaviour draft, 27 code hunks, every hunk read against tools/disasm.py 0x50e0d0 0x160: no missing / extra statement, constant or callee (the 16-record purse loop, the two 8-argument sender calls, the four send groups, the judge religion fields 0 / 1) - the meaning is equal. Left is allocation / scheduling: the original keeps the two judge send slots in esi / edi across the wait group (push edi at 0x50e143, a mid-function live range), ours spills the first to [esp+0x44] and keeps the second in esi. spell.py: 50e10e slots (w1 / w2 frame slots) and 50e1a0 reload - the values are already int locals kept across the calls, no source spelling moved them; 50e143 / 50e16c / 50e1db / 50e244 / 50e32d have no menu rule. Caller: 0x511400 at 0x511423 (the new-game setup). Readability done: Q1 init_StartNewGame, Q2 cleared, Q3 INIT_START_CHARACTERS_END, Q4 INIT_CHAR view, Q5 marker removed, Q6 described. */


void init_StartNewGame(void)
{
    INIT_CHAR *c;
    int i;
    int rec[16];                /* the send slots of the created characters (never read) */
    int m;
    int w1, w2;

    rand();                     /* the first random value of the game is dropped */
    c = (INIT_CHAR *)ai_Chars[0];
    do {
        if (c->id != 0xffff && c->kind < 5) {
            w1 = gm_PlayerFactor((util_RandomU(0x122) & 0xffff) + 10, ai_OwnPlayer);
            cm_BeginFieldBlock((char *)c, c->netId);
            cm_AddFieldValues(4, 1, INIT_FIELD_MONEY, &w1);
            cm_SendFieldDeltas();
        }
        c++;
    } while ((int)c < INIT_START_CHARACTERS_END);
    cm_BeginSendGroup();
    w1 = ((int (*)(unsigned char, int, int, unsigned short, int, unsigned char, unsigned char, unsigned char))cm_SendCreateCharacter)(0xc, -1, -1, 0x12, -1, 0, 0, 0);
    w2 = ((int (*)(unsigned char, int, int, unsigned short, int, unsigned char, unsigned char, unsigned char))cm_SendCreateCharacter)(0xc, -1, -1, 0x12, -1, 0, 0, 0);
    cm_EndSendGroup();
    i = cm_SendGroupState();
    while (i == 0) {
        cm_FlushSendQueue();
        cm_ReceiveMessages();
        cm_ExecCommands();
        i = cm_SendGroupState();
    }
    ai_JudgeCatholic = (char *)cm_GetSlotValue(w1);
    ai_JudgeProtestant = (char *)cm_GetSlotValue(w2);
    cm_BeginSendGroup();
    m = 0;
    cm_BeginFieldBlock(ai_JudgeCatholic, ((INIT_CHAR *)ai_JudgeCatholic)->netId);
    cm_AddFieldDelta(1, 1, (ai_JudgeCatholic - cm_FieldObject) + INIT_FIELD_RELIGION, &m);
    cm_SendFieldDeltas();
    m = 1;
    cm_BeginFieldBlock(ai_JudgeProtestant, ((INIT_CHAR *)ai_JudgeProtestant)->netId);
    cm_AddFieldDelta(1, 1, (ai_JudgeProtestant - cm_FieldObject) + INIT_FIELD_RELIGION, &m);
    cm_SendFieldDeltas();
    cm_EndSendGroup();
    i = cm_SendGroupState();
    while (i == 0) {
        cm_FlushSendQueue();
        cm_ReceiveMessages();
        cm_ExecCommands();
        i = cm_SendGroupState();
    }
    cm_BeginSendGroup();
    i = 0;
    do {
        rec[i] = ((int (*)(unsigned char, int, int, unsigned short, int, unsigned char, unsigned char, unsigned char))cm_SendCreateCharacter)(0, -1, -1, util_RandomU(8) + 0x10, -1, (char)util_RandomU(3) + 4, 0, 0);
        i++;
    } while (i < 8);
    cm_EndSendGroup();
    i = cm_SendGroupState();
    while (i == 0) {
        sv_PumpNet();
        i = cm_SendGroupState();
    }
    cm_BeginSendGroup();
    i = 0;
    do {
        rec[i] = ((int (*)(unsigned char, int, int, unsigned short, int, unsigned char, unsigned char, unsigned char))cm_SendCreateCharacter)(0xb, -1, -1, util_RandomU(8) + 0x10, -1, 0, 0x13, 0);
        cm_SendExSellObject(-2, -1, 0x154, 1, ai_OwnPlayer, 0);
        i++;
    } while (i < 8);
    cm_EndSendGroup();
    i = cm_SendGroupState();
    while (i == 0) {
        sv_PumpNet();
        i = cm_SendGroupState();
    }
}

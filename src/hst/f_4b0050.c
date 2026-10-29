/* 0x4b0050, the WIRBELSTURM text command (callback 0x601b50[4], name 0x601070[4]), the same as ai_CommandFire:
   message 0x50 (value 1000, flags 2), today 16:00
   @deep byte already (the event value at 0x9c via the G12EVENTMSG view, as ai_CommandFire). Q4 only left: the TMX/NOWX macro (am.h deref cast); the typed AITIMEX view on AITIME in ai.h regresses ai_Unk483a60 from byte to the [esp+0x10]/[esp+0x18] slot swap (documented in ai_CommandFire's reason and ai_StartBuildingWork's note) - shared-header decision. Q1 has evidence (callback 0x601b50[4], name 0x601070[4] WIRBELSTURM), Q2/Q3/Q5/Q6/Q7 pass. */

int ai_CommandTornado(void)
{
    AIMSG msg;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x50;
    msg.to = *(int *)((char *)ai_PlayerChar + 4);
    msg.fromId = -1;
    msg.car = -1;
    msg.flags = 2;
    ((G12EVENTMSG *)&msg)->value = 1000;
    TMX(msg) = NOWX;
    msg.time.hour = 16;
    cm_SendLongMessage(&msg);
    return 1;
}

/* 0x4affc0, the BRAND text command (callback 0x601b50[3], name 0x601070[3]): sends message 0x4e (value 1000) for
   the character ai_PlayerChar, today 7:00
   @deep byte already (the event value at 0x9c via the G12EVENTMSG view, fixed this session; AIMSG.unka0 at 0xa0 was the wrong offset). Q4 only left: the TMX/NOWX macro of am.h is a deref-cast macro; a typed view needs an AITIMEX member on AITIME in ai.h and both spellings tried (struct AITIME{union{...;AITIMEX x;};} and union AITIME{AITIMEX x; struct{...};}) regress ai_Unk483a60 in ai.c from byte to the [esp+0x10]/[esp+0x18] slot swap (same regression as ai_StartBuildingWork's note for an AIMSG char view) - reverted, shared-header decision. Q1 evidence: callback 0x601b50[3], name 0x601070[3] BRAND; Q2/Q3/Q5/Q6/Q7 pass. */

/* the 0x4e / 0x50 event messages (the BRAND / WIRBELSTURM commands): an AIMSG (0xf8 bytes) with the event's
   value at 0x9c (1000 in both; the same dword is a name / a flag byte in the other long-message kinds) */
typedef struct G12EVENTMSG {
    char pad00[0x9c];
    int value;                          /* 0x9c */
    char padA0[0x58];
} G12EVENTMSG;

int ai_CommandFire(void)
{
    AIMSG msg;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x4e;
    msg.to = *(int *)((char *)ai_PlayerChar + 4);
    msg.fromId = -1;
    msg.car = -1;
    msg.flags = 1;
    ((G12EVENTMSG *)&msg)->value = 1000;
    TMX(msg) = NOWX;
    msg.time.hour = 7;
    cm_SendLongMessage(&msg);
    return 1;
}

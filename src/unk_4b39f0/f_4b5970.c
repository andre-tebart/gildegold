/* 0x4b5970, the mission complete dialog (after 0x4b5460): a new mission of the same kind (message 0x0a, then
   0x4b5360), the end of the missions, or quitting (0x4b5950)
   @deep behaviour draft (readable): 157 differing instructions, code 195 - allocation only. Read every hunk against tools/disasm.py 0x4b5970: the three msg branches, the merged sv_PopRequest tail and the closing again/quit/sample-bank block are the same statements; ours spills h to a stack slot where the original keeps it in ebx (frame 0x194 vs 0x190, exactly h's 4 bytes: 8 locals vs 7), keeps the shared 0-constant in esi instead of ebx, and flags in ebx instead of eax. spell.py menu: reload x2 (prologue/slot reads - the frame shift, nothing to drop: id is the parameter and is read once; h is needed twice per branch), dup-tail (the sv_PopRequest store is already shared on both sides), width (no 16/32-bit field read found). Leads tried, one edit + one build each, all still 157: G13TYPE *t at function scope instead of block scope; int h declared inside the do body; nm/again/quit as declaration initializers (zero web). c2prio unavailable (no frida). What is left is the register/stack allocation of h and flags - match-deep's slot/priority work. */

void cm_SendExSysMessage(unsigned char id, void *msg);                                             /* 0x4c4db0, defined elsewhere (hmt.h: void) */
int cm_GetSlotState(int h);                                                         /* 0x4c3ae0, defined elsewhere */
void sv_PumpNet(void);                                                         /* 0x52fe00, defined elsewhere */
int sb_UnloadSampleBank(struct SBBANK *bank);                                             /* 0x426ba0 */
void * ai_MissionSampleBank = 0;  /* 0x6f072c, a sample bank */

void ai_MissionCompleteDialog(int id)
{
    int msg1[0x1f];
    int msg2[0x1f];
    int msg3[0x1f];
    AICHAR *c;
    G13MIS *nm;
    int again, quit, o1, o2, o3, view, flags, h;

    c = &ai_CharsT[ai_PlayerId];
    nm = 0;
    again = 0;
    quit = 0;
    {
        G13TYPE *t = ai_GetMissionType(ai_Missions[0].type);

        memset(&ai_Missions[0], 0, sizeof(G13MIS));
        id = (id & ~9) | 0x100;
        ai_MissionSuccessDialog(t, id);
    }
    view = d2_OpenForm(0, 0, "special\\mission");
    d2_CentreViewWindows(view);
    d2_SetForm(view, 1);
    io_PrintText((char *)0x1b95, c->id);
    o1 = d2_GetWinObj(view, 1, io_PrintText((char *)0x1b96, 0x1b98));
    o2 = d2_GetWinObj(view, 1, io_PrintText((char *)0x1b96, 0x1b99));
    o3 = d2_GetWinObj(view, 1, io_PrintText((char *)0x1b96, 0x1b9a));
    d2_ClickedId = -1;
    flags = id | 0x80;
    do {
        if (d2_ClickedId != -1) {
            if (d2_PointerObj == o1) {
                nm = ai_NewRandomMission(c->netId, ai_MissionMode, 1);
                msg1[0] = c->netId;
                msg1[4] = 0;
                again = 1;
                msg1[8] = nm->type;
                h = ((int (*)(int, int))cm_SendExSysMessage)(0x0a, msg1);
                while (!cm_GetSlotState(h))
                    sv_PumpNet();
                sv_PopRequest = 1;
            } else if (d2_PointerObj == o2) {
                ai_MissionMode = -1;
                msg2[0] = c->netId;
                msg2[4] = 0;
                msg2[8] = 0;
                h = ((int (*)(int, int))cm_SendExSysMessage)(0x0a, msg2);
                while (!cm_GetSlotState(h))
                    sv_PumpNet();
                sv_PopRequest = 1;
            } else if (d2_PointerObj == o3) {
                ai_MissionMode = -1;
                msg3[0] = c->netId;
                quit = 1;
                msg3[4] = 1;
                msg3[8] = 0;
                h = ((int (*)(int, int))cm_SendExSysMessage)(0x0a, msg3);
                while (!cm_GetSlotState(h))
                    sv_PumpNet();
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame((void (*)(void))ai_MissionCompleteDialog, flags));
    d2_CloseView(view);
    if (again)
        ai_MissionInfoDialog(nm->type, id);
    else if (quit)
        ai_QuitGame(id);
    if (ai_MissionSampleBank != 0)
        sb_UnloadSampleBank(ai_MissionSampleBank);
    ai_MissionSampleBank = 0;
}

/* 0x557190, the run handler of the event message box (only for the player's figure while a game runs: 0x6f25b8 / 0x6f25ac; ROUGH skeleton, the flags of +0xf0: 1 = quick jump, 2 = big, 4 = cutscene, 8 = scene window, 0x10 = speech (the text +0x7c is spoken 0x4c27d0 with the shape +0xec), 0x20 = history, 0x40 = help window, else the plain box): state 0 opens the window of the flavour "misc\EventMessagebox_QJ_BIG" / "_QJ" / "_BIG" / "EventMsgbox_History" / "EventMessagebox_Cutscenes" / "EventMessagebox_Help" / "EventMessagebox" (0x53d6f0 (ev, 0, 0, name, +0xac)), shows the news 0x1ba1 / 0x1ba2 / 0x1ba3 / 0x7d and the text +0x7c (0x515960 "%s"), plays the sound "NeueMessage_s_01" (0x426fa0) unless 0x6f25ac == 2, and sets both clocks (+0x44 / +0x52) to now (the due clock +0x14 hours for a quick jump, the minute word 6); state 1 waits for the due time (0x4fe210) or the click and then carries out the quick jump (0x4e50a0 / 0x4e5210 / 0x4c4fd0 / 0x521960 on the contact +0xb0 / +0xb4) and closes the window (0x53d800, 0x410620 / 0x427290 for the help windows); states -2 / -1 free the record */

int cm_PartyById();
int ai_Unk4e5210();
void ai_Unk4c4fd0(int a, int b);
void sb_Stop();
int sv_SelectPanelObject();
int snd_QueueVoice(int id, int bank, char *text, int var, unsigned int delay);

void he_Unk557190(int ev)
{
    int st = *(int *)(ev + 0x70);
    int pl = 0, fl, b, w;
    char speak[0x7d0];

    if (*(unsigned short *)(unsigned)&ai_PlayerId != *(unsigned short *)(ev + 8) || *(unsigned char *)(unsigned)&ai_GameState == 0)
        return;
    fl = *(unsigned char *)(ev + 0xf0);
    switch (st) {
    case -2:
    case -1:
        if ((fl & 2) != 0) {
            if (U_I((unsigned)&ai_PlayerBuilding) != 0)
                pl = U_I((unsigned)&ai_PlayerBuilding);
            else if (U_I((unsigned)&ai_FallbackBuilding) != 0)
                pl = U_I((unsigned)&ai_FallbackBuilding);
            he_Unk53d6f0(ev, 0, 0, (int)((fl & 1) ? "misc\EventMessagebox_QJ_BIG" : "misc\EventMessagebox_QJ"), *(int *)(ev + 0xac));
            if (*(int *)(ev + 0x74) == 0) {
                he_Unk53e1f0((int *)ev);
                return;
            }
            d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 1);
            if (pl != 0 && *(int *)(pl + 1) == *(int *)(ev + 0xb0) && *(unsigned char *)(ev + 0xbc) == 0)
                io_PrintText(0x1ba2);
            else
                io_PrintText(0x1ba1);
        } else if ((fl & 1) != 0) {
            he_Unk53d6f0(ev, 0, 0, (int)"misc\EventMessagebox_BIG", *(int *)(ev + 0xac));
            if (*(int *)(ev + 0x74) != 0) {
                d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 1);
                io_PrintText(0x1ba3);
            }
        } else if ((fl & 0x20) != 0) {
            he_Unk53d6f0(ev, 0, 0, (int)"misc\EventMsgbox_History", *(int *)(ev + 0xac));
            if (*(int *)(ev + 0x74) != 0) {
                d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 1);
                io_PrintText(0x1ba3);
            }
        } else if ((fl & 4) != 0) {
            he_Unk53d6f0(ev, 0, 0, (int)"misc\EventMessagebox_Cutscenes", *(int *)(ev + 0xac));
        } else if ((fl & 0x40) != 0) {
            he_Unk53d6f0(ev, 0, 0, (int)"misc\EventMessagebox_Help", *(int *)(ev + 0xac));
        } else {
            he_Unk53d6f0(ev, 0, 0, (int)"misc\EventMessagebox", *(int *)(ev + 0xac));
            if (*(int *)(ev + 0x74) != 0) {
                d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 1);
                io_PrintText(0x7d);
            }
        }
        if (*(int *)(ev + 0x74) == 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        d2_CentreViewWindows(*(int *)(*(int *)(ev + 0x74) + 8));
        d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 0);
        io_PrintText("%s", *(int *)(ev + 0x7c));
        if (*(unsigned char *)(unsigned)&ai_GameState != 2)
            sb_PlaySample(U_I((unsigned)&full_data_6f2400), "NeueMessage_s_01", 1, 0x3f, 0x7f, 0, 0);
        if ((fl & 8) != 0)
            he_Unk53dcb0(*(int *)(ev + 0x74));
        if ((fl & 0x10) != 0) {
            strcpy(speak, (char *)(*(int *)(ev + 0x7c) + strlen((char *)*(int *)(ev + 0x7c)) + 1));
            *(int *)(ev + 0xec) = snd_QueueVoice(-7, 0, speak, 0, 0x7d0);
        }
        *(HETIME *)(ev + 0x44) = ai_Time;
        *(HETIME *)(ev + 0x52) = ai_Time;
        if ((fl & 2) != 0 && *(unsigned short *)(ev + 0x56) < 6)
            gm_AddTime(ev + 0x52, 0, 0, 1);
        else
            gm_AddTime(ev + 0x52, 0, 0, 1);
        return;
    case 0:
    case 1:
        b = gm_CompareTime((unsigned)&ai_Time, ev + 0x52);
        if (b > 0 || ((fl & 2) != 0 && *(int *)(ev + 0xb0) != -1 && cm_PartyById() != 0)) {
            if ((fl & 2) != 0 && *(int *)(ev + 0xb0) != -1) {
                w = gm_BuildQuery(1, 1, *(int *)(ev + 0xb0));
                if (w != 0)
                    sv_SelectPanelObject(w);
                ai_Unk4e5210();
                ((void (*)())ai_Unk4c4fd0)();
            }
            if ((fl & 0x40) != 0 && (U_I((unsigned)&ai_GameFlags) & 4) != 0)
                di_ClearButtons();
            sb_Stop(*(int *)(ev + 0xec));
            he_Unk53d800(ev);
            he_Unk53e1f0((int *)ev);
        }
        return;
    }
}

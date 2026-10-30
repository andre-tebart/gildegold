/* 0x5da030, the poker game (the card game against two opponents) of the gypsy poker table (2752 B). The player's record
   (gm_PlayerRecord) decides whether he plays: the record's float at +0x60 must exceed a random number, it must be past
   11 o'clock, and the player's worth (gm_GetWorth, converted to the player's money) at least 10000. Then the float is
   decayed (* 0.66 - 0.1), the key state 0x1e is cleared, the poker window (ai_OpenPokerWindow) opened and the game started
   (ai_PokerStart fills ai_PokerState; 0 closes the window and ends). Otherwise the form "Misc\Messagebox" shows the text
   0x4239 (with 0x4238) until the right button or the key 1 pops it. Returns 1 when a game was played, else 0.
   The pump runs sv_RunFrame (this function, 0x777c7); every frame the buttons follow the mask the game step
   (ai_PokerStep) returns: bit 1 the first button (ai_PokerUi.buttons[0]), bit 8 the second, bit 0x10 the fourth; bits 1 and
   0x20 show the bet slider (ai_PokerBetControl). The second button asks (text 0x4263, message box 0x101) and leaves, the
   fifth opens the rules window ai_PokerWindow2 with the poker view disabled. Then ai_PokerState.phase (1 .. 11) selects the
   step: the seats are redrawn with ai_PokerSeatPanel (mode 0 .. 5) when the phase changes, the pot text 0x4252 and a random
   remark 0x4253 .. 0x4255 (with the hand text 0x4256 / 0x4257 / 0x4258) are printed, the first button takes the bet from the
   slider (gm_PlayerFactor of its value) and steps the game; phase 6 lets the player mark one of the first two cards to
   exchange (the objects' checked flag, ai_Unk4a8630), phase 7 shows the two opponents' texts (0x425c when the seat put a card away in the exchange, else 0x425d: seat dropped),
   phase 10 shows the result (the winner's names and the hand text 0x4266 + the rank), phase 11 waits for the fourth button.
   Reasoned: the button roles and the phase meanings follow from the texts and the mask bits; the text ids are not read
   from the text files. @deep Body reconstructed whole from the disassembly (prelude with the record / worth / chance gate, the Misc\Messagebox refusal loop, the pump with the mask-driven buttons and the 11-phase switch, ai_PokerState as a POKERGAME at 0x6f3500 with 25 of 991 instructions differing). After the build asmdiff has 2 hunks left: (1) regs 2 at 0x5da290, the byte-copy store of the texts [edx+eax] vs the original's [eax+edx] (s[d] / d[s] compile the same); (2) code 23 at 0x5dadbd, which is not code: the 11-entry jump table of the switch plus its 3-byte alignment fill (lea ecx,[ecx]) that our object keeps inside the function symbol (size 3515 vs 3469) while the original's walker ends the function at 0x5dadbc and files the fill and the table at 0x5dadc0 apart (dump: func@0x5da030..0x5dadbd fill@0x5dadbd jumptable@0x5dadc0); B1 compares 1 of 3520 bytes (the sib byte). Tried: print inside / after the branches (fixed two hunks), branch polarity of the opponent texts (fixed), s[d] vs d[s]. Nothing in the C is left to change: the table range needs a tool decision (exclude our trailing jump table from the code hunks when the original's range stops before its own fill). */

typedef struct PLAYERRECVIEW {          /* f3gm.h's player record (gm_PlayerRecord, 0x56 words): only the float the poker game reads */
    char pad00[0x60];
    float luck;                         /* +0x60, the chance that the player sits down to play (decays by * 0.66 - 0.1 per game; reasoned) */
} PLAYERRECVIEW;

int ai_PokerGame(void)
{
    char msg[0x100];
    char text[0x100];
    int lastPhase;
    unsigned short mask;
    PLAYERRECVIEW *rec;
    int worth, form;
    char *s, c;
    int d;

    lastPhase = -1;
    rec = (PLAYERRECVIEW *)gm_PlayerRecord(ai_CharsC + ai_PlayerId * 0x21c);
    if (rec == 0)
        return 0;
    worth = gm_ConvertMoney(gm_GetWorth(ai_PlayerId), ai_OwnPlayer);
    if (util_RandomF() < rec->luck && ai_TimeHour > 11 && worth >= 10000) {
        rec->luck = rec->luck * 0.66f - 0.1f;
        kb_Down[0x1e] = 0;
        if (ai_OpenPokerWindow() == 0)
            return 0;
        if (ai_PokerStart((struct AIPER *)(ai_CharsC + ai_PlayerId * 0x21c), (struct G10GAME *)&ai_PokerState) == 0) {
            ai_ClosePokerWindow();
            return 0;
        }
        mask = ai_PokerStep();
        d2_ClickedId = -1;
        do {
            if (dm_L.rup != 0)
                sv_PopRequest = 1;
            d2_SetObjEnabled(ai_PokerUi.buttons[0], mask & 1);
            d2_SetObjEnabled(ai_PokerUi.buttons[1], mask & 8);
            d2_SetObjEnabled(ai_PokerUi.buttons[3], mask & 0x10);
            ai_PokerBetControl((mask & 1) != 0 && (mask & 0x20) != 0);
            if (d2_ClickedId != -1) {
                if (ai_PokerUi.buttons[1] == d2_PointerObj) {
                    txt_Format(msg, 0x4263);
                    if (sv_MessageBox((int)msg, 0x101) != 0)
                        sv_PopRequest = 1;
                }
                if (ai_PokerUi.buttons[4] == d2_PointerObj) {
                    d2_SetViewEnabled(ai_PokerUi.window, 0);
                    ai_PokerWindow2();
                    d2_SetViewEnabled(ai_PokerUi.window, 1);
                }
            }
            switch (ai_PokerState.phase) {
            case 1:
                if (lastPhase != 1) {
                    lastPhase = 1;
                    ai_PokerSeatPanel(1, 0, 0);
                    ai_PokerSeatPanel(2, 0, 0);
                    ai_PokerSeatPanel(0, 0, 0);
                }
                d2_SetForm(ai_PokerUi.window, 4);
                if (ai_PokerState.actor == 0) {
                    s = txt_Ptrs[0x4250];
                    d = text - s;
                    do {
                        c = *s;
                        s[d] = c;
                        s++;
                    } while (c != 0);
                    io_PrintText(text);
                    if (d2_ClickedId != -1 && ai_PokerUi.buttons[0] == d2_PointerObj) {
                        ai_PokerState.bet = gm_PlayerFactor(d2_GetObjValue(ai_PokerUi.betSlider), ai_OwnPlayer);
                        mask = ai_PokerStep();
                    }
                } else {
                    sprintf(text, txt_Ptrs[0x424f], ai_PokerState.seat[ai_PokerState.actor].who->vname, ai_PokerState.seat[ai_PokerState.actor].who->nname);
                    io_PrintText(text);
                    txt_Format(text, 0x4251, ai_PokerState.minBet);
                    io_PrintText(text);
                    if (d2_ClickedId != -1 && ai_PokerUi.buttons[3] == d2_PointerObj)
                        mask = ai_PokerStep();
                }
                break;
            case 2:
                d2_SetForm(ai_PokerUi.window, 4);
                txt_Format(text, 0x4252, ai_PokerState.pot);
                io_PrintText(text);
                txt_Format(text, util_RandomU(3) + 0x4253, 0x4256);
                io_PrintText(text);
                if (ai_PokerState.actor != 0) {
                    txt_Format(text, 0x425a, ai_PokerState.bet);
                    io_PrintText(text);
                } else {
                    io_PrintText(txt_Ptrs[0x4259]);
                }
                ai_PokerSeatPanel(1, 3, ai_PokerState.seat[1].card);
                ai_PokerSeatPanel(2, 3, ai_PokerState.seat[2].card);
                ai_PokerSeatPanel(0, 3, ai_PokerState.seat[0].card);
                mask = ai_PokerStep();
                break;
            case 3:
                if (d2_ClickedId != -1) {
                    if (ai_PokerUi.buttons[0] == d2_PointerObj) {
                        if (ai_PokerState.actor == 0)
                            ai_PokerState.bet = gm_PlayerFactor(d2_GetObjValue(ai_PokerUi.betSlider), ai_OwnPlayer);
                        mask = ai_PokerStep();
                        ai_PokerSeatPanel(0, 4, ai_PokerState.seat[0].card);
                        ai_PokerSeatPanel(1, 4, ai_PokerState.seat[1].card);
                        ai_PokerSeatPanel(2, 4, ai_PokerState.seat[2].card);
                    } else if (ai_PokerUi.buttons[1] == d2_PointerObj) {
                        sv_PopRequest = 1;
                    }
                }
                break;
            case 4:
                d2_SetForm(ai_PokerUi.window, 4);
                txt_Format(text, 0x4252, ai_PokerState.pot);
                io_PrintText(text);
                txt_Format(text, util_RandomU(3) + 0x4253, 0x4257);
                io_PrintText(text);
                if (ai_PokerState.actor != 0) {
                    txt_Format(text, 0x425a, ai_PokerState.bet);
                    io_PrintText(text);
                } else {
                    io_PrintText(txt_Ptrs[0x4259]);
                }
                if (lastPhase != 4) {
                    lastPhase = 4;
                    ai_PokerSeatPanel(1, 4, ai_PokerState.seat[1].card);
                    ai_PokerSeatPanel(2, 4, ai_PokerState.seat[2].card);
                    ai_PokerSeatPanel(0, 4, ai_PokerState.seat[0].card);
                }
                mask = ai_PokerStep();
                break;
            case 5:
                if (d2_ClickedId != -1) {
                    if (ai_PokerUi.buttons[0] == d2_PointerObj) {
                        if (ai_PokerState.actor == 0)
                            ai_PokerState.bet = gm_PlayerFactor(d2_GetObjValue(ai_PokerUi.betSlider), ai_OwnPlayer);
                        mask = ai_PokerStep();
                        ai_PokerSeatPanel(0, 4, ai_PokerState.seat[0].card);
                        ai_PokerSeatPanel(1, 4, ai_PokerState.seat[1].card);
                        ai_PokerSeatPanel(2, 4, ai_PokerState.seat[2].card);
                    } else if (ai_PokerUi.buttons[1] == d2_PointerObj) {
                        sv_PopRequest = 1;
                    }
                }
                break;
            case 6:
                d2_SetForm(ai_PokerUi.window, 4);
                txt_Format(text, 0x4252, ai_PokerState.pot);
                io_PrintText(text);
                io_PrintText(txt_Ptrs[0x425b]);
                d2_Objs[ai_PokerUi.panel[0].slot[0]].greyed = 0;
                d2_Objs[ai_PokerUi.panel[0].slot[1]].greyed = 0;
                d2_Objs[ai_PokerUi.panel[0].slot[2]].greyed = 1;
                d2_Objs[ai_PokerUi.panel[0].slot[3]].greyed = 1;
                d2_Objs[ai_PokerUi.panel[0].slot[0]].interactive = 1;
                d2_Objs[ai_PokerUi.panel[0].slot[0]].style = 3;
                d2_Objs[ai_PokerUi.panel[0].slot[1]].interactive = 1;
                d2_Objs[ai_PokerUi.panel[0].slot[1]].style = 3;
                if (d2_ClickedId != -1) {
                    if (ai_PokerUi.panel[0].slot[0] == d2_PointerObj) {
                        d2_Objs[ai_PokerUi.panel[0].slot[0]].checked = d2_Objs[ai_PokerUi.panel[0].slot[0]].checked == 0;
                        d2_Objs[ai_PokerUi.panel[0].slot[1]].checked = 0;
                        if (d2_Objs[ai_PokerUi.panel[0].slot[0]].checked != 0)
                            d2_SetObjEnabled(ai_PokerUi.buttons[2], 1);
                        else
                            d2_SetObjEnabled(ai_PokerUi.buttons[2], 0);
                    } else if (ai_PokerUi.panel[0].slot[1] == d2_PointerObj) {
                        d2_Objs[ai_PokerUi.panel[0].slot[1]].checked = d2_Objs[ai_PokerUi.panel[0].slot[1]].checked == 0;
                        d2_Objs[ai_PokerUi.panel[0].slot[0]].checked = 0;
                        if (d2_Objs[ai_PokerUi.panel[0].slot[1]].checked != 0)
                            d2_SetObjEnabled(ai_PokerUi.buttons[2], 1);
                        else
                            d2_SetObjEnabled(ai_PokerUi.buttons[2], 0);
                    } else if (ai_PokerUi.buttons[2] == d2_PointerObj) {
                        d2_SetObjEnabled(ai_PokerUi.buttons[2], 0);
                        if (d2_Objs[ai_PokerUi.panel[0].slot[0]].checked != 0)
                            ai_PokerExchange(0);
                        else if (d2_Objs[ai_PokerUi.panel[0].slot[1]].checked != 0)
                            ai_PokerExchange(1);
                        mask = ai_PokerStep();
                        ai_PokerSeatPanel(0, 4, ai_PokerState.seat[0].card);
                        ai_PokerSeatPanel(1, 4, ai_PokerState.seat[1].card);
                        ai_PokerSeatPanel(2, 4, ai_PokerState.seat[2].card);
                    } else if (ai_PokerUi.buttons[3] == d2_PointerObj) {
                        d2_SetObjEnabled(ai_PokerUi.buttons[2], 0);
                        mask = ai_PokerStep();
                        ai_PokerSeatPanel(0, 4, ai_PokerState.seat[0].card);
                        ai_PokerSeatPanel(1, 4, ai_PokerState.seat[1].card);
                        ai_PokerSeatPanel(2, 4, ai_PokerState.seat[2].card);
                    } else if (ai_PokerUi.buttons[1] == d2_PointerObj) {
                        sv_PopRequest = 1;
                    }
                }
                break;
            case 7:
                d2_Objs[ai_PokerUi.panel[0].slot[0]].greyed = 1;
                d2_Objs[ai_PokerUi.panel[0].slot[1]].greyed = 1;
                d2_Objs[ai_PokerUi.panel[0].slot[2]].greyed = 0;
                d2_Objs[ai_PokerUi.panel[0].slot[3]].greyed = 0;
                d2_Objs[ai_PokerUi.panel[0].slot[0]].interactive = 0;
                d2_Objs[ai_PokerUi.panel[0].slot[0]].style = 0;
                d2_Objs[ai_PokerUi.panel[0].slot[1]].interactive = 0;
                d2_Objs[ai_PokerUi.panel[0].slot[1]].style = 0;
                d2_SetForm(ai_PokerUi.window, 4);
                txt_Format(text, 0x4252, ai_PokerState.pot);
                io_PrintText(text);
                if (ai_PokerState.seat[1].dropped != -1) {
                    sprintf(text, txt_Ptrs[0x425c], ai_PokerState.seat[1].who->vname, ai_PokerState.seat[1].who->nname);
                    io_PrintText(text);
                } else {
                    sprintf(text, txt_Ptrs[0x425d], ai_PokerState.seat[1].who->vname, ai_PokerState.seat[1].who->nname);
                    io_PrintText(text);
                }
                if (ai_PokerState.seat[2].dropped != -1) {
                    sprintf(text, txt_Ptrs[0x425c], ai_PokerState.seat[2].who->vname, ai_PokerState.seat[2].who->nname);
                    io_PrintText(text);
                } else {
                    sprintf(text, txt_Ptrs[0x425d], ai_PokerState.seat[2].who->vname, ai_PokerState.seat[2].who->nname);
                    io_PrintText(text);
                }
                if (d2_ClickedId != -1 && ai_PokerUi.buttons[3] == d2_PointerObj)
                    mask = ai_PokerStep();
                break;
            case 8:
                d2_SetForm(ai_PokerUi.window, 4);
                txt_Format(text, 0x4252, ai_PokerState.pot);
                io_PrintText(text);
                txt_Format(text, util_RandomU(3) + 0x4253, 0x4258);
                io_PrintText(text);
                if (ai_PokerState.actor != 0) {
                    txt_Format(text, 0x425a, ai_PokerState.bet);
                    io_PrintText(text);
                } else {
                    io_PrintText(txt_Ptrs[0x4259]);
                }
                ai_PokerSeatPanel(1, 5, ai_PokerState.seat[1].card);
                ai_PokerSeatPanel(2, 5, ai_PokerState.seat[2].card);
                ai_PokerSeatPanel(0, 5, ai_PokerState.seat[0].card);
                mask = ai_PokerStep();
                break;
            case 9:
                if (d2_ClickedId != -1) {
                    if (ai_PokerUi.buttons[0] == d2_PointerObj) {
                        if (ai_PokerState.actor == 0)
                            ai_PokerState.bet = gm_PlayerFactor(d2_GetObjValue(ai_PokerUi.betSlider), ai_OwnPlayer);
                        mask = ai_PokerStep();
                        ai_PokerSeatPanel(0, 5, ai_PokerState.seat[0].card);
                        ai_PokerSeatPanel(1, 5, ai_PokerState.seat[1].card);
                        ai_PokerSeatPanel(2, 5, ai_PokerState.seat[2].card);
                    } else if (ai_PokerUi.buttons[1] == d2_PointerObj) {
                        sv_PopRequest = 1;
                    }
                }
                break;
            case 10:
                ai_PokerStep();
                ai_PokerSeatPanel(0, 5, ai_PokerState.seat[0].card);
                ai_PokerSeatPanel(1, 5, ai_PokerState.seat[1].card);
                ai_PokerSeatPanel(2, 5, ai_PokerState.seat[2].card);
                d2_SetForm(ai_PokerUi.window, 4);
                if (ai_PokerState.winner == 0)
                    txt_Format(text, 0x4265, ai_PokerState.winRank + 0x4266, ai_PokerState.pot);
                else
                    txt_Format(text, 0x4264, ai_PokerState.seat[(unsigned short)ai_PokerState.winner].who->vname, ai_PokerState.seat[(unsigned short)ai_PokerState.winner].who->nname, ai_PokerState.winRank + 0x4266, ai_PokerState.pot);
                io_PrintText(text);
                break;
            case 11:
                if (d2_ClickedId != -1) {
                    if (ai_PokerUi.buttons[3] == d2_PointerObj)
                        sv_PopRequest = 1;
                }
                break;
            }
        } while (sv_RunFrame((int)ai_PokerGame, 0x777c7) != 0);
        ai_ClosePokerWindow();
        return 1;
    } else {
        form = d2_OpenForm(0, 0, "Misc\\Messagebox");
        if (form == 0)
            return 0;
        d2_CentreViewWindows(ai_PokerUi.window);
        txt_Format(text, 0x4239, 0x4238);
        io_PrintText(text);
        while (sv_RunFrame((int)ai_PokerGame, 0x777c7) != 0) {
            if (dm_L.rup != 0 || kb_Down[1] != 0)
                sv_PopRequest = 1;
        }
        d2_CloseView(form);
        return 0;
    }
}

/* 0x5dadf0, builds the panel of one poker seat (445 B): seat 0 is the player, 1 and 2 the opponents; n the number of
   cards to show, cards the seat's card array (ai_PokerState.seat[seat].card, indices into the deck). The panel's window
   (ai_PokerUi.panel[seat].win) becomes the current one and its text is cleared; the seat's title and names are printed
   with "%s %s %s" (the title text txt_Ptrs[0x114 + 10 * female + title], the first and the family name) into a centred
   label (font 0x54) at the window's top; the character's picture (the player's own, or the seat's model) is placed left
   of the cards. Then each of the n cards becomes a picture object 0x20 apart, stored in the panel's slot i: the card
   back (picture 0x775 + ai_CardBack) when the seat has folded (foldPhase >= 0), or while an opponent's first two
   cards are shown at the end of the game (winner 4); else the card's face (0x775 + the card index), and the player's
   first two cards are greyed (they are chosen for the exchange in phase 6). Void. Reasoned: the picture and text
   roles follow from the offsets (0x775 + index into d2_Pics, the label with the centred flag). @deep Body reconstructed whole from the disassembly (label, centred title text, portrait, then one picture per card: face, card back for a folded seat or an opponent's first two cards at the end, the player's first two greyed); 231 vs 230 instructions, no meaning difference left. asmdiff (49 instructions, hunks at 0x5dae76 / 0x5daf9e / 0x5db00a / 0x5db0d7): (1) the original reads the card-back constant 0x5ed1bc once per picture (mov ecx,[0x5ed1bc]; lea edx,[ecx+0x775] and the picture height index from the same ecx) while ours, with ai_CardBack as const volatile int (the original does not fold it), reads it twice in each of the two back blocks; a local copy (pic = ai_CardBack + 0x775) makes the compiler merge the two identical back blocks (size 704, the original keeps two copies), so the original's two blocks must differ in a way not visible here; (2) the title index scheduling (push edx before sbb ecx,ecx; and ecx,0xa / add ecx,0x114 order); (3) 6 order instructions in the loop increment (inc eax / add edx,ecx vs add edi,0x20). Tried: win / mid locals (the call in both seat branches without a window local fixed 130 instructions), branch order (seat != 0 first), pic local. */

void ai_PokerSeatPanel(int seat, int n, int *cards)
{
    char text[0x100];
    POKERWHO *who;
    int o, pic, x, w, h, i;

    d2_LastWindow = ai_PokerUi.panel[seat].win;
    io_PrintText("$C");
    w = d2_Wins[d2_LastWindow].w;
    h = d2_Wins[d2_LastWindow].h;
    x = (w - 0xc4) / 2;
    who = ai_PokerState.seat[seat].who;
    sprintf(text, "%s %s %s", txt_Ptrs[0x114 + (who->female != 0 ? 10 : 0) + who->title], who->vname, who->nname);
    o = d2_CreateWinText(0, 5, text, ai_PokerUi.panel[seat].win);
    d2_ObjsV[o].w = w;
    d2_ObjsV[o].centred = 1;
    d2_SetObjFont(o, 0x54);
    if (seat != 0) {
        pic = ai_PokerState.seat[seat].model;
        d2_WinOpenPic(x - 0x37, h / 2 - d2_PicsV[pic].h / 2, pic, ai_PokerUi.panel[seat].win);
    } else {
        pic = ((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->pic;
        d2_WinOpenPic(x - 0x37, h / 2 - d2_PicsV[pic].h / 2, pic, ai_PokerUi.panel[0].win);
    }
    for (i = 0; i < n; i++) {
        if (ai_PokerState.seat[seat].foldPhase >= 0) {
            ai_PokerUi.panel[seat].slot[i] = d2_WinOpenPic(x, h / 2 - d2_PicsV[ai_CardBack + 0x775].h / 2, ai_CardBack + 0x775, ai_PokerUi.panel[seat].win);
        } else if (seat != 0 && i < 2 && ai_PokerState.winner == 4) {
            ai_PokerUi.panel[seat].slot[i] = d2_WinOpenPic(x, h / 2 - d2_PicsV[ai_CardBack + 0x775].h / 2, ai_CardBack + 0x775, ai_PokerUi.panel[seat].win);
        } else {
            ai_PokerUi.panel[seat].slot[i] = d2_WinOpenPic(x, h / 2 - d2_PicsV[cards[i] + 0x775].h / 2, cards[i] + 0x775, ai_PokerUi.panel[seat].win);
            if (seat == 0 && i < 2)
                d2_ObjsV[ai_PokerUi.panel[0].slot[i]].greyed = 1;
        }
        x += 0x20;
    }
}

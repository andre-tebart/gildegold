/* 0x5d9f50, shows the poker bet slider (159 B): arg on, nothing happens for 0. The slider is made once, on the first
   call, in the form 5 of the poker window (ai_PokerUi.window): d2_WinOpenSlider, as wide as the window less 0xb4, 0x7b
   picture, flags 2, scale 0 .. 100) and stored in ai_PokerUi.betSlider; its range is the bet limits converted to the
   player's money (gm_ConvertMoney of ai_PokerState.minBet, ai_PokerState.maxBet and ai_PokerState.minBet again as the start value, step 1).
   Later calls enable / disable the slider (d2_EnableObject). The limits are the poker game's POKERGAME.minBet / maxBet (0x6f35a4 / 0x6f35a8). */

void ai_PokerBetControl(int on)
{
    int slider;

    if (on != 0) {
        if (ai_PokerUi.betSlider == -1) {
            d2_SetForm(ai_PokerUi.window, 5);
            slider = d2_WinOpenSlider(0, 0, d2_Wins[d2_LastWindow].w - 0xb4, 0, 100, 0x7b, 2, d2_LastWindow);
            ai_PokerUi.betSlider = slider;
            d2_SetObjValue5(ai_PokerUi.betSlider, gm_ConvertMoney(ai_PokerState.minBet, ai_OwnPlayer), gm_ConvertMoney(ai_PokerState.maxBet, ai_OwnPlayer), gm_ConvertMoney(ai_PokerState.minBet, ai_OwnPlayer), 1);
        } else {
            d2_EnableObject(ai_PokerUi.betSlider, on);
        }
    }
}

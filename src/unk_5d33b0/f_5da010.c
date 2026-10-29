/* 0x5da010, closes the poker window (24 B): closes the window's view (ai_PokerUi) and clears
   the handle (-1). */

void ai_ClosePokerWindow(void)
{
    d2_CloseView(ai_PokerUi.window);
    ai_PokerUi.window = -1;
}

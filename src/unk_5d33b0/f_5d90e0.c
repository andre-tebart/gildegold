/* 0x5d90e0, the curse target picker "Locations\zigeuner\blutrache" (900 B): param_1 the gypsy
   building. The time record (0x770ae0 / e4 / e8 / ec) is pushed 0x18 game minutes (gm_AddTime) and its
   value +0x15 + util_Random (2) is the first pick; the window (0x4741f0, 0x476f80, 0x475600,
   d2_SetForm (0 / 2 / 3)) gets the share slider 0x46a960 ((screen height - 300) / 2, 10, 300, 0, 100,
   0x75, 0x42, screen) and the victim slider 0x46a960 (0x57, 7, 200, 0, 100, 0x7b, 2, screen), the
   slider value 0x50d0b0 (param_1) - 1, the controls 0x52e350 (.., 0x144a998, 0x144a99c). Each pass
   (the clock 0x6333cc): a new target 0x5741d0 (param_1); when none, the time record is re-pushed and
   a random day 0x15 + util_Random (2) set, the share slider resets to 100; else the record's
   +0x52 / +0x56 / +0x5a / +0x5e time, the distance shares 0x4fe280 (record + 0x44, ..) twice and the
   __ftol share to the slider. The texts 0x515960 (0x41f5 / 0x41f4, class (2 if v < 4 else 3 + (v > 6)),
   v + 0x41fb, ..), (0x41f8, 0x41f4, record + 0xac + 0x41fb, ..) or (0x41f9, 0x41f4). The buttons
   enable 0x475e40 (.., none ? 1 : 0), 0x475aa0 (share slider, has target), 0x475aa0 (victim slider,
   none). The confirm 0x520f90 (.., class) -> 0x4c5f60 (.., -class), a message record (+1 = 0xa0) via
   0x4c4b70, 0x6f22a0 = 1. The pump 0x5309a0 (0x5d90e0, 0x677c7) ends with 0x4758f0 and 0x52d7a0.
   ROUGH SKELETON (layout 2026-10-24). */

int ai_CurseTargetWindow(int param_1)
{
    return 0;
}

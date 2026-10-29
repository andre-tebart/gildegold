/* 0x5d8c90, the "Fluch wirken" (curse) window "Locations\zigeuner\blutrache" 0x62dc68 (905 B):
   param_1 the gypsy building. When the player's record byte 0x770d4c [0x6f25b8 * 0x87] has bit 0x10,
   0x4c2760 (-7, 0, "RAUSWURF" 0x62dca0, -1) and 0x521410 (0x144a8c4, 0x144a8c8, 0) end it. Otherwise a
   request 0x5c1130 (record 0x14 dwords, +0 = 1, +4 = 0x400, +0x10 = 6, +0xc = 1, +8 = 0xa7fdfbb6,
   0x144a8cc) gives the target; the window (0x4741f0, 0x476f80, 0x475600, d2_SetForm (0 / 2)) shows the
   slider 0x46a960 (0x57, 7, 200, 0, 100, 0x7b, 2, screen) with 0x475cb0 (.., 0, 5, 0, 1) and the
   button 0x46bca0 (.., 6, 0x144a8e8 ..), d2_SetForm (3) and the controls 0x52e350 (.., 0x144a920,
   0x144a924). Each pass: the limit 0x50cfd0 (param_1) - 1 and the slider value 0x475980 (min), the
   number 0x5176d0 (buf, value + 0x41d0, *target), the texts 0x515960 (0x41d6, 0x41c1, value + 0x41c4,
   buf) and for another owner (0x6f25b8 != word param_1 + 0x27) 0x515960 (0x41d7, max (value, 1) *
   32000). A click on the button 0x521040 (.., value, 0x6f1bc5) sends the curse: 0x4c4a60 (.., 0x1cc,
   4, 0x10, 0), the money 0x4c4830 (.., max (value, 1) * 32000) for another owner, a message record
   (0x3e dwords: +1 = 0x9d, -1, +0x3a = 2, the player record, 0x770ae0 / e4 / e8 / ec, the target dword,
   the value, +0x48 = 0x62dda0) through 0x4c4b70, 0x5176d0 (0x41da, 0x41c1), sprintf
   "_WAHRSAGERIN_FLUCH_0%d" 0x62dc88 and 0x4c2760 (-7, 0, .., -1), then 0x521150 (buf, 0) and
   0x6f22a0 = 1. The cancel 0x6f22a0 = 1. The pump 0x5309a0 (0x5d8c90, 0x677c7) ends with 0x4758f0 and
   0x52d7a0.
   ROUGH SKELETON (layout 2026-10-24): the record fields and the slider/button constants above. */
/* batch 3: the gypsy / poker / particle stretch 0x5d8c90 .. 0x5dd3e0 */
void sv_ButtonRow(int f, int y, int *out, int n, ...); int sv_CheckGoodAmount(); int sv_Unk525220(); int sv_ClosePlayerBar();
struct GCLOCK;
int gm_ConvertMoney(int a, unsigned char k); int gm_PlayerFactor(int a, unsigned char k); int gm_Unk50cf60();
int gm_Unk50cfd0(); int gm_Unk50d0b0(); int ai_Unk5741d0(); int ai_Unk4a7140();
int ai_Unk4a74e0(); int gm_BuildingSkill(); float ai_NormalizeEffectScale(float x);

int ai_CurseWindow(int param_1)
{
    return 0;
}

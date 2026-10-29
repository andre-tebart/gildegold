/* 0x5d85e0, the gypsy blood revenge window "Locations\zigeuner\blutrache" 0x62dc68 (2588 B): when the
   character record byte 0x770d4c [0x6f25b8 * 0x87] has bit 8, 0x521410 (0x144a888, 0x144a88c, 0) is
   shown and the window returns. Otherwise the window (0x4741f0, 0x476f80 (0, 0), 0x475600, d2_SetForm
   (0 / 1), 0x52ea30 (screen 0x6ed938)) and five choice rows: the up to five character records
   0x50cf60 (param_1), 0x5be1d0 / 0x5be3b0 (.., 0x6ed938, 0x54, ..) with the control fields +0x4c,
   +0x1c0 = 3, +0x2e8 = the record id and +0x50 = visible; the total 0x4e7c40 (the record byte +0x166)
   / 3 + 2 goes to the texts 0x515960 (&0x602568, total) and 0x515960 (0x41b6, total). A click
   0x5fe710 == 0x520f90 (.., total) confirms: 0x5176d0 (0x41bf, ..), per chosen record 0x504e20 (.., 4),
   the __ftol share, 0x4c4b00 (.., -0x32 + share, 1), 0x4c5f60, 0x4c4a60 (.., 0x1cc, 4, 8, 0), and
   0x5a9470 (the chosen ids). Else a row click opens 0x5c1130 (the request 0x144a890 / 0x144a894) and
   stores the result id +4 in the row. The pump 0x5309a0 (0x5d85e0, 0x677c7) ends with 0x4758f0 and
   0x52d7a0.
   ROUGH SKELETON (layout 2026-10-24): the record array and the per-row request fields are comments;
   the five row records are 0xe dwords apart, the choice ids and the totals are kept as above. */

int ai_BloodRevengeWindow(int param_1)
{
    return 0;
}

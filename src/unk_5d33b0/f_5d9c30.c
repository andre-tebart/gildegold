/* 0x5d9c30, the gypsy production building listener (348 B): param_1 the building. On the flags
   0x6f231c (bit 0x200) the contacts contact_BETTELN, contact_PRODUKTION_ZIGEUNER,
   contact_ZUTATEN_SUCHEN_ZIGEUNER and (bit 0x400) contact_LAGER / contact_TRANSPORT /
   contact_PERSONALBUCH (ob_PERSONALBUCH) / contact_MEISTERBRIEF are created 0x52e1c0; the click
   0x6f22e4 dispatches to 0x584d70 (the "HANDEL_SUCHEN" helper), 0x58bd60, 0x5912c0, 0x5aab80
   (param_1, "ob_PERSONALBUCH"), 0x5af1a0, 0x5b26b0 (param_1, the dword +0x5d), 0x5c3200 (param_1) and
   0x5d9890, between 0x52e190 calls and the pump 0x5309a0 (0x5d9c30, 0x67fff).
   ROUGH (layout 2026-10-24): the id-to-contact order follows the string block; the exact flags are
   not read. */




int ai_Unk5d9c30(int param_1)
{
    return 0;
}

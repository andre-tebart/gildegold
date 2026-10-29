/* 0x5ba600, the teardown of the window 0x62c418 (32 B, ROUGH): 0x476c70, then 0x472ce0 (0x62c418) and its
   reset */

void ai_Unk5ba600(void)
{
    d2_ClearPointerItems();
    d2_RemoveWindow(U_I((unsigned)&full_data_62c418));
    U_I((unsigned)&full_data_62c418) = -1;
}

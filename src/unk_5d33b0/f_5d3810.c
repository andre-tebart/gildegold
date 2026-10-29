/* 0x5d3810, a message box (481 B): param_1 the text id (0x515960), param_2 the flags: 2 -> the
   "misc\Messagebox_Autosize", 0x10 -> _BIG, 0x20 -> _VERY_BIG, 0x100 -> _NONE_PERGA (pump form 1),
   else "misc\Messagebox"; bit 3 (8) replaces the pump flags with 0x717c6, bit 0x40 calls 0x4741a0 per
   frame and bit 2 (4) closes the box when 0x6333cc has advanced 500 past the start. The box is created
   with 0x4741f0 / 0x475600; the control list 0x46c490 walks the 0x6ed980 + 0x18 ids (the count +0x1c)
   and adds every control record 0xf8f080 + id * 0x2ec whose +0x18 is not 0x40 (0x46c560). The pump
   0x5309a0 runs with this function as its callback (0x46c6c0 per frame); a click id 0 (0x475600?) means
   yes -> 1, id 1 -> 0. 0x6f2f95 byte 1 marks the box open (a second box returns 0 at once).
   Returns 1 / 0.
   @deep meaning matches the original: the six messagebox names by flags 2/0x10/0x20/0x100, the 0x717c6 override, 0x46c490/0x46c560 control filter (!= 0x40), the pump loop (0x46c6c0, 0x4741a0 on 0x40, 0xf8a668 0/1 -> ret 1/0, 0x14673a0, the +500 ms timeout on 0x6333cc) and the 0x6f2f95 open flag all match 0x5d3810's disassembly; the loop bound is re-read each iteration now. 273 code hunks remain from codegen only: frame 0x0c vs 0x10 (a 4th original slot with no reader), register allocation (list ebp, flags ebx, name edi vs the original's esi loop index) and the store/scheduling order of the flag tests */

int d2_CreateRadioGroup(int n, ...); void d2_RadioAdd(int g, int obj); void d2_MenuNavigate(int g); void d2_RaiseViewWindows(int view); void d2_RadioClear(int g);

int ai_MessageBox(int param_1, int param_2)
{
    int win;
    int list;
    int id;
    int i;
    int n;
    int ret;
    int pump_flags;
    unsigned int start;
    char *name;

    start = (unsigned int)U_I((unsigned)&ai_Unk6333cc);
    ret = 0;
    if (U_B(((unsigned)&ai_GfxResolution + 0x1)) != 0)
        return 0;
    pump_flags = U_I((unsigned)&sv_FrameMask) & 0xffffffc7;
    if ((param_2 & 2) == 0) {
        if ((param_2 & 0x10) == 0) {
            if ((param_2 & 0x20) == 0) {
                if ((param_2 & 0x100) != 0) {
                    win = d2_OpenForm(0, 0, "misc\Messagebox_NONE_PERGA");
                    d2_SetForm(win, 1);
                    goto created;
                }
                name = "misc\Messagebox";
            } else {
                name = "misc\Messagebox_VERY_BIG";
            }
        } else {
            name = "misc\Messagebox_BIG";
        }
    } else {
        name = "misc\Messagebox_Autosize";
    }
    win = d2_OpenForm(0, 0, name);
created:
    if ((param_2 & 8) != 0)
        pump_flags = 0x717c6;
    io_PrintText(param_1);
    d2_CentreViewWindows(win);
    U_B(((unsigned)&ai_GfxResolution + 0x1)) = 1;
    list = d2_CreateRadioGroup(0);
    for (i = 0; i < *(short *)(U_I((unsigned)&d2_LastWindowPtr) + 0x1c); i++) {
        id = *(int *)(*(int *)(U_I((unsigned)&d2_LastWindowPtr) + 0x18) + i * 4);
        if (U_I(U_I((unsigned)&d2_Objs) + 0x18 + id * 0x2ec) != 0x40)
            d2_RadioAdd(list, id);
    }
    while (sv_RunFrame((int)ai_MessageBox, pump_flags) != 0) {
        d2_MenuNavigate(list);
        if ((param_2 & 0x40) != 0)
            d2_RaiseViewWindows(win);
        if (U_I((unsigned)&d2_ClickedId) == 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
            ret = 1;
        } else if (U_I((unsigned)&d2_ClickedId) == 1) {
            U_I((unsigned)&sv_PopRequest) = 1;
            ret = 0;
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if ((param_2 & 4) != 0 && start + 500 < (unsigned int)U_I((unsigned)&ai_Unk6333cc))
            U_I((unsigned)&sv_PopRequest) = 1;
    }
    U_B(((unsigned)&ai_GfxResolution + 0x1)) = 0;
    d2_RadioClear(list);
    d2_CloseView(win);
    return ret;
}

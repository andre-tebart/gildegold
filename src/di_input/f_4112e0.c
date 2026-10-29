/* 0x4112e0, drain the buffered keyboard events into kb_Down / kb_Pressed, remember the last key and repeat it while it is held */

void di_PollKeyboard(void)
{
    int count;
    DIDEVICEOBJECTDATA od;
    int hr;

    kb_Last = 0;
    while (1) {
        count = 1;
        hr = di_Keyboard->lpVtbl->GetDeviceData(di_Keyboard, sizeof(od), &od, &count, 0);
        if (hr < 0 || count == 0)
            break;
        if (od.dwData & 0x80) {
            if (di_Disabled == 0) {
                kb_Last = (unsigned char)od.dwOfs;
                kb_Down[kb_Last] = 1;
            }
        } else if (di_Disabled == 0) {
            kb_Down[od.dwOfs] = 0;
            kb_Pressed[od.dwOfs] = 1;
        }
    }
    if (kb_Last != 0) {
        kb_RepeatTime = tb_Calls + 9;
        kb_RepeatKey.b = kb_Last;
    }
    if (kb_RepeatKey.b != 0) {
        if (kb_Last == 0 && kb_Down[kb_RepeatKey.i & 0xff] != 0 && tb_Calls > kb_RepeatTime + 5) {
            kb_RepeatTime = tb_Calls;
            kb_Last = kb_RepeatKey.b;
        }
        if (kb_Down[kb_RepeatKey.i & 0xff] == 0) {
            kb_RepeatKey.b = 0;
            kb_RepeatTime = 0x4e729fff;
        }
    }
}

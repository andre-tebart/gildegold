/* 0x475e40, enables / disables an object: clickDisabled and greyed are set when on is 0; obj -1 is ignored */

void d2_SetObjEnabled(int obj, int on)
{
    if (obj != -1) {
        d2_Objs[obj].clickDisabled = (on == 0);
        d2_Objs[obj].greyed = (on == 0);
    }
}

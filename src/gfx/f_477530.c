/* 0x477530, ends the editing of the input field */

void d2_EndInputEdit(void)
{
    if (d2_FocusedInput != 0) {
        d2_Objs[D2CUR->obj].highlighted = 0;
        d2_FocusedInput = 0;
        if (d2_LimitsChanged != -1) {
            d2_LimitsChanged = -1;
            di_MaxY = d2_SavedMaxY;
            di_MinY = d2_SavedMinY;
        }
    }
}

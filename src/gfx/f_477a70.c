/* 0x477a70, mouse drag on a number input */

void d2_DragInput(void)
{
    int step;
    char fl;

    if (d2_FocusedInput != 0 && (D2CUR->flags & 2) != 0) {
        if (dm_L.lextra != 0 && D2CUR->value < D2CUR->maxval)
            D2CUR->value = D2CUR->maxval;
        if (dm_L.ldown != 0 && D2CUR->value != -1) {
            if (d2_DragActive == 0) {
                step = d2_InputStep(D2CUR);
                d2_DragActive = 1;
                d2_SavedMaxY = di_MaxY;
                d2_SavedMinY = di_MinY;
                d2_DragStartValue = D2CUR->value;
                d2_DragStartX = dm_L.x;
                d2_DragStartY = dm_L.y;
                fl = (char)D2CUR->flags;
                if (fl & 0x80) {
                    di_MaxY = ((d2_DragStartValue - D2CUR->minval) * 4) / step + d2_DragStartY;
                    di_MinY = ((d2_DragStartValue - D2CUR->maxval) * 4) / step + d2_DragStartY;
                }
                D2CUR->frameY = ((d2_DragStartValue - D2CUR->minval) * 4) / step + d2_DragStartY;
            }
            d2_DragInputValue(D2CUR, d2_DragStartValue, d2_DragStartY, dm_L.y);
        }
    }
}

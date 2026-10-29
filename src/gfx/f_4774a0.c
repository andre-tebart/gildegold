/* 0x4774a0, sets the value of a number input from the mouse drag (val at y0, now at y1) */

void d2_DragInputValue(D2INPUT *in, int val, int y0, int y1)
{
    int step, max, min;

    step = d2_InputStep(in);
    in->value = val - step * ((y1 - y0) / 4);
    val = in->value;
    max = in->maxval;
    if (val > max) {
        in->value = max;
    } else {
        min = in->minval;
        if (val < min) {
            if (((char)in->flags & 0x40) == 0 || val != -1 || min != 0 || max != 0)
                in->value = min;
            else
                in->value = -1;
        }
    }
    _itoa(in->value, in->text, 10);
}

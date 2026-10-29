/* 0x4760b0, sets the font of an object (and of its window / input) */

void d2_SetObjFont(int obj, int font)
{
    D2OBJ *o = &d2_Objs[obj];

    o->font = (short)font;
    switch (o->type) {
    case 0x40:
        d2_Wins[o->input].font = (short)font;
        break;
    case 0x41:
        d2_Inputs[o->input].font = font;
        break;
    }
}

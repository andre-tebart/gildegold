/* 0x5049e0, the seat object of the building class: class 5 by the byte +9 of the selected character (0x1e .. 0x21 -> the buildings 2 .. 5, the object 0x142); class 3 the object 0x115 of the building 1. 0 else */

int gm_Unk5049e0(char *b)
{
    int cls, p;
    char k;

    cls = gm_BuildingClass((unsigned char)*b);
    if (cls == 5) {
        k = ai_Chars[ai_PlayerId][9];
        if (k == 0x1e)
            p = 2;
        else if (k == 0x1f)
            p = 3;
        else if (k == ' ')
            p = 4;
        else {
            if (k != '!')
                return 0;
            p = 5;
        }
        cls = (int)gm_FirstBuilding(p);
        if (cls != 0)
            return (int)gm_Query(*(int *)(cls + 0x5d), 2, 6, 0, 0x142);
    } else if (cls == 3) {
        cls = (int)gm_FirstBuilding(1);
        return (int)gm_Query(*(int *)(cls + 0x5d), 2, 6, 0, 0x115);
    }
    return 0;
}

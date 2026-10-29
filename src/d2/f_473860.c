/* 0x473860, creates a text label on the screen */

int d2_CreateScreenText(short x, short y, char *text)
{
    int obj;
    short tw;

    obj = d2_NewObject();
    d2_Objs[obj].input = (int)mem_Alloc(0x101, "d2:ShowTxt");
    if ((char *)d2_Objs[obj].input == 0)
        return -1;
    strcpy((char *)d2_Objs[obj].input, text);
    d2_Objs[obj].font = d2_DefaultFont;
    d2_Objs[obj].type = 0x43;
    d2_Objs[obj].x = x;
    d2_Objs[obj].y = y;
    tw = d2_TextWidth(text, d2_Objs[obj].font);
    d2_Objs[obj].w = tw + 0x60;
    d2_Objs[obj].h = d2_FontHeight;
    d2_Objs[obj].drawFrames = 2;
    d2_Objs[obj].clipy0 = 0;
    d2_Objs[obj].clipy1 = d2_ScreenH;
    d2_Objs[obj].clipx0 = 0;
    d2_Objs[obj].clipx1 = d2_ScreenW;
    return obj;
}

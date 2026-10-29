/* 0x46be00, creates a slider widget */

int d2_CreateSlider(int x, int y, int kind, int unk)
{
    int id;

    id = d2_NewObject();
    d2_Objs[id].type = 9;
    d2_Objs[id].interactive = 1;
    d2_Objs[id].style = 3;
    d2_Objs[id].x = x;
    d2_Objs[id].h = d2_Pics[kind].h;
    d2_Objs[id].y = y;
    d2_Objs[id].pic = kind;
    d2_Objs[id].clipy0 = 0;
    d2_Objs[id].clipy1 = d2_ScreenH;
    d2_Objs[id].clipx0 = 0;
    d2_Objs[id].clipx1 = d2_ScreenW;
    d2_Objs[id].font = d2_DefaultFont;
    d2_Objs[id].data = d2_GetPic(kind);
    d2_SetCaption(id, (char *)unk);
    d2_AddRefcount(kind);
    return id;
}

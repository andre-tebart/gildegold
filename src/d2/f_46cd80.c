/* 0x46cd80, thunk */

int d2_CreatePicObj(x, y, pic)
short x;
short y;
int pic;
{
    return d2_OpenPic(x, y, pic);
}

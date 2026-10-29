/* 0x478890, adds a hit rectangle, returns its number or 0 if the table is full */

int d2_AddHitRect(short x, short y, short w, short h, int user)
{
    D2HIT *p;
    int i = d2_HitCount + 1;

    d2_HitCount = i;
    d2_HitUnderPointer = i;
    if (i > 0x40)
        return 0;
    p = &d2_HitRects[i];
    p->x = x;
    p->y = y;
    p->w = w;
    p->h = h;
    p->user = user;
    p->used = 1;
    return i;
}

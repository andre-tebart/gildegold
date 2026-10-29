/* 0x468c70, draws the saved background back for the shapes of a buffer and removes them */

void d2_RestoreShapes(D2BUF *buf)
{
    D2SHAPE *s;
    int i;

    if (buf->active && buf->loaded) {
        for (i = 0; i < 0x300; i++) {
            s = &d2_Shapes[i];
            if (s->owner == buf) {
                d2_BlitPart(s->x, s->y, s->w, s->h, buf->srcSurf, s->x, s->y, buf->dstSurf);
                memset(s, 0, sizeof(D2SHAPE));
            }
        }
    }
}

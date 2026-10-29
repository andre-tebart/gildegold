/* 0x4759f0, size of a picture object; 0 if it has none. The picture's per-frame offset table starts at
   byte 0x45 of the picture header; its entry for the object's input holds the frame's offset. */

int d2_GetObjPicSize(int obj, int *w, int *h)
{
    D2OBJ *o;
    char *p;
    int *frameOffsets;
    D2FRAME *f;

    if (obj != -1) {
        o = &d2_Objs[obj];
        if (o->type == 1 || o->type == 8 || o->type == 5) {
            p = (char *)d2_GetPic(o->pic);
            if (p) {
                if (o->type != 8 && o->type != 5) {
                    *w = ((D2PICHDR *)p)->w;
                    *h = ((D2PICHDR *)p)->h;
                    return 1;
                }
                frameOffsets = (int *)(p + 0x45);
                f = (D2FRAME *)(p + frameOffsets[o->input]);
                if (f) {
                    *w = f->w;
                    *h = f->h;
                }
                return 1;
            }
        }
    }
    return 0;
}

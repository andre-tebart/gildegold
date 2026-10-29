/* 0x4721e0, d2_SetBackgroundTexture (error string): sets (or removes) the background texture of a window */

void d2_SetBackgroundTexture(name, win)
char *name;
int win;
{
    char buf[1024];
    D2WIN *w = &d2_Wins[win];

    if (w->spr) {
        d3if_FreeSprite((int)w->spr);
        w->spr = 0;
    }
    if (name && *name) {
        w->spr = (D2TEX *)d3if_LoadSprite(name, 1);
        if (!w->spr) {
            sprintf(buf, "d2_SetBackgroundTexture(): Could not open %s.BMP name");
            err_Message(buf);
        }
        w->alpha = 0.5f;
    }
}

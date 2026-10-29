/* 0x4022e0, fades characters in or out over 50 time units */

void ch_UpdateFades(void)
{
    int i;
    CHARACTER *ch;
    float a;
    D3TEXFLAGS f;

    for (i = 0; i < 16; i++) {
        ch = ch_Fades[i].ch;
        if (ch) {
            a = ((float)ch_Time - ch_Fades[i].start) * 0.02f;
            if (!ch_Fades[i].in)
                a = 1.0f - a;
            a = max(a, 0.0f);
            a = min(a, 1.0f);
            a *= 255.0f;
            memset(&f, 0, sizeof(f));
            f.b[2] &= 0xfe;
            f.b[0] = (unsigned char)a;
            if (a != 255.0f && a != 0.0f) {
                d3_SetObjectAlpha(ch->obj, ch->obj->info, f);
                if (ch->transport)
                    d3_SetObjectAlpha(*ch->transport, ch->obj->info, f);
            }
            if (a == 0.0f || a == 255.0f) {
                ch->flags.fading = 0;
                f.b[0] = 0xff;
                d3_SetObjectAlpha(ch->obj, ch->obj->info, f);
                if (ch->transport)
                    d3_SetObjectAlpha(*ch->transport, (*ch->transport)->info, f);
                if (a == 0.0f)
                    ch_SetVisible(ch, 0);
                else if (a == 255.0f)
                    ch_SetVisible(ch, 1);
                ch_Fades[i].ch = 0;
            }
        }
    }
}

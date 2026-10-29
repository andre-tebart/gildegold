/* 0x40ac00, action callback 0x38: fades the character in (args[0] != 0) or out over 50 time units */

void ch_ActFade(CHACTION *a)
{
    CHARACTER *ch = a->ch;
    D3TEXFLAGS f;
    float t;
    CHARACTER *others[32];
    D3TEXFLAGS g;

    if (!a->count) {
        if (!ch->obj->info)
            goto done;
        if (ch_CurrentGroup != ch->group || ch_CurrentScriptGroup != ch->scriptGroup) {
            if (ch->group != -1)
                ch_SetVisible(ch, 0);
            else
                ch_SetVisible(ch, a->args[0]);
            goto done;
        }
        ch_SetVisible(ch, 1);
        memset(&g, 0, sizeof(g));
        g.b[2] &= 0xfe;
        g.b[0] = 0xff;
        d3_SetObjectAlpha(ch->obj, ch->obj->info, g);
        if (ch->transport)
            d3_SetObjectAlpha(*ch->transport, (*ch->transport)->info, g);
        if (ch->lowPolyBody)
            d3_SetObjectAlpha(ch->lowPolyBody, ch->lowPolyBody->info, g);
        CHACT_FSTART(a) = (float)(ch_Time - 1);
        ch->flags.all = ch->flags.all & ~CH_NOFADE | 0x40;
    }
    t = a->args[0] ? ((float)ch_Time - CHACT_FSTART(a)) * 0.02f : 1.0f - ((float)ch_Time - CHACT_FSTART(a)) * 0.02f;
    if (!(t > 0.0f))
        t = 0.0f;
    else if (!(t < 1.0f))
        t = 1.0f;
    if (ch->scene == d3_DefaultScene && a->args[0] && !(ch->flags.all & CH_NOFADE) && t < 0.1
        && ch_FindNear(ch, others, 30.0f)) {
        CHACT_FSTART(a) = (float)(ch_Time - 1);
        others[0]->flags.all |= CH_NOFADE;
        ch_InsertAction(ch, a, 0x3b, 75);
        return;
    }
    t *= 255.0f;
    memset(&f, 0, sizeof(f));
    f.b[2] &= 0xfe;
    f.b[0] = (unsigned char)t;
    if (t != 255.0f && t != 0.0f) {
        d3_SetObjectAlpha(ch->obj, ch->obj->info, f);
        if (ch->transport)
            d3_SetObjectAlpha(*ch->transport, (*ch->transport)->info, f);
        if (ch->lowPolyBody)
            d3_SetObjectAlpha(ch->lowPolyBody, ch->lowPolyBody->info, f);
    }
    if (t == 0.0f || t == 255.0f) {
        ch->flags.fading = 0;
        f.b[2] &= 0xfe;
        f.b[0] = 0xff;
        d3_SetObjectAlpha(ch->obj, ch->obj->info, f);
        if (ch->transport)
            d3_SetObjectAlpha(*ch->transport, (*ch->transport)->info, f);
        if (ch->lowPolyBody)
            d3_SetObjectAlpha(ch->lowPolyBody, ch->lowPolyBody->info, f);
        if (t == 0.0f)
            ch_SetVisible(ch, 0);
        else if (t == 255.0f)
            ch_SetVisible(ch, 1);
    done:
        ch_RemoveAction(a);
    }
}

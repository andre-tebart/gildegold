/* 0x46d220, draws the caption of an object (style bit 4: below it, bit 0x20: with a sprite behind) @deep draft (71 differing instructions of 131 vs 140; 155/400 bytes). spell.py dup-tail: statements in both arms (the form before this session: 116 differing, 150 instructions - reverted), pull after the branch (this form: 71, best; x/y are set in the style&4 arm and one d2_DrawTextBox follows), swap polarity has no non-empty source form here (the arms differ only in x/y). What is left reads as scheduling: the original hoists the shared argument pushes (caption/picW+0x60/h/surf) above the style&4 test and duplicates the 4-instruction epilogue in both arms (add esp,0x1c / pop edi / pop esi / pop ebp), and its register map is ebp/esi/edi where ours is ebx/esi/edi (the 46d223 prologue: ours pushes ebx, esi and takes o from [esp+0x14], the original ebp, esi, edi and o from [esp+0x18]). The reload entry at 46d223 names no source form: o is the parameter and every use already re-reads it through o; no local holds it. Readability passes Q1..Q7 (renamed this session from d2_Unk46d220). Left: the argument-push hoisting and the epilogue duplication - scheduling/register allocation (listing/c2prio/siblings). */

void d2_DrawObjCaption(o)
D2OBJ *o;
{
    int flags, x, y, font;

    if (o->caption[0] != 0 && (o->style & 0x40) == 0) {
        flags = 8;
        if (o->greyed)
            flags = 9;
        if (o->rightAligned)
            flags |= 0x10;
        if (o->textShadow)
            flags |= 4;
        if (o->style & 8)
            flags |= 0x20;
        x = o->x;
        if (o->captionX != -1)
            x += o->captionX;
        y = o->y;
        if (o->captionY != -1)
            y += o->captionY;
        else
            y -= 0x10;
        font = o->capfont;
        if (font == 0)
            font = d2_DefaultFont + 1;
        d2_SetFont(font);
        if (o->style & 0x20) {
            d2_UnlockSurface(d2_ScreenSurf);
            d3if_DrawSprite(d2_CaptionTex, o->picW / 2 + o->x, o->picH / 2 + o->y, 0,
                         (float)o->picW / (float)d2_CaptionTex->size,
                         (float)o->picH / (float)d2_CaptionTex->size, 0.5f);
            d2_LockSurface(d2_ScreenSurf);
        }
        if (o->style & 4) {
            x = o->x;
            y = o->y + o->h;
        }
        d2_DrawTextBox(x - 0x30, y, o->picW + 0x60, o->h, d2_ScreenSurf, o->caption, flags);
    }
}

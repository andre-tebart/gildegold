/* 0x479bf0, draws the outline of the w x h rectangle at (x, y) in the colour (r, g, b): the left / right columns,
   then the top / bottom rows, pixel by pixel through d2_PutPixelRGB; nothing at all when the blue byte is 0 or
   h < 1 (the d2.c window border with 0xff, 0xff, 0xff comes through here)
   @deep B3 close: 16 differing instructions of 92 (12 of 224 bytes), one register pair plus the stack slot it frees. Original: surf=esi, r=edi, g=ebp, and r's dead slot [esp+0x24] takes x-1+w (lea ecx,[edx+ecx-1]); ours: surf=esi, g=edi, r=ebp, x-1+w goes to g's slot [esp+0x28]. Two instructions at 0x479c11 / 0x479c15 (mov edi / mov ebp from r and g) swap the roles; everything else follows (the push pairs push ebp,edi vs edi,ebp at 0x479c3f, 0x479c52, 0x479c98, 0x479cab and the [esp+0x3c]/[esp+0x40] reload). Meaning read against 0x479bf0..0x479cd0: b loaded as a byte and tested < 1 first, h >= 1, then the h loop (two d2_PutPixelRGB calls per row) and the w loop (two per column), the x-1+w hoist and the h/w entry tests - same statements, same order, no meaning error. spell.py 0x479c11 / 0x479c98 dup-tail: no if/else arm to duplicate or pull (the body is two plain loops) and the 2-instruction 'tail' is exactly the r/g load pair; the branch-polarity form tried - early return 'if ((unsigned char)b < 1 || h < 1) return;' - compiles identical (12/224, 16 lines), reverted; the old note's param types / decl order / nested ifs / j removed stay without effect. The edi/ebp choice for two equally used unsigned int parameters is the allocator's tie-break -> deep (variant rounds). */

void d2_DrawRectOutline(int x, int y, int w, int h, unsigned int r, unsigned int g, unsigned int b, D2SURF *surf)
{
    int i, j;

    if ((unsigned char)b >= 1 && h >= 1) {
        for (i = 0; i < h; i++) {
            d2_PutPixelRGB(x, y + i, r, g, b, surf);
            d2_PutPixelRGB(x - 1 + w, y + i, r, g, b, surf);
        }
        for (i = 0; i < w; i++) {
            d2_PutPixelRGB(x + i, y, r, g, b, surf);
            d2_PutPixelRGB(x + i, y - 1 + h, r, g, b, surf);
        }
    }
}

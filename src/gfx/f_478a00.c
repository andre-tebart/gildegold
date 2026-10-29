/* 0x478a00, reads the pixels of a 24 bit bmp (bottom-up files flipped, bgr turned to rgb) to dst @deep B3 127 code hunks (93 of 132 instructions differ): register allocation only. Original: dst=ebx (loaded at 0x478a7e), i=edi, pad=ebp, ebx saved after the fopen guard at 0x478a20, w*3 re-materialised per iteration (lea edx,[ecx+ecx*2] at 0x478ab2); ours: the w3 local owns ebx, i=ebp, pad=edi, ebx pushed in the prologue at 0x478a07, dst reloaded from the stack in the fread address. Listing read end to end (fopen/fseek/fread/fclose, abs height, flip kept in the dead name slot, n = w*h, bgr->rgb swap): no meaning error left. spell.py entries tried, one edit/build each, all reverted as WORSE: (1) w3 inlined into the fread args -> 255/336; (2) early-return fopen guard -> 251/320 (size 320 vs 336); (3) flip as ternary y -> 191/336 (gets the register test but loses the push order). dup-tail: both fclose arms already duplicated in the source; push-pop: the callee-saved save position has no source lever left. Readability done: Q1 fixed, named from the body and caller (reads a 24 bit bmp's pixels into a surface buffer, flips bottom-up rows, bgr->rgb). Remaining: register assignment / re-materialisation, deep's toolbox. */

int d2_ReadBmpPixels(char *name, unsigned char *dst)
{
    FILE *f;
    BITMAPINFOHEADER bi;
    int i, y, flip, n;
    unsigned int pad;
    int w3;
    unsigned char *p, t;

    f = fopen(name, "rb");
    if (f != 0) {
        fseek(f, 0xe, 0);
        fread(&bi, 1, 0x28, f);
        fseek(f, bi.biClrUsed * 4 + 0x36, 0);
        if (bi.biCompression == 0 && bi.biPlanes == 1 && bi.biBitCount == 0x18) {
            flip = bi.biHeight >= 0;
            bi.biHeight = abs(bi.biHeight);
            i = 0;
            w3 = bi.biWidth * 3;
            if (bi.biHeight > 0) {
                pad = w3 & 3;
                do {
                    y = i;
                    if (flip)
                        y = bi.biHeight - i - 1;
                    fread(dst + y * bi.biWidth * 3, 1, w3, f);
                    if (pad != 0)
                        fseek(f, 4 - pad, 1);
                    i++;
                } while (i < bi.biHeight);
            }
            fclose(f);
            n = bi.biWidth * bi.biHeight;
            if (n > 0) {
                p = dst + 2;
                do {
                    t = p[-2];
                    p[-2] = *p;
                    *p = t;
                    p += 3;
                    n--;
                } while (n != 0);
                return 1;
            }
        } else {
            fclose(f);
        }
        return 1;
    }
    return 0;
}

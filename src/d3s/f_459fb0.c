/* 0x459fb0, steps down the rows of a triangle (rows of w pixels) along the edges d3s_XL / d3s_XR (left /
   right x with slopes d3s_SlopeL / d3s_SlopeR). Fills the byte buffer with the interpolated value (flags & 2) and
   marks the span (flags & 5: value 0xb for the border when flags & 1 is clear), widening it by d3s_ShadowRadius rows
   and pixels around.
   @deep B code hunks 91: the same operations in other registers / stack slots (span pointer in ebx vs memory, the hoisted neg of w from j * w, the [ebp-0x14..-0x24] homes swapped); no missing statement, constant or call - the 24 parameter orders and 16 loop-step orders were tried (notes/units/d3s.md [shfix]) */

/* the span loop of d3s_DrawSpans: one byte per pixel from the 16.16 value u, which grows by step; the carry of the
   low half is carried into the next add (adc), the byte is the middle of the value (ror 16) */
__inline void d3s_Span(unsigned int u, int n, unsigned char *dst, int step)
{
    __asm mov eax, u
    __asm mov edx, n
    __asm mov ebx, dst
    __asm mov ecx, step
    __asm ror eax, 16
    __asm ror ecx, 16
    __asm clc
    __asm d3s_span_loop: mov [ebx], al
    __asm adc eax, ecx
    __asm inc ebx
    __asm dec edx
    __asm jne d3s_span_loop
}

void d3s_DrawSpans(int rows, int w, unsigned char flags, char top, int y0)
{
    int x0, row, i, j, e, t, lim, span, yy, k;
    int fill2, fill1, lastx;
    D3SPANPIX *base;
    char drawn, marked;

    fill2 = 0;
    fill1 = 0;
    drawn = 0;
    marked = (flags & 5) != 0;
    if (marked && !(flags & 1))
        fill2 = fill1 = 0xb;
    lastx = -1;
    row = 0;
    if (rows > 0) {
        do {
            x0 = d3s_XL + 0xffff >> 16;
            if ((d3s_SpanLen = (d3s_XR + 0xffff >> 16) - x0) > 0) {
                base = d3s_SpanRow;
                lastx = x0;
                span = d3s_SpanLen;
                if (flags & 2)
                    d3s_Span(d3s_CL + d3s_MulShr16((x0 << 16) - d3s_XL, d3s_Area), d3s_SpanLen, d3s_Dst + x0, d3s_Area);
                if (marked) {
                    if ((k = d3s_ShadowRadius) > 0) {
                        d3s_MarkSpan(fill1, min(d3s_SpanLen + k * 2, lim = w - x0),
                                      (int *)(d3s_SpanRow + max(x0 - k, 0)));
                        if (top && !drawn) {
                            j = d3s_ShadowRadius;
                            if (j > 0) {
                                i = j * w;
                                e = row - j + y0;
                                do {
                                    if (e > 0) {
                                        t = d3s_ShadowRadius - j;
                                        d3s_MarkSpan(fill1, min(d3s_SpanLen + t * 2, lim),
                                                      (int *)(d3s_SpanRow + (max(x0 - t, 0) - i)));
                                    }
                                    j--;
                                    e++;
                                    i -= w;
                                } while (j > 0);
                            }
                            drawn = 1;
                        }
                    }
                    d3s_MarkSpan(fill2, d3s_SpanLen, (int *)(d3s_SpanRow + x0));
                }
            }
            d3s_XR += d3s_SlopeR;
            d3s_XL += d3s_SlopeL;
            d3s_CL += d3s_SlopeC;
            d3s_Dst += w;
            d3s_SpanRow += w;
            row++;
        } while (row < rows);
        if (lastx >= 0 && marked) {
            j = d3s_ShadowRadius;
            if (j > 0) {
                yy = j + rows + y0;
                do {
                    if (yy <= w) {
                        t = d3s_ShadowRadius - j;
                        d3s_MarkSpan(fill1, min(span + t * 2, w - lastx), (int *)(base + (max(lastx - t, 0) + j * w)));
                    }
                    yy--;
                    j--;
                } while (j > 0);
            }
        }
    }
}

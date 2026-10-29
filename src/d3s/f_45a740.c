/* 0x45a740, fills n scanlines between d3s_XL and d3s_XR with d3s_Fill */

void d3s_FillRows(int n)
{
    int x, i;
    char *p;

    if (d3s_Byte) {
        for (i = 0; i < n; i++) {
            x = d3s_XL + 0xffff >> 16;
            if ((d3s_SpanLen = (d3s_XR + 0xffff >> 16) - x) > 0) {
                p = (char *)(d3s_Dst + x);
                __asm mov edi, p
                __asm mov ecx, d3s_SpanLen
                __asm mov eax, d3s_Fill
                __asm rep stosb
            }
            d3s_XL += d3s_SlopeL;
            d3s_XR += d3s_SlopeR;
            d3s_Dst += d3_Pitch8;
        }
    } else {
        for (i = 0; i < n; i++) {
            x = d3s_XL + 0xffff >> 16;
            if ((d3s_SpanLen = (d3s_XR + 0xffff >> 16) - x) > 0) {
                p = (char *)(d3s_Dst16 + x);
                __asm mov edi, p
                __asm mov ecx, d3s_SpanLen
                __asm mov eax, d3s_Fill
                __asm rep stosw
            }
            d3s_XL += d3s_SlopeL;
            d3s_XR += d3s_SlopeR;
            d3s_Dst16 += d3_Pitch16;
        }
    }
}

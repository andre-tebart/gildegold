/* 0x43eab0, blends n x n pixels of the four corner textures src[0..3] with the weights w into dst @deep B: read all 5 hunks (asmdiff: frame sub esp 0x24 vs 0x30, slots 43ead8/43eb3e, code 43eb51 + 43ed22 = the two loops): the operations, constants, loop structure and the four source pointers match (208/214 instructions equal); the differences are the register roles of s0..s3 / the weight bytes (esi/ebx/edi/ebp swapped) and the stack slots of d, ww and the loop counters, plus 12 bytes more locals in ours (the a,b,c,e weight locals are materialized in ours) - the same operation in another register/slot spelling, no missing statement or wrong constant/field; readable already */
void d3fl_BlendC(unsigned short *dst, unsigned long *w, unsigned char **src, unsigned int n)
{
    unsigned int i, j;
    unsigned short *d;
    unsigned char *s, *s0, *s1, *s2, *s3;
    unsigned long *ww, wv;
    int a, b, c, e;

    if (src[0] == src[1] && src[1] == src[2] && src[2] == src[3]) {
        for (j = 0; j < n; j++) {
            s = src[0];
            d = dst;
            for (i = 0; i < n; i++) {
                *d++ = (unsigned short)d3fl_ColorTab[2][s[2]] + (unsigned short)d3fl_ColorTab[1][s[1]] + (unsigned short)d3fl_ColorTab[0][s[0]];
                s += d3fl_SrcStep;
            }
            dst = (unsigned short *)((char *)dst + d3fl_DstPitch);
            src[0] += d3fl_SrcPitch;
        }
    } else {
        for (j = 0; j < n; j++) {
            s0 = src[0];
            s1 = src[1];
            s2 = src[2];
            s3 = src[3];
            d = dst;
            ww = w;
            for (i = 0; i < n; i++) {
                wv = *ww;
                a = wv & 0xff;
                b = wv >> 8 & 0xff;
                c = wv >> 16 & 0xff;
                e = wv >> 24;
                ww = (unsigned long *)((char *)ww + d3fl_SrcStep);
                *d++ = (unsigned short)d3fl_ColorTab[2][(s0[2] * a + s1[2] * b + s2[2] * c + s3[2] * e) >> 8 & 0xff]
                     + (unsigned short)d3fl_ColorTab[1][(s0[1] * a + s1[1] * b + s2[1] * c + s3[1] * e) >> 8 & 0xff]
                     + (unsigned short)d3fl_ColorTab[0][(s0[0] * a + s1[0] * b + s2[0] * c + s3[0] * e) >> 8 & 0xff];
                s0 += d3fl_SrcStep;
                s1 += d3fl_SrcStep;
                s2 += d3fl_SrcStep;
                s3 += d3fl_SrcStep;
            }
            dst = (unsigned short *)((char *)dst + d3fl_DstPitch);
            src[0] += d3fl_SrcPitch;
            w = (unsigned long *)((char *)w + d3fl_WPitch);
            src[1] += d3fl_SrcPitch;
            src[2] += d3fl_SrcPitch;
            src[3] += d3fl_SrcPitch;
        }
    }
}

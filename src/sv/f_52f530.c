/* 0x52f530, closes a book: frees the model (0x411b70, back to the parent 0x447190 when it had one), the page widgets (0x474d10 / 0x4758f0), the two buttons, restores 0x437060 and plays "Zuklappen_s" */

void sv_Unk52f530(int *bk)
{
    int i, *p;

    if (bk != 0) {
        d3t_ReleaseObjectTextures(bk[0]);
        if (bk[1] != 0) {
            d3if_Show(bk[1], 1);
            d3if_Delete((struct D3OBJECT *)bk[0]);
            bk[0] = bk[1];
        }
        i = 0;
        if (0 < *((char *)bk + 0x271)) {
            p = bk + 0x8a;
            do {
                d2_ClearViewTextures(*p);
                d2_CloseView(*p);
                i++;
                p++;
            } while (i < *((char *)bk + 0x271));
        }
        if (bk[0x9a] != -1)
            d2_RemoveObj(bk[0x9a]);
        if (bk[0x9b] != -1)
            d2_RemoveObj(bk[0x9b]);
        if (*((char *)bk + 0x272) != 0)
            d3d_SetWBuffer(1);
        mem_Free(bk);
        sb_PlaySample(SV_I((unsigned)&full_data_6f240c), "Zuklappen_s", 1, 0x3f, 0x7f, 0, 0);
    }
}

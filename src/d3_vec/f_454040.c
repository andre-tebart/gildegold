/* 0x454040, transpose */

void d3_MatTranspose(D3MATRIX *src, D3MATRIX *dst)
{
    int i, j;

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            ((float *)dst)[i * 4 + j] = ((float *)src)[j * 4 + i];
}

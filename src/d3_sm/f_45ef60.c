/* 0x45ef60, frees a height map with its entries and data; prints "d3sm_Destroy(shadow)" when it is the
   global d3sm_Shadow map */

void d3sm_Destroy(D3SM *h)
{
    if (h) {
        if (h == d3sm_Shadow)
            OutputDebugString("d3sm_Destroy(shadow)\n");
        else
            OutputDebugString("d3sm_Destroy\n");
        if (h->entries) {
            mem_Free(h->entries);
            h->entries = 0;
        }
        if (h->data) {
            mem_Free(h->data);
            h->data = 0;
        }
        mem_Free(h);
    }
}

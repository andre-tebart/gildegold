/* 0x45eeb0, size < 0: shadow map (no ground types) */

D3SM *d3sm_Create(D3TERRAIN *t, int size, int mask)
{
    D3SM *h, *ret = 0;

    if (size) {
        h = mem_Alloc(sizeof(D3SM), "d3_sm:Create");
        h->mask = mask;
        h->size = abs(size);
        h->data = mem_Alloc(h->size * h->size, "d3_sm:CreateHeight");
        if (size > 0)
            h->entries = mem_Alloc(h->size * h->size * sizeof(D3SMENTRY), "d3_sm:CreateEntries");
        else
            h->entries = 0;
        if (h->data)
            d3sm_Build(h, t);
        if (size < 0) {
            OutputDebugString("d3sm_Create(shadow)\n");
            return h;
        }
        OutputDebugString("d3sm_Create\n");
        ret = h;
    }
    return ret;
}

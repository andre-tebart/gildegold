/* 0x43b300, sorts the n entries of d3_SortBuf by key (radix sort on the low two or all four bytes,
   ping-ponging through d3_SortTmp). */

void d3_RadixSort(unsigned int n, unsigned char twobytes)
{
    if (n > 1) {
        d3_RadixSrc = d3_SortBuf;
        d3_RadixDst = d3_SortTmp;
        D3_ZERO_AT(offset d3_RadixCount, 0x400);
        D3E_RADIX(n, 0);
        d3_RadixSrc = d3_SortTmp;
        d3_RadixDst = d3_SortBuf;
        D3_ZERO_AT(offset d3_RadixCount, 0x400);
        D3E_RADIX(n, 1);
        if (!twobytes) {
            d3_RadixSrc = d3_SortBuf;
            d3_RadixDst = d3_SortTmp;
            D3_ZERO_AT(offset d3_RadixCount, 0x400);
            D3E_RADIX(n, 2);
            d3_RadixSrc = d3_SortTmp;
            d3_RadixDst = d3_SortBuf;
            D3_ZERO_AT(offset d3_RadixCount, 0x400);
            D3E_RADIX(n, 3);
        }
    }
}

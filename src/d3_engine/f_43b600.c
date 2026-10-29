/* 0x43b600, draws the sort buffer: sorts it back to front, calls the D3D or the software draw and
   resets the frame counters (sort buffer, triangle / vertex counts). */

void d3_DrawSorted(void)
{
    d3_ClipExtraTris = 0;
    if (d3_Hardware) {
        d3_RadixSort(d3_SortCount, 1);
        d3_DrawSortedD3D();
    } else {
        d3_RadixSort(d3_SortCount, 0);
        d3_DrawSortedSW();
    }
    d3_SortCount = 0;
    d3_TriCount = 0;
    d3_VertCount = 0;
    d3_DrawnVerts = 0;
    d3_SortPtr = d3_SortBuf;
}

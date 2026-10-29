/* 0x44f870, reads a BGF file into a description */

/* 0x5fc6f8 */
static BGIENT bgi_TabMat[16] = {
    {6, bgi_SkipInt, 0}, {7, bgi_Bitmap0, 0}, {9, bgi_Bitmap1, 0}, {8, bgi_Bitmap2, 0},
    {10, bgi_MatKind, 0}, {0x26, bgi_MatC1, 0}, {0xb, bgi_MatFlags1, 0}, {0xc, bgi_MatFlags2, 0},
    {0xd, bgi_MatC8, 0}, {0xe, bgi_MatC9, 0}, {0xf, bgi_MatSu, 0}, {0x10, bgi_MatSv, 0},
    {0x11, bgi_MatOu, 0}, {0x12, bgi_MatOv, 0}, {0x13, bgi_MatRot, 0}, {0x28, bgi_NextMat, 0}
};

/* 0x5fc7b8 */
static BGIENT bgi_TabMats[3] = {
    {4, bgi_NumMats, 0}, {5, bgi_TabMat, 0x10}, {0x28, 0, 0}
};

/* 0x5fc7dc */
static BGIENT bgi_TabPoly[5] = {
    {0x1d, bgi_PolyVerts, 0}, {0x1e, bgi_PolyUV, 0}, {0x1f, bgi_SkipInt3, 0}, {0x20, bgi_PolyMat, 0},
    {0x28, 0, 0}
};

/* 0x5fc818 */
static BGIENT bgi_TabGeom[6] = {
    {0x18, bgi_Unk04, 0}, {0x19, bgi_NumPoints, 0}, {0x1a, bgi_NumPolys, 0}, {0x1b, bgi_Points, 0},
    {0x1c, bgi_TabPoly, 5}, {0x28, agi_EndObject, 0}
};

/* 0x5fc860 */
static BGIENT bgi_TabObj[4] = {
    {0x15, bgi_SkipStr, 0}, {0x16, bgi_SkipInt2, 0}, {0x17, bgi_TabGeom, 6}, {0x28, 0, 0}
};

/* 0x5fc890 */
static BGIENT bgi_TabTop[7] = {
    {2, bgi_Flags, 0}, {1, bgi_SkipInt, 0}, {0, bgi_SkipDword, 0}, {3, bgi_TabMats, 3},
    {0x14, bgi_TabObj, 4}, {0x37, bgi_Attach, 0}, {0x28, 0, 0}
};
int bgi_Depth = 0;  /* 0x633b00 */

void d3_ReadBGF(void *f, D3BGFDESC *desc)
{
    int magic, n;

    vfs_fread(&magic, 4, 1, f);
    if (bgi_ReadId(f) == 0x2e) {
        bio_rd_i(f, &n);
        bgi_Parse(f, bgi_TabTop, 7, (BGIRD *)desc);
    }
}

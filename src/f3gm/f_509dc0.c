/* 0x509dc0 */

int gm_Unk509dc0(int unused, int kind)
{
    char buf[0x24];
    int v;

    v = gm_BuildingKindValue(kind);
    gv_GetTableRecord(6, buf);
    return v;
}

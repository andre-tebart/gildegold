/* 0x479ee0, frees the light table at shutdown unless it is external (d2_LightTableExternal); clears the
   handle */

void d2_FreeLightTable(void)
{
    if (d2_LightTableExternal == 0 && d2_ShpLightTable != 0) {
        mem_Free(d2_ShpLightTable);
        d2_ShpLightTable = 0;
    }
}

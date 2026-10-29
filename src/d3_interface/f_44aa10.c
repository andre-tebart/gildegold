/* 0x44aa10, writes the scene (into f, or into the new file name) */

void d3_SaveScene(char *name, VFSFILE *f)
{
    VFSFILE *out;
    D3OBJECT *o;
    int i, k, n;

    out = f;
    if (f || (out = vfs_Tfopen(name, "wb", (int)vfs_Root))) {
        bio_wr_i(out, 0x3a6c00bb);
        bio_wr_s(out, (char *)d3if_MegaCam);
        bio_wr_i(out, ((D3IFBITS *)&d3if_Gray)->bits);
        bio_wr_v3(out, (int *)&d3o_Amb);
        bio_wr_v3(out, (int *)&d3o_Dir);
        bio_wr_i(out, ((D3IFSCENE *)d3_DefaultScene)->mapSize);
        bio_wr_i(out, d3_FogColor);
        bio_wr_i(out, ((D3IFBITS *)&d3_FogStart1)->bits);
        bio_wr_i(out, ((D3IFBITS *)&d3_FogEnd2)->bits);
        for (k = 0; k < 7; k++) {
            bio_wr_v3(out, (int *)&d3o_Sets[k].amb);
            bio_wr_v3(out, (int *)&d3o_Sets[k].dir);
            for (i = 0; i < 6; i++) {
                bio_wr_i(out, ((D3IFBITS *)&d3o_Sets[k].fog[i].col)->bits);
                bio_wr_i(out, ((D3IFBITS *)&d3o_Sets[k].fog[i].start)->bits);
                bio_wr_i(out, ((D3IFBITS *)&d3o_Sets[k].fog[i].end)->bits);
            }
        }
        n = 0;
        for (o = d3_TopFirst; o != &d3_ObjEnd; o = d3_NextGroup(o))
            n++;
        bio_wr_i(out, n);
        for (o = d3_TopFirst; o != &d3_ObjEnd; o = d3_NextGroup(o))
            d3_WriteObject(out, o);
        bio_wr_c(out, d3_Terrain != 0);
        if (d3_Terrain)
            d3_WriteMap(out, d3_Terrain);
        d3e_SaveEvents(out, ((D3IFSCENE *)d3_DefaultScene)->events);
        if (!f)
            vfs_fclose(out);
    }
}

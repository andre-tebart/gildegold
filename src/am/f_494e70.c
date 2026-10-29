/* 0x494e70, loads the action table from /gamedata/ai/ai_data_2.dfn (or, with ai_Unk6f2464 set, registers
   the actions from ai_method_addon.ini and writes the table) */

int ai_LoadActionTable(void)
{
    unsigned int n;
    int hdr[2];
    G5ACT r;
    G5ACT *act;
    void *f;
    unsigned int i;

    memset(ai_Unkf83ec0, 0, 0x49 * sizeof(G5ACT));
    n = 0x49;
    hdr[0] = 0x6144466e;
    if (ai_Unk6f2464 == 0) {
        f = vfs_Tfopen("/gamedata/ai/ai_data_2.dfn", "rb", vfs_Root);
        if (f == 0)
            return 0;
        if (ai_Unk4a16f0(ai_Unk6f2464) != 0 && ai_Unk490c10(ai_Unk6f2464) != 0 &&
            ai_Unk48fb70(ai_Unk6f2464) != 0 && ai_Unk4abed0(ai_Unk6f2464) != 0 &&
            vfs_fread(hdr, 4, 1, f) != 0) {
            if (hdr[0] != 0x6144466e)
                return 0;
            if (vfs_fread(&n, 4, 1, f) != 0) {
                for (i = 0; i < n; i++) {
                    if (vfs_fread(&r.id, 1, 1, f) == 0 || vfs_fread(r.name, 0x20, 1, f) == 0 ||
                        vfs_fread(&r.w1[0].k, 1, 1, f) == 0 || vfs_fread(&r.w1[0].w, 4, 1, f) == 0 ||
                        vfs_fread(&r.w1[1].k, 1, 1, f) == 0 || vfs_fread(&r.w1[1].w, 4, 1, f) == 0 ||
                        vfs_fread(&r.w1[2].k, 1, 1, f) == 0 || vfs_fread(&r.w1[2].w, 4, 1, f) == 0 ||
                        vfs_fread(&r.w1[3].k, 1, 1, f) == 0 || vfs_fread(&r.w1[3].w, 4, 1, f) == 0 ||
                        vfs_fread(&r.w2[0].k, 1, 1, f) == 0 || vfs_fread(&r.w2[0].w, 4, 1, f) == 0 ||
                        vfs_fread(&r.w2[1].k, 1, 1, f) == 0 || vfs_fread(&r.w2[1].w, 4, 1, f) == 0 ||
                        vfs_fread(&r.w2[2].k, 1, 1, f) == 0 || vfs_fread(&r.w2[2].w, 4, 1, f) == 0 ||
                        vfs_fread(&r.w2[3].k, 1, 1, f) == 0 || vfs_fread(&r.w2[3].w, 4, 1, f) == 0)
                        goto fail;
                    memcpy(r.w3, r.w1, 0x20);
                    if (r.id < 0x49) {
                        act = &G5_ACTS[r.id];
                        act->id = r.id;
                        strcpy(act->name, r.name);
                        memcpy(act->w1, r.w1, 0x20);
                        memcpy(act->w2, r.w2, 0x20);
                        memcpy(act->w3, r.w3, 0x20);
                    }
                }
            }
            vfs_fclose(f);
            return 1;
        }
    } else {
        hdr[1] = 0x49;
        GetModuleFileNameA(main_Instance, ai_Unkf83da0, 0x104);
        strrchr(ai_Unkf83da0, '\\')[1] = 0;
        strcat(ai_Unkf83da0, "ai_method_addon.ini");
        if (ai_Unk4a16f0(ai_Unk6f2464) == 0)
            return 0;
        if (ai_Unk490c10(ai_Unk6f2464) == 0)
            return 0;
        if (ai_Unk48fb70(ai_Unk6f2464) == 0)
            return 0;
        if (ai_Unk4abed0(ai_Unk6f2464) == 0)
            return 0;
        f = vfs_Tfopen("/gamedata/ai/ai_data_2.dfn", "wb", vfs_Root);
        if (f == 0)
            return 0;
        if (vfs_fwrite(&hdr[0], 4, 1, f) != 0 && vfs_fwrite(&hdr[1], 4, 1, f) != 0) {
            for (act = G5_ACTS; vfs_fwrite(&act->id, 1, 1, f) != 0 && vfs_fwrite(act->name, 0x20, 1, f) != 0 &&
                 vfs_fwrite(&act->w1[0].k, 1, 1, f) != 0 && vfs_fwrite(&act->w1[0].w, 4, 1, f) != 0 &&
                 vfs_fwrite(&act->w1[1].k, 1, 1, f) != 0 && vfs_fwrite(&act->w1[1].w, 4, 1, f) != 0 &&
                 vfs_fwrite(&act->w1[2].k, 1, 1, f) != 0 && vfs_fwrite(&act->w1[2].w, 4, 1, f) != 0 &&
                 vfs_fwrite(&act->w1[3].k, 1, 1, f) != 0 && vfs_fwrite(&act->w1[3].w, 4, 1, f) != 0 &&
                 vfs_fwrite(&act->w2[0].k, 1, 1, f) != 0 && vfs_fwrite(&act->w2[0].w, 4, 1, f) != 0 &&
                 vfs_fwrite(&act->w2[1].k, 1, 1, f) != 0 && vfs_fwrite(&act->w2[1].w, 4, 1, f) != 0 &&
                 vfs_fwrite(&act->w2[2].k, 1, 1, f) != 0 && vfs_fwrite(&act->w2[2].w, 4, 1, f) != 0 &&
                 vfs_fwrite(&act->w2[3].k, 1, 1, f) != 0 && vfs_fwrite(&act->w2[3].w, 4, 1, f) != 0; ) {
                act++;
                if (act >= G5_ACTS + 0x49) {
                    vfs_fclose(f);
                    return 1;
                }
            }
        }
    }
fail:
    vfs_fclose(f);
    return 0;
}

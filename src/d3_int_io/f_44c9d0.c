/* 0x44c9d0, the file name(s) of a stock object's LOD: lod < 0 the "_s" variant, lod 0 the best one found
   (mode 2) or the base name, else "name_<lod - 1>" */

char d3io_LodName(char *name, char *name2, int lod, char *out, char *out2)
{
    if (lod < 0) {
        if (d3io_Lod.shadow) {
            strcpy(out, name);
            strcat(out, "_s");
            if (name2 && out2) {
                strcpy(out2, name2);
                strcat(out2, "_s");
            }
            return 1;
        }
    } else if (lod == 0) {
        if (d3io_Lod.mode == 2) {
            for (lod = 2; lod >= 0; lod--) {
                if (lod > 0) {
                    sprintf(out, "%s_%i", name, lod - 1);
                    if (name2 && out2)
                        sprintf(out2, "%s_%i", name2, lod - 1);
                } else {
                    strcpy(out, name);
                    if (name2 && out2)
                        strcpy(out2, name2);
                }
                if (d3io_StockFile(out, ".bgf"))
                    return 1;
            }
        } else {
            strcpy(out, name);
            if (name2 && out2)
                strcpy(out2, name2);
            return 1;
        }
    } else {
        int k = d3io_Lod.mode == 2 ? 2 - lod : lod;
        sprintf(out, "%s_%i", name, k - 1);
        if (name2 && out2)
            sprintf(out2, "%s_%i", name2, k - 1);
        return 1;
    }
    return 0;
}

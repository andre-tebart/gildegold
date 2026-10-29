/* 0x460de0, copies the surfaces of a texture to its aliases */

void ts_SyncAliases(TSTEXTURE *tex)
{
    TSTEXTURE *t;
    unsigned int i;
    int index;

    if (!tex->alias && tex->name[0] != '*') {
        index = tex - d3_Textures;
        for (i = 0, t = d3_Textures; i < d3_TexCount; i++, t++) {
            if (t->refs > 0 && t->alias && t->mask == index) {
                t->surf = tex->surf;
                t->d3dtex = tex->d3dtex;
                t->data = tex->data;
                t->size = tex->size;
                t->origsize = tex->origsize;
            }
        }
    }
}

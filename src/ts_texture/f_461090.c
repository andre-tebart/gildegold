/* 0x461090, reloads all textures; cb(1, done, total) reports progress */

void tex_ReloadAll(char keep, void (*cb)(int, int, int))
{
    TSTEXTURE *t;
    unsigned int i;
    int n, total;

    total = 0;
    if (cb) {
        for (total = 0, i = 0, t = d3_Textures; i < d3_TexCount; i++, t++) {
            if (t->refs > 0 && !t->alias)
                total++;
        }
    }
    for (n = 0, i = 0, t = d3_Textures; i < d3_TexCount; i++, t++) {
        if (t->refs > 0 && !t->alias) {
            tex_Reload(t, keep);
            if (cb)
                cb(1, n, total);
            n++;
        }
    }
    ts_Unk633f00 = 0;
}

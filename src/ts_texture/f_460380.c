/* 0x460380, a name ending in "_A?<n>" is frame n of an animation: loads frame n + 1 if its file
   exists, else marks the group complete with n + 1 frames */

void ts_LoadAnim(TSTEXTURE *tex, TSFLAGS flags, unsigned char opts, unsigned char fmt)
{
    char name[0x40];
    char base[0x40];
    char upper[0x40];
    char path[0x100];
    char digits = 0;
    unsigned int i;
    TSFLAGS f;
    int n;
    char *p, *s;
    TSTEXTURE *t;

    strcpy(name, tex->name);
    strcpy(base, name);
    strcpy(upper, name);
    _strupr(upper);
    i = strlen(name);
    while (isdigit(name[--i]) && i > 0)
        digits = 1;
    if (i < strlen(name) - 1 && i > 1 && upper[i - 1] == 'A' && upper[i - 2] == '_' && digits) {
        n = atoi(&name[i + 1]);
        if (!n || (flags.b[2] & 4)) {
            f = flags;
            f.b[2] |= 4;
            tex->group = ts_NextGroup;
            tex->frame = n;
            base[i + 1] = 0;
            p = tex_FindFile(name);
            if (p) {
                s = strrchr(p, '/');
                if (s)
                    *s = 0;
                sprintf(path, "%s/%s%i.bmp", p, base, n + 1);
                if (file_Exists(path)) {
                    sprintf(path, "%s%i", base, n + 1);
                    tex_Load(path, f, opts, fmt);
                    return;
                }
                for (i = 0, t = d3_Textures; i < d3_TexCount; i++, t++) {
                    if (t->refs > 0 && t->group == ts_NextGroup && strstr(t->name, base))
                        t->frames = n + 1;
                }
                ts_NextGroup++;
            }
        }
    }
}

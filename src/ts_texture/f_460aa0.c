/* 0x460aa0, the opts and fmt tex_Load was called with */

void tex_GetParams(TSTEXTURE *tex, unsigned char *opts, unsigned char *fmt)
{
    if (tex && opts && fmt) {
        *opts = tex->f0 | tex->f4 << 1 | tex->speed << 2 | tex->f5 << 6;
        *fmt = tex->f6a | tex->f69 << 4;
    }
}

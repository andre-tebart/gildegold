/* 0x4614c0 */

TSTEXTURE *tex_Prepare(TSTEXTURE *tex)
{
    if (d3_Hardware && tex->hicolor)
        ts_CacheFetch(tex, 0, 0);
    return tex;
}

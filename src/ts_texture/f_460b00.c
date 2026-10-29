/* 0x460b00, the slot holding frame `frame` of the animation of d3_Textures[index] (0 = the texture itself, NULL if the
   frame is not loaded; `frame` is unsigned char: the original compares the byte directly) */

TSTEXTURE *tex_GetFrame(int index, unsigned char frame)
{
    TSTEXTURE *tex = &d3_Textures[index];
    int group;

    if (frame) {
        group = tex->group;
        if (tex->flags.b[0] >= 0xff && !(tex->flags.b[2] & 1)) {
            for (tex = d3_Textures; tex <= &d3_Textures[d3_TexCount - 1]; tex++) {
                if (tex->refs > 0 && tex->group == group && tex->frame == frame)
                    return tex;
            }
        } else {
            for (tex = d3_Textures + d3_TexCount - 1; tex >= d3_Textures; tex--) {
                if (tex->refs > 0 && tex->group == group && tex->frame == frame)
                    return tex;
            }
        }
        tex = 0;
    }
    return tex;
}

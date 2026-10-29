/* 0x4481e0, releases a sprite texture */

void d3if_FreeSprite(D3TEXHANDLE *t)
{
    if (t)
        tex_Release(t);
}

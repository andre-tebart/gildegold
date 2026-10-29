/* 0x402140, creates the height map of the character's scene on first use */
struct D3HMAP *ch_GetHMap(CHARACTER *ch)
{
    int cur = d3_CurrentScene;
    unsigned int flags = 0;
    unsigned int df;
    struct D3HMAP *h;

    if (!(h = ch->scene->hmap)) {
        df = d3_SceneFlags;
        if (ch->scene != d3_DefaultScene)
            df = D3SCENE_UNK3D8(ch->scene);
        if (!ch->scene->dirty && df) {
            d3if_SelectScene(d3t_SceneIndex(ch->scene), 1);
            flags = df & ~2;
            d3_InflateScene(flags);
        }
        ch->scene->hmap = d3sm_Create(d3_Terrain, D3SCENE_UNKB4(ch->scene), 2);
        h = ch->scene->hmap;
        if (ch->scene != &d3_Scenes[0] && d3_DefaultScene == ch->scene && h)
            ch_SaveGround(h);
        if (flags) {
            d3_DeflateScene(flags);
            d3if_SelectScene(cur, 1);
        }
    }
    return h;
}

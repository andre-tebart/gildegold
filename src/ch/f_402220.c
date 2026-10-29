/* 0x402220, the same for a scene */

struct D3HMAP *ch_GetSceneHMap(D3SCENE *scene)
{
    int cur = d3_CurrentScene;
    unsigned int flags = 0;
    unsigned int df;
    struct D3HMAP *h;

    if (!(h = scene->hmap)) {
        df = d3_SceneFlags;
        if (scene != d3_DefaultScene)
            df = D3SCENE_UNK3D8(scene);
        if (!scene->dirty && df) {
            d3if_SelectScene(d3t_SceneIndex(scene), 1);
            flags = df & ~2;
            d3_InflateScene(flags);
        }
        h = scene->hmap = d3sm_Create(d3_Terrain, D3SCENE_UNKB4(scene), 2);
        if (scene != &d3_Scenes[0] && d3_DefaultScene == scene && h)
            ch_SaveGround(h);
        if (flags) {
            d3_DeflateScene(flags);
            d3if_SelectScene(cur, 1);
        }
    }
    return h;
}

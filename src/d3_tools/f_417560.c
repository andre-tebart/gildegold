/* 0x417560, removes the sun rays object obj: switches to the object's scene, deletes it and clears the
   fade state, then switches back to the previous scene */

void d3t_RemoveSunRays(D3OBJECT *obj)
{
    int old = d3_CurrentScene;
    int idx = -1;
    if (obj->scene != d3_DefaultScene) {
        idx = d3t_SceneIndex(obj->scene);
        d3if_SelectScene(idx, 1);
    }
    d3if_Delete(obj);
    d3t_SunFade = 0;
    if (idx != -1)
        d3if_SelectScene(old, 1);
}

/* 0x411be0, the index of scene in d3_Scenes, -1 when it is not in the array */

int d3t_SceneIndex(D3SCENE *scene)
{
    int i = ((char *)scene - (char *)d3_Scenes) / sizeof(D3SCENE);
    if (i < 0 || i > D3_MAX_SCENES - 1)
        i = -1;
    return i;
}

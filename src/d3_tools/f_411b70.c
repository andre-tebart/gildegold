/* 0x411b70, releases every named texture of the object's material */

int d3t_ReleaseObjectTextures(D3OBJECT *obj)
{
    int i;
    for (i = 0; i < 3; i++) {
        if (obj->material && obj->material->tex[i].name)
            d3_ReleaseTexture(obj->material->animCtl, obj->material->tex[i].name);
    }
    return 1;
}

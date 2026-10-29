/* 0x411b00, releases the named texture of the object's material (up to 3) and updates the object */

int d3t_ReleaseObjectTexture(D3OBJECT *obj, char *name)
{
    int i;
    for (i = 0; i < 3; i++) {
        if (obj->material && obj->material->tex[i].name && _strcmpi(obj->material->tex[i].name, name) == 0) {
            d3_ReleaseTexture(obj->material->animCtl, name);
            d3_UpdateObject(obj, 0);
        }
    }
    return 1;
}

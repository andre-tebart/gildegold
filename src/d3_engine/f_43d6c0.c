/* 0x43d6c0, attaches obj to the bone `name` of its parent's animated mesh: the driver's bone table
   (D3ANIMMESH bones at +0x74, 0x58 bytes each, nbones at +0x208) is searched by name, the name is stored
   in the object's mesh at +0xb4 (the field d3anim_MapBones matches its children by) and the parent is
   re-mapped and posed. Returns 1 when the bone was found, 0 otherwise. */

char d3_AttachToBone(D3OBJECT *obj, char *name)
{
    D3OBJECT *parent;
    char *t;
    int i;

    if (obj && name && obj->parent && obj->material) {
        strcpy((char *)D3_MESH(obj) + 0xb4, "");
        parent = obj->parent;
        if (parent->material && (t = (char *)D3_MESH(parent)->surf[0].info.mesh)) {
            for (i = 0; i < ((unsigned char *)t)[0x208]; i++)
                if (!strcmp(t + 0x74 + i * 0x58, name))
                    break;
            if (i < ((unsigned char *)t)[0x208]) {
                strcpy((char *)D3_MESH(obj) + 0xb4, name);
                d3anim_MapBones(parent);
                d3anim_PoseBones(parent);
                return 1;
            }
        }
    }
    return 0;
}

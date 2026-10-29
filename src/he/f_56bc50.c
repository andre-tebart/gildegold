/* 0x56bc50, creates the two scene objects of the building event ev (arg5): the model of the building type *type (a table of 0x50-byte records at 0x627e98: four codes, then the model name at +0x10, the last record ends it), named "he_<id>" (+4), and the group "model" (arg4, the scaffolding); both are placed at pos (arg2) and rot (arg1), the second is dropped on the ground (0x412a70) and hooked to the building (0x50d380 (o, type, 2)); returns the second object (0 = a load failed) */

struct D3OBJECT * d3_LoadGroup();
int d3t_DropObject();

int he_Unk56bc50(void *rot, float *pos, unsigned char *type, char *model, int ev)
{
    char name[0x100];
    int scene, obj, obj2, n, k, found;
    int *rec;

    scene = U_I((unsigned)&d3_CurrentScene);
    found = 0;
    n = 0;
    rec = (int *)(unsigned)&full_data_627e98;
    if (rec[0] != 0) {
        while (!found) {
            for (k = 0; k < 4; k++) {
                if (*type == rec[k]) {
                    strcpy(name, (char *)rec + 0x10);
                    found = 1;
                    break;
                }
            }
            rec = (int *)((char *)rec + 0x50);
            n++;
            if (rec[0] == 0)
                break;
        }
    }
    if (!found)
        strcpy(name, (char *)((unsigned)&full_data_627ea8 + n * 0x50));
    d3if_SelectScene(0, 1);
    obj = d3_LoadGroup(name, 0, 0);
    if (obj == 0) {
        d3if_SelectScene(scene, 1);
        return 0;
    }
    d3_SetObjectPos(obj, pos);
    d3_SetObjectRot(obj, rot);
    *(unsigned char *)(obj + 0x211) = *(unsigned char *)(obj + 0x211) & 0xfb | 2;
    sprintf((char *)obj, "he_%i", *(int *)(ev + 4));
    obj2 = d3_LoadGroup(model, 0, 0);
    if (obj2 == 0) {
        d3if_SelectScene(scene, 1);
        return 0;
    }
    d3_SetObjectPos(obj2, pos);
    d3_SetObjectRot(obj2, rot);
    unk_Unk511600(obj2);
    d3t_DropObject(obj2, 1, 0);
    if (*(float *)(obj2 + 0x50) < pos[1]) {
        *(float *)(obj2 + 0x4c) = pos[0];
        *(float *)(obj2 + 0x50) = pos[1];
        *(float *)(obj2 + 0x54) = pos[2];
    }
    d3if_ShowTree(obj2, 0);
    d3_RelightObjects(obj2);
    *(int *)(ev + 0xc8) = -1;
    *(int *)(ev + 0xbc) = -1;
    he_ShowConstruction(ev);
    init_HookObject(obj2, type, 2);
    d3if_SelectScene(scene, 1);
    return obj2;
}

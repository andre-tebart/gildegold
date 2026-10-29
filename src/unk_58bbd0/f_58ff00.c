/* 0x58ff00, the placement of a building of the kind param_1 (2709 B, ROUGH SKELETON from the Ghidra pseudocode:
   the frame is 0x4110 bytes, the 128 name slots are 0x60 bytes each): the kind record 0x7e3f30 + kind * 0x24d;
   a kind 2 building first needs the player's character (0x501190 query, 0x521150); then the 128 candidate names
   are generated (the kind name upcased, 0x5ea986; the strided name table; 0x42fca0 filters them) into names[];
   one of them is picked by util_Random (masked 0xffff; for the kind '!' by the flag 0x770b89) or by 0x58f640;
   the collected candidate objects (0x58f7a0 into 0x6f26b0) are loaded one by one (0x44c3f0, position 0x43b790,
   the alpha 0x43d430, the colour 0x413640), the nearest to the camera 0xfc2dec picked, and the interactive loop
   (0x5309a0 pump, 0x527e90, the move 0x58feb0 / 0x58fd90 / 0x58fe10, the message 0x4c4b70) runs until the
   player confirms; returns the chosen index or 0 */

unsigned char * gm_BuildQuery(int n, ...); unsigned char * gm_BuildNext(void); char unk_Unk5020e0(int k, int c);
int sv_MessageBox(); char *_strupr(char *); int sprintf(char *buf, const char *fmt, ...); char * vfs_GetPath();
struct D3OBJECT * d3_LoadGroup(); int d3t_InflateObjectTree(); int d3t_DropObject();
int ai_Unk527e90();
struct D3OBJECT;
char d3if_Delete(struct D3OBJECT *obj);  /* 0x447190, d3_interface.h */
int gm_Unk509dc0(int unused, int kind); void txt_Format(char *out, int id0, ...); int sv_Unk527640();
int d3sm_Pick(); int d3sm_GetVertex();  /* @original: the original callers read an int result (gv.h declares it int) */

#define B_REC(k) (U_I((unsigned)&ai_BTypes) + (k) * 0x24d)

int ai_Unk58ff00(int kind, char *out)
{
    int list1[0x100];
    int list2[0x100];
    char names[128][0x60];
    char chosen[0x60];
    char text[0x60];
    int i;
    int n;
    int obj;
    int sel;
    int best;
    int it;
    float bd;
    float d;
    float pos[3];
    int a[3];
    int b[3];

    sel = 0;
    obj = 0;
    n = 0;
    best = 0;
    bd = 1e+09f;
    if (*(char *)B_REC(kind) == 2) {
        /* the player's character of the kind: the first of the build query whose byte 0 is the kind */
        for (it = ((void *(*)())gm_BuildQuery)(); it != 0; it = gm_BuildNext()) {
            /* ROUGH: the query arguments are lost in the pseudocode */
            break;
        }
        /* 0x5176d0 / 0x521150: the character must exist, else 0 */
    }
    memset(list1, 0, 0x400);
    memset(list2, 0, 0x400);
    /* the base name: the kind record + 1 upcased */
    strcpy(text, (char *)(B_REC(kind) + 1));
    _strupr(text);
    for (d = 0.0f; d < 128.0f; d += 1.0f) {
        if (d == 0.0f)
            sprintf(chosen, "%s", text);
        else
            sprintf(chosen, "%s%i", text, (int)d);
        if (vfs_GetPath(chosen) == 0) {
            if (d != 0.0f)
                break;
        } else {
            strcpy(names[n], chosen);
            n++;
        }
    }
    if (n == 0) {
        /* 0x5de652 / 0x4222b0: "not a multiple ..." path */
        sprintf(out, "%s", text);
        err_Message(text);
        return 0;
    }
    best = ((int (*)())util_Random)() & 0xffff;
    if (*(char *)B_REC(kind) == '!')
        best = (*(unsigned char *)((unsigned)&ai_Unk770b89 + (U_W((unsigned)&ai_PlayerId) * 0x21c)) != 1);
    if (ai_BuildingName(kind, chosen) != 0) {
        n = ai_FindBauplaetze(names[best], list2);
        U_I((unsigned)&full_data_6f26b0) = (int)list2;
        for (i = 0; i < n; i++) {
            if (list2[i] != 0) {
                d3_PointToWorld(list2[i], list2[i] + 0x4c, pos);
                sprintf(chosen, "%s", text);
                obj = d3_LoadGroup(chosen);
                list1[i] = obj;
                d3t_InflateObjectTree(obj);
                d3_SetObjectPos(obj, list2[i]);
                *(unsigned char *)(obj + 0x211) &= 0xfd;
                ((void (*)(void *, void *, int))d3_SetObjectAlpha)(obj, 0, 0);
                d3t_SetTreeColor(obj);
                if ((((pos[0] - *(float *)((unsigned)&ai_Unkfc2dec + 0x4c)) * (pos[0] - *(float *)((unsigned)&ai_Unkfc2dec + 0x4c))) +
                     ((pos[1] - *(float *)((unsigned)&ai_Unkfc2dec + 0x50)) * (pos[1] - *(float *)((unsigned)&ai_Unkfc2dec + 0x50))) +
                     ((pos[2] - *(float *)((unsigned)&ai_Unkfc2dec + 0x54)) * (pos[2] - *(float *)((unsigned)&ai_Unkfc2dec + 0x54)))) < bd) {
                    /* ROUGH: the distance of the original is a sqrt of the three axes */
                    sel = obj;
                    bd = 0.0f;
                    best = obj;
                }
                d3t_PlayAnim(obj);
            }
        }
        if (best != 0) {
            di_ClearEvents(2);   /* the call at 0x59035d pushes 2 */
            *(char *)(unsigned)&kb_Last = '2';
            /* ROUGH: the interactive loop -- the chosen object is moved with 0x58fd90 / 0x58feb0 to the site
               (0x58fe10 nearest, the construction record 0x6f22a0 = 1), the frame pump 0x5309a0
               (ai_Unk5909a0, 0x657c7) and the confirm key 0x527e90 / 0x527640 run until the exit */
        }
        sv_MessageBox();
    }
    return sel;
}

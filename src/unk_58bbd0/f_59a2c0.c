/* 0x59a2c0, creates the city point object of a town (256 B): the name "dummy_<CITY>" (the uppercase city via
   _strupr, sprintf 0x5de652) looked up by 0x449690 (0, 0x140, 0); its position (obj + 0x4c) copied by 0x458a20
   into the vector v and the object "sp_STADTPUNKT_K" created there (0x446e10, the origin 0x5ebba4); +0x220 = 4,
   +0x224 = 1, byte +0x211 |= 4, byte +0x212 = (b & 0xf7) | 4; 0x453280 and the model
   "sonstiges\STADTPUNKT_ANM_K.baf" (0x411960, flag 1). Returns the object (0 when the dummy is missing) */

int d3t_PlayAnim(); void d2_SetBackgroundTexture();
char d3if_ShowTree(struct D3OBJECT *obj, char show); int ls_GetLanguage(); int sv_ClearNamePlates(); struct D3OBJECT * d3if_Pick(float x, float y, int flags, int mask);
unsigned char * gm_BuildNext(void); int ai_StorageWindow(); void ai_MasterLetterWindow(); int ai_OpenTransport();
int ai_TransportWindow(); 

int ai_Unk59a2c0(char *param_1)
{
    char upper[0x100];
    char name[0x100];
    int v[4];
    int obj;

    v[0] = 0;
    v[1] = 0;
    v[2] = 0;
    v[3] = 0;
    strcpy(upper, param_1);
    _strupr(upper);
    sprintf(name, "dummy_%s", upper);
    obj = d3_FindObject(0, 0x140, 0, name);
    if (obj == 0)
        return 0;
    d3_PointToWorld(obj, (float *)(obj + 0x4c), (float *)v);
    obj = d3_CreateObject(0, (float *)v, (float *)(unsigned)&ai_Unk5ebba4, "sp_STADTPUNKT_K");
    U_I(obj + 0x220) = 4;
    U_I(obj + 0x224) = 1;
    *(unsigned char *)(obj + 0x211) |= 4;
    *(unsigned char *)(obj + 0x212) = (*(unsigned char *)(obj + 0x212) & 0xf7) | 4;
    d3_RelightObject(obj);
    d3t_PlayAnim(obj, "sonstiges\\STADTPUNKT_ANM_K.baf", 1);
    return obj;
}

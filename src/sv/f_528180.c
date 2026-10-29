/* 0x528180, the callback of 0x5282f0 over the model parts of the building b (+0x61 its model): for the part named "dummy_FAHNE" it creates the banner object "sp_WIMPEL" (model "sonstiges\sp_WIMPEL.baf") at the
   dummy's position (0x458a20), turned by pi around the y axis (0x4545c0), coloured for the owner like 0x528290, and flags it (+0x211 bit 1 cleared, +0x212 = +0x212 & 0xf7 | 0x44); returns 1; ROUGH (the 4th argument of
   0x458a20 is an uninitialised register in the original) */

int d3t_PlaceObject(struct D3OBJECT *camera, float px, float py, float pz, float dx, float dy, float dz, int unk, char flags);                     /* 0x411680 */
int _stricmp();                     /* 0x5eaa30 (_strcmpi) */
char d3if_SelectTextureSet(struct D3OBJECT *obj, struct D3SURF *surf, unsigned char set, char reload);                     /* 0x446f30 */
unsigned char * gm_BuildQuery(int n, ...);                     /* 0x501190 */
unsigned char * gm_BuildNext(void);                     /* 0x501030 */
int d3t_SceneIndex();                     /* 0x411be0 */
extern int ai_Unk75fb80, ai_Unk75fb84, ai_Unk75fb88, ai_Unk75fb90, ai_Unk75fb94, ai_Unk75fb98;  /* 0x75fb80, 0x75fb84, 0x75fb88, 0x75fb90, 0x75fb94, 0x75fb98 */
void d3_SetWorldPosRot();                     /* 0x449920 */
int d3t_PlayAnim();                     /* 0x411960 */

char sv_Unk528180(char *part, char *b)
{
    float z[3], w[3], m[16];   /* m is a D3MATRIX: d3_MatFromAngles writes 0x40 bytes (m[3] overflowed the stack) */
    char *o;
    int junk;

    if (_stricmp(part, "dummy_FAHNE") == 0) {
        z[0] = 0;
        z[1] = 0;
        z[2] = 0;
        d3_PointToWorld(part, part + 0x4c, w, junk);   /* @original: the 4th argument is an uninitialised register in the original (0x528180) */
        o = (char *)d3_CreateObject(*(int *)(b + 0x61), w, z, "sp_WIMPEL");
        z[0] = 0;
        z[1] = 3.1415927f;
        z[2] = 0;
        d3_MatFromAngles(z, m);
        d3_SetWorldPosRot(o, w, m);
        if (*(unsigned short *)(b + 0x27) != 0xffff) {
            d3if_SelectTextureSet(o, *(int *)(o + 0x1cc), (char)(*(char *)(ai_Chars[*(unsigned short *)(b + 0x27)] + 0x54) + 'f'), 1);
            *(int *)(o + 0x220) = 4;
        }
        d3t_PlayAnim(o, "sonstiges\\sp_WIMPEL.baf", 1);
        o[0x212] = (o[0x212] & 0xf7) | 0x44;
        o[0x211] &= 0xfd;
    }
    return 1;
}

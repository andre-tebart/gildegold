/* 0x4fe7f0, frees every building of the instance table (0x5012e0) and clears the table (0x100 records of 0xa9 bytes) */

extern int f3gm_ObjectCount;
extern int ai_Unk6178f8[];
extern char ai_PlayerSlots[][0x2f4];      /* 0x7e3f40, the player slots */
int gm_FreeBuilding();                     /* 0x5012e0 */
int gm_RemoveObject();                     /* 0x4ffff0 */
int gm_FindById(int *bld, int *obj, int *chr, int id);
int gm_Unk4ff950();                     /* 0x4ff950 */
int gm_Unk4ff8c0();                     /* 0x4ff8c0 */
float gm_ItemPrice(int item, unsigned char n); /* 0x5087f0 */

void gm_ClearBuildings(void)
{
    int i;

    i = 0;
    do {
        if (ai_Blds[i] != 0)
            gm_FreeBuilding(ai_Blds + i);
        i += 0xa9;
    } while (i < 0xa900);
    memset(ai_Blds, 0, 0xa900);
}

/* 0x55d310, the allocation handler of a building event: the building +0xac (none: free) gets a due time of 0x18 units + a random 0x5092e0 (building, 0x10, 1) part, +0x58 = 0 */

struct G15PLANT * veg_FindPlantId(struct G15PLANT *p, int id);
void cm_SetPlant(int a, unsigned char b, unsigned char c, short d, unsigned char e, int f);
void ai_Unk4c5970(int a, int b, int c);
void cm_SendFieldValues(void);
int cm_AddFieldValue(unsigned char size, unsigned char n, unsigned short field, void *v);
int gm_Opinion(int a, int b);
void ai_Unk4c5fa0(int a, int b, int c);
int gm_PlayerFactor(int a, unsigned char k);

void he_Unk55d310(int ev)
{
    int b;

    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    gm_BuildingSkill(b, 0x10, 1);
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0, 0x18);
    *(int *)(ev + 0x58) = 0;
}

/* 0x548230, the run handler of a one-shot event: state 0 (flag 2 of +0x78) posts it again with the clock now, a freed state ends it (ROUGH) */

int gm_GetWorth(unsigned short id);
void gm_Unk50ca10(unsigned short a, int b);
void ai_Unk4c6020(int a, int b, int c);
void ai_Unk4c5870(int a);
void ai_Unk4c59f0(unsigned char a, int b, int c, unsigned char d, unsigned char e, unsigned char f, const char *s);
void ai_Unk5a91c0(void);
void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d);
int ai_Unk4a63f0(struct AIPER *p, int a, __int64 mask, int c, float f, int *out);
void amt_ChangeDistrictOpinion();

int gm_BuildingClass();
unsigned char * gm_BuildNext(void);
int gm_ListAmount();
unsigned short *gm_QueryNext(void);
void cm_SendExSellObject(int a, int b, short c, int d, unsigned char e, int f);
void gm_Unk50cad0(unsigned char *b, int d);
float gm_GoodsPrice(unsigned id, unsigned char p);

int gm_TitleGroup();
int gm_JobSlots();
int cm_AddFieldValues(unsigned char size, unsigned char n, unsigned short field, void *v);
void ai_Unk4c59b0(int a, unsigned char b);
void cm_SendCreateCharacter(unsigned char a, int b, int c, unsigned short d, int e, unsigned char f, unsigned char g, unsigned char h);
short *gm_PlayerRecord(char *c);
int ai_Unk5d08c0();

void he_Unk548230(int ev)
{
    int st;

    if (*(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    st = *(int *)(ev + 0x70);
    *(int *)(ev + 0x84) = -1;
    if (st >= -2) {
        if (st < 0) {
            he_Unk53e1f0((int *)ev);
        } else if (st == 0 && (*(unsigned char *)(ev + 0x78) & 2) != 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 2, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        }
    }
}

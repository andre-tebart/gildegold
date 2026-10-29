/* 0x53e040, creates the event record of the message `m` (type m+4 < 0xa7 with both handlers, player m+8): the first free of the 0x400 records at 0x6fab40 (0x14c bytes, type byte first), copies the message into it, calls the alloc handler and counts it (0x7585e4); 0 when no handler / record is free (ROUGH) */

#include <stdarg.h>
#include <math.h>
void he_Unk55e3a0(int ev);
void he_Unk55eba0(int p);
void he_Unk56fbc0(int rec, unsigned short num);

int d3t_SetColor();
struct D3OBJECT;
char d3if_Delete(struct D3OBJECT *obj);  /* 0x447190, d3_interface.h */
char d3if_Show(struct D3OBJECT *obj, char show);
int d3t_GetHeightRange();
char *gm_FindBuildingById(int id);
char * ahm_OwnerChar();
unsigned char *gm_BuildQuery(int n, ...);
unsigned char * gm_BuildNext(void);
int cm_SendLongMessage(char *msg);
int cm_GetSlotState();
void sv_PumpNet();
int gm_GetGoodsValue();
void cm_SendMoveGoods(int a, int b, unsigned char c, int d);
char *gm_Query(void *list, int n, ...);
void cm_BeginGroup(const char *s);
void cm_EndGroup(const char *s);
void ai_Unk4c5d80(int *p);
void ai_Unk4c5d40(void *p);
int crt_Sprintf(char *buf, const char *fmt, ...);
void mem_Free(void *p);
void err_Message(char *text);

/* HE_SET_CLOCK: the game clock (0x770ae0 ..) into a message at +0x28 .. +0x34, flag byte 2 at +0x36 */
#define HE_CLK(m) do { HM_I(m, 0x28) = U_I((unsigned)&ai_Time); HM_I(m, 0x2c) = U_I((unsigned)&ai_TimeHour); HM_I(m, 0x30) = U_I(((unsigned)&ai_Time + 0x8)); *(short *)((m) + 0x34) = *(short *)((unsigned)&ai_Time + 0xc); HM_B(m, 0x36) = 2; } while (0)

unsigned char *he_Unk53e040(unsigned char *m)
{
    unsigned short *pl = 0;
    int i, j, k, off, saved;
    char *pc;
    unsigned char *r;
    int *d, *s;

    if (m[4] < 0xa7 && U_I(((unsigned)&he_QuickList + 0x400) + m[4] * 4) != 0 && U_I((unsigned)&he_RunHandlers + m[4] * 4) != 0) {
        if (*(int *)(m + 8) != -1 && (pl = (unsigned short *)gm_FindChar(*(int *)(m + 8))) == 0)
            return 0;
        i = 0;
        pc = (char *)(unsigned)&he_Events;
        do {
            if (*pc == 0)
                break;
            pc += 0x14c;
            i++;
        } while ((int)pc < (unsigned)((unsigned)&he_Events + 0x53000));
        if (i != 0x400) {
            off = i * 0x14c;
            r = (unsigned char *)((unsigned)&he_Events + off);
            if (U_I((unsigned)&ai_Unk6f261c) < i)
                U_I((unsigned)&ai_Unk6f261c) = i;
            if (*(int *)(m + 8) == -1) {
                U_I(((unsigned)&he_Events + 0xc) + off) = -1;
                *(unsigned short *)(((unsigned)&he_Events + 0x8) + off) = 0xffff;
            } else {
                U_I(((unsigned)&he_Events + 0xc) + off) = *(int *)(pl + 2);
                *(unsigned short *)(((unsigned)&he_Events + 0x8) + off) = *pl;
            }
            saved = U_I((unsigned)&ai_Unk6f2620);
            if ((m[0x36] & 8) != 0) {
                j = 0;
                do {
                    if (U_I((unsigned)&he_QuickList + j * 4) == 0)
                        break;
                    j++;
                } while (j < 0x100);
                if (j == 0x100)
                    return 0;
                U_I((unsigned)&he_QuickList + j * 4) = (int)r;
                if (saved < j)
                    U_I((unsigned)&ai_Unk6f2620) = j;
            }
            *r = m[4];
            U_I(((unsigned)&he_Events + 0x4) + off) = U_I((unsigned)&ai_Unk6f2618);
            U_I((unsigned)&ai_Unk6f2618)++;
            U_I(((unsigned)&he_Events + 0x10) + off) = *(int *)(m + 0xc);
            d = (int *)(((unsigned)&he_Events + 0x14) + off);
            s = (int *)(m + 0x10);
            for (k = 0; k < 0xc; k++)
                *d++ = *s++;
            U_I(((unsigned)&he_Events + 0x44) + off) = *(int *)(m + 0x28);
            U_I(((unsigned)&he_Events + 0x48) + off) = *(int *)(m + 0x2c);
            U_I(((unsigned)&he_Events + 0x4c) + off) = *(int *)(m + 0x30);
            *(short *)(((unsigned)&he_Events + 0x50) + off) = *(short *)(m + 0x34);
            *(unsigned char *)(((unsigned)&he_Events + 0x78) + off) = m[0x36];
            U_I(((unsigned)&he_Events + 0x70) + off) = 0;
            U_I(((unsigned)&he_Events + 0x74) + off) = 0;
            U_I(((unsigned)&he_Events + 0x7c) + off) = 0;
            U_I(((unsigned)&he_Events + 0x84) + off) = -1;
            d = (int *)(((unsigned)&he_Events + 0x8c) + off);
            s = (int *)(m + 0x38);
            for (k = 0; k < 8; k++)
                *d++ = *s++;
            s = (int *)(m + 0x58);
            d = (int *)(((unsigned)&he_Events + 0xac) + off);
            for (k = 0; k < 0x28; k++)
                *d++ = *s++;
            ((void (*)(unsigned char *))U_I(((unsigned)&he_QuickList + 0x400) + m[4] * 4))(r);
            U_I((unsigned)&ai_Unk7585e4)++;
            return r;
        }
    }
    return 0;
}

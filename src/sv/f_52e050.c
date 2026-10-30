/* 0x52e050, draws the hint text 0xf8aae0 at the mouse position when the hint flag (0x6f3618 +2) is set */

#include <stdarg.h>
extern int ai_GameState;
extern unsigned char d3_MipSkip;

struct D3OBJECT * d3t_FindObject(char *name, char flags);                     /* 0x411900 */
int d2_TextWidth();                     /* 0x46ea10 */
int d2_WinCreateSlider();                     /* 0x46bf10 */
void d2_SetObjWidth(int obj, short w);                     /* 0x46c260 */

void sv_Unk52e050(void)
{
    if (SV_B((unsigned)&ai_Unkf8aae0) != 0 && SV_B(((unsigned)&ai_GameSwitches + 0x2)) != 0) {
        d2_SetFont(0x54);
        d2_DrawTextBox(SV_S((unsigned)&d2_MouseX) - 0x36, SV_S((unsigned)&d2_MouseY) + 0x34, 0xa0, 0x60, d2_ScreenSurf, (char *)(unsigned)&ai_Unkf8aae0, 0xa8);
        d2_AddDirty(SV_I((unsigned)&d2_TextLeft), SV_I((unsigned)&d2_TextTop), SV_I((unsigned)&d2_TextDrawnW), SV_I((unsigned)&d2_TextDrawnH));
    }
}

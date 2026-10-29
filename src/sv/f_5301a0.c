/* 0x5301a0, the weather of the current hour h (the clock 0x770ae4): the rain / snow emitters (0x76061c, 0x760618) get the cloud counter of the hour (0x760488, 24 counters), the wind strength follows the three
   hours around h, and the sky layers 0x760620 / 0x760624 (fading in and out 0x45d9a0) get a new sky model of the group "Sky_Schoen_01 .. " (0x6238a8, 0x4c bytes per entry) / "Sky_Mittel_01" 0x6239d8 /
   "sky_schwer_01" 0x623ac0 chosen by the strongest of the three counters (< 0x32, < 0x96, more); the float operands of the emitter calls are lost in the pseudocode (placeholders 0); ROUGH */

int d3t_SetSnow();                     /* 0x414c60 */
int d3t_SetRain();                     /* 0x413d30 */
int d3sky_SetLayerTex();                     /* 0x45d9f0 */
void d3sky_FadeLayer(struct D3SKY *s, struct D3SKYLAYER *l, unsigned char alpha, float time);   /* 0x45d9a0 */
void d3sky_SetLayerSpeed(struct D3SKY *s, struct D3SKYLAYER *l, float speed);        /* 0x45d960 */
int d3t_DrawRain();                     /* 0x414590 */
int d3t_DrawSnow();                     /* 0x415380 */
int d3t_CreateSunRays();                     /* 0x417190 */
int d3t_FadeOutSunRays();                     /* 0x417150 */
int ai_Unk5763c0();                     /* 0x5763c0 */
void ai_Unk576040(int a, int b, int c, int d);                     /* 0x576040 */
unsigned ai_Unk623ecc = 4294967295;  /* 0x623ecc */

void sv_Unk5301a0(void)
{
    float f1, f2, w;
    int h, c, k, cl;
    char *name;
    unsigned u;
    int *cnt;

    cnt = (int *)(unsigned)&ai_Unk760488;
    SV_I((unsigned)&ai_Unk760614) = SV_US((unsigned)&ai_TimeHour);
    h = SV_I((unsigned)&ai_Unk760614);
    if (SV_I((unsigned)&ai_Unk76061c) != 0)
        d3t_SetSnow(SV_I((unsigned)&ai_Unk76061c), 1, 1, cnt[h], 1000);
    if (SV_I((unsigned)&ai_Unk760618) != 0) {
        u = cnt[h];
        if (SV_I((unsigned)&ai_Unk76061c) != 0) {
            if ((u & 1) == 0)
                u = 0;
            else
                u = (int)u / 5;
        }
        d3t_SetRain(SV_I((unsigned)&ai_Unk760618), 1, 1, u, 1000);
    }
    f1 = (float)((int *)(unsigned)&ai_Unk760550)[h];
    f2 = (float)((int *)(unsigned)&ai_Unk7605b0)[h];
    c = cnt[(h + 0x17) % 0x18];
    if (c <= cnt[h])
        c = cnt[h];
    if (c <= cnt[(h + 1) % 0x18])
        c = cnt[(h + 1) % 0x18];
    if (SV_I((unsigned)&ai_Unk76061c) != 0)
        d3t_SetSnow(SV_I((unsigned)&ai_Unk76061c), 1, 2, (int)(int)0.0f);
    if (SV_I((unsigned)&ai_Unk760618) != 0)
        d3t_SetRain(SV_I((unsigned)&ai_Unk760618), 1, 2, (int)(int)0.0f);
    if (SV_I((unsigned)&d3_Sky) != 0) {
        w = (float)(c + 0x96) * (float)sqrt(f1 * f1 + f2 * f2) * 0.125f;
        if (*(char *)(SV_I((unsigned)&ai_Unk760624) + 0x26) == 0) {
            if (c < 0x32) {
                k = util_Random(4) & 0xffff;
                if (_strcmpi(*(char **)(SV_I((unsigned)&ai_Unk760624) + 0x1c), (char *)((unsigned)&ai_Unk6238a4 + 0x4) + k * 0x4c) == 0)
                    k++;
                name = (char *)((unsigned)&ai_Unk6238a4 + 0x4) + k * 0x4c;
            } else if (c < 0x96) {
                k = util_Random(3) & 0xffff;
                if (_strcmpi(*(char **)(SV_I((unsigned)&ai_Unk760624) + 0x1c), (char *)(unsigned)&full_data_6239d8 + k * 0x4c) == 0)
                    k++;
                name = (char *)(unsigned)&full_data_6239d8 + k * 0x4c;
            } else {
                k = util_Random(2) & 0xffff;
                if (_strcmpi(*(char **)(SV_I((unsigned)&ai_Unk760624) + 0x1c), (char *)(unsigned)&ai_Unk623ac0 + k * 0x4c) == 0)
                    k++;
                name = (char *)(unsigned)&ai_Unk623ac0 + k * 0x4c;
            }
            d3sky_SetLayerTex(SV_I((unsigned)&ai_Unk760624), name);
            *(int *)SV_I((unsigned)&ai_Unk760624) = *(int *)(name + 0x40);
            *(int *)(SV_I((unsigned)&ai_Unk760624) + 0x18) = *(int *)(name + 0x44);
            *(int *)(SV_I((unsigned)&ai_Unk760624) + 0x14) = *(int *)(name + 0x48);
            d3sky_FadeLayer(SV_I((unsigned)&d3_Sky), SV_I((unsigned)&ai_Unk760624), 0xff, 1500.0f);
        }
        if (*(char *)(SV_I((unsigned)&ai_Unk760624) + 0x26) == -1) {
            if (c < 0x32) {
                k = util_Random(4) & 0xffff;
                if (_strcmpi(*(char **)(SV_I((unsigned)&ai_Unk760620) + 0x1c), (char *)((unsigned)&ai_Unk6238a4 + 0x4) + k * 0x4c) == 0)
                    k++;
                name = (char *)((unsigned)&ai_Unk6238a4 + 0x4) + k * 0x4c;
            } else if (c < 0x96) {
                k = util_Random(3) & 0xffff;
                if (_strcmpi(*(char **)(SV_I((unsigned)&ai_Unk760620) + 0x1c), (char *)(unsigned)&full_data_6239d8 + k * 0x4c) == 0)
                    k++;
                name = (char *)(unsigned)&full_data_6239d8 + k * 0x4c;
            } else {
                k = util_Random(2) & 0xffff;
                if (_strcmpi(*(char **)(SV_I((unsigned)&ai_Unk760620) + 0x1c), (char *)(unsigned)&ai_Unk623ac0 + k * 0x4c) == 0)
                    k++;
                name = (char *)(unsigned)&ai_Unk623ac0 + k * 0x4c;
            }
            d3sky_SetLayerTex(SV_I((unsigned)&ai_Unk760620), name);
            *(int *)SV_I((unsigned)&ai_Unk760620) = *(int *)(name + 0x40);
            *(int *)(SV_I((unsigned)&ai_Unk760620) + 0x18) = *(int *)(name + 0x44);
            *(int *)(SV_I((unsigned)&ai_Unk760620) + 0x14) = *(int *)(name + 0x48);
            d3sky_FadeLayer(SV_I((unsigned)&d3_Sky), SV_I((unsigned)&ai_Unk760624), 0, 1500.0f);
        }
        d3sky_SetLayerSpeed(SV_I((unsigned)&d3_Sky), SV_I((unsigned)&ai_Unk760620), w * 0.75f);
        d3sky_SetLayerSpeed(SV_I((unsigned)&d3_Sky), SV_I((unsigned)&ai_Unk760624), w);
        if (c == 0) {
            u = 0;
            w = 2500.0f;
        } else {
            u = ((c << 6) / 1000) & 0xff | 0x60;
            w = 1000.0f;
        }
        d3sky_FadeLayer(SV_I((unsigned)&d3_Sky), SV_I((unsigned)&ai_Unk760628), u, w);
        d3sky_SetLayerSpeed(SV_I((unsigned)&d3_Sky), SV_I((unsigned)&ai_Unk760628), w * 1.5f);
    }
}

/* 0x52f2c0, draws the rubber band rectangle (0x41e8f0, colour -1) between the corners 0x760444 / 0x760448 and 0x76044c / 0x760450 while 0x760440 is set */

void dd_Line();                     /* 0x41e8f0 */
void * mem_Alloc(int size, char *tag);                      /* 0x4223c0 */
void mem_Free(void *p);                     /* 0x422620 */
int d3t_ReleaseObjectTextures();                     /* 0x411b70 */
char d3if_Show(struct D3OBJECT *obj, char show);                     /* 0x4471b0 */
int d2_LoadView();                     /* 0x474b30 */
void d2_FillViewTextures(int view, char *tex1, char *tex2); /* 0x474d90 */
void d2_RedrawView(int view);                     /* 0x474e80 */
int d2_SetViewTextures(int view, char *tex1, char *tex2);  /* 0x474ca0 */
void d2_ClearViewTextures(int view);                      /* 0x474d10 */
void d2_EnableViewObjects();                     /* 0x4754d0 */
void d3d_SetWBuffer(char w);                     /* 0x437060 */
char *util_BaseName();                   /* 0x401040 */
int sv_Unk52f710(int *book, char dir);
void sv_Unk52f9f0(int *book);
void sv_Unk52fa00(int *book);

void sv_Unk52f2c0(void)
{
    if (SV_I((unsigned)&ai_Unk760440) != 0) {
        dd_Line(SV_I((unsigned)&full_data_760444), SV_I((unsigned)&full_data_760448), SV_I((unsigned)&full_data_76044c), SV_I((unsigned)&full_data_760448), -1);
        dd_Line(SV_I((unsigned)&full_data_76044c), SV_I((unsigned)&full_data_760448), SV_I((unsigned)&full_data_76044c), SV_I((unsigned)&full_data_760450) + 1, -1);
        dd_Line(SV_I((unsigned)&full_data_76044c), SV_I((unsigned)&full_data_760450), SV_I((unsigned)&full_data_760444), SV_I((unsigned)&full_data_760450), -1);
        dd_Line(SV_I((unsigned)&full_data_760444), SV_I((unsigned)&full_data_760450), SV_I((unsigned)&full_data_760444), SV_I((unsigned)&full_data_760448), -1);
    }
}

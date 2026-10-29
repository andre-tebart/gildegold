/* 0x4b3d50, the mission offer dialog of mission type type ("special\mission" with its texts and the speech
   "_AUFTRAEGE_VERGABE_HS_nn"): 1 if accepted */

int d2_OpenForm(int x, int y, char *name);                                     /* 0x4741f0 */
void d2_CentreViewWindows(int view);                                                     /* 0x475600 */
void d2_CloseView(int view);                                                     /* 0x4758f0 */
int d2_SetForm(int view, int n);                                                 /* 0x475f70 */
extern int d2_ClickedId;                /* 0xf8a668 */
int io_PrintText(char *fmt, ...);                                                /* 0x515960, defined elsewhere */
int sv_RunFrame(int cb, unsigned mask);                                      /* 0x5309a0, defined elsewhere */
int snd_PlayVoice(int id, int bank, char *text, int var);                             /* 0x4c2760, plays a speech sample */
int sb_IsPlaying(struct SBSAMPLE *s);                                                       /* 0x426f20 */
void sb_Stop(struct SBSAMPLE *s, int fade);                                                 /* 0x427290 */
extern int sv_PopRequest;                /* 0x6f22a0 */

int ai_Unk4b3d50(int type, int id)
{
    char name[0x100];
    G13TYPE *t;
    int view, r;
    void *snd;

    r = 0;
    t = ai_GetMissionType(type);
    view = d2_OpenForm(0, 0, "special\\mission");
    d2_CentreViewWindows(view);
    d2_SetForm(view, 1);
    io_PrintText((char *)(t->text + 1));
    io_PrintText("$N");
    io_PrintText((char *)0x7d);
    io_PrintText("$N");
    io_PrintText((char *)0x7e);
    sprintf(name, "_AUFTRAEGE_VERGABE_HS_%.2d", t->unk0c);
    snd = snd_PlayVoice(-7, 0, name, -1);
    while (sv_RunFrame((void (*)(void))ai_Unk4b3d50, id)) {
        if (dm_L.rup != 0 || d2_ClickedId == 1)
            sv_PopRequest = 1;
        if (d2_ClickedId == 0) {
            r = 1;
            sv_PopRequest = 1;
        }
    }
    if (view != -1)
        d2_CloseView(view);
    if (sb_IsPlaying(snd))
        sb_Stop(snd, 0x19);
    return r;
}

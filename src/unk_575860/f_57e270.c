/* 0x57e270, the tutorial cannon scene (2305 B, ROUGH: the whole call sequence and the main loop are written, the
   3D setup details are comments): arg 1 = the record; the scene struct sc (local_704, 0x39 floats: the camera /
   target, its control ids at 0xbc / 0xc8 / 0xcc / 0xd0, the selected index at 0xa0, the objects at 0x8c) zeroed;
   the clock 0x6333cc saved; returns at once without rec + 0x61; the run record 0x4eded0 (rec + 1), else the loader
   0x4eea70 (rec) waited out on 0x4c3ae0 (pump 0x20007), then 0x4eded0 again (0 -> return); the table callback
   0x4eeee0 (rec + 1, 0x57e230, 0x6fa020). The camera globals of 0xfc2dec (+0x4c .. +0x54, +0x84 .. +0x8c), the
   screen 0xfb9060 / 0x64 / 0x68 / 0x6c and 0x633444 saved; the window cleanup 0x476f80 (0, 0) / 0x52ae40 /
   0x476c70 / 0x520f20, 0x75e8c0 = 0, 0xf8aae0 = 0; the BLOCK "BLACK" (0x5fea38) drawn by 0x4767d0 (0, 0,
   0xfb9072, 0xfb9070, "BLACK", 0x5a, 1) and waited on (*pb & 4, pump mask 0x10180); the window table 0xf8f080
   (stride 0x2ec, 0x300 entries) scanned against the list 0xfb11c0 (stride 0xab, to 0xfb9200): an entry with
   +0x18 set, +0x40 = the list record - 0x68 and +0x38 clear gets +0x38 = 1 and its index into the list; the
   screen 0x43b950 ("BLACK", the rect 0x6248a4 .. 0x6248b0, 0xfe73b4, 0xfc2564, 0xfc5348), 0x522100 (0), the
   camera restored, 0x4478e0 (0, 0), 0x45d690 (0x633dd4, 1); 0x413150 (two 4-byte locals, rec + 0x61); the
   0x925100 block reset (the record + 1 changed) or reused (0x925108 / 0x92510c / 0x925110 = the camera);
   0x458a20 (rec + 0x61, rec + 0x61 + 0x4c, sc); sc + 0x10 = 0x925108 / 0c / 10, sc + 4 += (a - b) * 0.8; the four
   scene objects 0x446e10 ("KANONE_UNTERBAU_Aa" 0x62a350, "KANONE_ROHR_Aa" 0x62a340, "KANONE_DOPPEL_ROHR_Aa"
   0x62a328, "KANONE_DREIFACH_ROHR_Aa" 0x62a310) placed (0x43b790 / 0x43b860) and the wimpel object "AO_KANONE_
   WIMPEL_Aa" 0x62a2fc with 0x411960 ("ADDON_AO_KANONE_WIMPEL_Aa" 0x62a2dc, 1) and its bytes +0x212 = (b & 0xf7)
   | 4, then | 0x40, +0x211 &= 0xfd; the mouse 0x4796c0 (0x6ed924), the pump 0x86 (twice); the BLOCK redrawn
   (0x476860, 0x4767d0 ... 0x5a, 10); 0x580a90 (sc, the run) and 0x57f890 (rec, sc, the run); the selected index
   = MIN (sc + 0xdc, ...); 0x471b0 (obj[i], 0) for 0..2 and (obj[selected], 1) (the control 0x447300 / 0x53ec10,
   rec + 0x61, 0); 0xf8a668 = -1; 0x57f5d0 (sc); the 3-entry table 0x6f9f60 / 0x98 / 0xd0 = -1; the main loop
   while 0x5309a0 (this handler, 0x2656c7): 0x522100 (0), dt = (the clock - last) * 0.013513514; the record + 4
   set and the escape / click -> 0x6f22a0 = 1, else 0x57ec40 (sc, dt) / 0x57f0e0 (sc) / 0x57f890 / 0x57f690 (sc), the
   clicks on the ids sc + 0xc8 / 0xcc / 0xd0 -> 0x57fdd0 (rec, sc, the run, 0 / 1 / 2, 0x2656c7), and
   0x57f620 (sc) -> 0x580160 (sc, rec) and the table reset. The teardown: 0x52e330 (0x631090), the BLOCK, the
   windows freed (0x4758f0 x3, 0x46e710 x2, 0x47190 x5), 0x447300 / 0x53ec10 (rec + 0x61, 1), the scanned table
   entries +0x38 = 0, the screen 0x43b950, the camera / 0xfb9060 .. restored, 0x522100 (1), 0x524010,
   0x4478e0 (saved, 0), 0x45d690, 0x43b790 / 0x43b860 (0xfc2dec, saved), 0x513590 (0x633738, 0), 0x447860 (1),
   the BLOCK closed, 0x4eeee0 (rec + 1, 0, 0) */

char * gv_FindEffect(int id); void unk_Unk4eea70(char *o); int cm_GetSlotState(); void unk_Unk4eeee0();
struct D2FADE *d2_StartFade(int x, int y, int w, int h, char *name, int dur, unsigned int flags); /* 0x4767d0, d2.h */
void d3_SetViewport(float w, float h, float x, float y, float zn, float zf, float scale, int a, int b); void sv_SetPanelEnabled(); void d3sky_Init(struct D3SKY *s, char build);
void d3t_GetObjectBounds(); 
void ai_Unk580a90(); void ai_Unk57f890(); char d3if_ShowTree(struct D3OBJECT *obj, char show);
struct D3OBJECT;
char d3if_Delete(struct D3OBJECT *obj);                 /* 0x447190, d3_interface.h */
char d3if_Show(struct D3OBJECT *obj, char show);        /* 0x4471b0, d3_interface.h */
void he_Unk53ec10(); void ai_Unk57f5d0(); void ai_Unk57ec40(char *sc, float dt); void ai_Unk57f0e0();
void ai_Unk57f690(); void ai_Unk57fdd0(char *rec, float *sc, char *run, float sel, int mask); int ai_Unk57f620(); void ai_Unk580160();
void sv_SetStatusText(char *s); void d2_RemoveObj(); void sv_RedrawScreen();
void d3if_Prerender(char map); 

void ai_Unk57e270(char *rec)
{
    char sc[0xe4];
    int list[0x300];
    int obj[4];
    int sel;
    int n;
    int item;
    char *run;
    char *pb;
    int m;
    int last;
    int now;
    float dt;
    char *cam;
    int cam3[3];
    int save3[3];
    int bg[4];
    int bg5;
    char a4[4];
    char b4[4];
    int *pp;

    *(unsigned char *)((unsigned)&kb_Down + 0x2f) = 0;
    memset(sc, 0, 0xe4);
    last = U_I((unsigned)&ai_Unk6333cc);
    if (!U_I(rec + 0x61))
        return;
    run = (char *)gv_FindEffect(U_I(rec + 1));
    if (!run) {
        m = ((int (*)(int))unk_Unk4eea70)(rec);
        if (m != -1)
            while (cm_GetSlotState(m) == 0)
                sv_RunFrame(0, 0x20007);
        run = (char *)gv_FindEffect(U_I(rec + 1));
        if (!run)
            return;
    }
    unk_Unk4eeee0(U_I(rec + 1), ai_Unk57e230, (char *)(unsigned)&full_data_6fa020);
    cam = (char *)U_I((unsigned)&ai_Unkfc2dec);
    cam3[0] = U_I(cam + 0x4c);
    cam3[1] = U_I(cam + 0x50);
    cam3[2] = U_I(cam + 0x54);
    save3[0] = U_I(cam + 0x84);
    save3[1] = U_I(cam + 0x88);
    save3[2] = U_I(cam + 0x8c);
    bg[0] = U_I((unsigned)&ai_Unkfb9068);
    bg[1] = U_I((unsigned)&ai_Unkfb9060);
    bg[2] = U_I((unsigned)&ai_Unkfb9064);
    bg[3] = U_I((unsigned)&ai_Unkfb906c);
    bg5 = U_I((unsigned)&d3_CurrentScene);
    d2_SetPointerSprite(0, 0);
    sv_Unk52ae40();
    d2_ClearPointerItems();
    sv_ResetScreenClip();
    U_I((unsigned)&sv_PanelFlag) = 0;
    U_I((unsigned)&ai_Unkf8aae0) = 0;
    pb = (char *)d2_StartFade(0, 0, U_I((unsigned)&d2_ScreenW), U_I((unsigned)&d2_ScreenH), (char *)(unsigned)"BLACK", 0x5a, 1);
    while (!(*(unsigned char *)pb & 4))
        sv_RunFrame(0, 0x10180);
    n = 0;
    {
        int *lp = (int *)((unsigned)&ai_Unkfb1030 + 0x190);
        for (; (int)lp < (unsigned)&d3s_QuadUV; lp += 0xab / 4) {
            if (lp[-4] != 0 && lp[0] != 0) {
                int i = 0;
                char *e = (char *)U_I((unsigned)&d2_Objs);
                while (e < (char *)0x8c400 + U_I((unsigned)&d2_Objs)) {
                    if (*(int *)(e + 0x18) != 0 && *(int *)(e + 0x40) == (int)(lp - 0x68) && *(int *)(e + 0x38) == 0) {
                        *(int *)(e + 0x38) = 1;
                        list[n++] = i;
                    }
                    i++;
                    e += 0x2ec;
                }
            }
        }
    }
    d3_SetViewport((float)U_I((unsigned)&d2_ScreenW), (float)U_I((unsigned)&d2_ScreenH) - 150.0f, 0.0f, 75.0f, *(float *)&U_I((unsigned)&d3_ZNear), *(float *)&U_I((unsigned)&d3_ZFar), *(float *)&U_I((unsigned)&d3_ScaleX), (float)U_I((unsigned)&d2_ScreenW), (float)U_I((unsigned)&d2_ScreenH));
    U_I((unsigned)&ai_Unkfb9060) = U_I((unsigned)&d2_ScreenW);
    U_I((unsigned)&ai_Unkfb906c) = U_I((unsigned)&d2_ScreenH) - 0x4b;
    U_I((unsigned)&ai_Unkfb9068) = 0;
    U_I((unsigned)&ai_Unkfb9064) = 0x4b;
    sv_SetPanelEnabled(0);
    cam3[0] = U_I(cam + 0x4c);
    cam3[1] = U_I(cam + 0x50);
    cam3[2] = U_I(cam + 0x54);
    save3[0] = U_I(cam + 0x84);
    save3[1] = U_I(cam + 0x88);
    save3[2] = U_I(cam + 0x8c);
    d3if_SelectScene(0, 0);
    if (U_I((unsigned)&d3_Sky))
        d3sky_Init(U_I((unsigned)&d3_Sky), 1);
    d3t_GetObjectBounds(a4, b4, U_I(rec + 0x61));
    if (U_I((unsigned)&ai_Unk925100) != U_I(rec + 1)) {
        U_I((unsigned)&ai_Unk925114) = 0;
        U_I((unsigned)&ai_Unk925118) = 0;
        U_I((unsigned)&ai_Unk92511c) = 0;
        U_I((unsigned)&ai_Unk925108) = save3[0];
        U_I(((unsigned)&ai_Unk925108 + 0x4)) = save3[1];
        U_I(((unsigned)&ai_Unk925108 + 0x8)) = save3[2];
        U_I((unsigned)&ai_Unk925100) = U_I(rec + 1);
    }
    d3_PointToWorld(U_I(rec + 0x61), U_I(rec + 0x61) + 0x4c, (float *)sc);
    *(int *)(sc + 0x10) = U_I((unsigned)&ai_Unk925108);
    *(int *)(sc + 0x14) = U_I(((unsigned)&ai_Unk925108 + 0x4));
    *(int *)(sc + 0x18) = U_I(((unsigned)&ai_Unk925108 + 0x8));
    *(float *)(sc + 4) = (*(float *)(sc + 0xe8) - *(float *)(sc + 0xf8)) * 0.8f + *(float *)(sc + 4);
    pp = 0;
    obj[3] = d3_CreateObject(0, (char *)(unsigned)&ai_Unk5ebba4, (char *)&pp, (char *)(unsigned)&full_data_62a350);
    obj[0] = d3_CreateObject(obj[3], (char *)(unsigned)&ai_Unk5ebba4, (char *)(unsigned)&ai_Unk5ebbb4, (char *)(unsigned)&full_data_62a340);
    obj[1] = d3_CreateObject(obj[3], (char *)(unsigned)&ai_Unk5ebba4, (char *)(unsigned)&ai_Unk5ebbb4, (char *)(unsigned)&full_data_62a328);
    obj[2] = d3_CreateObject(obj[3], (char *)(unsigned)&ai_Unk5ebba4, (char *)(unsigned)&ai_Unk5ebbb4, (char *)(unsigned)&full_data_62a310);
    sel = U_I((unsigned)&ai_Unk92511c);
    *(int *)(sc + 0x20) = 0;
    d3_SetObjectPos(obj[3], (float *)sc);
    d3_SetObjectRot(obj[3], sc + 0x10);
    item = d3_CreateObject(obj[3], (char *)((unsigned)&ai_Unk5eeab8 + 0x1c8), (char *)(unsigned)&ai_Unk5ebbb4, (char *)(unsigned)&full_data_62a2fc);
    d3t_PlayAnim(item, (char *)(unsigned)&full_data_62a2dc, 1);
    *(unsigned char *)(item + 0x212) = (*(unsigned char *)(item + 0x212) & 0xf7) | 4;
    *(unsigned char *)(item + 0x212) |= 0x40;
    *(unsigned char *)(item + 0x211) &= 0xfd;
    d2_ClearSurface(U_I((unsigned)&d2_ScreenSurf));
    sv_RunFrame(0, 0x86);
    d2_ClearSurface(U_I((unsigned)&d2_ScreenSurf));
    d2_ReleaseFade(pb);
    d2_StartFade(0, 0, U_I((unsigned)&d2_ScreenW), U_I((unsigned)&d2_ScreenH), (char *)(unsigned)"BLACK", 0x5a, 10);
    ai_Unk580a90((float *)sc, run);
    ai_Unk57f890(rec, (float *)sc, run);
    if (*(int *)(sc + 0xdc) < sel)
        sel = *(int *)(sc + 0xdc);
    for (m = 0; m < 3; m++)
        d3if_Show((struct D3OBJECT *)obj[m], 0);
    d3if_Show((struct D3OBJECT *)obj[sel], 1);
    d3if_ShowTree(U_I(rec + 0x61), 0);
    he_Unk53ec10(U_I(rec + 0x61), 0);
    U_I((unsigned)&d2_ClickedId) = -1;
    ai_Unk57f5d0((float *)sc);
    U_I((unsigned)&full_data_6f9fd0) = -1;
    U_I((unsigned)&full_data_6f9f98) = -1;
    U_I((unsigned)&full_data_6f9f60) = -1;
    while (sv_RunFrame(ai_Unk57e270, 0x2656c7) != 0) {
        sv_SetPanelEnabled(0);
        now = U_I((unsigned)&ai_Unk6333cc);
        dt = (float)(now - last) * 0.013513514f;
        if (U_I(run + 4) == 0 && (U_I(((unsigned)&dm_L + 0x20)) != 0 || (U_I((unsigned)&d2_ClickedId) != -1 && U_I((unsigned)&ai_Unk5fe710) == *(int *)(sc + 0xbc)))) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else {
            ai_Unk57ec40((float *)sc, dt);
            ai_Unk57f0e0((float *)sc);
            ai_Unk57f890(rec, (float *)sc, run);
            ai_Unk57f690((float *)sc);
            if (U_I(run + 4) == 0) {
                if (U_I((unsigned)&d2_ClickedId) != -1) {
                    if (U_I((unsigned)&ai_Unk5fe710) == *(int *)(sc + 0xc8))
                        ai_Unk57fdd0(rec, (float *)sc, run, 0, 0x2656c7);
                    else if (U_I((unsigned)&ai_Unk5fe710) == *(int *)(sc + 0xcc))
                        ai_Unk57fdd0(rec, (float *)sc, run, 1, 0x2656c7);
                    else if (U_I((unsigned)&ai_Unk5fe710) == *(int *)(sc + 0xd0))
                        ai_Unk57fdd0(rec, (float *)sc, run, 2, 0x2656c7);
                }
                if (U_I(run + 4) == 0 && ai_Unk57f620((float *)sc) != 0) {
                    ai_Unk580160((float *)sc, rec);
                    U_I((unsigned)&full_data_6f9fd0) = -1;
                    U_I((unsigned)&full_data_6f9f98) = -1;
                    U_I((unsigned)&full_data_6f9f60) = -1;
                }
            }
        }
        last = now;
    }
    sv_SetStatusText((char *)(unsigned)&ch_EmptyName);
    pb = (char *)d2_StartFade(0, 0, U_I((unsigned)&d2_ScreenW), U_I((unsigned)&d2_ScreenH), (char *)(unsigned)"BLACK", 0x5a, 1);
    while (!(*(unsigned char *)pb & 4))
        sv_RunFrame(0, 0x10080);
    d2_CloseView(*(int *)(sc + 0xa4));
    d2_CloseView(*(int *)(sc + 0xc0));
    d2_CloseView(*(int *)(sc + 0x74));
    d2_RemoveObj(*(int *)(sc + 0xb4));
    d2_RemoveObj(*(int *)(sc + 0xb8));
    for (m = 0; m < 4; m++)
        if (obj[m])
            d3if_Delete((struct D3OBJECT *)obj[m]);
    if (item)
        d3if_Delete((struct D3OBJECT *)item);
    d3if_ShowTree(U_I(rec + 0x61), 1);
    he_Unk53ec10(U_I(rec + 0x61), 1);
    for (m = 0; m < n; m++)
        *(int *)(U_I((unsigned)&d2_Objs) + 0x38 + list[m] * 0x2ec) = 0;
    d3_SetViewport((float)(U_I((unsigned)&sv_ViewRectRight) - U_I((unsigned)&sv_ViewRectLeft)), (float)(U_I((unsigned)&sv_ViewRectBottom) - U_I((unsigned)&sv_ViewRectTop)), (float)U_I((unsigned)&sv_ViewRectLeft), (float)U_I((unsigned)&sv_ViewRectTop), *(float *)&U_I((unsigned)&d3_ZNear), *(float *)&U_I((unsigned)&d3_ZFar), *(float *)&U_I((unsigned)&d3_ScaleX), (float)U_I((unsigned)&d2_ScreenW), (float)U_I((unsigned)&d2_ScreenH));
    U_I((unsigned)&ai_Unkfb9068) = bg[0];
    U_I((unsigned)&ai_Unkfb9060) = bg[1];
    U_I((unsigned)&ai_Unkfb9064) = bg[2];
    U_I((unsigned)&ai_Unkfb906c) = bg[3];
    sv_SetPanelEnabled(1);
    sv_RedrawScreen();
    d3if_SelectScene(bg5, 0);
    if (U_I((unsigned)&d3_Sky))
        d3sky_Init(U_I((unsigned)&d3_Sky), 1);
    d3_SetObjectPos(U_I((unsigned)&ai_Unkfc2dec), (float *)cam3);
    d3_SetObjectRot(U_I((unsigned)&ai_Unkfc2dec), (char *)save3);
    init_SetDetailLevel(U_I((unsigned)&ai_Unk633738), 0);
    d3if_Prerender(1);
    d2_ReleaseFade(pb);
    d2_StartFade(0, 0, U_I((unsigned)&d2_ScreenW), U_I((unsigned)&d2_ScreenH), (char *)(unsigned)"BLACK", 0x5a, 10);
    unk_Unk4eeee0(U_I(rec + 1), 0, 0);
}

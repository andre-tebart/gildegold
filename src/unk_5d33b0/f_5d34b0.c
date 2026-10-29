/* 0x5d34b0, the screen file selector window (849 B): param_1 the pump record (0x5309a0), param_2 the
   flags (bit 0: 1 returns the chosen name through param_6 instead of selecting it), param_3 the caption
   text id (0x515960), param_4 the directory / name prefix, param_5 the extension and param_6 the output
   buffer. The window "s_menu_fileselector" 0x62d7e8 (0x4741f0 / 0x476f80) shows the list of 0x5d33b0
   (param_4, param_5) as up to 32 entries of 0x210 bytes: the control ids go into the first dword of each
   slot, the control records 0xf8f080 + id * 0x2ec get visible +0x4c = 1, +0x5c = 1, the word +0x14 from
   0x6ed980 + 8 and the picture +0x1c0 = 3; the count text comes from 0x5e56c4 (itoa) and 0x4734d0. The
   pump 0x5309a0 runs with this function as its callback; a click (0x5fe710) on an entry selects it
   (0x475cb0) or sprintf's "%s\\%s%s" 0x62d7e0 into param_6, the cancel control (the text 0x1ccc)
   does the same with the empty name.
   ROUGH (layout 2026-10-24): the cancel-path sprintf has one more argument in the original than
   Ghidra recovered, and the two control fields +0x5c / +0x14 are unchecked.
   @deep meaning matches the original (head read: win creation 0x4741f0/0x475600/0x476f80/0x475f70, the 6-arg 0x475ff0 control, the 4-arg 0x52e860, ai_ListFiles, the 0x210 record writes; tail read: click branch 0x475cb0/sprintf, cancel branch d2_GetObjValue + sprintf with the 5 effective args); after removing the duplicated __chkstk call (ai_Unk5de9c0 = 0x5de9c0, automatic) and adding the missed x14 = -1 init, 246 code hunks remain from register allocation (win ebp vs ebx), local slot order and VC6 push/cleanup scheduling (the original reuses the pending d2_GetObjValue argument slot as the sprintf 5th arg: add esp,4 + add esp,0x14; ours re-pushes it, add esp,0x1c) */

void d2_CentreViewWindows(int view); void d2_SetPointerSprite(int a, int pic); int d2_SetForm(int view, int n);
void d2_CloseView(int view);
int io_PrintText(char *fmt, ...); int sv_CreateFormSlider(); int d2_GetWinObj(int view, int idx, int k);
/* Callees whose units have no header here (type debt: the canonical prototypes live in d2.h, f3gm.c,
   gv.h, cm.h and he.h; the pointer views follow this TU's declarations). d2_GetViewWin keeps the K&R
   declaration for now: 0x5d34b0 pushes six arguments to the two-parameter d2.c function, see @original
   at its call; the prototype below covers 0x5d8080. */
int d2_GetViewWin();                        /* 0x475ff0, d2.c: window i of a view, -1 if out of range */
int gm_BuildingSkill(struct AIBLD *bld, int a, int b); /* 0x5092e0, f3gm.c: the building skill (ai.h's
                                               prototype; f3snd.h's building view is unsigned char *) */
char *gv_FindAccused(char *c, int max, int *ids, int *cnt);  /* 0x4f41b0, gv.c: the character with the
                                               most records of its table not yet paired with c */
void cm_SendProofExecuted(int record, int character); /* 0x4c4ed0, cm.c: message 0x24, the pair (character,
                                               record) into the executed table 0x91b6e0 */
void cm_SendMoneyDelta(int character, int delta); /* 0x4c5f60, cm.c: message 0x5a, adds delta to the
                                               character's money (+0x194), clamped 0 .. 0x32 */
int he_ShowNews(int pid, int fl, char *text, int unused, int ev, char *text2); /* 0x53dda0, he.c */
void d2_SetObjValue(); int d2_GetObjValue(); int d2_CreateWinText(short x, short y, char *text, int win); void d2_SetObjText(int obj, char *text);
int ai_Unk5e56c4(); void d2_SetObjFont(int obj, int font); int sv_RunFrame(int cb, unsigned mask);

int ai_Unk5d34b0(int param_1, int param_2, int param_3, int param_4, int param_5, int param_6)
{
    char list[0x4200];
    char num[0x20];
    int win;
    int n;
    int i;
    int ctrl;
    int sel;
    int cancel;
    int x14;
    int x18;
    int flag;
    int ret;

    ret = 0;
    sel = -1;
    cancel = -1;
    x14 = -1;
    win = d2_OpenForm(0, 0, "s_menu_fileselector");
    d2_CentreViewWindows(win);
    d2_SetPointerSprite(0, 0);
    d2_SetForm(win, 2);
    io_PrintText(param_3);
    d2_SetForm(win, 0);
    ctrl = d2_GetViewWin(win, 1, 0x18, &x14, &x18, &sel); /* @original: the original pushes
        six arguments (win, 1, 0x18, &x14, &x18, &sel) to the two-parameter 0x475ff0; the K&R
        declaration at the top of the file keeps those pushes */
    sv_CreateFormSlider(0x140, 0x100, U_I((unsigned)&d2_LastWindow), ctrl);
    for (i = 0; i < 0x1080; i++)
        ((int *)list)[i] = 0;
    n = ai_ListFiles((char *)param_4, (char *)param_5, list);
    flag = param_2 & 1;
    if (flag == 0) {
        d2_SetForm(win, 3);
        i = io_PrintText(0x1ccc);
        cancel = d2_GetWinObj(win, 3, i);
        d2_SetForm(win, 0);
        x14 = d2_GetWinObj(win, 0, 0);
        d2_SetObjValue(x14, U_I((unsigned)&full_data_14414f4));
    }
    d2_SetForm(win, 1);
    for (i = 0; i < n; i++) {
        ctrl = d2_CreateWinText(0, i * 0x18, (int)(list + i * 0x210 + 9), U_I((unsigned)&d2_LastWindow));
        ((int *)(list + i * 0x210))[0] = ctrl;
        U_I(U_I((unsigned)&d2_Objs) + 0x4c + ctrl * 0x2ec) = 1;
        U_I(U_I((unsigned)&d2_Objs) + 0x5c + ctrl * 0x2ec) = 1;
        *(unsigned short *)(U_I((unsigned)&d2_Objs) + 0x14 + ctrl * 0x2ec) = *(unsigned short *)(U_I((unsigned)&d2_LastWindowPtr) + 8);
        U_I(U_I((unsigned)&d2_Objs) + 0x1c0 + ctrl * 0x2ec) = 3;
    }
    ai_Unk5e56c4(n, num, 10);
    d2_SetObjText(sel, num);
    d2_SetObjFont(sel, 0x54);
    while (sv_RunFrame((int)ai_Unk5d34b0, param_1) != 0) {
        if (U_I(((unsigned)&dm_L + 0x20)) != 0)
            U_I((unsigned)&sv_PopRequest) = 1;
        if (U_I(((unsigned)&dm_L + 0x18)) != 0) {
            for (i = 0; i < n; i++) {
                if (U_I((unsigned)&ai_Unk5fe710) == ((int *)(list + i * 0x210))[0]) {
                    if (flag == 0) {
                        d2_SetObjValue(x14, (int)(list + i * 0x210 + 9));
                    } else {
                        crt_Sprintf(param_6, "%s\\%s%s", param_4, list + i * 0x210 + 9, param_5);
                        U_I((unsigned)&sv_PopRequest) = 1;
                        ret = 1;
                    }
                    break;
                }
            }
        }
        if (U_I((unsigned)&d2_ClickedId) != -1 && U_I((unsigned)&ai_Unk5fe710) == cancel) {
            if (flag == 0) {
                i = d2_GetObjValue(x14, param_5);
                crt_Sprintf(param_6, "%s\\%s%s", param_4, i, param_5);
            }
            U_I((unsigned)&sv_PopRequest) = 1;
            ret = 1;
        }
    }
    d2_CloseView(win);
    return ret;
}

/* 0x5d7f10, the message filter window "menu\messagefilter" 0x62dbc8 (390 B), opened from the message
   box's menu: the nine message kinds 0x605 .. 0x60d become a column of checkbox rows (d2_WinOpenPic
   0x4731d0 at x 8, y i * 0x1c + 10, the picture i + 0x605, on the window 0x6ed938): each row is a
   checkbox (checkable and interactive), gets the font 0x55 and the caption ai_MessageFilterTexts[i],
   a 0x78 x 0x1e text box (picW, captionX 0x1e, captionY 3) and is checked while the kind is not
   filtered (he_IsMessageKindFiltered 0x575640). The screen is pumped (sv_RunFrame 0x5309a0 (this,
   0x677c7)) until the pump returns 0; a mouse event / the key 1 closes it (sv_PopRequest 0x6f22a0)
   after every row's checkbox state has been written back into the filter mask
   (he_SetMessageKindFiltered 0x575710, checked = not filtered), and d2_CloseView 0x4758f0 frees the
   view. Void.
   @deep meaning matches (read against build/ghidra/5d7f10.c and the disassembly 0x5d7f10..0x5d8098: the nine checkbox rows, the mask get/set, both loops and the close path are the same statements); readable done (Q1-Q7). The 77 code hunks are VC6's induction-variable strength reduction - ours turns 'i * 0x1c + 10' and 'i + 0x605' into incremented IVs (mov ebp,0x605 / mov [esp+0x10],0xa / add esi,0x1c / add ebx,4) where the original recomputes the lea chain per iteration (lea ecx,[ebp*8]; sub ecx,ebp; lea edx,[ecx*4+0xa]) - plus the stack frame (ours 0x2c vs the original 0x44: the original spills wnd at [esp+0x10] and re-reads it before d2_CloseView, ours keeps it in a register; rows[i] at [esp+0x14]). tools/spell.py: no menu rule on the code hunks; the three reload leads read as that same slot allocation (keep the value in a local / drop the local re-read) - register/slot/IV allocation is match-deep's toolbox */

/* The message filter's row captions (the text system fills the 0x1441500 .. slots at run time) and
   the getter / setter of its mask (he.c: message kind 0x605 .. 0x60d <-> bit 0 .. 0x100 of 0x6f263c). */
/* ai_MessageFilterTexts: view of txt_Ptrs (build/mk_txt_views.py) */
int he_IsMessageKindFiltered(int kind); /* 0x575640, he.c: nonzero when the kind is filtered */
void he_SetMessageKindFiltered(int kind, int filtered); /* 0x575710, he.c: sets / clears the kind's bit */
void cm_SendSetField(int a, int b, int c, int d, int e); /* 0x4c4a60, cm.c: message 0x19 (the field block) */
void d2_SetCaption(int obj, char *text);           /* 0x473540, d2.h: sets the caption and its width */

void ai_MessageFilterScreen(void)
{
    int wnd, pump, i, id;
    int rows[9];

    wnd = d2_OpenForm(0, 0, "menu\\messagefilter");
    d2_CentreViewWindows(wnd);
    d2_SetForm(wnd, 2);
    i = 0;
    do {
        id = d2_WinOpenPic(8, i * 0x1c + 10, i + 0x605, d2_LastWindow);
        rows[i] = id;
        d2_Objs[id].checkable = 1;
        d2_Objs[id].interactive = 1;
        d2_SetObjFont(id, 0x55);
        d2_SetCaption(id, ai_MessageFilterTexts[i]);
        d2_Objs[id].picW = 0x78;
        d2_Objs[id].captionX = 0x1e;
        d2_Objs[id].captionY = 3;
        d2_SetObjValue(id, he_IsMessageKindFiltered(i + 0x605) == 0); /* @original: this call site
            pushes 2 arguments against the 5-parameter K&R definition (0x5d7fe5) */
        i++;
    } while (i < 9);
    pump = sv_RunFrame(ai_MessageFilterScreen, 0x677c7);
    while (pump != 0) {
        if (dm_L.rup != 0 || kb_Last == 1) {
            sv_PopRequest = 1;
            i = 0;
            do {
                he_SetMessageKindFiltered(i + 0x605, d2_GetObjValue(rows[i]) == 0);
                i++;
            } while (i < 9);
        }
        pump = sv_RunFrame(ai_MessageFilterScreen, 0x677c7);
    }
    d2_CloseView(wnd);
}

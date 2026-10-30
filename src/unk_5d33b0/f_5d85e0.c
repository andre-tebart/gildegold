/* 0x5d85e0, the gypsy blood revenge window "Locations\zigeuner\blutrache" 0x62dc68 (1709 B); the building is the
   gypsy house. When the player's status bit 8 (+0x1cc of its record) is set the green message box 0x41b2 / 0x41b3 is shown
   and the window ends. Otherwise the form is opened and five rows are laid out in a pentagon (the top row at the middle,
   the others at thirds of the width, the rows of the list ai_AmountRow). Row 0 is the avenger (the first record), rows 1 .. 4
   are the chosen targets: each is picked with the special-amount menu ai_AmountMenu (a config of 0x14 dwords: +0 = 1, +4 =
   0x400, +8 / +0xc the 64-bit filter 0x1a7fdfbb6, +0x10 = 6, +0x14 = 2, +0x1c the character ids of the other rows, 0xffff for
   an empty one). The price of the revenge is the sum over the rows of the level of the record's office / 3 + 2 (row 0 and
   empty rows cost nothing), shown through the texts "$C" and 0x41b6. Every 0x8c ms the rows are refreshed: a record that is no
   longer in use or whose net id changed is dropped. The first button of the row (ids[0]) confirms when the player can pay
   (sv_CheckMoney): the avenger's text 0x41bf, the relation of every target to the avenger drops by 50 - the mix of the two
   (cm_SendRelations), the price is paid (gm_SendMoneyDelta), the power bit 8 of +0x1cc is set (cm_SendSetField) and the
   delivery text for the chosen targets is made (ai_GypsyDeliveryText) and shown (0x41b9 with 0x41b2); the second button and
   the right mouse button leave. The pump ends with d2_CloseView and the camera follow. Returns the result of the green box
   when the window is refused. @deep body reconstructed from the disassembly (the pentagon layout, the five-row refresh with the amount-row callees, the 0x8c ms record check, the confirm branch with relations / money / power bit / delivery text, the row click building the 0x14-dword amount menu config, the pump and the tail call to the camera follow); 492 vs 496 instructions, meaning read whole, locals in the original slot order (msg, rows, cfg, sel, ids, tick, refresh, form, total). What is left is register allocation and constant hoisting: the original keeps the constants 1 / 0 in ebx / esi for the whole body (mov ebx,1 / xor esi,esi, the loop is entered past the xor esi,esi, sel[] = -1 stored through eax, form in edi with a spill at [esp+0x10], the row count in ebp, refresh at [esp+0x14]) while ours hoists -1 into ebp and keeps form in ebx, so every later store / compare differs (asmdiff: 250 instructions in 4 big code hunks, 14 order, 4 slots). Also left: the filter constant cfg[2..3] = 0xa7fdfbb6 / 1 is loaded by the original as an 8-byte copy from .rdata 0x5eed68 (mov eax,[0x5eed68]; mov ecx,[0x5eed6c]) - ours stores two immediates (tried *(__int64 *)&cfg[2] = 0x1a7fdfbb6: same code, Q4 worse, reverted). Tried: local declaration orders (slots now right), -(0x32 - x) via a temp so the neg stays (fixed that hunk), 0x5da picture index. */

typedef struct GYPSYROW {               /* 0x38 bytes: one row of the special-amount list (ai_AmountRowStatus / ai_AmountRow read it as ints) */
    AICHARVIEW *rec;                    /* +0x00, the character the row shows (0 = empty) */
    int ctl;                            /* +0x04, the row's control object (d2_Objs index) */
    int pad08[2];
    int x;                              /* +0x10, the position of the row */
    int y;                              /* +0x14 */
    int pad18[8];
} GYPSYROW;

typedef struct D2PICVIEW {              /* 0x54 bytes, d2.h's D2PIC: only the size */
    char pad00[0x50];
    short w;                            /* +0x50 */
    short h;                            /* +0x52 */
} D2PICVIEW;
extern struct D2PIC *d2_Pics;              /* 0x6ed918, d2.h: D2PIC * */

void ai_BloodRevengeWindow(int building)
{
    char msg[0x800];
    GYPSYROW rows[5];
    AMOUNTCFG cfg;                      /* the config of the amount menu */
    int sel[9];                         /* 0 .. 4 the net id of the record in each row (-1 empty), 5 .. 8 the chosen records */
    int ids[2];                         /* the two buttons the row of buttons creates */
    int tick, refresh, form, total;
    unsigned short *idp;
    GYPSYROW *row;
    int n, i, k, r;

    memset(rows, 0, sizeof(rows));
    sel[0] = -1;
    sel[1] = -1;
    sel[2] = -1;
    sel[3] = -1;
    sel[4] = -1;
    refresh = 1;
    total = 0;
    tick = 0;
    if (((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->flags1cc & 8) {
        sv_MessageBoxGreen((int)txt_Ptrs[0x41b2], (int)txt_Ptrs[0x41b3], 0);
        return;
    }
    form = d2_OpenForm(0, 0, "Locations\\zigeuner\\blutrache");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(form);
    d2_SetForm(form, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(form, 1);
    rows[0].y = 0x1e;
    rows[0].x = (d2_Wins[d2_LastWindow].w - d2_PicsV[0x5da].w) / 2;
    rows[1].y = 0x7d;
    rows[1].x = (d2_Wins[d2_LastWindow].w - d2_PicsV[0x5da].w) / 3 - 10;
    rows[2].y = 0x7d;
    rows[2].x = (d2_Wins[d2_LastWindow].w - d2_PicsV[0x5da].w) * 2 / 3 + 10;
    rows[3].y = 0xdc;
    rows[3].x = (d2_Wins[d2_LastWindow].w - d2_PicsV[0x5da].w) / 3 - 100;
    rows[4].y = 0xdc;
    rows[4].x = (d2_Wins[d2_LastWindow].w - d2_PicsV[0x5da].w) * 2 / 3 + 100;
    d2_SetForm(form, 3);
    sv_ButtonRow(d2_LastWindow, 0, ids, 2, txt_Ptrs[0x41b7], txt_Ptrs[0x41b8]);
    do {
        if (refresh != 0) {
            n = gm_BathObjectCount((struct AIBLD *)building) + 1;
            tick = d3_Time;
            refresh = 0;
            d2_SetForm(form, 1);
            io_PrintText("$C");
            io_PrintText((char *)0x41b2);
            total = 0;
            for (i = 0; i < 5; i++) {
                row = &rows[i];
                if (i >= n) {
                    row->rec = 0;
                    sel[i] = -1;
                }
                ai_AmountRowStatus((int *)row);
                ai_AmountRow(row->x, row->y, d2_LastWindow, 0x54, (int *)row, 1);
                if (i < n) {
                    d2_SetObjEnabled(row->ctl, 1);
                    d2_ObjsV[row->ctl].interactive = 1;
                    d2_ObjsV[row->ctl].style = 3;
                    d2_ObjsV[row->ctl].owner = (int)row->rec;
                    d2_ObjsV[row->ctl].greyed = 0;
                } else {
                    d2_ObjsV[row->ctl].greyed = 1;
                }
                if (row->rec == 0 || i == 0) {
                    r = 0;
                } else {
                    r = amt_GetKindLevel(row->rec->officeKind);
                    r = r / 3 + 2;
                }
                total += r;
            }
            d2_SetForm(form, 2);
            io_PrintText("$C", total);
            io_PrintText((char *)0x41b6, total);
            d2_SetObjEnabled(ids[0], rows[0].rec != 0 && total != 0);
        }
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId == -1) {
            if (d3_Time > (unsigned)(tick + 0x8c)) {
                for (i = 0; i < 5; i++) {
                    if (rows[i].rec != 0 && (rows[i].rec->id == 0xffff || rows[i].rec->netId != (unsigned)sel[i])) {
                        rows[i].rec = 0;
                        sel[i] = -1;
                    }
                }
                refresh = 1;
            }
        } else if (d2_PointerObj == ids[0]) {
            if (sv_CheckMoney(ai_CharsC + ai_PlayerId * 0x21c, total) == 1) {
                txt_Format(msg, 0x41bf, rows[0].rec->id);
                k = 0;
                for (i = 1; i < 5; i++) {
                    if (rows[i].rec != 0) {
                        r = 0x32 - (int)(gm_Mix((int)(ai_CharsC + ai_PlayerId * 0x21c), (int)rows[i].rec, 4) * -50.0f);
                        cm_SendRelations(rows[0].rec->id, rows[i].rec->id, -r, 1);
                        sel[5 + k] = (int)rows[i].rec;
                        k++;
                    }
                }
                gm_SendMoneyDelta(((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->netId, -total);
                cm_SendSetField(((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->netId, 0x1cc, 4, 8, 0);
                ai_GypsyDeliveryText((struct AIPER *)rows[0].rec, k, (struct AIPER **)&sel[5]);
                txt_Format(msg, 0x41b9, 0x41b2);
                sv_MessageBox((int)msg, 0);
                sv_PopRequest = 1;
            }
        } else if (d2_PointerObj == ids[1]) {
            sv_PopRequest = 1;
        } else {
            for (i = 0; i < 5; i++) {
                if (d2_PointerObj == rows[i].ctl)
                    break;
            }
            if (i < 5) {
                memset(&cfg, 0, sizeof(cfg));
                idp = cfg.ids;
                for (k = 0; k < 5; k++) {
                    if (k != i) {
                        *idp = rows[k].rec != 0 ? rows[k].rec->id : 0xffff;
                        idp++;
                    }
                }
                cfg.filter = gypsyFilter;
                cfg.flags = 1;
                cfg.mode = 0x400;
                cfg.kind = 6;
                cfg.option14 = 2;
                rows[i].rec = (AICHARVIEW *)ai_AmountMenu((int)&cfg, (int)txt_Ptrs[0x41b4 + (i != 0)], 0, 0);
                if (rows[i].rec != 0)
                    sel[i] = rows[i].rec->netId;
                else
                    sel[i] = -1;
                refresh = 1;
            }
        }
    } while (sv_RunFrame((int)ai_BloodRevengeWindow, 0x677c7) != 0);
    d2_CloseView(form);
    sv_CameraFollow();
}

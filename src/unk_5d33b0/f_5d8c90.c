/* 0x5d8c90, the "Fluch wirken" (curse) window "Locations\zigeuner\blutrache" 0x62dc68 (905 B): the building is the
   gypsy house. When the player's status bit 0x10 (+0x1cc of its record) is set the voice "RAUSWURF" is played and the green
   message box 0x41c1 / 0x41c2 refuses. Otherwise the target is chosen with the special-amount menu ai_AmountMenu (the same
   config as the blood revenge window: +0 = 1, +4 = 0x400, +8 / +0xc the filter 0x1a7fdfbb6, +0x10 = 6, the id list from +0x1c all
   0xffff); no target ends the window. The form shows a slider (0 .. 5 at first, then 0 .. the limit gm_CurseObjectCount - 1) with
   the six strength names 0x41ca .. 0x41cf and the two buttons 0x41d4 / 0x41d5. Each pass the strength v is clamped to the limit,
   the texts 0x41d0 + v (with the target's id), 0x41d6 / 0x41c1 / 0x41c4 + v and, for a building of another owner, the price
   max (v, 1) * 32000 (0x41d7) are shown. The right mouse button and the second button leave; the first button curses when the
   player has the goods (sv_CheckGoodAmount): the status bit 0x10 is set (cm_SendSetField), another owner is paid the price
   (cm_SendMoveGoodsUnchecked), the message 0x9d (arg 0 = the strength, arg 2 = the target's net id, the game time, flags 2) is sent
   (cm_SendLongMessage), the voice "_WAHRSAGERIN_FLUCH_0<v>" is played and the text 0x41da is shown. The pump ends with
   d2_CloseView and the camera follow. @deep body reconstructed and read whole against the disassembly: 316 = 316 instructions, 1103 = 1103 bytes, the amount-menu config (AMOUNTCFG, the 64-bit filter loaded from a const volatile object like the original's .rdata pair), form / slider / buttons, the strength loop, the confirm branch with the 0xf8-byte message and the tail jumps all match. ONE hunk of 4 differing instructions is left at 0x5d9026: the original stores the last word of the game time copy (mov [esp+0xa8],ax) after the push of the message pointer (mov ecx,[ebp+4]; lea edx,[esp+0x70]; push edx; mov [esp+0xa8],ax; mov [esp+0xd0],ecx) while ours stores it before (mov [esp+0xa4],ax; ...; push edx), a pure scheduling difference of one store across the push. Tried the order of msg.to / the time copy / msg.arg[2] (three orders tried: this one is the best, the others 16 and 49 instructions), memcpy vs a struct copy (the struct copy needs the Q4 cast of the char ai_Time[]). */
/* batch 3: the gypsy / poker / particle stretch 0x5d8c90 .. 0x5dd3e0 */
void sv_ButtonRow(int f, int y, int *out, int n, ...); int sv_CheckGoodAmount();
struct GCLOCK;
int gm_ConvertMoney(int a, unsigned char k); int gm_PlayerFactor(int a, unsigned char k);
int ai_PokerStart();
int ai_PokerStep(); int gm_BuildingSkill(); float ai_NormalizeEffectScale(float x);

void ai_CurseWindow(int building)
{
    char text[0x200];
    char name[100];
    CURSEMSG msg;
    AMOUNTCFG cfg;                      /* the config of the amount menu */
    int ids[2];                         /* the two buttons */
    int slider, form;
    AICHARVIEW *target;
    int limit, v;

    if (((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->flags1cc & 0x10) {
        snd_PlayVoice(-7, 0, "RAUSWURF", -1);
        sv_MessageBoxGreen((int)txt_Ptrs[0x41c1], (int)txt_Ptrs[0x41c2], 0);
        return;
    }
    memset(&cfg, 0, sizeof(cfg));
    memset(cfg.ids, 0xff, sizeof(cfg.ids));
    cfg.flags = 1;
    cfg.mode = 0x400;
    cfg.kind = 6;
    cfg.filter = gypsyFilter;
    target = (AICHARVIEW *)ai_AmountMenu((int)&cfg, (int)txt_Ptrs[0x41c3], 0, 0);
    if (target == 0) {
        sv_CameraFollow();
        return;
    }
    form = d2_OpenForm(0, 0, "Locations\\zigeuner\\blutrache");
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(form);
    d2_SetForm(form, 0);
    sv_FitForm(d2_LastWindow);
    d2_SetForm(form, 2);
    slider = d2_WinOpenSlider(0x57, 7, 200, 0, 100, 0x7b, 2, d2_LastWindow);
    d2_SetObjValue5(slider, 0, 5, 0, 1);
    d2_SetList(slider, 6, txt_Ptrs[0x41ca], txt_Ptrs[0x41cb], txt_Ptrs[0x41cc], txt_Ptrs[0x41cd], txt_Ptrs[0x41ce],
               txt_Ptrs[0x41cf]);
    d2_SetForm(form, 3);
    sv_ButtonRow(d2_LastWindow, 0, ids, 2, txt_Ptrs[0x41d4], txt_Ptrs[0x41d5]);
    do {
        limit = gm_CurseObjectCount((struct AIBLD *)building) - 1;
        v = d2_GetObjValue(slider);
        if (v >= limit)
            v = limit;
        d2_SetObjValue5(slider, 0, limit, v, 1);
        txt_Format(text, v + 0x41d0, target->id);
        d2_SetForm(form, 1);
        io_PrintText("$C");
        io_PrintText((char *)0x41d6, 0x41c1, v + 0x41c4, text);
        if (ai_PlayerId != ((CURSEBLD *)building)->owner)
            io_PrintText((char *)0x41d7, (v > 1 ? v : 1) * 32000);
        if (dm_L.rup != 0) {
            sv_PopRequest = 1;
        } else if (d2_ClickedId != -1) {
            if (d2_PointerObj == ids[0]) {
                if (sv_CheckGoodAmount(ai_CharsC + ai_PlayerId * 0x21c, v, ai_OwnPlayer) != 0) {
                    cm_SendSetField(((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->netId, 0x1cc, 4, 0x10, 0);
                    if (ai_PlayerId != ((CURSEBLD *)building)->owner)
                        cm_SendMoveGoodsUnchecked(-1, ((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->netId, ai_OwnPlayer,
                                                  (v > 1 ? v : 1) * 32000);
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0x9d;
                    msg.fromId = -1;
                    msg.flags = 2;
                    msg.arg[0] = v;
                    msg.to = ((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->netId;
                    memcpy(&msg.time, ai_Time, sizeof(GAMECLOCK));
                    msg.arg[2] = target->netId;
                    cm_SendLongMessage((char *)&msg);
                    txt_Format(text, 0x41da, 0x41c1);
                    sprintf(name, "_WAHRSAGERIN_FLUCH_0%d", v);
                    snd_PlayVoice(-7, 0, name, -1);
                    sv_MessageBox((int)text, 0);
                    sv_PopRequest = 1;
                }
            } else if (d2_PointerObj == ids[1]) {
                sv_PopRequest = 1;
            }
        }
    } while (sv_RunFrame((int)ai_CurseWindow, 0x677c7) != 0);
    d2_CloseView(form);
    sv_CameraFollow();
}

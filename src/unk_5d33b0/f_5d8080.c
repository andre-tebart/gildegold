/* 0x5d8080, the tailor chat request window "locations\schneider\plausch" 0x62dbf8 (733 B, a
   Ghidra-missed start; the contact prompt 0x5d8360 opens it through 0x5c1130): the window lists the
   character with the most unclaimed graveyard evidence records (gv_FindAccused, up to the building
   skill 0x5092e0 + 1 record ids) and prints its name (the request id word, the count). A click on
   the list marks those records executed for the player (message 0x24, cm_SendProofExecuted), clears the
   +0x194 value of the player and of the request handle (message 0x5a, cm_SendMoneyDelta) and sends the
   event texts 0x422f / (for the request kinds 6 / 7) 0x4230 + "_NACHRICHTEN_NEU_15"; without a
   found character only the request handle is cleared. The mouse event 0x14673a0 / the answer
   0xf8a668 == 1 sets sv_PopRequest 1, a click on the control (d2_PointerObj == obj) 3. The pump
   0x5309a0 (this function, 0x67fff) ends with 0x4758f0 (wnd). Void. */

/* The request record of the screen callbacks (0x5c bytes, opened by 0x5c1130): the id word, the
   kind byte (6 / 7 = a player character) and the player handle. */
typedef struct AIREQ {
    unsigned short id;                  /* +0x00 */
    unsigned char kind;                 /* +0x02 */
    char pad03;
    int handle;                         /* +0x04 */
    char pad08[0x54];
} AIREQ;

int d2_GetViewWin(int view, int i);         /* 0x475ff0: the prototype of the K&R declaration above */

void ai_TailorChatWindow(int request)
{
    AIREQ *req;
    AICHARVIEW *rec;
    int count;
    int obj;
    int found;
    int ids[4];                         /* the record ids the query fills (its max is 4) */
    char buf[0x400];                    /* the two event texts are built at buf and buf + 0x10 */
    int limit;
    int wnd;
    int pump;
    int i;

    req = (AIREQ *)request;
    rec = (AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c);
    wnd = d2_OpenForm(0, 0, "locations\\schneider\\plausch");
    found = gv_FindAccused((char *)(ai_CharsC + ai_PlayerId * 0x21c), 4, ids, &count);
    if (ai_PlayerBuildingB != 0)
        limit = gm_BuildingSkill((struct AIBLD *)ai_PlayerBuildingB, 0x24, 1);
    else
        limit = 1;
    if (count >= limit + 1)
        count = limit + 1;
    sv_FitForm(d2_GetViewWin(wnd, 0));
    d2_CentreViewWindows(wnd);
    sv_ReleaseNamePlates();
    d2_SetForm(wnd, 1);
    io_PrintText(0x4227);
    io_PrintText(0x422b, req->id, count, 0x422c, req->id, 1);
    d2_SetForm(wnd, 2);
    obj = d2_GetWinObj(wnd, 2, io_PrintText("%ia[%s]$N", 0x422d));
    io_PrintText("%in[%s]", 0x422e);
    if (found == 0 || count == 0)
        d2_SetObjEnabled(obj, 0);
    pump = sv_RunFrame(ai_TailorChatWindow, 0x67fff);
    while (pump != 0) {
        if (dm_L.rup != 0 || d2_ClickedId == 1)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1 && obj == d2_PointerObj) {
            if (found != 0) {
                for (i = 0; i < count; i++)
                    cm_SendProofExecuted(ids[i], ((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->netId);
                cm_SendMoneyDelta(((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->netId, -1);
                cm_SendMoneyDelta(req->handle, -1);
                cm_SendSetField(rec->netId, 0x1cc, 4, 0x20000, 0);
                txt_Format(buf + 0x10, 0x422f, req->id);
                he_ShowNews(((AICHARVIEW *)(ai_CharsC + ai_PlayerId * 0x21c))->netId, -1, buf + 0x10,
                            0, 0x5ec, 0);
                if (req->kind == 6 || req->kind == 7) {
                    txt_Format(buf, 0x4230, ai_PlayerId, 1);
                    he_ShowNews(req->handle, -1, buf, 0, 0x5ec, "_NACHRICHTEN_NEU_15");
                }
            } else {
                cm_SendMoneyDelta(req->handle, -1);
            }
            sv_PopRequest = 3;
        }
        pump = sv_RunFrame(ai_TailorChatWindow, 0x67fff);
    }
    d2_CloseView(wnd);
}

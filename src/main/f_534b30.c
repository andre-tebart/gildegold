/* 0x534b30, the character model choice of the character creation: the two tables of the 0x20-byte model names (the men: "patrizier_MANN2", "wirt_DICKER", .. ending with ""; the women: "patrizierin_FRAU", ..
   ending with ""; they are built on the stack by the original), the three buttons (previous 0x1a, next 0x1b, ok 0x51b: the screen positions are float expressions lost in the pseudocode, placeholders 0) and the
   preview model of the current entry put at the dummy "dummy_SPIELER" with the effect "Effekte/choose_character.esc"; the chosen name goes to 0x75e5f5, its id to 0x75e628; p is the start index
   (0x504ad0); returns 1 when ok was pressed; ROUGH (note: the original counts both tables through the men table) */

int main_Unk534b30(int p)
{
    char men[17][0x20] = {"patrizier_MANN2", "wirt_DICKER", "arbeiter_MANN", "wirt3_MANN", "wirt2_DICKER", "buerger_MANN", "buerger2_MANN", "domherr_KUTTE", "buerger3_DICKER",
                          "patrizier_MANN2", "offizier_SOLDAT", "DIEB2_KUTTE", "raeubermeister_SOLDAT", "Nekromant_MEISTER", "zigeunermeister_MANN3", "schneider3_DICKER", ""};
    char women[17][0x20] = {"patrizierin_FRAU", "handwerkerin3_FRAU", "handwerkerin2_FRAU", "handwerkerin_FRAU", "magd_FRAU", "buergerin3_FRAU", "Bauerin_FRAU", "minerin2_FRAU", "buergerin2_FRAU",
                            "buergerin2_FRAU", "buergerin_FRAU", "arbeiterin_FRAU", "minerin_FRAU", "feldarbeiterin_FRAU", "zigeunerin_FRAU", "schneiderin3_FRAU", ""};
    char cur[0x30];
    float pos[4];
    int result, dummy, bnext, bprev, bok, idx, last, obj, nmen, nwomen, run, fx, k, m;
    char *s;

    result = 0;
    obj = 0;
    idx = gm_TitleGroup(p);
    dummy = d3_FindObject(0, 0x100, 0, "dummy_SPIELER");
    if (dummy != 0)
        d3_PointToWorld(dummy, dummy + 0x4c, pos);
    bnext = d2_CreatePicObj((int)0.0f, (int)0.0f, 0x1a);
    SV_WI2(bnext, 0x4c) = 1;
    SV_WI2(bnext, 0x1c0) = 3;
    bprev = d2_CreatePicObj((int)0.0f, (int)0.0f, 0x1b);
    SV_WI2(bprev, 0x4c) = 1;
    SV_WI2(bprev, 0x1c0) = 3;
    bok = d2_CreatePicObj((int)0.0f, (int)0.0f, 0x51b);
    SV_WI2(bok, 0x4c) = 1;
    SV_WI2(bok, 0x1c0) = 3;
    nmen = 0;
    if (men[0][0] != 0) {
        s = men[0];
        do {
            s += 0x20;
            nmen++;
        } while (*s != 0);
    }
    nwomen = 0;
    if (men[0][0] != 0) {
        s = men[0];
        do {
            s += 0x20;
            nwomen++;
        } while (*s != 0);
    }
    last = -1;
    run = sv_RunFrame((int)main_Unk534b30, 0x12d6);
    if (run != 0) {
        do {
            if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
                U_I((unsigned)&sv_PopRequest) = 1;
                result = 0;
            }
            if (*(char *)(unsigned)&kb_Last == 'N' || (U_I((unsigned)&d2_ClickedId) != -1 && ai_Unk5fe710 == bnext))
                idx++;
            else if (*(char *)(unsigned)&kb_Last == 'J' || (U_I((unsigned)&d2_ClickedId) != -1 && ai_Unk5fe710 == bprev))
                idx--;
            else if (*(char *)(unsigned)&kb_Last == 0x1c || (U_I((unsigned)&d2_ClickedId) != -1 && ai_Unk5fe710 == bok)) {
                U_I((unsigned)&sv_PopRequest) = 1;
                result = 1;
            }
            if (obj != 0)
                *(unsigned char *)(obj + 0x8c) |= 8;
            if (last != idx) {
                last = (int)evt_Load("Effekte/choose_character.esc");
                if (last != 0) {
                    fx = (int)0.0f;
                    evt_RunScriptArgs((EVTSCRIPT *)last, 6, (int)0.0f);
                }
                if (obj != 0) {
                    ch_AddAction(obj, 0x38, 0);
                    do {
                        run = sv_RunFrame(0, 0x12d6);
                        if (run == 0)
                            break;
                    } while (*(int *)(obj + 0x128) != 0);
                    ch_Delete(obj);
                }
                if (*(char *)((unsigned)&init_PlayerProfile + 0x8) == 0) {
                    m = idx % nmen;
                    if (m < 0)
                        m = nmen - 1;
                    strcpy(cur, men[m]);
                } else {
                    m = idx % nwomen;
                    if (m < 0)
                        m = nwomen - 1;
                    strcpy(cur, women[m]);
                }
                strcpy((char *)(unsigned)&full_data_75e5f5, cur);
                U_I((unsigned)&full_data_75e628) = unk_Unk4fb770(cur) + 0x639;
                obj = ch_Create(cur, pos);
                ch_PlaceAtObject(obj, dummy);
                ch_SetVisible(obj, 0);
                *(unsigned char *)(*(int *)(obj + 0x34) + 0x211) &= 0xfb;
                *(unsigned char *)(obj + 0x8c) |= 8;
                ch_AddAction(obj, 0x38, 1);
                idx = m;
            }
            run = sv_RunFrame((int)main_Unk534b30, 0x12d6);
        } while (run != 0);
        if (obj != 0)
            ch_Delete(obj);
    }
    d2_RemoveObj(bprev);
    d2_RemoveObj(bnext);
    d2_RemoveObj(bok);
    return result;
}

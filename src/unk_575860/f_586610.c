/* 0x586610, the graveyard seance (419 B): rec = the record (+0x65 the state record). 0x4ef7c0 (the character,
   rec, 0, 0x4ef740, 0xff) the grave entry -> none: 0x521150 (U_I (0x144a514), 0) and out. The window record w:
   its +0x1c the entry, +0x20 its id, +0x24 the 0x4ef6b0 flag, the text 0x4107 (the name 0x5eecd0[id]);
   0x583b70 (w, U_I (0x144a4f0)) opens the detail window -> 0: 0x52d7a0 (). 0x586560 (rec, w, out) the selection;
   0x4758f0 (w[0]). When a choice: 0x4c4a60 (the character + 4, 0x1cc, 4, 0x800000, 0); out[1] -> 0x50cc50 (the
   character, -out[1]); out[0] 1 -> 0x585cb0 (rec, out), 2 -> 0x586200 (rec, out), 3 -> 0x585f30 (rec, out);
   0x52d7a0 () */

void gm_Unk50cc50(int a, int b);

void ai_Unk586610(char *rec)
{
    int w[0x23];
    int out[2];
    char line[0x100];
    int *p;

    p = (int *)gv_FindEntry((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, rec, 0, (char *)(unsigned)&gv_EntrySameId, 0xff);
    if (p == 0) {
        sv_MessageBox((char *)U_I((unsigned)&full_data_144a514), 0);
        return;
    }
    w[7] = (int)p;
    w[8] = *p;
    *(unsigned char *)((char *)w + 0x24) = gv_EntryIndex(U_I(rec + 0x65), p);
    txt_Format(line, (char *)0x4107, U_I(((unsigned)&ai_Unk5eeab8 + 0x218) + *p * 4));
    if (ai_Unk583b70(w, (char *)U_I((unsigned)&full_data_144a4f0)) == 0) {
        sv_CameraFollow();
        return;
    }
    p = (int *)ai_Unk586560(rec, w, out);
    d2_CloseView(w[0]);
    if (p != 0) {
        cm_SendSetField(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), 0x1cc, 4, ((unsigned)&ai_Relations + 0x1aca0), 0);
        if (out[1] != 0)
            gm_Unk50cc50(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), -out[1]);
        if (out[0] == 1)
            ai_Unk585cb0(rec, (char *)out);
        else if (out[0] == 2) {
            ai_Unk586200(rec, (char *)out);
            sv_CameraFollow();
            return;
        } else if (out[0] == 3) {
            ai_Unk585f30(rec, (char *)out);
            sv_CameraFollow();
            return;
        }
    }
    sv_CameraFollow();
}

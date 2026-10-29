/* 0x5871e0, the graveyard rent-a-grave handler (278 B): rec = the contact record (+0x65 the state record). The
   entry of the player's character (0x770b80 + 0x6f25b8 * 0x21c, rec, 0, 0x4ef740, 0xff) through 0x4ef7c0;
   without an entry: 0x4f25d0 (the character, &bld, &p) == 0 -> 0x5847a0 (rec) and 0x52d7a0 (), else 0x521150
   (U_I (0x144ada0), 0). With the entry p: the window record w (+0x1c p, +0x20 *p, +0x24 the 0x4ef6b0 flag), the
   line 0x4107 (the name 0x5eecd0[*p]); 0x583b70 (w, the line) -> 0: return; 0x584410 (rec, w), 0x4758f0 (w[0]),
   0x52d7a0 () */

int unk_Unk4f25d0();

void ai_Unk5871e0(char *rec)
{
    int w[0x23];
    char line[0x100];
    char *bld;
    int *p;

    p = (int *)gv_FindEntry((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, rec, 0, (char *)(unsigned)&gv_EntrySameId, 0xff);
    if (p == 0) {
        if (unk_Unk4f25d0((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, &bld, &p) == 0) {
            ai_Unk5847a0(rec);
            sv_CameraFollow();
            return;
        }
        sv_MessageBox((char *)U_I(((unsigned)&ai_Unk144ad78 + 0x28)), 0);
        return;
    }
    w[7] = (int)p;
    w[8] = *p;
    *(unsigned char *)((char *)w + 0x24) = gv_EntryIndex(U_I(rec + 0x65), p);
    txt_Format(line, (char *)0x4107, U_I(((unsigned)&ai_Unk5eeab8 + 0x218) + *p * 4));
    if (ai_Unk583b70(w, line) == 0)
        return;
    ai_Unk584410(rec, w);
    d2_CloseView(w[0]);
    sv_CameraFollow();
}

/* 0x4e0770 */

/* 0x4e0770, the wedding (the message m: +0x34 / +0x38 the couple): the scenes "cutscenes\\hochzeit\\hochzeit-create / -intro / -reden1 .. -reden3",
   "KIRCHE_HOCHZEIT.ed3", "cd2\\Hochzeit.mp3", the sample bank "ADDON\\HOCHZEIT_NEU.sbf"; each speech: the text (0x1aa9 .. 0x1aad), the voice
   "_HOCHZEIT_NEU_MAIN_0n" waited for with 0x426f20, pauses 0x3e8 / 0x1f4 / 0x5dc through 0x4e3490, which calls the cleanup 0x4e06d0 and
   returns nonzero when the player aborts. ROUGH: the pauses between the speeches are from the disassembly, not checked */
#define G25_STEP(ms) if (ai_Unk4e3490((ms), ai_Unk4e06d0, st) != 0) goto done
#define G25_VOICE(tag)     h = ai_Unk4e32b0(-7, tag, 0);     if (sb_IsPlaying(h) != 0) {         do {             G25_STEP(0x3e8);         } while (sb_IsPlaying(h) != 0);     }     G25_STEP(0x3e8)

int ai_Unk4e0770(char *m)
{
    char buf[0x100];
    char *a, *b, *t;
    int st[4];
    int v, h;

    a = (char *)gm_FindChar(*(int *)(m + 0x34));
    if (a == 0)
        return 0;
    b = (char *)gm_FindChar(*(int *)(m + 0x38));
    if (b == 0)
        return 0;
    if (a[2] == 6)
        ai_Unk4b44e0(a, 0x28);
    if (b[2] == 6)
        ai_Unk4b44e0(b, 0x28);
    ai_Unk60d360 = 0x702c6;
    d2_CutsceneActive = 1;
    cm_SendSetField(*(int *)(a + 4), 0x1c8, 4, 0, 0x40000);
    cm_SendSetField(*(int *)(b + 4), 0x1c8, 4, 0, 0x40000);
    if (a[9] != 0) {
        t = a;
        a = b;
        b = t;
    }
    sprintf(buf, "%s %s %i", unk_Unk4fb390(*(unsigned short *)a) + 4, unk_Unk4fb390(*(unsigned short *)b) + 4, 1);
    st[2] = ai_Unk4e3270("ADDON\\HOCHZEIT_NEU.sbf", 0);
    ai_Unk4e2e60("KIRCHE_HOCHZEIT.ed3");
    ai_Unk4e3620("cd2\\Hochzeit.mp3");
    v = d2_OpenForm(0, 0, "cutscenes\\hochzeit");
    ((void (*)(int, int))d2_PlaceMainWindow)(v, 3);
    d2_SetViewEnabled(v, 0);
    st[1] = v;
    st[3] = 0;
    st[0] = ai_Unk4e2c30("cutscenes\\hochzeit\\hochzeit-create.esc", buf);
    ai_Unk4e2c30("cutscenes\\hochzeit\\hochzeit-intro.esc", &ch_EmptyName);
    st[0] = ai_Unk4e2c30("cutscenes\\hochzeit\\hochzeit-reden1.esc", &ch_EmptyName);
    G25_STEP(0x5dc);
    d2_SetViewEnabled(v, 1);
    d2_SetForm(v, 0);
    io_PrintText(0x1aa9, *(unsigned short *)b, *(unsigned short *)a);
    G25_VOICE("_HOCHZEIT_NEU_MAIN_00");
    d2_SetForm(v, 0);
    io_PrintText("$C");
    d2_SetViewEnabled(v, 0);
    if (ai_Unk4e2cc0(st[0], ai_Unk4e06d0, st) != 0)
        goto done;
    G25_STEP(0x3e8);
    st[0] = ai_Unk4e2c30("cutscenes\\hochzeit\\hochzeit-reden2.esc", &ch_EmptyName);
    d2_SetForm(v, 0);
    d2_SetViewEnabled(v, 1);
    io_PrintText(0x1aac, *(unsigned short *)b, *(unsigned short *)a);
    G25_VOICE("_HOCHZEIT_NEU_MAIN_03");
    d2_SetForm(v, 0);
    io_PrintText("$C");
    d2_SetViewEnabled(v, 0);
    G25_STEP(0x1f4);
    d2_SetViewEnabled(v, 1);
    d2_SetForm(v, 0);
    io_PrintText(0x1aab);
    G25_VOICE("_HOCHZEIT_NEU_M");
    if (ai_Unk4e2cc0(st[0], ai_Unk4e06d0, st) != 0)
        goto done;
    st[0] = ai_Unk4e2c30("cutscenes\\hochzeit\\hochzeit-reden3.esc", &ch_EmptyName);
    d2_SetViewEnabled(v, 0);
    d2_SetForm(v, 0);
    io_PrintText("$C");
    G25_STEP(0x5dc);
    d2_SetViewEnabled(v, 1);
    d2_SetForm(v, 0);
    io_PrintText(0x1aaa, *(unsigned short *)b, *(unsigned short *)a);
    G25_VOICE("_HOCHZEIT_NEU_MAIN_01");
    G25_STEP(0x1f4);
    d2_SetViewEnabled(v, 0);
    d2_SetForm(v, 0);
    io_PrintText("$C");
    G25_STEP(0x1f4);
    d2_SetViewEnabled(v, 1);
    d2_SetForm(v, 0);
    io_PrintText(0x1aab);
    G25_VOICE("_HOCHZEIT_NEU_MAIN_02");
    G25_STEP(0x1f4);
    if (ai_Unk4e2cc0(st[0], ai_Unk4e06d0, st) != 0)
        goto done;
    st[0] = ai_Unk4e2c30("cutscenes\\hochzeit\\hochzeit-reden4.esc", &ch_EmptyName);
    d2_SetForm(v, 0);
    io_PrintText(0x1aad);
    G25_VOICE("_HOCHZEIT_NEU_MAIN_04");
    G25_STEP(0x1f4);
    d2_SetViewEnabled(v, 0);
    ai_Unk4e06d0(st);
done:
    return 1;
}

/* 0x4b5460, the mission success dialog of t: three general speeches ("AUFTRAEGE_ALLGEMEIN" 0..2), then
   "_AUFTRAEGE_ERFOLG_HS_nn", each cut short by the button
   @deep behaviour: 4 code hunks (20 differing instructions), all one pattern: 'unsigned long _end = d3_Time + 0xfa;' compiles here as mov ebx,[d3_Time]; add ebx,0xfa, the original as mov eax,[d3_Time]; lea ebx,[eax+0xfa] (the load goes to a temp, the sum into _end); the following load for 'while (_end > d3_Time)' and every other instruction match (the loop structure top test + bottom cmp ebx,[d3_Time] is identical). Meaning checked: same end time, same volatile reads. Tried, one build each: 0xfa + d3_Time (identical bytes), the inline while (d3_Time + 0xfa > d3_Time) without the local (much WORSE: 578 -> 606 bytes, 20 -> 222 asm lines, reverted). spell.py: the four code hunks have no menu rule (meaning check first); the dup-tail hunk 0x4b58ac is the alignment fill of the 4-instruction size difference (no if/else at the tail). Readability done this session: Q1/Q2 cleared (sb_IsInit is the canonical sblib.c name of 0x4273c0, G13TYPE.unk10 -> speech). */

extern volatile unsigned long d3_Time;  /* 0x6333cc, milliseconds */
int sb_IsInit(void);                                                          /* 0x4273c0, sound on */
void snd_ClearQueue(void);                                                         /* 0x4c01b0, defined elsewhere */

/* G13_PLAY: plays snd until it ends or button ok is pressed; without sound waits 250 ms (or the button) */
#define G13_PLAY(snd, ok, id) {                                     \
    if (sb_IsPlaying(snd)) {                                        \
        do {                                                        \
            sv_RunFrame(0, (id) | 0x80);                           \
            if (d2_ClickedId != -1 && d2_PointerObj == (ok)) {       \
                if (sb_IsPlaying(snd))                              \
                    sb_Stop(snd, 1);                                \
                break;                                              \
            }                                                       \
        } while (sb_IsPlaying(snd));                                \
    }                                                               \
}
#define G13_WAIT(ok, id) {                                          \
    unsigned long _end = d3_Time + 0xfa;                            \
    while (_end > d3_Time) {                                        \
        sv_RunFrame(0, (id) | 0x80);                               \
        if (d2_ClickedId != -1 && d2_PointerObj == (ok))             \
            break;                                                  \
    }                                                               \
}

void ai_MissionSuccessDialog(G13TYPE *t, int id)
{
    char name[0x100];
    int view, ok;
    void *snd;

    snd_ClearQueue();
    view = d2_OpenForm(0, 0, "special\\mission");
    d2_CentreViewWindows(view);
    d2_SetForm(view, 1);
    io_PrintText((char *)0x1b9b);
    d2_SetForm(view, 2);
    io_PrintText("$C");
    ok = d2_GetWinObj(view, 2, io_PrintText((char *)0x7d));
    if (sb_IsInit()) {
        snd = snd_PlayVoice(-8, 0, "AUFTRAEGE_ALLGEMEIN", 0);
        G13_PLAY(snd, ok, id);
    } else
        G13_WAIT(ok, id);
    d2_SetForm(view, 1);
    io_PrintText((char *)0x1b9c);
    if (sb_IsInit()) {
        snd = snd_PlayVoice(-8, 0, "AUFTRAEGE_ALLGEMEIN", 1);
        G13_PLAY(snd, ok, id);
    } else
        G13_WAIT(ok, id);
    d2_SetForm(view, 1);
    io_PrintText((char *)0x1b9d);
    if (sb_IsInit()) {
        snd = snd_PlayVoice(-8, 0, "AUFTRAEGE_ALLGEMEIN", 2);
        G13_PLAY(snd, ok, id);
    } else
        G13_WAIT(ok, id);
    d2_SetForm(view, 1);
    io_PrintText("$C");
    io_PrintText((char *)0x1b9e);
    io_PrintText((char *)(t->text + 2));
    if (sb_IsInit()) {
        sprintf(name, "_AUFTRAEGE_ERFOLG_HS_%.2d", t->speech);
        snd = snd_PlayVoice(-7, 0, name, -1);
        G13_PLAY(snd, ok, id);
    } else
        G13_WAIT(ok, id);
    d2_CloseView(view);
}

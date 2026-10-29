/* 0x4d1eb0, the outro: stops the stream ai_Unk6f0d4c and runs the cutscene "Kampf_outro_flug" */

void ai_Unk4d1eb0()
{
    char *s;

    if (ai_Unk6f0d4c != 0)
        msx_StopAmbient(ai_Unk6f0d4c);
    sv_FrameMask = 0x7d6c6;
    d2_CutsceneActive = 1;
    s = (char *)evt_Load("cutscenes\\kampf\\Kampf_outro_flug.esc");
    if (s != 0) {
        evt_RunScript((EVTSCRIPT *)s);
        ai_Unk4ceb80(*(int *)(s + 0x100));
    }
    d2_CutsceneActive = 0;
}

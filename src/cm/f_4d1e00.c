/* 0x4d1e00, the intro of the fight: a random fight track (when the music is on) and the cutscene "Kampf_Intro_Fluege" */

void ai_Unk4d1e00()
{
    int r;
    char *s;

    d2_CutsceneActive = 1;
    if (snd_MusicEnabled != 0) {
        r = util_Random(100) & 0xffff;
        if (r < 0x14)
            snd_PlaySpecialTrack("cd2\\Krieg.mp3");
        else if (r < 0x28)
            snd_PlaySpecialTrack("cd1\\RuestetEuch.mp3");
        else if (r < 0x3c)
            snd_PlaySpecialTrack("cd1\\Flucht.mp3");
        else if (r < 0x50)
            snd_PlaySpecialTrack("cd2\\DerSeuchenzug.mp3");
        else
            snd_PlaySpecialTrack("cd2\\AmKuehlenGrabe.mp3");
    }
    sv_FrameMask = 0x7d6c6;
    sv_Unk5301a0();
    sv_Unk525ad0(ai_Time);
    s = (char *)evt_Load("cutscenes\\kampf\\Kampf_Intro_Fluege.esc");
    if (s != 0) {
        evt_RunScript((EVTSCRIPT *)s);
        ai_Unk4ceb80(*(int *)(s + 0x100));
    }
    d2_CutsceneActive = 0;
}

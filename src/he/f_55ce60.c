/* 0x55ce60, the run handler of the "Berufe\Meisterpruefung_BESTANDEN" ceremony (the exam passed; for the player's figure only: 0x6f25b8; ROUGH skeleton, 1199 bytes): opens the window, shows the person's new title (gm_TitleGroup / gm_TitlesLeft, the texts "BERUF_BESTANDEN", "ADDON_BERUF_LISTE_MAENNLICH" / "_WEIBLICH", "ADDON_GEBAEUDE_LISTE", the sound banks "brk_%02i" 0x4c27d0 x8), waits for the click and closes (0x53d800) and frees the record */

void he_Unk55ce60(int ev)
{
    int st = *(int *)(ev + 0x70);
    int p;
    char name[0x40];

    if (*(unsigned short *)(unsigned)&ai_PlayerId != *(unsigned short *)(ev + 8))
        return;
    if (st < 0 || st > 2) {
        if (*(int *)(ev + 0x74) != 0)
            he_Unk53d800(ev);
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (st == 0) {
        he_Unk53d6f0(ev, 0, 0, (int)"Berufe\\Meisterpruefung_BESTANDEN", 0x5fd);
        if (*(int *)(ev + 0x74) == 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        d2_CentreViewWindows(*(int *)(*(int *)(ev + 0x74) + 8));
        p = gm_FindChar(*(int *)(ev + 0xac));
        sprintf(name, "brk_%02i", p != 0 ? *(unsigned char *)(p + 0xd) : 0);
        snd_QueueVoice(-8, 0, name, 0, 0);
        io_PrintText("BERUF_BESTANDEN");
        *(int *)(ev + 0x70) = 1;
        return;
    }
    if (U_I((unsigned)&d2_ClickedId) == -1)
        return;
    he_Unk53d800(ev);
    he_Unk53e1f0((int *)ev);
}

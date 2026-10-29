/* 0x55cc10, the run handler of the master exam announcement ("Berufe\Messagebox_Meisterpruefung", 4 states by the jump table 0x55cdec; ROUGH): returns while the due time is not reached (0x4fe210); state 0 opens the window (0x53d6f0 (ev, 0, 0, name, 0x5fd), none: free), state 1 looks up the person +0xac (0x505df0), its title group (0x504ce0, 0x504ad0 = titles left; the sound names "BERUF_TRAINING_SPIELER_MAENNLICH" / "_WEIBLICH" with the ADDON variants by the sex and by 0xc titles, 0x426290 lookups) and speaks it (0x4c27d0 (-8, .., name, ..), the news 0x159a through 0x515960), state 2 / 3 close the window (0x4ffe90 contact, 0x521960, 0x53d800) and free the record */

int gm_TitlesLeft();
int gm_TitleGroup();
struct SBBANK * sb_FindBank();

void he_Unk55cc10(int ev)
{
    int st = *(int *)(ev + 0x70);
    int p, n;
    char *snd;

    if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) == 0)
        return;
    if (st < 0 || st > 3) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    switch (st) {
    case 0:
        he_Unk53d6f0(ev, 0, 0, (int)"Berufe\\Messagebox_Meisterpruefung", 0x5fd);
        if (*(int *)(ev + 0x74) == 0)
            he_Unk53e1f0((int *)ev);
        return;
    case 1:
        p = gm_FindChar(*(int *)(ev + 0xac));
        if (p == 0)
            return;
        n = gm_TitlesLeft(p);
        if (gm_TitleGroup(p, n) != 0) {
            if (n == 0xc)
                snd = "BERUF_TRAINING_SPIELER_MAENNLICH";
            else
                snd = "ADDON_BERUF_TRAINING_SPIELER_MAENNLICH";
        } else {
            snd = *(unsigned char *)(p + 9) == 1 ? "BERUF_TRAINING_SPIELER_WEIBLICH" : "ADDON_BERUF_TRAINING_SPIELER_WEIBLICH";
        }
        sb_FindBank(snd);
        snd_QueueVoice(-8, 0, snd, 0, 0);
        io_PrintText(0x159a);
        return;
    default:
        he_Unk53d800(ev);
        he_Unk53e1f0((int *)ev);
        return;
    }
}

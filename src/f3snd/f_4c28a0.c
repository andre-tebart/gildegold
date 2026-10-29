/* 0x4c28a0, loads the voice banks for building b: the workers' orders / click comments by kind (thieves 4,
   robbers 0x10, mercenaries 0x13, addon kinds 0x1d / 0x20 / 0x1f / 0x1e, else craftsmen) and the greeting of
   its kind */

void snd_LoadBuildingVoices(unsigned char *b)
{
    char greet[0x100];
    char order[0x100];
    char click[0x100];
    char *t;

    greet[0] = ch_EmptyName[0];
    memset(greet + 1, 0, sizeof(greet) - 1);
    t = ai_BTypes + *b * 0x24d;
    if (*t == 4) {
        strcpy(order, "ARBEITER_KOMMENTARE_BEFEHL_DIEBE.sbf");
        strcpy(click, "ARBEITER_KOMMENTARE_KLICK_DIEBE.sbf");
    } else if (*t == 0x10) {
        strcpy(order, "ARBEITER_KOMMENTARE_BEFEHL_RAEUBER.sbf");
        strcpy(click, "ARBEITER_KOMMENTARE_KLICK_RAEUBER.sbf");
    } else if (*t == 0x13) {
        strcpy(order, "ARBEITER_KOMMENTARE_BEFEHL_SOELDNER.sbf");
        strcpy(click, "ARBEITER_KOMMENTARE_KLICK_SOELDNER.sbf");
    } else if (*t == 0x1d) {
        strcpy(order, "ADDON\\ARBEITERKOMMENTARE_BEFEHL_GEHILFEN.sbf");
        strcpy(click, "ADDON\\ARBEITERKOMMENTARE_KLICK_GEHILFEN.sbf");
        strcpy(greet, "ADDON\\BEGRUESSUNG_FRIEDHOF.sbf");
    } else if (*t == 0x20) {
        strcpy(order, "ADDON\\ARBEITERKOMMENTARE_BEFEHL_SCHNEIDER.sbf");
        strcpy(click, "ADDON\\ARBEITERKOMMENTARE_KLICK_SCHNEIDER.sbf");
        strcpy(greet, "ADDON\\BEGRUESSUNG_SCHNEIDER.sbf");
    } else if (*t == 0x1f) {
        strcpy(order, "ADDON\\ARBEITERKOMMENTARE_BEFEHL_ZIGEUNER.sbf");
        strcpy(click, "ADDON\\ARBEITERKOMMENTARE_KLICK_ZIGEUNER.sbf");
        strcpy(greet, "ADDON\\BEGRUESSUNG_ZIGEUNER.sbf");
    } else if (*t == 0x1e) {
        click[0] = 0;
        strcpy(order, "ADDON\\ARBEITER_KOMMENTARE_BEFEHL_ALCHIMIST.sbf");
        greet[0] = 0;
    } else {
        strcpy(order, "ARBEITER_KOMMENTARE_BEFEHL_HANDWERK.sbf");
        strcpy(click, "ARBEITER_KOMMENTARE_KLICK_HANDWERK.sbf");
        if (*t == 7)
            strcpy(greet, "BEGRUESSUNG_KIRCHE.sbf");
        else if (*t == 5)
            strcpy(greet, "BEGRUESSUNG_GELDLEIHE.sbf");
        else if (*t == 8 || *t == 0xe)
            strcpy(greet, "BEGRUESSUNG_KRAEUTERLADEN_PARFUEMERIE.sbf");
        else if (*t == 9)
            strcpy(greet, "BEGRUESSUNG_LAGERHAUS.sbf");
        else if (*t == 0x15 || *t == 0x14 || *t == 0x12)
            strcpy(greet, "BEGRUESSUNG_SCHMIEDE_STEIMETZ_TISCHLER.sbf");
        else if (*t == 0xb || *t == 0xc || *t == 0xd)
            strcpy(greet, "BEGRUESSUNG_WALDSTUECK_STEINBRUCH_MINE.sbf");
        else if (*t == 0x16)
            strcpy(greet, "BEGRUESSUNG_WIRTSHAUS.sbf");
        else if (*t == 0x1e)
            strcpy(greet, "ADDON\\BEGRUESSUNG_DOMIZIL.sbf");
    }
    if (order[0] != 0)
        snd_VoiceBankOrders = snd_LoadVoiceBank(order, 0);
    if (click[0] != 0)
        snd_VoiceBankClick = snd_LoadVoiceBank(click, 0);
    if (*t == 0x1d || *t == 0x1e)
        snd_VoiceBankNoise = 0;
    else
        snd_VoiceBankNoise = snd_LoadVoiceBank("ARBEITER_KOMMENTARE_KLICK_GERAEUSCHE.sbf", 0);
    if (greet[0] != 0)
        snd_VoiceBankGreeting = snd_LoadVoiceBank(greet, 0);
}

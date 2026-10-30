/* 0x513a20, reads the city file "<gfx dir><cities dir>/<name>.ini" (sections "A - ALLGEMEIN", "A - EINWOHNER", "A - PRUNK", "B - WETTER", "C - PRIVILEGIEN", "E - UMLAND", "F - GESETZE", "KONTOR") into the town slot `slot` of the table 0x7e3f40 (0x2f4 bytes each): the name "Stadtname" (+0), the map position "KartenPosition" (+0x40, two numbers), the faith "Glaube" (+0x49), the history "HistorieStart" / "HistorieEnde" (+0x4c / +0x50), the currency "Waehrung" (+0x54, an object name 3 characters in), "Land" (+0x60) and "Sprache" (+0x61), the map offset "KartenOffset" (+0x2ec), "MaxPlayer" (+0x48), the ten "Einwohner_%i" / "Prunk_%i" pairs (+0x64 / +0xb4), the weather chances (+0x108 / +0x118 four numbers, +0x128 the freezing chance), the eleven privileges (+0x12c), the eight "Umland%i" entries (+0x145: type, number, up to eight object ids), the six law lists of 12 numbers (+0x1dc .. +0x290), and the "Import" / "Export" lists of up to 16 goods (+0x2ac / +0x2cc); slot 0 additionally clears the whole table first and loads the neighbour cities "NachbarStadt" (up to 8, recursive with the slot numbers 1 ..). Returns 1. @deep readability advanced, Q4 open, far draft: Q2 (unk_Unk513970 -> init_ParseNumberList), Q5 (the ROUGH marker replaced by the state) and Q7 (int gm_FindBuildingKind(char *) 0x502060) cleared. The body was read against the section list: GetPrivateProfileStringA of A - ALLGEMEIN (Stadtname / KartenPosition / Glaube / HistorieStart / HistorieEnde / Waehrung / Land / Sprache / KartenOffset / NachbarStadt / MaxPlayer), A - EINWOHNER + A - PRUNK (10 pairs + 10 pairs), B - WETTER (regen / schnee / zufrieren), C - PRIVILEGIEN (11), E - UMLAND (8 entries of kind / number / 8 ids), F - GESETZE (6 lists of 12) and KONTOR (import / export, 16 shorts) matches the draft's calls and keys. Left: 537 code hunks / 292 slot hunks / 425 differing instructions - the draft's city record offsets overlap in the six law lists (0x1dc, 0x20c, 0x22c, 0x25c, 0x274, 0x290: finLaw 0x20c+0x30 crosses penalLaw 0x22c), so the record must be re-read from the disassembly before an INIT_CITY view can replace the 9 raw accesses - Q4 is the typed-view work on that verified layout. */


int init_LoadCityINI(char *city, int slot)
{
    char ini[260], val[256], key[512];
    int nums[64];
    char *s, *p, *dst;
    int i, k, j, n;
    char names[8][0x40];
    char sep[4];

    s = ai_PlayerSlots[slot];
    if (slot == 0)
        memset(ai_PlayerSlots[0], 0, 0x2f4 * 4);
    sprintf(ini, "%s%s/%s.ini", "\\project\\gfx\\", "gamedata/cities", city);
    GetPrivateProfileStringA("A - ALLGEMEIN", "Stadtname", "Unknown", val, 0x100, ini);
    strcpy(s, val);
    GetPrivateProfileStringA("A - ALLGEMEIN", "KartenPosition", "0,0", val, 0x100, ini);
    init_ParseNumberList(val, (int *)(s + 0x40), 2);
    GetPrivateProfileStringA("A - ALLGEMEIN", "Glaube", "0", val, 0x100, ini);
    s[0x49] = atoi(val);
    GetPrivateProfileStringA("A - ALLGEMEIN", "HistorieStart", "1400", val, 0x100, ini);
    *(int *)(s + 0x4c) = atoi(val);
    GetPrivateProfileStringA("A - ALLGEMEIN", "HistorieEnde", "1400", val, 0x100, ini);
    *(int *)(s + 0x50) = atoi(val);
    GetPrivateProfileStringA("A - ALLGEMEIN", "Waehrung", "ob_GOLDGULDEN", val, 0x100, ini);
    *(short *)(s + 0x54) = gm_FindObjectProto(val + 3);
    GetPrivateProfileStringA("A - ALLGEMEIN", "Land", "0", val, 0x100, ini);
    s[0x60] = atoi(val);
    GetPrivateProfileStringA("A - ALLGEMEIN", "Sprache", "0", val, 0x100, ini);
    s[0x61] = atoi(val);
    GetPrivateProfileStringA("A - ALLGEMEIN", "KartenOffset", "0, 0", val, 0x100, ini);
    init_ParseNumberList(val, (int *)(s + 0x2ec), 2);
    if (slot == 0) {
        GetPrivateProfileStringA("A - ALLGEMEIN", "NachbarStadt", "", val, 0x100, ini);
        p = strtok(val, ",");
        n = 0;
        while (p != 0 && n <= 7) {
            while (*p == ' ')
                p++;
            strcpy(names[n], p);
            p = strtok(0, ",");
            n++;
        }
        for (i = 0; i < n; i++)
            init_LoadCityINI(names[i], i + 1);
    }
    GetPrivateProfileStringA("A - ALLGEMEIN", "MaxPlayer", "4", val, 0x100, ini);
    s[0x48] = atoi(val);
    for (i = 0; i < 10; i++) {
        sprintf(key, "Einwohner_%i", i);
        GetPrivateProfileStringA("A - EINWOHNER", key, "1400,0", val, 0x100, ini);
        init_ParseNumberList(val, nums, 2);
        *(int *)(s + 0x64 + i * 8) = nums[0];
        *(int *)(s + 0x68 + i * 8) = nums[1];
    }
    for (i = 0; i < 10; i++) {
        sprintf(key, "Prunk_%i", i);
        GetPrivateProfileStringA("A - PRUNK", key, "1400,0", val, 0x100, ini);
        init_ParseNumberList(val, nums, 2);
        *(int *)(s + 0xb4 + i * 8) = nums[0];
        *(int *)(s + 0xb8 + i * 8) = nums[1];
    }
    GetPrivateProfileStringA("B - WETTER", "Regenwahrscheinlichkeit", "0,0,0,0", val, 0x100, ini);
    init_ParseNumberList(val, (int *)(s + 0x108), 4);
    GetPrivateProfileStringA("B - WETTER", "Schneewahrscheinlichkeit", "0,0,0,0", val, 0x100, ini);
    init_ParseNumberList(val, (int *)(s + 0x118), 4);
    GetPrivateProfileStringA("B - WETTER", "Zufrierenwahrscheinlichkeit", "0", val, 0x100, ini);
    *(int *)(s + 0x128) = atoi(val);
    GetPrivateProfileStringA("C - PRIVILEGIEN", "Privilegien", "0,0,0,0,0,0,0,0,0,0,0", val, 0x100, ini);
    init_ParseNumberList(val, nums, 0xb);
    for (i = 0; i < 0xb; i++)
        s[0x12c + i] = ((char *)nums)[i];
    dst = s + 0x145;
    k = 0;
    j = 0xa1;
    do {
        sprintf(key, "Umland%i", k);
        GetPrivateProfileStringA("E - UMLAND", key, "", val, 0x100, ini);
        p = strtok(val, ",");
        for (i = 0; p != 0 && i < 10; i++) {
            while (*p == ' ')
                p++;
            if (i == 0)
                dst[-1] = gm_FindBuildingKind(p + 3);
            else if (i == 1)
                *dst = atoi(p);
            else
                *(short *)(s + (i + j) * 2) = gm_FindObjectProto(p + 3);
            p = strtok(0, ",");
        }
        j += 9;
        k++;
        dst += 0x12;
    } while (j < 0xe9);
    GetPrivateProfileStringA("F - GESETZE", "Verfassungsgesetze", "0,0,0,0", val, 0x100, ini);
    init_ParseNumberList(val, (int *)(s + 0x1dc), 0xc);
    GetPrivateProfileStringA("F - GESETZE", "Finanzgesetze", "0,0,0,0", val, 0x100, ini);
    init_ParseNumberList(val, (int *)(s + 0x20c), 0xc);
    GetPrivateProfileStringA("F - GESETZE", "Strafgesetze", "0,0,0,0", val, 0x100, ini);
    init_ParseNumberList(val, (int *)(s + 0x22c), 0xc);
    GetPrivateProfileStringA("F - GESETZE", "Gildengesetze", "0,0,0,0", val, 0x100, ini);
    init_ParseNumberList(val, (int *)(s + 0x25c), 0xc);
    GetPrivateProfileStringA("F - GESETZE", "Kirchengesetze", "0,0,0,0", val, 0x100, ini);
    init_ParseNumberList(val, (int *)(s + 0x274), 0xc);
    GetPrivateProfileStringA("F - GESETZE", "DiebeRaeubergesetze", "0,0,0,0", val, 0x100, ini);
    init_ParseNumberList(val, (int *)(s + 0x290), 0xc);
    strcpy(sep, ",");
    GetPrivateProfileStringA("KONTOR", "Import", "", val, 0x100, ini);
    p = strtok(val, sep);
    i = 0;
    if (p != 0) {
        short *q = (short *)(s + 0x2ac);

        do {
            if (i > 0xf)
                break;
            while (isspace((unsigned char)*p))
                p++;
            *q = gm_FindObjectProto(p + 3);
            p = strtok(0, sep);
            i++;
            q++;
        } while (p != 0);
    }
    strcpy(sep, ",");
    i = 0;
    GetPrivateProfileStringA("KONTOR", "Export", "", val, 0x100, ini);
    p = strtok(val, sep);
    if (p != 0) {
        short *q = (short *)(s + 0x2cc);

        do {
            if (i > 0xf)
                return 1;
            while (isspace((unsigned char)*p))
                p++;
            *q = gm_FindObjectProto(p + 3);
            p = strtok(0, sep);
            i++;
            q++;
        } while (p != 0);
    }
    return 1;
}

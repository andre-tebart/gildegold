/* 0x4a47c0, amt_GetDesireByName */

int ai_Unk4a47c0(char *s)
{
    char buf[128];

    if (_strcmpi(s, "APS") == 0)
        return 0;
    if (_strcmpi(s, "UNVERSEHRTHEIT") == 0)
        return 1;
    if (_strcmpi(s, "WOHNUNG") == 0)
        return 2;
    if (_strcmpi(s, "GELD") == 0)
        return 3;
    if (_strcmpi(s, "BERUF") == 0)
        return 4;
    if (_strcmpi(s, "VERGNUEGEN") == 0)
        return 5;
    if (_strcmpi(s, "ANSEHEN") == 0)
        return 6;
    if (_strcmpi(s, "AMT") == 0)
        return 7;
    if (_strcmpi(s, "BILDUNG") == 0)
        return 8;
    if (_strcmpi(s, "RECHTSCHAFFENHEIT") == 0)
        return 9;
    if (_strcmpi(s, "GEMEINHEIT") == 0)
        return 10;
    if (_strcmpi(s, "SICHERHEIT") == 0)
        return 11;
    if (_strcmpi(s, "FORTPFLANZUNG") == 0)
        return 12;
    if (_strcmpi(s, "TRAEGHEIT") == 0)
        return 13;
    sprintf(buf, "amt_GetDesireByName(): Unbekanntes Bed\xfc" "rfnis: %s", s);
    return -1;
}

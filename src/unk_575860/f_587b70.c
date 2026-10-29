/* 0x587b70, the building flag mask of a character (201 B), called from f3gm.c 0x50359b: ch = the character
   (+0 the word), b = the building record (+0 the kind, +0x25 / +0x27 the owner words), x = a record (+0 a word)
   or 0. bldg = 0x7e3f30 + the kind * 0x24d; v = 0x5044b0 (the character's word, 0x5048e0 (the building kind));
   the mask 0x81, 0x10bd when the building's +0x25 is the character's word; x with the word 0x211 / 0x205 ->
   bit 0 cleared. When the building's +0x27 is the character's word: 0x4461 set and, when one of the six bytes at
   bldg + 0x223 is v (a 0 byte ends the scan), 0x2200; x with the word 0x21b -> 0x5000; U_I (0x6f2448) != 0 ->
   -1 */

int gm_BuildingJob(); 

int ai_Unk587b70(char *ch, char *b, char *x)
{
    char *bldg;
    int kind;
    int v;
    int f;
    int i;

    kind = *(unsigned char *)b;
    bldg = (char *)U_I((unsigned)&ai_BTypes) + kind * 0x24d;
    v = gm_Unk5044b0(*(unsigned short *)ch, gm_BuildingJob(*(unsigned char *)bldg));
    f = 0x81;
    if (*(unsigned short *)(b + 0x25) == *(unsigned short *)ch)
        f = 0x10bd;
    if (x != 0 && (*(unsigned short *)x == 0x211 || *(unsigned short *)x == 0x205))
        f &= ~1;
    if (*(unsigned short *)(b + 0x27) == *(unsigned short *)ch) {
        f |= 0x4461;
        for (i = 0; i < 6; i++) {
            if (*(unsigned char *)(bldg + 0x223 + i) == 0)
                break;
            if (*(unsigned char *)(bldg + 0x223 + i) == v) {
                f |= 0x2200;
                break;
            }
        }
    }
    if (x != 0 && *(unsigned short *)x == 0x21b)
        f |= 0x5000;
    if (U_I((unsigned)&ai_Unk6f2448) != 0)
        return -1;
    return f;
}

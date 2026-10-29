/* 0x53ec40, makes the event record `r` due now (state 0x70 = -2, clock +0x52 = the game clock) when its player's figure changed / is gone, or for the types of the list (ROUGH) */

void he_Unk53ec40(unsigned char *r)
{
    unsigned short us = *(unsigned short *)(r + 8);
    int b;

    if (us == 0xffff)
        return;
    b = us * 0x21c;
    if (U_I((unsigned)&ai_CharInts + us * 0x21c) == *(int *)(r + 0xc) && *(char *)(((unsigned)&ai_Chars + 0x2) + b) != 0xf) {
        if (*(unsigned char *)(((unsigned)&ai_CharInts + 0x4) + b) <= 0) {
            switch (*r) {
            case 0x16: case 0x18: case 0x19: case 0x1a: case 0x2d: case 0x2e: case 0x2f: case 0x35:
            case 0x36: case 0x37: case 0x38: case 0x41: case 0x45: case 0x46: case 0x47: case 0x4a:
            case 0x4b: case 0x5e: case 0x5f: case 0x60: case 0x69: case 0x6a: case 0x6f: case 0x7e:
                goto L_reset;
            default:
                break;
            }
        }
    } else {
L_reset:
        *(int *)(r + 0x70) = -2;
        *(HETIME *)(r + 0x52) = ai_Time;
    }
}

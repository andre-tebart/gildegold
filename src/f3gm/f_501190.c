/* 0x501190, starts a building query and returns the first match (0x501030): n criteria follow as (kind, value) pairs: 0 = type class (byte, 0x61a66c), 1 = id, 2 = a string (copied to *0x6f1bd0, unused), 3 / 4 = the words +0x25 / +0x27, 5 = the building type, 6 = stop at the first (no value). Used all over the exe as gm_BuildQuery (1, 5, 10) .. */

unsigned char *gm_BuildQuery(int n, ...)
{
    int *ap;
    int i;
    char *s, *d;
    char c;

    if (ai_BTypes != 0) {
        ai_Unk61a66c = 0xff;
        ai_Unk61a678 = (char)0xff;
        ai_Unk61a670 = -1;
        ai_Unk6f1bd0 = 0;
        ai_Unk61a674 = 0x7fff;
        ai_Unk61a676 = 0x7fff;
        ai_Unk6f1bd5 = 1;
        ai_Unk6f1bd8 = 0;
        ai_Unk6f1bd4 = 0;
        if (n > 0) {
            ap = &n;
            for (i = n; i != 0; i--) {
                switch ((char)ap[1]) {
                case 0:
                    ai_Unk61a66c = (unsigned char)ap[2];
                    ap += 2;
                    break;
                case 1:
                    ai_Unk61a670 = ap[2];
                    ap += 2;
                    break;
                case 2:
                    s = (char *)ap[2];
                    ap += 2;
                    d = ai_Unk6f1bd0;
                    do {
                        c = *s++;
                        *d++ = c;
                    } while (c != 0);
                    break;
                case 3:
                    ai_Unk61a674 = (short)ap[2];
                    ap += 2;
                    break;
                case 4:
                    ai_Unk61a676 = (short)ap[2];
                    ap += 2;
                    break;
                case 5:
                    ai_Unk61a678 = (char)ap[2];
                    ap += 2;
                    break;
                case 6:
                    ai_Unk6f1bd4 = 1;
                    ap += 1;
                    break;
                default:
                    ap += 1;
                }
            }
        }
        ai_Unk6f1bf0 = ai_Blds;
        return gm_BuildNext();
    }
    return 0;
}

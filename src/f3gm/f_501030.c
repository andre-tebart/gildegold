/* 0x501030, the next building matching the query of 0x501190 (type 0x61a66c, id 0x61a670, words +0x25 / +0x27, building type 0x61a678; the flag 0x6f1bd4 stops at the first match), walking the building table from 0x6f1bf0; 0 when done */

unsigned char *gm_BuildNext(void)
{
    int m;
    unsigned char *end, *p;

    m = 0;
    end = (unsigned char *)ai_Blds + 0xa900;
    if (ai_BTypes == 0)
        return 0;
    if (ai_Unk6f1bd8 > 0x100)
        return 0;
    if (ai_Unk6f1bd5 == 0) {
        do {
            ai_Unk6f1bf0 += 0xa9;
            if ((unsigned char *)ai_Unk6f1bf0 >= end)
                return 0;
        } while (*ai_Unk6f1bf0 == 0);
    } else {
        ai_Unk6f1bd5 = 0;
        if ((unsigned char *)ai_Unk6f1bf0 >= end) {
            ai_Unk6f1bd5 = 0;
            return 0;
        }
        do {
            if (*ai_Unk6f1bf0 != 0)
                break;
            ai_Unk6f1bf0 += 0xa9;
        } while ((unsigned char *)ai_Unk6f1bf0 < end);
    }
    p = (unsigned char *)ai_Unk6f1bf0;
    if (p >= end)
        return 0;
    do {
        if (ai_Unk6f1bd4 != 0)
            m = 1;
        if (ai_Unk61a66c == 0xff || *p == ai_Unk61a66c) {
            if (ai_Unk61a66c != 0xff)
                m = 1;
            if (ai_Unk61a670 != -1) {
                if (*(int *)(p + 1) != ai_Unk61a670)
                    goto no;
                m = 1;
            }
            if (ai_Unk61a674 != 0x7fff) {
                if (*(short *)(p + 0x25) != ai_Unk61a674)
                    goto no;
                m = 1;
            }
            if (ai_Unk61a676 != 0x7fff) {
                if (*(short *)(p + 0x27) != ai_Unk61a676)
                    goto no;
                m = 1;
            }
            if (ai_Unk61a678 != -1)
                m = ai_BTypes[*p * 0x24d] == ai_Unk61a678;
        } else {
        no:
            m = 0;
        }
        ai_Unk6f1bd8++;
        if (!m) {
            do {
                p += 0xa9;
                if (p >= end)
                    goto done;
                ai_Unk6f1bf0 = (char *)p;
            } while (*p == 0);
        }
        if (p >= end) {
            if (!m) {
            done:
                p = 0;
                ai_Unk6f1bf0 = 0;
            }
            return p;
        }
        if (m)
            return p;
    } while (1);
}

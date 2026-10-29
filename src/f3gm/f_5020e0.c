/* 0x5020e0 */

int gm_BuildingJob();                     /* 0x5048e0 */
int gm_ListBuildingsOfType();                     /* 0x5020a0 */
extern int ai_Unk6f2448;

/* 0x5020e0, whether the character c may take the job of the building kind k: 0 no, 1 yes, 2 yes in a neighbouring building of the same type (0x5020a0 lists them) ; by the building's class (0x501d80): 1 / 7 / 8 the jobs of the kind (the six bytes at +0x223) match the character's job number (0x5044b0), 2 never, 3 by the character's kind (0xf: kinds 1 / 0xf / 6, 0xa: kind 3), 4 own list only, 6 by the rank byte at +0x9 of the character (limits 0 / 3 / 4 / 5 by the kind) giving 1 or 2. With the flag 0x6f2448 every person (kind 6 / 7) may. ROUGH */
#define JOB_FIND(t0, base, v, code)                                                  \
    do {                                                                             \
        t = (unsigned char *)(base) + 0x223;                                         \
        do {                                                                         \
            if (*t == 0)                                                             \
                break;                                                               \
            if ((v) == *t)                                                           \
                return code;                                                         \
            t++;                                                                     \
        } while ((int)(t - ((unsigned char *)(base) + 0x223)) < 6);                  \
    } while (0)

char unk_Unk5020e0(int k, int c)
{
    int cls, i, n;
    char *p, *ch;
    int job;
    unsigned v;
    unsigned char *t;
    int list[8];
    unsigned char lim;

    cls = gm_BuildingClass(k);
    p = ai_BTypes + k + k * 0x24c;
    job = gm_BuildingJob(*p);
    v = gm_Unk5044b0(c, job);
    if (c == 0xffff)
        return 0;
    ch = ai_Chars[c];
    if (ai_Unk6f2448 != 0 && (ch[2] == 6 || ch[2] == 7))
        return 1;
    if (cls == 1) {
        JOB_FIND(t, p, v, 1);
        n = gm_ListBuildingsOfType(*p, list);
        i = 0;
        if (0 < n) {
            do {
                JOB_FIND(t, ai_BTypes + list[i] * 0x24d, v, 2);
                i++;
                if (n <= i)
                    return 0;
            } while (1);
        }
    } else {
        if (cls == 2)
            return 0;
        if (cls == 3) {
            if (ch[2] == 0xf) {
                if (*p == 1)
                    return 1;
                if (*p == 0xf)
                    return 1;
                if (*p == 6)
                    return 1;
            }
            if (ch[2] == 10 && *p == 3)
                return 1;
            return 0;
        }
        if (cls == 7) {
            if (*p == 0x13) {
                JOB_FIND(t, p, v, 1);
                n = gm_ListBuildingsOfType(0x13, list);
                i = 0;
                if (0 < n) {
                    do {
                        JOB_FIND(t, ai_BTypes + list[i] * 0x24d, v, 2);
                        i++;
                        if (n <= i)
                            return 0;
                    } while (1);
                }
            }
        } else if (cls == 4) {
            t = (unsigned char *)(p + 0x223);
            do {
                if (*t == 0)
                    return 0;
                if (v == *t)
                    return 1;
                t++;
            } while ((int)(t - (unsigned char *)(p + 0x223)) < 6);
        } else {
            if (cls == 6) {
                lim = 0;
                switch (k) {
                case 4:
                    return ((unsigned char)ch[9] == 0) + 1;
                case 5:
                    return ((unsigned char)ch[9] < 3) + 1;
                case 6:
                case 0x4b:
                case 0x4c:
                case 0x4d:
                    return ((unsigned char)ch[9] < 4) + 1;
                case 7:
                    lim = 5;
                }
                return ((unsigned char)ch[9] < lim) + 1;
            }
            if (cls != 8)
                return 1;
            JOB_FIND(t, p, v, 1);
            n = gm_ListBuildingsOfType(*p, list);
            i = 0;
            if (0 < n) {
                do {
                    JOB_FIND(t, ai_BTypes + list[i] * 0x24d, v, 2);
                    i++;
                    if (n <= i)
                        return 0;
                } while (1);
            }
        }
    }
    return 0;
}

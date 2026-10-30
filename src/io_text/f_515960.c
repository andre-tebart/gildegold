/* 0x515960, the text engine: prints the text fmt (a literal or an id of the text table 0x143a1c0 below 0x4800) with the arguments of its tokens ("%" "$" "{" + optional digit + a letter, e.g. "%s" a text id, "%G" the name of a good, "%N" a character name (0x5143d0), "%T" a time (0x508a80), "%i" a number with the dots of the thousands, "%f" a float, "%W" / "$C" window commands, "%a" a plain string, "%l" / "%t" / "%m" arguments kept in the window's table for the later redraw, "%u" / "%U" / "%e" / "%E" / "%b" / "%B" the case of the next word (the grammar of 0x42c3b0: 1 .. 6), "%X" a blank, "%w" a word of the text table, "%r" a random text of the group, "%v" a skipped argument) into the buffer of the current text window (0xf8f13c + 0x3c4 * 0x6ed938) and redraws it (0x46e150 / 0x471a90 / 0x474e80). Returns the window's object count (nobjs, the index
   the object that this text's "%ia" adds will get; the redraw above has created it before the caller's 0x475b80),
   -1 for a missing window or an overlong text. ROUGH: only the structure of the token loop and the simple tokens; the grammar cases of "%s" / "%S" and the "%i" dot grouping are summarised, the rest of the repeated string moves is txt_Subst. @deep behaviour: the draft is a skeleton, 2912 of 7536 bytes (only the token loop and the simple tokens); missing: the grammar cases of %s/%S (the txt_Field word forms of 0x42c3b0 by case 1 .. 6), the %r random text of a group, the %i dot grouping, the case flags (the original's param_2 .. param_10) and the detail of the window record 0xf8f13c + 0x3c4 * window (+0x10 text buffer, +0x24c / +0x250 flags, +0x264 / +0x268 the 0x41-entry table with count +0x368); readability Q1/Q2: the skeleton identifiers and the unnamed window / argument globals */


extern int ai_Unkfe7c20;
extern int d2_LastWindow;                /* 0x6ed938, the current text window */
int ai_Unk6333b8 = 0;  /* 0x6333b8 */
int ai_Unk6ed978 = 0;  /* 0x6ed978 */
/* ai_Unkf8f13c: view of d2_Wins (tools/data_views.py) */
extern char *d2_Objs;  /* 0xf8f080, 0x2ec bytes each; @original: d2.h declares D2OBJ *; the units add byte offsets to it and cannot include d2.h (vfs_Root / vfs_Tfopen clash with vfs.h) */
/* ---- the text engine (0x515960 .. 0x519010) ---- */
int ai_Unkfe7c20 = {0}; /* 0xfe7c20 */
int ai_Unk114fe60[18440] = {0}; /* 0x114fe60, a byte per text id (the gender / class) */
int d2_SetForm(int view, int n);                    /* 0x475f70 */
void d2_DrawAll(void);                    /* 0x46e150 */
void d2_DrawWindow();                    /* 0x471a90 */
void d2_RedrawView(int view);                    /* 0x474e80 */
extern void *d2_ScreenSurf;              /* 0x6ed924, the screen surface (d2.c) */
void err_Message(char *text);                    /* 0x4222b0 */
void gm_Unk508a80(int v, unsigned char k, char *out);                   /* 0x508a80 */
int io_TrailingDigits();                /* 0x514360 */
int ls_GetLanguage();                     /* 0x5190a0, the language number */
#define TXT_ID(c) ((char)(c) + -0x30)

/* replaces the token buf[at .. at + toklen) by the string sub */
/* unlinked */
static __inline void txt_Subst(char *buf, int at, int toklen, const char *sub)
{
    int n = strlen(sub);

    memmove(buf + at + n, buf + at + toklen, strlen(buf + at + toklen) + 1);
    memcpy(buf + at, sub, n);
}

int io_PrintText(char *fmt, ...)
{
    char buf[0x17d0 + 0x20];
    char tmp[0x17d0];
    char word[0x40];
    int *ap = (int *)&fmt + 1;
    int base, st, pos, dig, code, len;
    int gcase, flag9, flag10, flag8, gender;
    int *w;
    char *s;
    int saved;
    int ret;

    gcase = 0;
    flag9 = 0;
    flag10 = 0;
    flag8 = 0;
    gender = 0;
    if (ai_Unkfe7c20 == 0)
        return -1;
    base = d2_LastWindow * 0x3c4;
    w = ai_Unkf8f13c[d2_LastWindow];
    if (w[0x99] == 0)
        return -1;
    memset(buf, 0, sizeof(buf));
    if ((unsigned)fmt < 0x4800) {
        s = txt_Ptrs[(int)fmt];
        if (s == 0)
            return -1;
        if (strlen(s) > 0x17c0)
            return -1;
        strncpy(buf, s, 0x17c0);
    } else {
        strcpy(buf, fmt);
    }
    pos = 0;
    while (strlen(buf) > 0 && pos < strlen(buf) && pos < 0x17d1) {
        if (buf[pos] != '%' && buf[pos] != '$' && buf[pos] != '{') {
            pos++;
            continue;
        }
        st = pos;
        pos++;
        dig = TXT_ID(buf[pos]);
        if (dig < 0 || dig > 9)
            dig = -1;
        else
            pos++;
        code = buf[pos];
        switch (code) {
        case '%':
            buf[pos - 1] = 0x16;
            memmove(buf + pos, buf + pos + 1, strlen(buf + pos));
            break;
        case 'B':
            st = pos - ((dig != -1) + 1);
            if (buf[st] == '%') {
                gcase = 4;
                memmove(buf + st, buf + st + 1, strlen(buf + st + 1) + 1);
                pos = st - 1;
            }
            break;
        case 'C':
            /* w is the record +0x1c, so the text-command argument table (D2WIN +0x284, args[0x40] values) is
               w + 0x9a and its count (record +0x384) is w[0xda]. The draft cleared w + 0x9a - 1 / w[0x92],
               i.e. the record's `active` (+0x280) and `visible` (+0x264): every "$C" text deactivated its own
               window, so the text was never drawn. The original also passes (win, all = 0). */
            memset((void *)w[4], 0, 0x80);
            memset(w + 0x9a, 0, 0x100);
            w[0xda] = 0;
            d2_RemoveWindowObjects(d2_LastWindow, 0);
            pos++;
            break;
        case 'D':
            /* the argument is a clock; the calendar record goes into word (the original's 10-byte stack
               slot) and the text is "<season name> <year>". The season name table 0x143a2f0 is not set up
               yet (no unit declares it), so the year alone is printed for now. */
            gm_ClockToDate((struct GCLOCK *)ap[0], (unsigned char *)word);
            ap++;
            sprintf(tmp, "%i", *(short *)(word + 2));
            txt_Subst(buf, st, pos - st + 1, tmp);
            break;
        case 'E':
            gcase = 6;
            txt_Subst(buf, pos - ((dig != -1) + 1), (dig != -1) + 2, "");
            pos = pos - ((dig != -1) + 1) - 1;
            break;
        case 'e':
            gcase = 5;
            txt_Subst(buf, pos - ((dig != -1) + 1), (dig != -1) + 2, "");
            pos = pos - ((dig != -1) + 1) - 1;
            break;
        case 'b':
            st = pos - ((dig != -1) + 1);
            if (buf[st] == '%') {
                gcase = 3;
                memmove(buf + st, buf + st + 1, strlen(buf + st + 1) + 1);
                pos = st - 1;
            }
            break;
        case 'u':
            gcase = 1;
            txt_Subst(buf, pos - ((dig != -1) + 1), (dig != -1) + 2, "");
            pos = pos - ((dig != -1) + 1) - 1;
            break;
        case 'U':
            gcase = 2;
            txt_Subst(buf, pos - ((dig != -1) + 1), (dig != -1) + 2, "");
            pos = pos - ((dig != -1) + 1) - 1;
            break;
        case 'G': {
            unsigned char *good = (unsigned char *)*ap++;
            int id = (int)*good * 0xe + 0x48e;

            s = id < 0x4801 ? txt_Ptrs[id] : 0;
            if (dig != -1)
                s = (char *)txt_Field(s, dig, 4);
            if (s == 0)
                s = txt_Ptrs[id];
            if (good[5] == 0)
                strcpy(word, s);
            else
                sprintf(word, "%s", s);
            if (gcase == 0) {
                if (strlen(word) + pos + 1 > 0x17cb)
                    return -1;
                txt_Subst(buf, st, pos - st + 1, word);
            } else {
                s = (char *)txt_Field(word, gcase, 4);
                sprintf(tmp, "%s", s);
                txt_Subst(buf, st, pos - st + 1, tmp);
                gcase = 0;
            }
            break;
        }
        case 'I':
        case 'a':
            /* a plain number: the call sites pass an int (main_Unk5384e0's "$LRunden APs:$R+%a$A" passes
               ai_Chars[i].+0x190, the save format's default 4). The draft read it as a char * and crashed. */
            sprintf(tmp, "%i", *ap++);
            txt_Subst(buf, st, pos - st + 1, tmp);
            break;
        case 'N':
            if (dig != -1 && buf[pos - 1] == '%' && buf[pos] != '$') {
                io_CharacterName(*ap++, gcase, dig, tmp);
                txt_Subst(buf, st, pos - st + 1, tmp);
            }
            pos++;
            break;
        case 'S':
        case 's': {
            int id = *ap++;

            if (id < 0x4801) {
                s = txt_Ptrs[id];
                gender = ai_Unk114fe60[id];
            } else {
                s = (char *)id;
            }
            if (gender == 0xff && s == 0)
                s = txt_Ptrs[id];
            if (gcase == 0) {
                if (strlen(s) + pos + 1 > 0x17cb)
                    return -1;
                txt_Subst(buf, st, pos - st + 1, s);
                /* the replacement can carry tokens: the tutorial's button text 0x7d is the text
                   "%ia[Weiter]", so the scan has to continue at the substitution (the original restarts
                   there too, 0x515c8a -> 0x515fce). Without it the %ia token was never seen, flag9
                   stayed 0, no redraw created the button and io_PrintText returned the old nobjs */
                pos = st - 1;
            } else {
                s = (char *)txt_Field(s, gcase, 4);
                sprintf(tmp, "%s", s);
                if (ls_GetLanguage() == 0 && (gcase == 2 || gcase == 4 || gcase == 6))
                    gcase = 0;
                txt_Subst(buf, st, pos - st + 1, tmp);
                gcase = 0;
            }
            flag8 = 0;
            break;
        }
        case 'T':
            if (buf[pos - 1] == '%') {
                s = ((int (*)(void *))gm_Unk508a80)(*ap++);
                txt_Subst(buf, pos - 1, 2, s);
            }
            pos++;
            break;
        case 'W': {
            char *wb = (char *)w[4];
            int n = strlen(wb) + strlen(buf);

            dig = dig < 1 || dig > 8 ? -1 : dig;
            if (n < 0x17cf) {
                strncat(wb, buf, pos);
                strcat(wb, "\x02");
                *(short *)(w + 0x93) = 2;
            }
            sprintf(tmp, "%s", buf);
            tmp[strlen(buf) - pos] = 0;
            ((int (*)())d2_SetForm)();
            if (dig != -1)
                pos += 2;
            break;
        }
        case 'X':
            buf[pos - 1] = ' ';
            buf[pos] = ' ';
            flag10 = 1;
            pos++;
            break;
        case 'f': {
            int prec = TXT_ID(buf[pos + 1]);

            if (prec < 1 || prec > 8)
                prec = 2;
            else
                pos++;
            sprintf(word, dig == -1 ? "%%.%df" : "%%%d.%df", prec);
            ap += 2;
            sprintf(tmp, word, *(double *)(ap - 2));
            txt_Subst(buf, pos - ((prec != -1) + 1), 2, tmp);
            pos++;
            break;
        }
        case 'i':
            if (buf[pos + 1] == 'i' || buf[pos + 1] == 'c' || buf[pos + 1] == 'b' || buf[pos + 1] == 'a'
                || buf[pos + 1] == 'n' || buf[pos + 1] == 't') {
                if (w[0xda] < 0x41) {
                    w[0x9a + w[0xda]] = -1;
                    if (buf[pos + 1] == 'a' || buf[pos + 1] == 'n')
                        w[0x9a + w[0xda]] = -1;
                    else if (buf[pos + 1] == 't')
                        w[0x9a + w[0xda]] = (((char *)w)[0x260] == 0x53 || ((char *)w)[0x260] == 0x55) ? 0 : 5;
                    else
                        w[0x9a + w[0xda]] = *ap++;
                    flag9 = 1;
                    w[0xda]++;
                } else {
                    ((int (*)())err_Message)();
                }
                pos += 2;
            } else {
                int v = *ap++;

                if (v > 1 || v == 0)
                    flag8 = 2;
                sprintf(word, "%d", v);
                /* the digits of the number are written with a dot between each three */
                len = strlen(word);
                strcpy(tmp, word);
                txt_Subst(buf, st, pos - st + 1, tmp);
            }
            break;
        case 'l':
        case 't':
            flag9 = 1;
            w[0x9a + w[0xda]] = *ap++;
            w[0xda]++;
            pos++;
            break;
        case 'm':
            flag9 = 1;
            if (buf[pos + 1] == 't') {
                w[0x9a + w[0xda]] = ap[0];
                w[0xda]++;
                w[0x9a + w[0xda]] = ap[1];
                w[0xda]++;
                pos += 2;
                ap += 2;
            } else if (buf[pos + 1] == 'b') {
                w[0x9a + w[0xda]] = -1;
                w[0xda]++;
                pos += 2;
            } else {
                w[0x9a + w[0xda]] = -1;
                w[0xda]++;
                pos++;
            }
            break;
        case 'r':
            if (code == 'r') {
                memset(word, 0, 0x20);
                ((int (*)())err_Message)();
            }
            break;
        case 'v':
            pos++;
            if (buf[pos] == 'v')
                ap++;
            break;
        case 'w': {
            /* 0x5181b8: the argument is a player index, not a string: the word +0x54 of the player slot is
               its city id, and that entry of the city name table 0x143c860 gives the text (txt_Field with
               type 8). The skeleton read the argument as a char *; sv_Unk522f10's "%I~ %w" (money, player)
               then crashed in txt_Field. */
            int who = *ap++;
            int city = *(unsigned short *)(ai_PlayerSlotsV[who] + 0x54);

            s = txt_Field(((char **)ai_Unk143c860)[city * 2], 1, 8);
            strcpy(tmp, s);
            txt_Subst(buf, st, pos - st + 1, tmp);
            break;
        }
        default:
            /* not a substitution: the original jumps back to the scan with pos on the command letter
               (0x515ac5 -> 0x515a72, the '$' / '%' drawing commands 'A', 'Z', 'R', 'L', 'Y', '[' and ']'
               among them), so the tail pos++ skips only that letter and a following '%' is still seen.
               Advancing here as well skipped "$Z%s"'s '%': the window got a lone "s" */
            break;
        }
        if (code != 'B' && code != 'b')
            pos++;
        if (pos >= (int)strlen(buf))
            break;
    }
    if (strlen((char *)w[4]) + strlen(buf) > 0x17cf)
        return -1;
    strcpy((char *)w[4] + strlen((char *)w[4]), buf);
    *(short *)(w + 0x93) = 2;
    saved = ai_Unk6ed978;
    ret = *(short *)w;
    if (flag9 == 0 && flag10 == 0)
        return ret;
    if (flag10 == 0) {
        ai_Unk6ed978 = 1;
        s = *(char **)(d2_Objs + 0x40 + w[0x94] * 0x2ec);
        if (s == 0 || *(int *)(s + 0x198) == 0)
            d2_DrawWindow(d2_LastWindow, d2_ScreenSurf);
        else
            d2_RedrawView(*(int *)s);
        ai_Unk6ed978 = saved;
        return ret;
    }
    d2_DrawAll();
    return ret;
}

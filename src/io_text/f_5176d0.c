/* 0x5176d0, the same token engine as 0x515960 into the buffer out instead of the text window: the text id (or literal) id0 and its arguments; the window's argument table is used by the tokens "%l" / "%t" / "%m" / "%i" as in the original but nothing is redrawn. ROUGH skeleton like 0x515960 (the original is a second copy of its 7.5 KB). @deep behaviour: the draft is a skeleton generated from the 0x515960 one, 2656 of 6464 bytes (the same token loop and simple tokens), but this copy takes the output buffer as first argument and has no window record; missing the same %s/%S grammar (the word forms of 0x42c3b0 by case 1 .. 6), the %r random text of a group, the %i dot grouping and the case flags of the original's param_3 .. param_11; readability Q1/Q2: the skeleton identifiers and the unnamed argument / state globals */


void txt_Format(char *out, int id0, ...)
{
    char buf[0x17d0 + 0x20];
    char tmp[0x17d0];
    char word[0x40];
    int *ap = (int *)&out + 2;
    char *fmt = (char *)id0;
    int st, pos, dig, code, len;
    int gcase, flag9, flag10, flag8, gender;
    int *w;
    char *s;

    gcase = 0;
    flag9 = 0;
    flag10 = 0;
    flag8 = 0;
    gender = 0;
    if (ai_Unkfe7c20 == 0)
        return;
    w = ai_Unkf8f13c[d2_LastWindow];
    memset(buf, 0, sizeof(buf));
    if ((unsigned)fmt < 0x4800) {
        s = txt_Ptrs[(int)fmt];
        if (s == 0)
            return;
        if (strlen(s) > 0x17c0)
            return;
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
                    return;
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
                    return;
                txt_Subst(buf, st, pos - st + 1, s);
                pos = st - 1;      /* rescan the replacement (see io_PrintText: "%ia[Weiter]") */
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
            int city = *(unsigned short *)(ai_PlayerSlots[who] + 0x54);

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
    strcpy(out, buf);
}

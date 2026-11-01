/* io_text.c -- the text builders and the text engine (0x5142e0 .. 0x519010), tag "io_Text():" (0x6235d8 /
   0x623618): the town display name, the character names with the "$A" markup, and the token engine
   (io_PrintText into a window, txt_Format into a buffer). The string block starts where f3init.c's ends
   (0x623564) and ends before the language getter 0x519010 (its global 0x62364c opens the "ls" unit).
   Open: the 0x21c character record is untyped; the name builders read it as unsigned char at raw offsets
   (0x9, 0xd, 0x164.., 0x166; 0x1cc through gv.h's ORDCHAR).
   Open: not included yet (dcp headers conflicts io_text HEADER): d2.h; held by own declarations that differ
   from the owner's: d2_Objs.
   unit of rea 8cef7d5b (src/game/io_text.c); boundaries in decl/text.dcl */
#include "d2_types.h"
extern D2WIN d2_Wins[];
#include "txt_views.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include "gv.h"
#include "txt.h"
#include "data_views.h"
#include "io_text.h"
#include "ahm.h"
#include "ai.h"
#include "f3gm.h"
#include "hst.h"
#include "ls.h"
#include "tr.h"
#include "ch.h"
#include "err.h"
#define ai_PlayerCharC (*(char **)&ai_PlayerChar)  /* 0x6f1bf8, ai_PlayerChar (ai.c: unsigned short *) read bytewise, as this unit does (ai_PlayerChar + 4, ch[2]) */

struct GCLOCK;
void gm_ClockToDate(struct GCLOCK *c, unsigned char *out);  /* 0x4fe2c0, f3gm.c: the calendar record of a clock */
int gm_Season(char *d);                 /* 0x4fe330, f3gm.c: the season of a calendar record */

extern char ai_PlayerSlots[4][0x2f4];      /* 0x7e3f40, the player slots */
extern int __mb_cur_max;                /* 0x62e3a0, the CRT multibyte state (MB_CUR_MAX of ctype.h) */
extern unsigned short *_pctype;         /* 0x62e194, the CRT ctype table (the 0x100 entry values) */
/* 0x5142e0, the display name of the town called name (the player's town, "General"/"Stadt" of the game INI): the first text of the town's "_STADTAUSWAHL_<NAME>_INFO" group of the text table (txt_FindKey finds its number), else the given name itself; copied to out. Returns 1. */
int io_TownDisplayName(char *name, char *out)
{
    char key[0x100];
    int idx;

    sprintf(key, "_STADTAUSWAHL_%s_INFO+0", name);
    _strupr(key);
    idx = txt_FindKey(key);
    if (idx != -1) {
        strcpy(out, txt_Ptrs[idx]);
        return 1;
    }
    strcpy(out, name);
    return 1;
}
/* 0x514360, the digits at the end of s[0 .. n] (scanning back from n while isdigit): copied to out, returns the start index */


int io_TrailingDigits(char *s, int n, char *out)
{
    int i;

    for (i = n; i >= 0; i--) {
        if (!isdigit(s[i]))
            break;
    }
    i++;
    memcpy(out, s + i, n - i + 1);
    out[n - i + 1] = 0;
    return i;
}



/* txt_PseudoCharName: view of txt_Ptrs (build/mk_txt_views.py) */
/* txt_NoCharacter: view of txt_Ptrs (build/mk_txt_views.py) */
#define G30_ENDS_S(p) ((p)[strlen(p) - 1] == 's' || (p)[strlen(p) - 1] == 'z' || (p)[strlen(p) - 1] == '\xdf' || (p)[strlen(p) - 1] == 'x')  /* the sharp s: on an unsigned char record name the original compares the zero-extended byte with -0x21, never equal */
#define CH_NAME(r) ((r) + 0x30)         /* the first name */
#define CH_FAMILY(r) ((r) + 0x40)       /* the family name */
/* 0x5143d0, writes the name of the character id (0xfffe: the empty record with the default first name
   txt_PseudoCharName) into out in the form kind: 1 the first name, 2 the level title + first + family name,
   3 the profession title + names, 4 the job / rank title + first name, 5 the profession title, 6 the family
   (else the first) name, 7 first + family name, 8 / 9 the profession (9: the privilege's office from flags1cc)
   or level title + first name; c 2 makes it a genitive ("'" after s / z / ss / x, else "s"), c is also the
   case txt_Field picks for 8 / 9. The player's own character is the player slot's name. Returns 0 for no
   character (the text txt_NoCharacter) */
int io_CharacterName(int id, unsigned char c, unsigned char kind, char *out)
{
    char A[0x100], B[0x100], C[0x100];
    char tmp[0x21c];
    unsigned char *rec;
    int v;

    A[0] = ch_EmptyName[0];
    memset(A + 1, 0, 0xff);
    C[0] = ch_EmptyName[0];
    memset(C + 1, 0, 0xff);
    B[0] = ch_EmptyName[0];
    memset(B + 1, 0, 0xff);
    if (id < 0 || id == 0xffff) {
        strcpy(out, txt_NoCharacter);
        return 0;
    }
    if (id != 0xfffe) {
        rec = (unsigned char *)ai_Chars[id];
    } else {
        memset(tmp, 0, 0x21c);
        rec = (unsigned char *)tmp;
        strcpy(CH_NAME(rec), txt_PseudoCharName);
    }
    if ((char *)rec == ai_PlayerCharC) {
        strcpy(out, ai_PlayerSlots[ai_OwnPlayer]);
        if (c == 2)
            strcat(out, "'s");
        return 1;
    }
    switch (kind) {
    case 1:
        strcpy(out, CH_NAME(rec));
        if (c == 2) {
            if (G30_ENDS_S(out))
                strcat(out, "'");
            else
                strcat(out, "s");
        }
        break;
    case 2:
        if (rec[0xd]) {
            if (*CH_FAMILY(rec) == 0)
                sprintf(out, "%s %s", txt_Field(txt_Ptrs[(rec[9] ? 0x11e : 0x114) + rec[0xd]], 1, 4),
                        CH_NAME(rec));
            else
                sprintf(out, "%s %s %s", txt_Field(txt_Ptrs[(rec[9] ? 0x11e : 0x114) + rec[0xd]], 1, 4),
                        CH_NAME(rec), CH_FAMILY(rec));
        } else {
            sprintf(out, "%s", CH_NAME(rec));
        }
        break;
    case 3:
        if (rec[0x166]) {
            if (*CH_FAMILY(rec))
                sprintf(out, "%s %s %s", txt_Field(txt_Ptrs[(rec[9] ? 0x27b : 0x253) + rec[0x166]], 1, 4),
                        CH_NAME(rec), CH_FAMILY(rec));
            else
                sprintf(out, "%s %s", txt_Field(txt_Ptrs[(rec[9] ? 0x27b : 0x253) + rec[0x166]], 1, 4),
                        CH_NAME(rec));
        } else if (*CH_FAMILY(rec)) {
            sprintf(out, "%s %s", CH_NAME(rec), CH_FAMILY(rec));
        } else {
            sprintf(out, "%s", CH_NAME(rec));
        }
        if (c == 2) {
            if (G30_ENDS_S(CH_FAMILY(rec)))
                strcat(out, "'");
            else
                strcat(out, "s");
        }
        break;
    case 4:
        if (rec[0x164])
            sprintf(out, "%s %s", txt_Field(txt_Ptrs[(rec[9] ? 0x18f : 0x130) + rec[0x164]], 1, 4),
                    CH_NAME(rec));
        else if (rec[0x165])
            sprintf(out, "%s %s", txt_Field(txt_Ptrs[(rec[9] ? 0x230 : 0x20d) + rec[0x165]], 1, 4),
                    CH_NAME(rec));
        else
            sprintf(out, "%s", CH_NAME(rec));
        if (c == 2) {
            if (G30_ENDS_S(CH_NAME(rec)))
                strcat(out, "'");
            else
                strcat(out, "s");
        }
        break;
    case 5:
        sprintf(out, "%s", txt_Field(txt_Ptrs[(rec[9] ? 0x27b : 0x253) + rec[0x166]], 1, 4));
        break;
    case 6:
        if (*CH_FAMILY(rec))
            strcpy(out, CH_FAMILY(rec));
        else
            strcpy(out, CH_NAME(rec));
        break;
    case 7:
        if (c == 2) {
            if (G30_ENDS_S(CH_FAMILY(rec))) {
                if (*CH_FAMILY(rec))
                    sprintf(out, "%s %s'", CH_NAME(rec), CH_FAMILY(rec));
                else
                    sprintf(out, "%s'", CH_NAME(rec));
            } else {
                if (*CH_FAMILY(rec))
                    sprintf(out, "%s %ss", CH_NAME(rec), CH_FAMILY(rec));
                else
                    sprintf(out, "%ss", CH_NAME(rec));
            }
        } else if (*CH_FAMILY(rec)) {
            sprintf(out, "%s %s", CH_NAME(rec), CH_FAMILY(rec));
        } else {
            sprintf(out, "%s", CH_NAME(rec));
        }
        break;
    case 8:
        if (rec[0xd])
            strcpy(C, txt_Field(txt_Ptrs[(rec[9] ? 0x11e : 0x114) + rec[0xd]], c, 4));
        if (rec[0x166])
            strcpy(B, txt_Field(txt_Ptrs[(rec[9] ? 0x27b : 0x253) + rec[0x166]], c, 4));
        if (c == 2) {
            if (G30_ENDS_S(CH_NAME(rec)))
                sprintf(A, "%s'", CH_NAME(rec));
            else
                sprintf(A, "%ss", CH_NAME(rec));
        } else {
            strcpy(A, CH_NAME(rec));
        }
        if (rec[0x166])
            sprintf(out, "%s %s", B, A);
        else
            sprintf(out, "%s %s", C, A);
        break;
    case 9:
        if (ORD_CHAR(rec)->flags1cc & 0x3e00) {
            if (ORD_CHAR(rec)->flags1cc & 0x1000)
                v = 0x26;
            else if (ORD_CHAR(rec)->flags1cc & 0x2000)
                v = 0x27;
            else if (ORD_CHAR(rec)->flags1cc & 0x800)
                v = 0x25;
            else if (ORD_CHAR(rec)->flags1cc & 0x400)
                v = 0x24;
            else
                v = (ORD_CHAR(rec)->flags1cc & 0x200) ? 0x23 : 0;
        } else {
            v = rec[0x166];
        }
        if (rec[0xd])
            strcpy(C, txt_Field(txt_Ptrs[(rec[9] ? 0x11e : 0x114) + rec[0xd]], c, 4));
        if (v)
            strcpy(B, txt_Field(txt_Ptrs[(rec[9] ? 0x27b : 0x253) + v], c, 4));
        if (c == 2) {
            if (G30_ENDS_S(CH_NAME(rec)))
                sprintf(A, "%s'", CH_NAME(rec));
            else
                sprintf(A, "%ss", CH_NAME(rec));
        } else {
            strcpy(A, CH_NAME(rec));
        }
        if (v)
            sprintf(out, "%s %s", B, A);
        else
            sprintf(out, "%s %s", C, A);
        break;
    }
    return 1;
}


/* 0x514e90, io_CharacterName with the text markup "$A" between the title and the name ("%s$A%s", "%s$A%ss",
   "%s$A%s'") in the kinds 2, 4, 7 and 8, for the ids 0 .. 0x2ff (anything else: the text txt_NoRelative,
   returns 0); kind 8 always takes the titles in case 1 and has the name only in the genitive (else the
   empty text), kinds 3 and 9 keep the plain space; the player's own
   name gets a plain "s" genitive. A kind 4 name without a title gets its genitive twice (as the original
   does: the "%s'" / "%ss" sprintf, then the append). */
int io_CharacterNameMarkup(int id, unsigned char c, unsigned char kind, char *out)
{
    char A[0x100], B[0x100], C[0x100];
    char tmp[0x21c];
    unsigned char *rec;
    int v;

    A[0] = ch_EmptyName[0];
    memset(A + 1, 0, 0xff);
    C[0] = ch_EmptyName[0];
    memset(C + 1, 0, 0xff);
    B[0] = ch_EmptyName[0];
    memset(B + 1, 0, 0xff);
    if (id < 0 || id >= 0x300) {
        strcpy(out, txt_NoRelative);
        return 0;
    }
    if (id != 0xfffe) {
        rec = (unsigned char *)ai_Chars[id];
    } else {
        memset(tmp, 0, 0x21c);
        rec = (unsigned char *)tmp;
        strcpy(CH_NAME(rec), txt_PseudoCharName);
    }
    if ((char *)rec == ai_PlayerCharC) {
        strcpy(out, ai_PlayerSlots[ai_OwnPlayer]);
        if (c == 2)
            strcat(out, "s");
        return 1;
    }
    switch (kind) {
    case 1:
        strcpy(out, CH_NAME(rec));
        if (c == 2) {
            if (G30_ENDS_S(CH_NAME(rec)))
                strcat(out, "'");
            else
                strcat(out, "s");
        }
        break;
    case 2:
        if (rec[0xd])
            sprintf(out, "%s$A%s", txt_Field(txt_Ptrs[(rec[9] ? 0x11e : 0x114) + rec[0xd]], 1, 4), CH_NAME(rec));
        else
            sprintf(out, "%s", CH_NAME(rec));
        break;
    case 3:
        if (rec[0x166]) {
            if (*CH_FAMILY(rec))
                sprintf(out, "%s %s %s", txt_Field(txt_Ptrs[(rec[9] ? 0x27b : 0x253) + rec[0x166]], 1, 4),
                        CH_NAME(rec), CH_FAMILY(rec));
            else
                sprintf(out, "%s %s", txt_Field(txt_Ptrs[(rec[9] ? 0x27b : 0x253) + rec[0x166]], 1, 4),
                        CH_NAME(rec));
        } else if (*CH_FAMILY(rec)) {
            sprintf(out, "%s %s", CH_NAME(rec), CH_FAMILY(rec));
        } else {
            sprintf(out, "%s", CH_NAME(rec));
        }
        if (c == 2) {
            if (G30_ENDS_S(CH_FAMILY(rec)))
                strcat(out, "'");
            else
                strcat(out, "s");
        }
        break;
    case 4:
        if (rec[0x164]) {
            sprintf(out, "%s$A%s", txt_Field(txt_Ptrs[(rec[9] ? 0x18f : 0x130) + rec[0x164]], 1, 4),
                    CH_NAME(rec));
        } else if (rec[0x165]) {
            sprintf(out, "%s$A%s", txt_Field(txt_Ptrs[(rec[9] ? 0x230 : 0x20d) + rec[0x165]], 1, 4),
                    CH_NAME(rec));
        } else if (c == 2) {
            if (G30_ENDS_S(CH_NAME(rec)))
                sprintf(out, "%s'", CH_NAME(rec));
            else
                sprintf(out, "%ss", CH_NAME(rec));
        } else {
            sprintf(out, "%s", CH_NAME(rec));
        }
        if (c == 2) {
            if (G30_ENDS_S(CH_NAME(rec)))
                strcat(out, "'");
            else
                strcat(out, "s");
        }
        break;
    case 5:
        sprintf(out, "%s", txt_Field(txt_Ptrs[(rec[9] ? 0x27b : 0x253) + rec[0x166]], 1, 4));
        break;
    case 6:
        if (*CH_FAMILY(rec))
            strcpy(out, CH_FAMILY(rec));
        else
            strcpy(out, CH_NAME(rec));
        break;
    case 7:
        if (c == 2) {
            if (G30_ENDS_S(CH_FAMILY(rec))) {
                if (*CH_FAMILY(rec))
                    sprintf(out, "%s$A%s'", CH_NAME(rec), CH_FAMILY(rec));
                else
                    sprintf(out, "%s'", CH_NAME(rec));
            } else {
                if (*CH_FAMILY(rec))
                    sprintf(out, "%s$A%ss", CH_NAME(rec), CH_FAMILY(rec));
                else
                    sprintf(out, "%ss", CH_NAME(rec));
            }
        } else if (*CH_FAMILY(rec)) {
            sprintf(out, "%s$A%s", CH_NAME(rec), CH_FAMILY(rec));
        } else {
            sprintf(out, "%s", CH_NAME(rec));
        }
        break;
    case 8:
        if (rec[0xd])
            strcpy(C, txt_Field(txt_Ptrs[(rec[9] ? 0x11e : 0x114) + rec[0xd]], 1, 4));
        if (rec[0x166])
            strcpy(B, txt_Field(txt_Ptrs[(rec[9] ? 0x27b : 0x253) + rec[0x166]], 1, 4));
        if (c == 2) {
            if (G30_ENDS_S(CH_NAME(rec)))
                sprintf(A, "%s'", CH_NAME(rec));
            else
                sprintf(A, "%ss", CH_NAME(rec));
        }
        if (rec[0x166])
            sprintf(out, "%s$A%s", B, A);
        else
            sprintf(out, "%s$A%s", C, A);
        break;
    case 9:
        if (ORD_CHAR(rec)->flags1cc & 0x3e00) {
            if (ORD_CHAR(rec)->flags1cc & 0x1000)
                v = 0x26;
            else if (ORD_CHAR(rec)->flags1cc & 0x2000)
                v = 0x27;
            else if (ORD_CHAR(rec)->flags1cc & 0x800)
                v = 0x25;
            else if (ORD_CHAR(rec)->flags1cc & 0x400)
                v = 0x24;
            else
                v = (ORD_CHAR(rec)->flags1cc & 0x200) ? 0x23 : 0;
        } else {
            v = rec[0x166];
        }
        if (rec[0xd])
            strcpy(C, txt_Field(txt_Ptrs[(rec[9] ? 0x11e : 0x114) + rec[0xd]], c, 4));
        if (v)
            strcpy(B, txt_Field(txt_Ptrs[(rec[9] ? 0x27b : 0x253) + v], c, 4));
        if (c == 2) {
            if (G30_ENDS_S(CH_NAME(rec)))
                sprintf(A, "%s'", CH_NAME(rec));
            else
                sprintf(A, "%ss", CH_NAME(rec));
        } else {
            strcpy(A, CH_NAME(rec));
        }
        if (v)
            sprintf(out, "%s %s", B, A);
        else
            sprintf(out, "%s %s", C, A);
        break;
    }
    return 1;
}


extern int txt_Loaded;
extern int d2_LastWindow;                /* 0x6ed938, the current text window */
int txt_Count = 0;  /* 0x6333b8 */
int d2_LayoutMode = 0;  /* 0x6ed978 */
extern char *d2_Objs;  /* 0xf8f080, 0x2ec bytes each; @original: d2.h declares D2OBJ *; the units add byte offsets to it and do not include d2.h yet (their own char * declarations of d2_Objs / d2_LastWindowPtr supersede d2.h's) */
#define D2_OBJS ((D2OBJ *)d2_Objs)     /* d2_Objs as d2.h types it */
#define ai_PlayerCity (*(unsigned short (*)[4][0x17a])((char *)&ai_PlayerSlots + 0x54))  /* 0x7e3f94, the city id (word +0x54) of each 0x2f4-byte player slot: ai_PlayerCity[player][0] */
/* ---- the text engine (0x515960 .. 0x519010) ---- */
int txt_Loaded = {0}; /* 0xfe7c20 */
int txt_Random[0x4800] = {0}; /* 0x114fe60, per text id its random group (9 + n of "{rn}") or its gender (0 .. 2, 0xff none) */
int d2_SetForm(int view, int n);                    /* 0x475f70 */
void d2_DrawAll(void);                    /* 0x46e150 */
void d2_DrawWindow(int win, void *surf);                    /* 0x471a90, draws a window (d2.h) */
int d2_RemoveWindowObjects(int win, int all);         /* 0x472d20, removes the objects of a window (d2.h) */
void d2_RedrawView(int view);                    /* 0x474e80 */
extern void *d2_ScreenSurf;              /* 0x6ed924, the screen surface (d2.c) */
void err_Message(char *text);                    /* 0x4222b0 */
void gm_FormatAmount(int v, unsigned char k, char *out);                   /* 0x508a80 */
int io_TrailingDigits();                /* 0x514360 */
int ls_GetLanguage();                     /* 0x5190a0, the language number */

/* 0x515960, the text engine: prints the text fmt (a literal or an id of txt_Ptrs below 0x4800) with the arguments of its tokens ("%" "$" "{" + an optional digit + a letter) appended to the text of the current window d2_LastWindow and redraws the window (or its view; "%X" redraws all). Returns the window's object count (nobjs: the index the object of this text's "%ia" gets, the redraw has created it before the caller's d2_GetWinObj), -1 when the texts are not loaded, the window is not active, or the text is missing / too long.
   The tokens: "%s" / "%S" a text id (or a string above 0x4800) whose grammar field dig (or 1) is taken when the text has a gender in txt_Random; after "%u" / "%U" / "%b" / "%B" / "%e" / "%E" (gcase 1 .. 6: the article tables txt_Ptrs[0x3b / 0x38 / 0x3e + gender], the even ones capitalised in German) the article goes in front, with a number found before the token ("3 %us") moved between them; the scan restarts at the substitution, so a substituted text's own tokens are expanded too ("%ia[Weiter]"). "$S" is ten blanks, "%G" a good's name (and ">variant<"), "%N" a character name (io_CharacterName, dig the case, the digit after N the kind), "%T" an amount (gm_FormatAmount), "%D" a clock as "<season> <year>", "%i" a number with a '.' between the thousands ("%Ni" zero padded to N; a count other than 1 makes the next "%s" plural), "%I" a number without dots, "%f" a double ("%Nf<p>"), "%a" a number followed by 0x14, "%w" the city name of a player, "{r<group><n>}" one of the next n texts of the random group, "%v" / "%vv" skips an argument, "%%" the char 0x16. "%ii" / "%ic" / "%ib" / "%ia" / "%in" / "%it", "%l" / "%t" and "%m" / "%mt" / "%mb" keep their argument (or -1) in the window's argument table args for the redraw (the icons and buttons of the text); "$C" clears the window's text and that table and removes its objects; "%W" flushes the text so far and switches the form (d2_SetForm(dig, n)). Every other '$' / '%' command ("$A", "$Z", "$R", "$L", "$Y", "$[", "$]" ...) stays in the text for the window's text drawing. @deep read the whole function against the original (0x515960..0x51760a, all 25 case bodies via the jump table 0x51760c / index table 0x517674) and reconstructed every statement: the 's'/'S' article blocks (txt_Ptrs[0x3b/0x38/0x3e + gender], the digits before the token via io_TrailingDigits, toupper in German), '$S' ten blanks, the F fallthrough, the '%i' dot grouping both ways, '%I', '%f' with _itoa formats, '%W' flush + d2_SetForm(dig, n), u/U/b/B/e/E removals, '%N' (case dig, kind digit), '%a' '%i%c' 0x14, '{r}' random group search + atoi + util_Random, '%D' season txt_Ptrs[0x4c + gm_Season], '%G' good name + '>%s<' variant, '%%' 0x16, the end redraw (ctrl->loaded / view). ours 2238 vs 2244 instructions, switch tables in the same case order. Left: (1) window addressing: the original keeps the window's byte offset d2_LastWindow*0x3c4 in [esp+0x20] and addresses [esi + d2_Wins+field] (asmdiff head: '- mov ecx,[eax*4+d2_Wins+0x280] / - lea eax,[eax*4+d2_Wins] / - mov [esp+0x24],eax' vs '+ shl eax,2 / + mov [esp+0x20],eax / + cmp [eax+0xf8f3a0],ebx'); ours a D2WIN *w ([esi+0x2c]). An index d2_Wins[win] factors args[n] into (win*0xf1+n)*4 (2074 differing); an offset variable with #define WIN (*(D2WIN *)((char *)d2_Wins + woff)) reproduces the addressing and prologue exactly but switches VC6 cross-jumping back on (2115 vs 2244: f, b/B, G tails merged) - the on/off switch also flipped with unrelated edits (r-case if/else, e/E toklen), a compiler-state threshold, not meaning. (2) cross-jump/scheduling: e/E jump into the u memmove tail in the original (ours own calls, memmove 24 vs 22), the r error block shared (err_Message 3 vs 2), the 'I' va_arg increment hoisted above the first sprintf, the 'i' table branch reloads buf[pos+1] after the args store; (3) slots: dig/st share [esp+0xc], art/k/len [esp+0x14], p/good/grp [esp+0x18] in the original (58 slots hunks). */
int io_PrintText(char *fmt, ...)
{
    char buf[0x17d0];                   /* the text being expanded */
    char tmp[0x17d0];                   /* the substitution */
    char fmtbuf[0x20];                  /* a sprintf format built at run time, or the plain digits of "%i" */
    char num[0x14];                     /* _itoa digits */
    union {
        unsigned char b[0xc];
        short w[6];                     /* w[1]: the year */
    } date;                             /* gm_ClockToDate's calendar record */
    char rnum[0x20];                    /* the count of a "{r}" token */
    char goodname[0x100];
    char digits1[0x40], digits2[0x40], digits3[0x40];
    va_list ap;
    D2WIN *w;                           /* the window d2_LastWindow on entry */
    int pos, dig, st, toklen, i, k, n, len, v, id;
    int gcase, gender, plural, redraw, drawall;
    char code, c;
    char *s, *art, *p;
    unsigned char *good;
    D2CTRL *ctrl;                       /* the drawing buffer of the window's object (a view) */
    int saved, ret;

    redraw = 0;
    drawall = 0;
    gcase = 0;
    gender = 0;
    plural = 0;
    if (txt_Loaded == 0)
        return -1;
    w = &d2_Wins[d2_LastWindow];
    if (w->active == 0)
        return -1;
    va_start(ap, fmt);
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
    for (pos = 0; pos < strlen(buf) && pos <= 0x17d0; pos++) {
        while (buf[pos] == '%' || buf[pos] == '$' || buf[pos] == '{') {
            pos++;
            dig = buf[pos] - '0';
            if (dig >= 0 && dig <= 9)
                pos++;
            else
                dig = -1;
            code = buf[pos];
            switch (code) {
            case 'C':
                pos++;
                memset(w->text, 0, 0x80);
                w->nargs = 0;
                memset(w->args, 0, sizeof(w->args));
                d2_RemoveWindowObjects(w->id, 0);
                break;
            case 'S':
            case 's':
                if (buf[pos - 1] == '$') {
                    memmove(&buf[pos + 9], &buf[pos + 1], strlen(buf) - pos);
                    memset(&buf[pos - 1], ' ', 10);
                    break;
                }
                gender = 0xff;
                if (buf[pos - 1] != '{') {
                    pos++;
                    id = va_arg(ap, int);
                    if (id > 0x4800) {
                        s = (char *)id;
                    } else {
                        s = txt_Ptrs[id];
                        gender = txt_Random[id];
                    }
                    toklen = (dig != -1) + 2;
                    if (gender != 0xff) {
                        if (dig == -1)
                            dig = 1;
                        if (buf[pos - 1] == 's' && plural == 0)
                            s = txt_Field(s, dig, 4);
                        else
                            s = txt_Field(s, dig, 8);
                    } else if (s == 0) {
                        s = txt_Ptrs[id];
                    }
                    if (gcase == 0) {
                        if (strlen(s) + pos > 0x17cb)
                            return -1;
                        st = pos - toklen;
                        memmove(&buf[st + strlen(s)], &buf[pos], strlen(buf) + 2 - pos);
                    } else {
                        /* a number right before the token (only blanks between) goes between the article and
                           the word */
                        for (i = pos - toklen - 1; i >= 0; i--) {
                            if (isdigit(buf[i]))
                                break;
                            if (buf[i] != ' ') {
                                i = -1;
                                break;
                            }
                        }
                        if (gcase == 1 || gcase == 2) {
                            if (buf[pos - 1] == 's' && plural == 0)
                                art = txt_Field(txt_Ptrs[gender + 0x3b], dig, 4);
                            else
                                art = txt_Field(txt_Ptrs[gender + 0x3b], dig, 8);
                            if (i <= -1) {
                                sprintf(tmp, "%s %s", art, s);
                            } else {
                                n = io_TrailingDigits(buf, i, digits1);
                                sprintf(tmp, "%s %s %s", art, digits1, s);
                                toklen = pos - n;
                            }
                            if (ls_GetLanguage() == 0 && gcase == 2)
                                tmp[0] = toupper(tmp[0]);
                        }
                        if (gcase == 3 || gcase == 4) {
                            if (buf[pos - 1] == 's' && plural == 0)
                                art = txt_Field(txt_Ptrs[gender + 0x38], dig, 4);
                            else
                                art = txt_Field(txt_Ptrs[gender + 0x38], dig, 8);
                            if (i <= -1) {
                                sprintf(tmp, "%s %s", art, s);
                            } else {
                                n = io_TrailingDigits(buf, i, digits2);
                                sprintf(tmp, "%s %s %s", art, digits2, s);
                                toklen = pos - n;
                            }
                            if (ls_GetLanguage() == 0 && gcase == 4)
                                tmp[0] = toupper(tmp[0]);
                        }
                        if (gcase == 5 || gcase == 6) {
                            if (buf[pos - 1] == 's' && plural == 0)
                                art = txt_Field(txt_Ptrs[gender + 0x3e], dig, 4);
                            else
                                art = txt_Field(txt_Ptrs[gender + 0x3e], dig, 8);
                            if (i <= -1) {
                                sprintf(tmp, "%s %s", art, s);
                            } else {
                                n = io_TrailingDigits(buf, i, digits3);
                                sprintf(tmp, "%s %s %s", art, digits3, s);
                                toklen = pos - n;
                            }
                            if (ls_GetLanguage() == 0 && gcase == 6)
                                tmp[0] = toupper(tmp[0]);
                        }
                        if (strlen(tmp) + pos > 0x17cb)
                            return -1;
                        st = pos - toklen;
                        memmove(&buf[st + strlen(tmp)], &buf[pos], strlen(buf) - pos + toklen);
                        gcase = 0;
                        s = tmp;
                    }
                    memcpy(&buf[st], s, strlen(s));
                    plural = 0;
                    pos = st;
                    break;
                }
                /* "{s": falls through */
            case 'F':
                pos++;
                break;
            case 'l':
            case 't':
                w->args[w->nargs] = va_arg(ap, int);
                redraw = 1;
                w->nargs++;
                pos++;
                break;
            case 'T':
                if (buf[pos - 1] == '%') {
                    gm_FormatAmount(va_arg(ap, int), ai_OwnPlayer, tmp);
                    memmove(&buf[pos - 1 + strlen(tmp)], &buf[pos + 1], strlen(buf) + 1 - pos);
                    memcpy(&buf[pos - 1], tmp, strlen(tmp));
                }
                pos++;
                break;
            case 'w':
                /* the argument is a player index: the city id (word +0x54) of its player slot picks the city
                   name (the text pairs from txt_Ptrs[0x9a8]) */
                strcpy(tmp, txt_Field(txt_Ptrs[0x9a8 + ai_PlayerCity[va_arg(ap, int)][0] * 2], 1, 8));
                memmove(&buf[pos - 1 + strlen(tmp)], &buf[pos + 1], strlen(buf) + 1 - pos);
                memcpy(&buf[pos - 1], tmp, strlen(tmp));
                break;
            case 'i':
                c = buf[pos + 1];
                if (c == 'i' || c == 'c' || c == 'b' || c == 'a' || c == 'n' || c == 't') {
                    if (w->nargs > 0x40) {
                        err_Message("io_Text(): too many icons before redraw!");
                        pos += 2;
                        break;
                    }
                    w->args[w->nargs] = -1;
                    if (buf[pos + 1] == 'a') {
                        if (d2_Wins[d2_LastWindow].font == 0x53 || d2_Wins[d2_LastWindow].font == 0x55)
                            w->args[w->nargs] = -1;
                        else
                            w->args[w->nargs] = -1;
                    } else if (buf[pos + 1] == 'n') {
                        if (d2_Wins[d2_LastWindow].font == 0x53 || d2_Wins[d2_LastWindow].font == 0x55)
                            w->args[w->nargs] = -1;
                        else
                            w->args[w->nargs] = -1;
                    } else if (buf[pos + 1] == 't') {
                        if (d2_Wins[d2_LastWindow].font == 0x53 || d2_Wins[d2_LastWindow].font == 0x55)
                            w->args[w->nargs] = 0;
                        else
                            w->args[w->nargs] = 5;
                    } else {
                        w->args[w->nargs] = va_arg(ap, int);
                    }
                    redraw = 1;
                    w->nargs++;
                    pos += 2;
                    break;
                }
                if (dig != -1) {
                    /* zero padded to dig places, then the dots: tmp -> fmtbuf backwards */
                    _itoa(dig, num, 10);
                    strcpy(fmtbuf, "%0");
                    strcat(fmtbuf, num);
                    strcat(fmtbuf, "i");
                    v = va_arg(ap, int);
                    if (v > 1 || v == 0)
                        plural = 2;
                    sprintf(tmp, fmtbuf, v);
                    k = -1;
                    len = strlen(tmp) / 3 + strlen(tmp);
                    n = len;
                    for (i = strlen(tmp); i >= 0; i--) {
                        fmtbuf[n--] = tmp[i];
                        if (i != 0 && k > 0 && (k + 1) % 3 == 0)
                            fmtbuf[n--] = '.';
                        k++;
                    }
                    fmtbuf[len] = 0;
                    memmove(&buf[pos + strlen(fmtbuf) - 3], &buf[pos], strlen(buf) + 1 - pos);
                    memcpy(&buf[pos - 2], fmtbuf, strlen(fmtbuf));
                } else {
                    /* the digits into fmtbuf, then with the dots: fmtbuf -> tmp backwards */
                    v = va_arg(ap, int);
                    if (v > 1 || v == 0)
                        plural = 2;
                    sprintf(fmtbuf, "%i", v);
                    k = -1;
                    len = (strlen(fmtbuf) - 1) / 3 + strlen(fmtbuf);
                    n = len;
                    for (i = strlen(fmtbuf); i >= 0; i--) {
                        tmp[n--] = fmtbuf[i];
                        if (i != 0 && k > 0 && (k + 1) % 3 == 0)
                            tmp[n--] = '.';
                        k++;
                    }
                    tmp[len] = 0;
                    memmove(&buf[pos - 1 + strlen(tmp)], &buf[pos + 1], strlen(buf) + 1 - pos);
                    memcpy(&buf[pos - 1], tmp, strlen(tmp));
                }
                break;
            case 'I':
                if (dig != -1) {
                    sprintf(fmtbuf, "%0%ii", dig);
                    sprintf(tmp, fmtbuf, va_arg(ap, int));
                    memmove(&buf[pos + strlen(tmp) - 3], &buf[pos], strlen(buf) + 1 - pos);
                    memcpy(&buf[pos - 2], tmp, strlen(tmp));
                } else {
                    sprintf(tmp, "%i", va_arg(ap, int));
                    memmove(&buf[pos - 1 + strlen(tmp)], &buf[pos + 1], strlen(buf) + 1 - pos);
                    memcpy(&buf[pos - 1], tmp, strlen(tmp));
                }
                break;
            case 'f':
                /* "%f<p>" / "%<w>f<p>": p decimals (1 .. 8, else 2), w the zero padded width */
                k = buf[pos + 1] - '0';
                if (k > 0 && k < 9)
                    pos++;
                else
                    k = 2;
                if (dig != -1) {
                    _itoa(dig, num, 10);
                    strcpy(fmtbuf, "%0");
                    strcat(fmtbuf, num);
                    _itoa(k, num, 10);
                    strcat(fmtbuf, ".");
                    strcat(fmtbuf, num);
                    strcat(fmtbuf, "f");
                    sprintf(tmp, fmtbuf, va_arg(ap, double));
                    if (strlen(tmp) + pos > 0x17cb)
                        return -1;
                    pos++;
                    p = &buf[pos - (k != -1) - 2];
                    memmove(p + strlen(tmp), &buf[pos], strlen(buf) + 1 - pos);
                    memcpy(p, tmp, strlen(tmp));
                } else {
                    _itoa(k, num, 10);
                    strcpy(fmtbuf, "%.");
                    strcat(fmtbuf, num);
                    strcat(fmtbuf, "f");
                    sprintf(tmp, fmtbuf, va_arg(ap, double));
                    if (strlen(tmp) + pos > 0x17cb)
                        return -1;
                    pos++;
                    p = &buf[pos - (k != -1) - 2];
                    memmove(p + strlen(tmp), &buf[pos], strlen(buf) + 1 - pos);
                    memcpy(p, tmp, strlen(tmp));
                }
                break;
            case 'v':
                pos++;
                if (buf[pos] == 'v')
                    va_arg(ap, int);
                break;
            case 'm':
                pos++;
                redraw = 1;
                if (buf[pos] == 't') {
                    w->args[w->nargs] = va_arg(ap, int);
                    w->nargs++;
                    w->args[w->nargs] = va_arg(ap, int);
                    w->nargs++;
                    pos++;
                } else if (buf[pos] == 'b') {
                    w->args[w->nargs] = -1;
                    w->nargs++;
                    pos++;
                } else {
                    w->args[w->nargs] = -1;
                    w->nargs++;
                }
                break;
            case 'X':
                buf[pos - 1] = ' ';
                drawall = 1;
                buf[pos] = ' ';
                pos++;
                break;
            case 'W':
                /* "%<view>W<n>": the text so far goes to the window, the rest stays in buf */
                if (dig == -1)
                    dig = 0;
                k = buf[pos + 1] - '0';
                if (k <= 0 || k >= 9)
                    k = -1;
                p = d2_Wins[d2_LastWindow].text;
                if (strlen(p) + strlen(buf) < 0x17cf) {
                    strncat(p, buf, pos);
                    strcat(d2_Wins[d2_LastWindow].text, " ");
                    d2_Wins[d2_LastWindow].drawFrames = 2;
                }
                memmove(buf, &buf[pos + 2], strlen(buf) - pos);
                buf[strlen(buf) - pos] = 0;
                d2_SetForm(dig, k);
                if (k == -1) {
                    pos++;
                    break;
                }
                pos += 2;
                break;
            case 'u':
                gcase = 1;
                st = pos - ((dig != -1) + 1);
                memmove(&buf[st], &buf[pos + 1], strlen(buf) - pos);
                pos = st - 1;
                break;
            case 'U':
                gcase = 2;
                st = pos - ((dig != -1) + 1);
                memmove(&buf[st], &buf[pos + 1], strlen(buf) - pos);
                pos = st - 1;
                break;
            case 'b':
                st = pos - ((dig != -1) + 1);
                if (buf[st] != '%') {
                    pos++;
                    break;
                }
                gcase = 3;
                memmove(&buf[st], &buf[pos + 1], strlen(buf) - pos);
                pos = st - 1;
                break;
            case 'B':
                st = pos - ((dig != -1) + 1);
                if (buf[st] != '%') {
                    pos++;
                    break;
                }
                gcase = 4;
                memmove(&buf[st], &buf[pos + 1], strlen(buf) - pos);
                pos = st - 1;
                break;
            case 'e':
                gcase = 5;
                if (buf[pos + 1] == ' ') {
                    st = pos - ((dig != -1) + 1);
                    memmove(&buf[st], &buf[pos + 1], strlen(buf) + 2 - pos);
                } else {
                    st = pos - ((dig != -1) + 1);
                    memmove(&buf[st], &buf[pos + 2], strlen(buf) + 2 - pos);
                }
                pos = st - 1;
                break;
            case 'E':
                gcase = 6;
                if (buf[pos + 1] == ' ') {
                    st = pos - ((dig != -1) + 1);
                    memmove(&buf[st], &buf[pos + 1], strlen(buf) + 2 - pos);
                } else {
                    st = pos - ((dig != -1) + 1);
                    memmove(&buf[st], &buf[pos + 2], strlen(buf) + 2 - pos);
                }
                pos = st - 1;
                break;
            case 'N':
                /* "%<case>N<kind>" */
                pos++;
                if (dig != -1 && buf[pos - 3] == '%' && buf[pos - 2] != '$') {
                    io_CharacterName(va_arg(ap, int), dig, buf[pos] - '0', tmp);
                    memmove(&buf[pos - 4 + strlen(tmp)], &buf[pos], strlen(buf) + 2 - pos);
                    memcpy(&buf[pos - 3], tmp, strlen(tmp));
                }
                break;
            case 'a':
                /* a number (the call sites pass an int: main_Unk5384e0's "$LRunden APs:$R+%a$A") */
                if (buf[pos - 1] == '%') {
                    sprintf(tmp, "%i%c", va_arg(ap, int), 0x14);
                    memmove(&buf[pos - 1 + strlen(tmp)], &buf[pos + 1], strlen(buf) + 1 - pos);
                    memcpy(&buf[pos - 1], tmp, strlen(tmp));
                } else {
                    sprintf(tmp, "%c", 0x14);
                    memmove(&buf[pos - 1 + strlen(tmp)], &buf[pos + 1], strlen(buf) + 1 - pos);
                    memcpy(&buf[pos - 1], tmp, strlen(tmp));
                }
                break;
            case 'r':
                /* "{r<group><n>}": a random one of the n texts of txt_Random group 9 + group from this text on */
                if (code == 'r') {
                    memset(rnum, 0, sizeof(rnum));
                    k = 0;
                    v = buf[pos + 1] - '1';
                    if (v >= 0 && v <= 9)
                        k = v + 9;
                    len = pos + 3;
                    while (isdigit(buf[len++]))
                        ;
                    len = len - 4 - pos;
                    strncpy(rnum, &buf[pos + 3], len);
                    n = atoi(rnum);
                    for (i = (int)fmt; i < txt_Count; i++) {
                        if (txt_Random[i] == k)
                            break;
                    }
                    if (i >= txt_Count) {
                        err_Message("io_Text(): Missing Random-Text (e.g. {r1})...");
                        break;
                    }
                    p = txt_Ptrs[i + (unsigned short)util_Random(n)];
                    if (p == 0)
                        break;
                    memmove(&buf[pos - len + strlen(p) - 5], &buf[pos], strlen(buf) + len - pos + 5);
                    memcpy(&buf[pos - 1], p, strlen(p));
                }
                break;
            case 'D':
                gm_ClockToDate(va_arg(ap, struct GCLOCK *), date.b);
                sprintf(tmp, "%s %i", txt_Ptrs[0x4c + gm_Season((char *)date.b)], date.w[1]);
                memmove(&buf[pos + strlen(tmp) - 2], &buf[pos], strlen(buf) + 2 - pos);
                memcpy(&buf[pos - 1], tmp, strlen(tmp));
                break;
            case 'G':
                /* the argument is a good record: its byte 0 is the good (the text pairs from 0x48e), +5 a
                   variant name */
                pos++;
                good = va_arg(ap, unsigned char *);
                id = good[0] * 0xe + 0x48e;
                if (id > 0x4800) {
                    s = (char *)id;
                } else {
                    gender = txt_Random[id];
                    s = txt_Ptrs[id];
                }
                if (dig != -1) {
                    s = txt_Field(s, dig, 4);
                    pos--;
                }
                if (s == 0)
                    s = txt_Ptrs[id];
                if (good[5] != 0)
                    sprintf(goodname, "%s >%s<", s, good + 5);
                else
                    strcpy(goodname, s);
                if (gcase == 0) {
                    if (strlen(goodname) + pos > 0x17cb)
                        return -1;
                    if (dig != -1)
                        memmove(&buf[pos + strlen(goodname) - 3], &buf[pos], strlen(buf) + 2 - pos);
                    else
                        memmove(&buf[pos + strlen(goodname) - 2], &buf[pos], strlen(buf) + 2 - pos);
                    memcpy(&buf[pos - 2], goodname, strlen(goodname));
                    pos -= 2;
                    break;
                }
                if (gcase == 1 || gcase == 2) {
                    art = txt_Field(txt_Ptrs[gender + 0x3b], dig, 4);
                    sprintf(tmp, "%s %s", art, goodname);
                    if (ls_GetLanguage() == 0 && gcase == 2)
                        tmp[0] = toupper(tmp[0]);
                }
                if (gcase == 3 || gcase == 4) {
                    art = txt_Field(txt_Ptrs[gender + 0x38], dig, 4);
                    sprintf(tmp, "%s %s", art, goodname);
                    if (ls_GetLanguage() == 0 && gcase == 4)
                        tmp[0] = toupper(tmp[0]);
                }
                if (gcase == 5 || gcase == 6) {
                    art = txt_Field(txt_Ptrs[gender + 0x3e], dig, 4);
                    sprintf(tmp, "%s %s", art, goodname);
                    if (ls_GetLanguage() == 0 && gcase == 6)
                        tmp[0] = toupper(tmp[0]);
                }
                if (strlen(tmp) + pos > 0x17cb)
                    return -1;
                if (dig != -1)
                    memmove(&buf[pos + strlen(tmp) - 3], &buf[pos], strlen(buf) + 2 - pos);
                else
                    memmove(&buf[pos + strlen(tmp) - 2], &buf[pos], strlen(buf) + 2 - pos);
                gcase = 0;
                memcpy(&buf[pos - 2], tmp, strlen(tmp));
                pos -= 2;
                break;
            case '%':
                buf[pos - 1] = 0x16;
                memmove(&buf[pos], &buf[pos + 1], strlen(buf) + 1 - pos);
                break;
            }
        }
    }
    if (strlen(d2_Wins[d2_LastWindow].text) + strlen(buf) > 0x17cf)
        return -1;
    strcat(d2_Wins[d2_LastWindow].text, buf);
    d2_Wins[d2_LastWindow].drawFrames = 2;
    ret = d2_Wins[d2_LastWindow].nobjs;
    if (redraw && !drawall) {
        saved = d2_LayoutMode;
        d2_LayoutMode = 1;
        ctrl = D2_OBJS[d2_Wins[d2_LastWindow].parent].buf;
        if (ctrl != 0) {
            if (ctrl->loaded != 0)
                d2_RedrawView(ctrl->view);
            else
                d2_DrawWindow(d2_LastWindow, d2_ScreenSurf);
        } else {
            d2_DrawWindow(d2_LastWindow, d2_ScreenSurf);
        }
        d2_LayoutMode = saved;
        return ret;
    }
    if (drawall)
        d2_DrawAll();
    return ret;
}
/* 0x5176d0, the text engine of io_PrintText into the buffer out instead of a window: expands the text id0 (an id of txt_Ptrs below 0x4800, else a literal) with the arguments of its tokens and copies the result to out (nothing when the texts are not loaded or the text is missing / too long). Its own token set: "%s" / "%S" a text id (or string) in its grammar field dig, with the article of "%u" / "%U" / "%b" / "%B" / "%e" / "%E" (gcase 1 .. 6) and a number before the token moved between them, "%G" a good's name (and ">variant<"), "%N" a character name, "%T" an amount, "%D" a clock as "<season> <year>", "%I" a number with a '.' between the thousands ("%NI" zero padded; negative numbers keep their sign), "%i" a number without dots ("%Ni" zero padded), "%f" a double ("%Nf<p>", p decimals, default none), "%a" a number followed by 0x14, "%w" the city name of a player, "{r<group><n>}" a random text of the group, "%v" / "%vv" skips an argument, "%%" the char 0x16, "$C" clears out. A count other than 1 ("%i" / "%I") makes the next "%s" plural. The window tokens "%ii" / "%ic" / "%ib" / "%ia" / "%in" are skipped without their argument; every other command stays in the text. @deep rebuilt from the original 0x5176d0..0x518f5a (all 21 case bodies via jump table 0x518f5c / index table 0x518fb4, read against the disassembly): its own case set (no l/t/m/W/X; window tokens %ii/%ic/%ib/%ia/%in skipped without an argument), 'i' = plain number with the original's literal "%0%ii" for %Ni, 'I' = dot grouping with the sign kept, plural = 1, 'f' default 0 decimals, b without and B with the '%' check, e/E like u, G with toklen 2/3 and its own article blocks (digits4..6), S/G tails with strlen(buf)+2-pos, 'C' clears out, frame sizes from the slots (goodname 0x80, num 0x20). ours 1913 vs 1906 instructions, same case order. Left: (1) allocation: the original keeps dig in esi and toklen/gcase/s in slots [esp+0x14]/[0x1c]/[0x20], ours dig in edi (spilled to [esp+0x18] at the dig parse: '- mov [esp+0x18],edi / - mov edi,[esp+0x18]' vs '+ or esi,-1'), toklen in esi in G ('- sub edx,esi' vs '+ mov ecx,[esp+0x14] / + sub edx,ecx'); (2) cross-jumping: the original's S gcase==0 tail jumps into G's tail (0x51790f jmp 0x518a26), 'a' else into T's memmove (0x518843 jmp 0x51815f), ours the other way round (T jumps into a's) and own tails; the I non-dig sprintf pushes merge differently; (3) slots: the loop counters k/i of the dot grouping swap [esp+0x10]/[esp+0x14]. */
void txt_Format(char *out, int id0, ...)
{
    char buf[0x17d0];                   /* the text being expanded */
    char tmp[0x17d0];                   /* the substitution */
    char fmtbuf[0x20];                  /* a sprintf format built at run time, or the plain digits of "%I" */
    char num[0x20];                     /* _itoa digits */
    union {
        unsigned char b[0xc];
        short w[6];                     /* w[1]: the year */
    } date;                             /* gm_ClockToDate's calendar record */
    char rnum[0x20];                    /* the count of a "{r}" token */
    char goodname[0x80];
    char digits1[0x40], digits2[0x40], digits3[0x40];   /* "%s": the number before the token */
    char digits4[0x40], digits5[0x40], digits6[0x40];   /* "%G": the number before the token */
    char sign[0x100];                   /* "%I": the grouped number with its sign */
    va_list ap;
    int pos, dig, st, toklen, i, k, n, len, v, id;
    int gcase, gender, plural;
    char code, c;
    char *s, *art, *p;
    unsigned char *good;

    gcase = 0;
    gender = 0;
    plural = 0;
    if (txt_Loaded == 0)
        return;
    va_start(ap, id0);
    memset(buf, 0, sizeof(buf));
    if (id0 < 0x4800) {
        s = txt_Ptrs[id0];
        if (s == 0)
            return;
        if (strlen(s) > 0x17c0)
            return;
        strncpy(buf, s, 0x17c0);
    } else {
        strcpy(buf, (char *)id0);
    }
    for (pos = 0; pos < strlen(buf) && pos <= 0x17d0; pos++) {
        while (buf[pos] == '%' || buf[pos] == '$' || buf[pos] == '{') {
            pos++;
            dig = buf[pos] - '0';
            if (dig >= 0 && dig <= 9)
                pos++;
            else
                dig = -1;
            code = buf[pos];
            switch (code) {
            case 'C':
                pos++;
                memset(out, 0, 0x80);
                break;
            case 'S':
            case 's':
                pos++;
                id = va_arg(ap, int);
                if (id > 0x4800) {
                    s = (char *)id;
                } else {
                    gender = txt_Random[id];
                    s = txt_Ptrs[id];
                }
                toklen = (dig != -1) + 2;
                if (gender != 0xff) {
                    if (dig == -1)
                        dig = 1;
                    if (buf[pos - 1] == 's' && plural == 0)
                        s = txt_Field(s, dig, 4);
                    else
                        s = txt_Field(s, dig, 8);
                } else if (s == 0) {
                    s = txt_Ptrs[id];
                }
                if (gcase == 0) {
                    if (strlen(s) + pos > 0x17cb)
                        return;
                    st = pos - toklen;
                    memmove(&buf[st + strlen(s)], &buf[pos], strlen(buf) + 2 - pos);
                    memcpy(&buf[st], s, strlen(s));
                    pos = st;
                    break;
                }
                /* a number right before the token (only blanks between) goes between the article and the word */
                for (i = pos - toklen - 1; i >= 0; i--) {
                    if (isdigit(buf[i]))
                        break;
                    if (buf[i] != ' ') {
                        i = -1;
                        break;
                    }
                }
                if (gcase == 1 || gcase == 2) {
                    if (buf[pos - 1] == 's' && plural == 0)
                        art = txt_Field(txt_Ptrs[gender + 0x3b], dig, 4);
                    else
                        art = txt_Field(txt_Ptrs[gender + 0x3b], dig, 8);
                    if (i <= -1) {
                        sprintf(tmp, "%s %s", art, s);
                    } else {
                        n = io_TrailingDigits(buf, i, digits1);
                        sprintf(tmp, "%s %s %s", art, digits1, s);
                        toklen = pos - n;
                    }
                    if (ls_GetLanguage() == 0 && gcase == 2)
                        tmp[0] = toupper(tmp[0]);
                }
                if (gcase == 3 || gcase == 4) {
                    if (buf[pos - 1] == 's' && plural == 0)
                        art = txt_Field(txt_Ptrs[gender + 0x38], dig, 4);
                    else
                        art = txt_Field(txt_Ptrs[gender + 0x38], dig, 8);
                    if (i <= -1) {
                        sprintf(tmp, "%s %s", art, s);
                    } else {
                        n = io_TrailingDigits(buf, i, digits2);
                        sprintf(tmp, "%s %s %s", art, digits2, s);
                        toklen = pos - n;
                    }
                    if (ls_GetLanguage() == 0 && gcase == 4)
                        tmp[0] = toupper(tmp[0]);
                }
                if (gcase == 5 || gcase == 6) {
                    if (buf[pos - 1] == 's' && plural == 0)
                        art = txt_Field(txt_Ptrs[gender + 0x3e], dig, 4);
                    else
                        art = txt_Field(txt_Ptrs[gender + 0x3e], dig, 8);
                    if (i <= -1) {
                        sprintf(tmp, "%s %s", art, s);
                    } else {
                        n = io_TrailingDigits(buf, i, digits3);
                        sprintf(tmp, "%s %s %s", art, digits3, s);
                        toklen = pos - n;
                    }
                    if (ls_GetLanguage() == 0 && gcase == 6)
                        tmp[0] = toupper(tmp[0]);
                }
                if (strlen(tmp) + pos > 0x17cb)
                    return;
                st = pos - toklen;
                memmove(&buf[st + strlen(tmp)], &buf[pos], strlen(buf) + 2 - pos);
                memcpy(&buf[st], tmp, strlen(tmp));
                gcase = 0;
                plural = 0;
                pos = st;
                break;
            case 'i':
                c = buf[pos + 1];
                if (c == 'i' || c == 'c' || c == 'b' || c == 'a' || c == 'n') {
                    /* a window token of io_PrintText: no window here */
                    pos += 2;
                    break;
                }
                if (dig != -1) {
                    v = va_arg(ap, int);
                    if (v > 1 || v == 0)
                        plural = 1;
                    sprintf(fmtbuf, "%0%ii", dig);    /* as the original: "%0%" is a '%', the format becomes "%ii" */
                    sprintf(tmp, fmtbuf, v);
                    memmove(&buf[pos + strlen(tmp) - 3], &buf[pos], strlen(buf) + 1 - pos);
                    memcpy(&buf[pos - 2], tmp, strlen(tmp));
                } else {
                    v = va_arg(ap, int);
                    if (v > 1 || v == 0)
                        plural = 1;
                    sprintf(tmp, "%i", v);
                    memmove(&buf[pos - 1 + strlen(tmp)], &buf[pos + 1], strlen(buf) + 1 - pos);
                    memcpy(&buf[pos - 1], tmp, strlen(tmp));
                }
                break;
            case 'I':
                if (dig != -1) {
                    /* zero padded to dig places, then the dots: tmp -> fmtbuf backwards */
                    _itoa(dig, num, 10);
                    strcpy(fmtbuf, "%0");
                    strcat(fmtbuf, num);
                    strcat(fmtbuf, "i");
                    v = va_arg(ap, int);
                    if (v > 1 || v == 0)
                        plural = 1;
                    sprintf(tmp, fmtbuf, v);
                    k = -1;
                    len = strlen(tmp) / 3 + strlen(tmp);
                    n = len;
                    for (i = strlen(tmp); i >= 0; i--) {
                        fmtbuf[n--] = tmp[i];
                        if (i != 0 && k > 0 && (k + 1) % 3 == 0)
                            fmtbuf[n--] = '.';
                        k++;
                    }
                    fmtbuf[len] = 0;
                    memmove(&buf[pos + strlen(fmtbuf) - 3], &buf[pos], strlen(buf) + 1 - pos);
                    memcpy(&buf[pos - 2], fmtbuf, strlen(fmtbuf));
                } else {
                    /* the digits of |v| into fmtbuf, then with the dots: fmtbuf -> tmp backwards, the sign in
                       front */
                    v = va_arg(ap, int);
                    if (v > 1 || v == 0)
                        plural = 1;
                    if (v < 0)
                        sprintf(fmtbuf, "%i", -v);
                    else
                        sprintf(fmtbuf, "%i", v);
                    k = -1;
                    len = (strlen(fmtbuf) - 1) / 3 + strlen(fmtbuf);
                    n = len;
                    for (i = strlen(fmtbuf); i >= 0; i--) {
                        tmp[n--] = fmtbuf[i];
                        if (i != 0 && k > 0 && (k + 1) % 3 == 0)
                            tmp[n--] = '.';
                        k++;
                    }
                    tmp[len] = 0;
                    if (v >= 0) {
                        strcpy(sign, tmp);
                    } else {
                        strcpy(sign + 1, tmp);
                        sign[0] = '-';
                    }
                    memmove(&buf[pos - 1 + strlen(sign)], &buf[pos + 1], strlen(buf) + 1 - pos);
                    memcpy(&buf[pos - 1], sign, strlen(sign));
                }
                break;
            case 'T':
                if (buf[pos - 1] == '%') {
                    gm_FormatAmount(va_arg(ap, int), ai_OwnPlayer, tmp);
                    memmove(&buf[pos - 1 + strlen(tmp)], &buf[pos + 1], strlen(buf) + 1 - pos);
                    memcpy(&buf[pos - 1], tmp, strlen(tmp));
                }
                break;
            case 'w':
                /* the argument is a player index: the city id (word +0x54) of its player slot picks the city
                   name (the text pairs from txt_Ptrs[0x9a8]) */
                strcpy(tmp, txt_Field(txt_Ptrs[0x9a8 + ai_PlayerCity[va_arg(ap, int)][0] * 2], 1, 8));
                memmove(&buf[pos - 1 + strlen(tmp)], &buf[pos + 1], strlen(buf) + 1 - pos);
                memcpy(&buf[pos - 1], tmp, strlen(tmp));
                break;
            case 'f':
                /* "%f<p>" / "%<w>f<p>": p decimals (1 .. 8, else 0), w the zero padded width */
                k = buf[pos + 1] - '0';
                if (k > 0 && k < 9)
                    pos++;
                else
                    k = 0;
                if (dig != -1) {
                    _itoa(dig, num, 10);
                    strcpy(fmtbuf, "%0");
                    strcat(fmtbuf, num);
                    _itoa(k, num, 10);
                    strcat(fmtbuf, ".");
                    strcat(fmtbuf, num);
                    strcat(fmtbuf, "f");
                    sprintf(tmp, fmtbuf, va_arg(ap, double));
                    if (strlen(tmp) + pos > 0x17cb)
                        return;
                    pos++;
                    memmove(&buf[pos - 4 + strlen(tmp)], &buf[pos], strlen(buf) + 1 - pos);
                    memcpy(&buf[pos - 4], tmp, strlen(tmp));
                } else {
                    _itoa(k, num, 10);
                    strcpy(fmtbuf, "%.");
                    strcat(fmtbuf, num);
                    strcat(fmtbuf, "f");
                    sprintf(tmp, fmtbuf, va_arg(ap, double));
                    if (strlen(tmp) + pos > 0x17cb)
                        return;
                    pos++;
                    memmove(&buf[pos - 3 + strlen(tmp)], &buf[pos], strlen(buf) + 1 - pos);
                    memcpy(&buf[pos - 3], tmp, strlen(tmp));
                }
                break;
            case 'v':
                pos++;
                if (buf[pos] == 'v')
                    va_arg(ap, int);
                break;
            case 'F':
                pos++;
                break;
            case 'u':
                gcase = 1;
                st = pos - ((dig != -1) + 1);
                memmove(&buf[st], &buf[pos + 1], strlen(buf) - pos);
                pos = st - 1;
                break;
            case 'U':
                gcase = 2;
                st = pos - ((dig != -1) + 1);
                memmove(&buf[st], &buf[pos + 1], strlen(buf) - pos);
                pos = st - 1;
                break;
            case 'b':
                gcase = 3;
                st = pos - ((dig != -1) + 1);
                memmove(&buf[st], &buf[pos + 1], strlen(buf) - pos);
                pos = st - 1;
                break;
            case 'B':
                st = pos - ((dig != -1) + 1);
                if (buf[st] != '%') {
                    pos++;
                    break;
                }
                gcase = 4;
                memmove(&buf[st], &buf[pos + 1], strlen(buf) - pos);
                pos = st - 1;
                break;
            case 'e':
                gcase = 5;
                st = pos - ((dig != -1) + 1);
                memmove(&buf[st], &buf[pos + 1], strlen(buf) - pos);
                pos = st - 1;
                break;
            case 'E':
                gcase = 6;
                st = pos - ((dig != -1) + 1);
                memmove(&buf[st], &buf[pos + 1], strlen(buf) - pos);
                pos = st - 1;
                break;
            case 'N':
                /* "%<case>N<kind>" */
                pos++;
                if (dig != -1 && buf[pos - 3] == '%' && buf[pos - 2] != '$') {
                    io_CharacterName(va_arg(ap, int), dig, buf[pos] - '0', tmp);
                    memmove(&buf[pos - 4 + strlen(tmp)], &buf[pos], strlen(buf) + 2 - pos);
                    memcpy(&buf[pos - 3], tmp, strlen(tmp));
                }
                break;
            case 'a':
                if (buf[pos - 1] == '%') {
                    sprintf(tmp, "%i%c", va_arg(ap, int), 0x14);
                    memmove(&buf[pos - 1 + strlen(tmp)], &buf[pos + 1], strlen(buf) + 1 - pos);
                    memcpy(&buf[pos - 1], tmp, strlen(tmp));
                } else {
                    sprintf(tmp, "%c", 0x14);
                    memmove(&buf[pos - 1 + strlen(tmp)], &buf[pos + 1], strlen(buf) + 1 - pos);
                    memcpy(&buf[pos - 1], tmp, strlen(tmp));
                }
                break;
            case 'D':
                gm_ClockToDate(va_arg(ap, struct GCLOCK *), date.b);
                sprintf(tmp, "%s %i", txt_Ptrs[0x4c + gm_Season((char *)date.b)], date.w[1]);
                memmove(&buf[pos + strlen(tmp) - 2], &buf[pos], strlen(buf) + 2 - pos);
                memcpy(&buf[pos - 1], tmp, strlen(tmp));
                break;
            case 'G':
                /* the argument is a good record: its byte 0 is the good (the text pairs from 0x48e), +5 a
                   variant name */
                pos++;
                good = va_arg(ap, unsigned char *);
                id = good[0] * 0xe + 0x48e;
                if (id > 0x4800) {
                    s = (char *)id;
                } else {
                    gender = txt_Random[id];
                    s = txt_Ptrs[id];
                }
                if (dig == -1) {
                    toklen = 2;
                    dig = 1;
                } else {
                    toklen = 3;
                }
                s = txt_Field(s, dig, 4);
                if (s == 0)
                    s = txt_Ptrs[id];
                if (good[5] != 0)
                    sprintf(goodname, "%s >%s<", s, good + 5);
                else
                    strcpy(goodname, s);
                if (gcase == 0) {
                    if (strlen(goodname) + pos > 0x17cb)
                        return;
                    st = pos - toklen;
                    memmove(&buf[st + strlen(goodname)], &buf[pos], strlen(buf) + 2 - pos);
                    memcpy(&buf[st], goodname, strlen(goodname));
                    pos = st;
                    break;
                }
                for (i = pos - toklen - 1; i >= 0; i--) {
                    if (isdigit(buf[i]))
                        break;
                    if (buf[i] != ' ') {
                        i = -1;
                        break;
                    }
                }
                if (gcase == 1 || gcase == 2) {
                    art = txt_Field(txt_Ptrs[gender + 0x3b], dig, 4);
                    if (i <= -1) {
                        sprintf(tmp, "%s %s", art, goodname);
                    } else {
                        n = io_TrailingDigits(buf, i, digits4);
                        sprintf(tmp, "%s %s %s", art, digits4, goodname);
                        toklen = pos - n;
                    }
                    if (ls_GetLanguage() == 0 && gcase == 2)
                        tmp[0] = toupper(tmp[0]);
                }
                if (gcase == 3 || gcase == 4) {
                    art = txt_Field(txt_Ptrs[gender + 0x38], dig, 4);
                    if (i <= -1) {
                        sprintf(tmp, "%s %s", art, goodname);
                    } else {
                        n = io_TrailingDigits(buf, i, digits5);
                        sprintf(tmp, "%s %s %s", art, digits5, goodname);
                        toklen = pos - n;
                    }
                    if (ls_GetLanguage() == 0 && gcase == 4)
                        tmp[0] = toupper(tmp[0]);
                }
                if (gcase == 5 || gcase == 6) {
                    art = txt_Field(txt_Ptrs[gender + 0x3e], dig, 4);
                    if (i <= -1) {
                        sprintf(tmp, "%s %s", art, goodname);
                    } else {
                        n = io_TrailingDigits(buf, i, digits6);
                        sprintf(tmp, "%s %s %s", art, digits6, goodname);
                        toklen = pos - n;
                    }
                    if (ls_GetLanguage() == 0 && gcase == 6)
                        tmp[0] = toupper(tmp[0]);
                }
                if (strlen(tmp) + pos > 0x17cb)
                    return;
                st = pos - toklen;
                memmove(&buf[st + strlen(tmp)], &buf[pos], strlen(buf) + 2 - pos);
                memcpy(&buf[st], tmp, strlen(tmp));
                gcase = 0;
                pos = st;
                break;
            case 'r':
                /* "{r<group><n>}": a random one of the n texts of txt_Random group 9 + group from this text on */
                if (code == 'r') {
                    memset(rnum, 0, sizeof(rnum));
                    k = 0;
                    v = buf[pos + 1] - '1';
                    if (v >= 0 && v <= 9)
                        k = v + 9;
                    len = pos + 3;
                    while (isdigit(buf[len++]))
                        ;
                    len = len - 4 - pos;
                    strncpy(rnum, &buf[pos + 3], len);
                    n = atoi(rnum);
                    for (i = id0; i < txt_Count; i++) {
                        if (txt_Random[i] == k)
                            break;
                    }
                    if (i >= txt_Count) {
                        err_Message("io_Text(): Missing Random-Text (e.g. {r1})...");
                        break;
                    }
                    p = txt_Ptrs[i + (unsigned short)util_Random(n)];
                    if (p == 0)
                        break;
                    memmove(&buf[pos - len + strlen(p) - 5], &buf[pos], strlen(buf) + len - pos + 5);
                    memcpy(&buf[pos - 1], p, strlen(p));
                }
                break;
            case '%':
                buf[pos - 1] = 0x16;
                memmove(&buf[pos], &buf[pos + 1], strlen(buf) + 1 - pos);
                break;
            }
        }
    }
    strcpy(out, buf);
}

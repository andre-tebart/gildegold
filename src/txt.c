/* txt.c -- original file name unknown ("txt_ParseRandomText", "txt_OpenText" strings).
   Text definition files: a definition file includes several text files whose lines are split into
   key/text fields and copied into one text pool. Extent in GildeGold.exe (inferred): starts at 0x42c3b0
   after the mss32 import thunk that ends sfx.c. Functions are kept in their original order.
   Names are ours except the two leaked ones.
   Open: 0x114fe60 has two names - txt.h's txt_Random (no definition) and io_text.c's ai_Unk114fe60 (the
   definition); one object, one owner to settle (io_text by the data layout).
   unit of rea 8cef7d5b (src/game/txt.c); boundaries in decl/text.dcl */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "txt.h"

/* 0x42c3b0, returns the n-th of the NUL-separated fields at p. */
char *txt_Field(char *p, int n, char type)
{
    int i = 0;
    char *s = p;
    char *res;
    int k;

    if (type == 8) {
        n += 4;
        res = p;
    }
    while (i < n - 1) {
        k = 0;
        while (s[k++])
            ;
        s = s + k - 1;
        if (!s)
            goto ret_p;
        s++;
        i++;
        if (i == 4)
            res = s;
        if (i == 6 && *s)
            res = s;
    }
    if (!*s) {
        if (type == 4) {
ret_p:
            return p;
        }
        if (type == 8)
            s = res;
    }
    return s;
}
/* 0x42c430, returns the first non-space character of s, or NULL when s is empty or all spaces */

char *txt_SkipSpaces(char *s)
{
    int i;

    for (i = 0; i < strlen(s);)
        if (s[i++] != ' ')
            break;
    if (i == strlen(s))
        return NULL;
    return &s[i - 1];
}
/* 0x42c480, truncates s before its trailing spaces (the last character is left alone) */

void txt_TrimRight(char *s)
{
    int i = strlen(s) - 1;

    if (s[i] == ' ' && i > 0) {
        while (i > 0) {
            if (s[i] != ' ') {
                s[i + 1] = 0;
                return;
            }
            i--;
        }
    }
}
/* 0x42c4b0, one line of a text file: "(m|w|s)", then up to four texts of the form
   prefix[alt1/alt2]suffix (or prefix[alt1]alt2]suffix); appends both variants of each to the pool. */

int txt_ParseLine(char *line, int group)
{
    unsigned char i;
    unsigned char mode;
    char suffix[128];
    char alt2[128];
    char alt1[128];
    char prefix[128];
    char tmp[256];
    char out1[4][160];
    char out2[4][160];
    char *p;
    char *q;
    char *r;
    int k;

    memset(out1, 0, sizeof(out1));
    memset(out2, 0, sizeof(out2));
    memset(alt1, 0, sizeof(alt1));
    memset(alt2, 0, sizeof(alt2));
    mode = 0;
    i = 0;
    suffix[0] = 0;
    prefix[0] = 0;
    p = strtok(line, ",\";\n");
    while (p) {
        if (i == 0) {
            strcpy(tmp, p);
            p = txt_SkipSpaces(tmp);
            txt_TrimRight(p);
            if (!_strcmpi(p, "(m)"))
                mode = 0;
            else if (!_strcmpi(p, "(w)"))
                mode = 1;
            else if (!_strcmpi(p, "(s)"))
                mode = 2;
        } else {
            p = txt_SkipSpaces(p);
            if (p) {
                txt_TrimRight(p);
                q = strchr(p, '[');
                if (!q)
                    strcpy(prefix, p);
                else {
                    if (q != p) {
                        memset(prefix, 0, sizeof(prefix));
                        strncpy(prefix, p, q - p);
                    }
                    r = strchr(q, '/');
                    if (r) {
                        memset(alt1, 0, sizeof(alt1));
                        q++;
                        strncpy(alt1, q, r - q - 1);
                        q = strchr(r, ']');
                        if (!q)
                            return 0;
                        memset(alt2, 0, sizeof(alt2));
                        r++;
                        strncpy(alt2, r, q - r - 1);
                        if (strlen(q + 1)) {
                            memset(suffix, 0, sizeof(suffix));
                            strcpy(suffix, q + 1);
                        }
                    } else {
                        r = strchr(q, ']');
                        if (!r)
                            return 0;
                        q++;
                        memset(alt1, 0, sizeof(alt1));
                        strncpy(alt1, q, r - q - 1);
                        memset(alt2, 0, sizeof(alt2));
                        q++;
                        strncpy(alt2, q, r - q - 1);
                        if (strlen(r + 1)) {
                            memset(suffix, 0, sizeof(suffix));
                            strcpy(suffix, r + 1);
                        }
                    }
                }
                sprintf(out1[i - 1], "%s%s%s", prefix, alt1, suffix);
                sprintf(out2[i - 1], "%s%s%s", prefix, alt2, suffix);
            }
        }
        p = strtok(NULL, ",\";\n");
        i++;
    }
    for (k = 0; k < 4; k++) {
        strcat(out1[k], "|");
        strcat(txt_Buffer + txt_Count * 0x1000, out1[k]);
        txt_Size += strlen(out1[k]);
    }
    for (k = 0; k < 4; k++) {
        strcat(out2[k], "|");
        strcat(txt_Buffer + txt_Count * 0x1000, out2[k]);
        txt_Size += strlen(out2[k]);
    }
    txt_Random[txt_Count] = mode;
    sprintf(txt_Keys[txt_Count], "%s+%i", txt_Names[group - 1], txt_Count - txt_GroupFirst[group - 1]);
    txt_Size++;
    txt_Count++;
    return 1;
}
/* 0x42c950, reads a "{rN}" random-text escape from p: the chosen variant (N + 9) is queued in
   txt_Random; on a syntax error it reports and queues the pointer itself.
   @deep behaviour only - grade.py code 18, fill 8; unitdiff 25 B / 18 asm lines (2026-10-26): the four stores and the branch structure are identical, the hunks differ in that the original reloads p from its home slot ([esp+0xc]/[esp+8]) where ours keeps it in esi, i.e. register allocation; readable met; reversed if polarity (2026-10-26, the store-p arm first) is 52 B / 24 lines, WORSE */

void txt_ParseRandomText(char *p)
{
    int n;

    if (!strncmp(p, "{r", 2)) {
        n = p[2] - '1';
        if (n >= 0 && n <= 9) {
            txt_Random[txt_Count] = n + 9;
            return;
        }
        err_Message("txt_ParseRandomText(): Syntax error...");
        txt_Random[txt_Count] = (int)p;
    } else
        txt_Random[txt_Count] = (int)p;
}
/* 0x42c9c0, reads the #include lines of a text definition file and loads each included file. */

int txt_LoadIncludes(char *file)
{
    FILE *f;
    char line[200];
    char inc[128][512];
    int n = 0;
    int i;
    char *p;
    char *q;

    txt_Count = 0;
    p = strrchr(file, '\\');
    if (p) {
        memcpy(txt_Dir, file, p - file + 1);
        txt_Dir[p - file + 1] = 0;
    } else
        strcpy(txt_Dir, "");
    f = fopen(file, "rt");
    if (!f) {
        MessageBox(NULL, "Could not open Textfiledefinition!", "Error", MB_ICONEXCLAMATION);
        mem_Free(txt_Buffer);
        txt_Buffer = NULL;
        return 0;
    }
    do {
        memset(line, 0, sizeof(line));
        fgets(line, sizeof(line), f);
        if (!strstr(line, "//") && (p = strstr(line, "#include"))) {
            p = strchr(p, '"');
            if (!p) {
                mem_Free(txt_Buffer);
                txt_Buffer = NULL;
                return 0;
            }
            p++;
            q = strchr(p, '"');
            if (!q) {
                mem_Free(txt_Buffer);
                txt_Buffer = NULL;
                return 0;
            }
            *q = 0;
            *strrchr(p, '.') = 0;
            strcpy(inc[n++], p);
            if (n > 128) {
                mem_Free(txt_Buffer);
                txt_Buffer = NULL;
                return 0;
            }
        }
    } while (!feof(f));
    fclose(f);
    for (i = 0; i < n; i++)
        if (!txt_LoadTextFile(inc[i]))
            return 0;
    txt_Loaded = 1;
    return 1;
}
/* 0x42cbc0, compiles the text files named in a definition file into <outputpath>\<name>.res files
   and optionally writes a C header with one #define per group. @deep match-base 2026-10-31: read the whole function against the original; the 106 calls, every block and the switch tables are in the original order. What is left is the home of i: the original keeps i in memory at [esp+0x10] (cached in edx, stored at every definition, reloaded after the char store txt_Ptrs[i][k] = 0, final values 0x80 / 0x4800 stored after the two init loops), ours enregisters it per loop. With i forced into memory (experiment, &i used once, not committed): ours 1773 vs 1774, switch tables equal, 12 ret epilogues on both sides (the extra Could-not-open-TextFile tail is the zero register: ours push ebx / mov [txt_Buffer],ebx cross-jumps into the HeaderFile tail, the original uses immediates there), hunks left regs 13 / slots 19 / order 2 / code 32 (MessageBoxA in ebp vs ebp = 0, lineno in ebx vs memory). No natural source form found that exposes i without code: q = (char *)&i, *&i, an __inline helper taking &i all fold away before allocation; an out-of-line empty helper adds a call. Remaining besides: the dead memset(txt_Random,0) (VC6 drops it before the 0xff fill unless the two use different symbols: shown with ai_Unk114fe60 for one of them, which is the same object under the io_text name, rejected: one object one name) and the empty if (txt_Count == 0x71b) as cmp/jne +0 (tried i = i, end = i, col = col, p = p, txt_GroupFirst[ngroups] = txt_Count (kept as a second store), memset(&i,0,0) (emits xor ecx/eax)). (size: 1729 vs 1774 instructions, explained: i is memory-homed in the original: its loads / stores at [esp+0x10] (mov [esp+0x10],edx after each i++, mov edx,[esp+0x10] after calls and after the byte store) and the two final-value stores are the 45 instructions: forcing i into memory gives ours 1773 vs the original 1774 with the same 12 epilogues and equal tables.) */

int txt_OpenText(char *file, int header)
{
    int i;
    unsigned int col;
    int ngroups;
    int lineno;
    int comment;
    char *name;
    FILE *f;
    int fi;
    int off;
    int ninc;
    int first;
    int needsemi;
    int ndefs;
    int labelpos[10];
    char line[200];
    char num[20];
    char msg[256];
    char cur[128];
    char path[260];
    char labels[10][196];
    char defs[96][256];
    char inc[128][256];
    char *p;
    char *q;
    int len;
    unsigned int end;
    int k;
    int n;

    labelpos[0] = 0;
    labelpos[1] = 20;
    labelpos[2] = 40;
    labelpos[3] = 60;
    labelpos[4] = 80;
    labelpos[5] = 100;
    labelpos[6] = 120;
    labelpos[7] = 140;
    labelpos[8] = 160;
    labelpos[9] = 180;
    ngroups = 0;
    first = 0;
    comment = 0;
    ndefs = 0;
    needsemi = 0;
    ninc = 0;
    txt_Count = 0;
    for (i = 0; i < 0x80; i++)
        txt_Files[i].buf = NULL;
    txt_Buffer = mem_Alloc(0x4800000, "txt_txt_t:TextArray");
    if (!txt_Buffer) {
        MessageBox(NULL, "Konnte speicher nicht reservieren!", "txt_OpenText", MB_ICONEXCLAMATION);
        return 0;
    }
    memset(txt_Names, 0, sizeof(txt_Names));
    memset(txt_Buffer, 0, 0x4800000);
    memset(txt_Keys, 0, sizeof(txt_Keys));
    memset(txt_GroupIds, 0, sizeof(txt_GroupIds));
    memset(txt_Ptrs, 0, sizeof(txt_Ptrs));
    memset(txt_Files, 0, sizeof(txt_Files));
    memset(txt_Random, 0, sizeof(txt_Random));
    for (i = 0; i < 0x4800; i++)
        txt_Random[i] = 0xff;
    txt_Count = 0;
    txt_Size = 0;
    p = strrchr(file, '\\');
    if (p) {
        memset(txt_BinPath, 0, sizeof(txt_BinPath));
        memset(path, 0, sizeof(path));
        memcpy(path, file, p - file + 1);
        strcpy(txt_Dir, path);
        strcat(txt_Dir, "text\\");
        strcpy(txt_BinPath, file);
        p = strrchr(txt_BinPath, '\\');
        if (p)
            *p = 0;
        p = strrchr(txt_BinPath, '\\');
        if (p)
            *p = 0;
        sprintf(path, "%s\\resources\\textbin_%s\\", "german");
        strcpy(txt_BinPath, path);
    } else {
        strcpy(txt_Dir, "text\\");
        strcpy(txt_BinPath, "\\project\\DieGilde\\gfx\\textbin\\");
    }
    f = fopen(file, "rt");
    if (!f) {
        MessageBox(NULL, "Could not open Textfiledefinition!", "Error", MB_ICONEXCLAMATION);
        mem_Free(txt_Buffer);
        txt_Buffer = NULL;
        return 0;
    }
    do {
        memset(line, 0, sizeof(line));
        fgets(line, sizeof(line), f);
        if (!strstr(line, "//")) {
            if (p = strstr(line, "#outputpath")) {
                p = strchr(p, '"');
                if (!p) {
                    mem_Free(txt_Buffer);
                    txt_Buffer = NULL;
                    return 0;
                }
                p++;
                q = strchr(p, '"');
                if (!q) {
                    mem_Free(txt_Buffer);
                    txt_Buffer = NULL;
                    return 0;
                }
                *q = 0;
                strcpy(txt_BinPath, p);
            }
            if (p = strstr(line, "#headerfile")) {
                p = strchr(p, '"');
                if (!p) {
                    mem_Free(txt_Buffer);
                    txt_Buffer = NULL;
                    return 0;
                }
                p++;
                q = strchr(p, '"');
                if (!q) {
                    mem_Free(txt_Buffer);
                    txt_Buffer = NULL;
                    return 0;
                }
                *q = 0;
                strcpy(txt_HeaderName, p);
            }
            if (p = strstr(line, "#include")) {
                p = strchr(p, '"');
                if (!p) {
                    mem_Free(txt_Buffer);
                    txt_Buffer = NULL;
                    return 0;
                }
                p++;
                q = strchr(p, '"');
                if (!q) {
                    mem_Free(txt_Buffer);
                    txt_Buffer = NULL;
                    return 0;
                }
                *q = 0;
                strcpy(inc[ninc++], p);
                if (ninc > 128) {
                    mem_Free(txt_Buffer);
                    txt_Buffer = NULL;
                    return 0;
                }
            }
        }
    } while (!feof(f));
    fclose(f);

    for (fi = 0; fi < ninc; fi++) {
        strcpy(line, txt_Dir);
        name = inc[fi];
        strcat(line, name);
        f = fopen(line, "rt");
        if (!f) {
            sprintf(msg, "Could not open TextFile: '%s' !", line);
            MessageBox(NULL, msg, "Error", MB_ICONEXCLAMATION);
            mem_Free(txt_Buffer);
            txt_Buffer = NULL;
            return 0;
        }
        lineno = 0;
        do {
            memset(line, 0, sizeof(line));
            fgets(line, sizeof(line), f);
            if (!strcmp(line, ""))
                break;
            lineno++;
            i = 0;
            if (strstr(line, "#define")) {
                line[strlen(line) - 1] = 0;
                sprintf(defs[ndefs++], "%s\n", line);
                if (ndefs > 0x60)
                    MessageBox(NULL, "To many defines!", "Error", MB_ICONEXCLAMATION);
                i = strlen(line);
            }
            if (comment) {
                if (!strchr(line, '"') && strstr(line, "*/"))
                    comment = 0;
                i = strlen(line);
            }
            while (line[i++] == ' ')
                ;
            i--;
            col = i;
            if (i < strlen(line)) {
                do {
                    switch (line[i]) {
                    case '\t':
                        col += 4;
                        break;
                    case '(':
                        if (!txt_ParseLine(&line[i], ngroups)) {
                            sprintf(msg, "Syntax error in file %s in line %i", name, lineno);
                            MessageBox(NULL, msg, "Error in textfile", MB_ICONEXCLAMATION);
                        }
                        i = strlen(line);
                        break;
                    case '{':
                        txt_ParseRandomText(&line[i]);
                        i += 4;
                        break;
                    case '/':
                        if (line[i + 1] == '*') {
                            comment++;
                            if (strstr(&line[i + 1], "*/"))
                                comment--;
                        }
                        i = strlen(line);
                        break;
                    case '*':
                        if (line[i + 1] == '/')
                            comment--;
                        if (comment < 0)
                            sprintf(msg, "txt_TextOpen(): nested comments found. in file %s in line %i", name, lineno);
                        break;
                    case '_':
                        if (needsemi) {
                            sprintf(msg, "Syntax error in file %s in line %i: missing ';'", inc[fi], lineno);
                            MessageBox(NULL, msg, "txt_OpenText", MB_ICONEXCLAMATION);
                            mem_Free(txt_Buffer);
                            txt_Buffer = NULL;
                            fclose(f);
                            return 0;
                        }
                        txt_GroupFirst[ngroups] = txt_Count;
                        if (txt_Count == 0x71b)
                            ;
                        for (end = i; end < strlen(line); end++)
                            if (line[end] == '\n' || line[end] == ' ' || line[end] == '\t')
                                break;
                        memset(txt_GroupIds[ngroups], 0, sizeof(txt_GroupIds[0]));
                        strncpy(txt_GroupIds[ngroups], &line[i], end - i);
                        for (k = 0; k < 8; k++) {
                            if ((int)col <= labelpos[k]) {
                                txt_GroupLevel[ngroups] = k;
                                labelpos[k] = col;
                                memset(labels[k], 0, 128);
                                memset(txt_Names[ngroups], 0, sizeof(txt_Names[0]));
                                labelpos[k + 1] = col + 10;
                                if (k == 0) {
                                    strncpy(labels[0], &line[i], end - i);
                                    strcpy(cur, labels[0]);
                                    for (k = 1; k < 8; k++)
                                        labelpos[k] = col + k * 10;
                                } else {
                                    strcpy(labels[k], labels[k - 1]);
                                    strncat(labels[k], &line[i], end - i);
                                    strcpy(cur, labels[k - 1]);
                                    strcat(cur, labels[k]);
                                }
                                strcpy(txt_Names[ngroups], labels[k]);
                                ngroups++;
                                if (ngroups > 0x4800) {
                                    sprintf(msg, "Zuviele Label! Zeile:%i", lineno);
                                    MessageBox(NULL, msg, "txt_OpenText", MB_ICONEXCLAMATION);
                                    mem_Free(txt_Buffer);
                                    txt_Buffer = NULL;
                                    fclose(f);
                                    return 0;
                                }
                                col += end - i - 2;
                                break;
                            }
                        }
                        i = end - 2;
                        col += end - i - 2;
                        break;
                    case '"':
                        i++;
                        col++;
                        len = strchr(&line[i], '"') - &line[i];
                        if (len < 0) {
                            sprintf(msg, "Syntax error in file %s in line %i", name, lineno);
                            MessageBox(NULL, msg, "Error in textfile", MB_ICONEXCLAMATION);
                            break;
                        }
                        memset(msg, 0, sizeof(msg));
                        if (len > 0xff) {
                            sprintf(msg, "Line too long in file %s in line %i", name, lineno);
                            MessageBox(NULL, msg, "Error in textfile", MB_ICONEXCLAMATION);
                            break;
                        }
                        strncpy(msg, &line[i], len);
                        p = strstr(msg, "/*");
                        if (p) {
                            i += len + 1;
                            col += len + 1;
                            do {
                                q = strstr(msg, "*/");
                                if (q < p) {
                                    sprintf(msg, "Syntax error in file %s in line %i: Missing '/*'", name, lineno);
                                    MessageBox(NULL, msg, "Error in textfile", MB_ICONEXCLAMATION);
                                    break;
                                }
                                if (!q) {
                                    sprintf(msg, "Syntax error in file %s in line %i: Missing '*/'", name, lineno);
                                    MessageBox(NULL, msg, "Error in textfile", MB_ICONEXCLAMATION);
                                    break;
                                }
                                memmove(p, q + 2, strlen(q + 2) + 1);
                                p = strstr(msg, "/*");
                            } while (p);
                            n = strlen(msg);
                            if (strlen(txt_Buffer + txt_Count * 0x1000) + n > 0xfff) {
                                sprintf(msg, "String too long in file %s in line %i", inc[fi], lineno);
                                mem_Free(txt_Buffer);
                                txt_Buffer = NULL;
                                return 0;
                            }
                            strncat(txt_Buffer + txt_Count * 0x1000, msg, n);
                        } else {
                            n = len;
                            if (strlen(txt_Buffer + txt_Count * 0x1000) + n > 0xfff) {
                                sprintf(msg, "String too long in file %s in line %i", inc[fi], lineno);
                                mem_Free(txt_Buffer);
                                txt_Buffer = NULL;
                                return 0;
                            }
                            strncat(txt_Buffer + txt_Count * 0x1000, &line[i], n);
                            i += len + 1;
                            col += len + 1;
                        }
                        txt_Size += n;
                        needsemi = 1;
                        while (line[i] == ',') {
                            i += 2;
                            len = strchr(&line[i], '"') - &line[i];
                            if (len < 0) {
                                sprintf(msg, "Syntax error in file %s in line %i", name, lineno);
                                MessageBox(NULL, msg, "Error in textfile", MB_ICONEXCLAMATION);
                                i = strlen(line);
                                break;
                            }
                            msg[0] = '|';
                            strcpy(&msg[1], &line[i]);
                            if (strlen(txt_Buffer + txt_Count * 0x4800000) + strlen(msg) > 0xfff) {
                                sprintf(msg, "String too long in file %s in line %i", inc[fi], lineno);
                                mem_Free(txt_Buffer);
                                txt_Buffer = NULL;
                                return 0;
                            }
                            strncat(txt_Buffer + txt_Count * 0x1000, msg, strlen(msg));
                            i += len + 1;
                            col += len + 1;
                            txt_Size += strlen(msg);
                        }
                        i--;
                        col--;
                        break;
                    case ';':
                        strcpy(txt_Keys[txt_Count], txt_Names[ngroups - 1]);
                        strcat(txt_Keys[txt_Count], "+");
                        itoa(txt_Count - txt_GroupFirst[ngroups - 1], num, 10);
                        strcat(txt_Keys[txt_Count], num);
                        txt_Size++;
                        txt_Count++;
                        if (txt_Count > 0x4800) {
                            MessageBox(NULL, "Zuviele Textenums!", "txt_OpenText", MB_ICONEXCLAMATION);
                            mem_Free(txt_Buffer);
                            txt_Buffer = NULL;
                            fclose(f);
                            return 0;
                        }
                        col++;
                        needsemi = 0;
                        break;
                    }
                    i++;
                    col++;
                } while (i < strlen(line));
            }
        } while (!feof(f));
        fclose(f);

        txt_Files[fi].buf = mem_Alloc(txt_Size, "txt_txt:txt_textbuffer");
        if (!txt_Files[fi].buf) {
            MessageBox(NULL, "Memory could not be allocated!", "txt_openText", MB_ICONEXCLAMATION);
            if (txt_Buffer) {
                mem_Free(txt_Buffer);
                txt_Buffer = NULL;
            }
            return 0;
        }
        memset(txt_Files[fi].buf, 0, txt_Size);
        off = 0;
        for (i = first; i < txt_Count; i++) {
            memcpy(txt_Files[fi].buf + off, txt_Buffer + i * 0x1000, strlen(txt_Buffer + i * 0x1000) + 1);
            txt_Ptrs[i] = txt_Files[fi].buf + off;
            off += strlen(txt_Buffer + i * 0x1000) + 1;
            if (off > txt_Size)
                err_Message("txt_OpenText():Zugriffsverletzung!");
            n = strlen(txt_Buffer + i * 0x1000);
            for (k = 0; k < n; k++)
                if (txt_Ptrs[i][k] == '|')
                    txt_Ptrs[i][k] = 0;
        }
        strcpy(line, txt_BinPath);
        strcat(line, name);
        p = strrchr(line, '.');
        if (p)
            *p = 0;
        strcpy(txt_Files[fi].name, name);
        p = strrchr(txt_Files[fi].name, '.');
        if (p)
            *p = 0;
        txt_Files[fi].first = first;
        txt_Files[fi].last = txt_Count - 1;
        strcat(line, ".res");
        f = fopen(line, "wb");
        if (!f) {
            MessageBox(NULL, "Could not write ResourceFile!", "txt_OpenText", MB_ICONEXCLAMATION);
            mem_Free(txt_Buffer);
            txt_Buffer = NULL;
            return 0;
        }
        fseek(f, 0, SEEK_SET);
        lineno = txt_Count - first;
        fwrite(&lineno, 4, 1, f);
        fwrite(&txt_Files[fi].first, 4, 1, f);
        fwrite(&txt_Files[fi].last, 4, 1, f);
        for (i = first; i < txt_Count; i++) {
            off = txt_Ptrs[i] - txt_Files[fi].buf;
            fwrite(&off, 4, 1, f);
        }
        for (i = first; i < txt_Count; i++)
            fwrite(txt_Keys[i], 0x50, 1, f);
        for (i = first; i < txt_Count; i++)
            fwrite(&txt_Random[i], 1, 1, f);
        fwrite(&txt_Size, 4, 1, f);
        fwrite(txt_Files[fi].buf, txt_Size, 1, f);
        fclose(f);
        first = txt_Count;
        txt_Size = 0;
        sprintf(msg, "txt_OpenText(): %s compiled!", name);
    }

    if (header) {
        strcpy(line, txt_Dir);
        strcat(line, txt_HeaderName);
        f = fopen(line, "wt");
        if (!f) {
            MessageBox(NULL, "Could not write HeaderFile!", "txt_OpenText", MB_ICONEXCLAMATION);
            mem_Free(txt_Buffer);
            txt_Buffer = NULL;
            return 0;
        }
        fseek(f, 0, SEEK_SET);
        for (i = 0; i < ngroups; i++) {
            memset(line, 0, sizeof(line));
            strcpy(line, "#define ");
            strcat(line, txt_Names[i]);
            strcat(line, "  ");
            itoa(txt_GroupFirst[i], num, 10);
            strcat(line, num);
            strcat(line, "\n");
            fputs(line, f);
        }
        for (i = 0; i < ndefs; i++)
            fputs(defs[i], f);
        fclose(f);
    }
    if (txt_Buffer) {
        mem_Free(txt_Buffer);
        txt_Buffer = NULL;
    }
    txt_GroupCount = ngroups;
    txt_Loaded = 1;
    return 1;
}
/* 0x42e2a0, frees the text buffers of all loaded files. */

void txt_FreeFiles(void)
{
    int i;

    for (i = 0; i < 0x80; i++) {
        if (txt_Files[i].buf) {
            mem_Free(txt_Files[i].buf);
            txt_Files[i].buf = NULL;
            memset(&txt_Files[i], 0, sizeof(TXTFILE));
        }
    }
}
/* 0x42e2e0, returns the index of a loaded text file, or -1. */

int txt_FindFile(char *name)
{
    int i;

    for (i = 0; i < 0x80; i++)
        if (!stricmp(txt_Files[i].name, name) && txt_Files[i].name[0])
            return i;
    return -1;
}
/* 0x42e320, frees the text buffer of one file. */

void txt_FreeFile(char *name)
{
    int i = txt_FindFile(name);

    if (i != -1) {
        mem_Free(txt_Files[i].buf);
        txt_Files[i].buf = NULL;
    }
}
/* 0x42e360, reads the texts of an already known file again from its .res file. */

int txt_ReloadTextFile(char *name)
{
    char path[0x100];
    void *f;
    int fi, i, n, size;
    int *offs;

    fi = txt_FindFile(name);
    if (fi == -1)
        return 0;
    sprintf(path, "textbin_%s\\%s.res", "german", txt_Files[fi].name);
    f = vfs_Tfopen(path, "rb", vfs_Root);
    if (!f)
        return 0;
    vfs_fseek(f, 0, SEEK_SET);
    vfs_fread(&n, 4, 1, f);
    vfs_fread(&txt_Files[fi].first, 4, 1, f);
    vfs_fread(&txt_Files[fi].last, 4, 1, f);
    offs = mem_Alloc(n * 4, "txt_txt:txt_ReloadTextFile:PointerTable");
    vfs_fread(offs, n * 4, 1, f);
    for (i = 0; i < n; i++)
        vfs_fread(txt_Keys[txt_Files[fi].first + i], 0x50, 1, f);
    for (i = 0; i < n; i++)
        vfs_fread(&txt_Random[txt_Files[fi].first + i], 1, 1, f);
    vfs_fread(&size, 4, 1, f);
    txt_Files[fi].buf = mem_Alloc(size, "txt_txt:txt_ReloadTextFile:Pointer");
    vfs_fread(txt_Files[fi].buf, size, 1, f);
    for (i = 0; i < n; i++)
        txt_Ptrs[txt_Files[fi].first + i] = txt_Files[fi].buf + offs[i];
    vfs_fclose(f);
    mem_Free(offs);
    return 1;
}
/* 0x42e530, loads a compiled text file (textbin_german\name.res) into a free slot. */

int txt_LoadTextFile(char *name)
{
    char msg[0x100];
    char path[0x104];
    void *f;
    int fi, i = 0, n = 0, size = 0;
    int *offs;

    for (fi = 0; fi < 0x80; fi++)
        if (!txt_Files[fi].name[0])
            break;
    if (fi >= 0x80)
        return 0;
    sprintf(path, "textbin_%s\\%s.res", "german", name);
    f = vfs_Tfopen(path, "rb", vfs_Root);
    if (!f) {
        sprintf(msg, "Could not open textfile:%s", path);
        err_Message(msg);
        return 0;
    }
    vfs_fseek(f, 0, SEEK_SET);
    strcpy(txt_Files[fi].name, name);
    vfs_fread(&n, 4, 1, f);
    vfs_fread(&txt_Files[fi].first, 4, 1, f);
    vfs_fread(&txt_Files[fi].last, 4, 1, f);
    offs = mem_Alloc(n * 4, "txt_txt:txt_loadTextFile:PointerTable");
    vfs_fread(offs, 4, n, f);
    for (i = 0; i < n; i++)
        vfs_fread(txt_Keys[txt_Files[fi].first + i], 0x50, 1, f);
    for (i = 0; i < n; i++)
        vfs_fread(&txt_Random[txt_Files[fi].first + i], 1, 1, f);
    vfs_fread(&size, 4, 1, f);
    txt_Files[fi].buf = mem_Alloc(size, "txt_txt:txt_loadTextFile:Pointer");
    vfs_fread(txt_Files[fi].buf, size, 1, f);
    for (i = 0; i < n; i++)
        txt_Ptrs[txt_Files[fi].first + i] = txt_Files[fi].buf + offs[i];
    if (txt_Files[fi].last > txt_Count + 1)
        txt_Count = txt_Files[fi].last + 1;
    vfs_fclose(f);
    mem_Free(offs);
    return 1;
}
/* 0x42e770, returns the index of a text key, or -1. */

int txt_FindKey(char *key)
{
    int i;

    for (i = 0; i < 0x4800; i++)
        if (!stricmp(key, txt_Keys[i]))
            return i;
    return -1;
}

int txt_GroupCount = 0;  /* 0x6333bc, text groups parsed by the last txt_OpenText, never read */
char * txt_Buffer = 0;  /* 0x6333c0, mem_Alloc'd text pool */
int txt_Size = 0;  /* 0x6333c4, bytes used in txt_Buffer */
char txt_BinPath[256] = {0};  /* 0xfe7c40 */
char txt_Dir[256] = {0};  /* 0xfe7d40 */
char txt_Keys[0x4800][0x50] = {0};  /* 0xfe7e40 */
char txt_GroupIds[0x4800][0x50] = {0};  /* 0x1161e80, the "_..." identifier of each group */
char txt_HeaderName[0x120] = {0};  /* 0x12c9e80, "#headerfile" */
char txt_Names[0x4800][0x50] = {0};  /* 0x12c9fa0, indexed by group - 1 */
TXTFILE txt_Files[0x80] = {0};  /* 0x1431fc0 */
unsigned char txt_GroupLevel[0x4800] = {0};  /* 0x14359c0, label column of each group */
int txt_GroupFirst[0x4800] = {0};  /* 0x144c1e0, first text of each group */

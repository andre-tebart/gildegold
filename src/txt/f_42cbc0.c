/* 0x42cbc0, compiles the text files named in a definition file into <outputpath>\<name>.res files
   and optionally writes a C header with one #define per group.
   @deep behaviour only - grade.py code 1909, regs 90, slots 68, fill 8; unitdiff 4299 B / 1463 asm lines (2026-10-26): all blocks, calls and the switch are in the original order, but the original keeps the char position i in memory ([esp+0x10], cached in edx), lineno in ebx and 0 in ebp where ours enregisters i per loop and keeps lineno in memory (the same line/column live-range finding is in notes/units/txt.md); readable met */

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

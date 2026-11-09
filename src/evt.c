/* evt.c -- original file name unknown ("evt_LoadScript", "evt_RunScript", "evt_EnterFunction").
   Interpreter for the C-like event scripts in x:\engine\gfx\scripts\: tokenizer, declarations,
   function calls, while/if, expression evaluation and calls into engine functions (EVTBUILTIN).
   Extent in GildeGold.exe: 0x4273d0 (after the sample library) .. 0x42b71a (sfx.c follows); own string
   block from 0x5f8484. Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/evt.c); boundaries in decl/text.dcl */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_anim.h"
#include "d3_sm.h"
#include "path.h"
#include "ch.h"
#include "evt.h"
#include "evtc.h"
#include "d3_interface.h"
#include "err.h"
#include "f3gm.h"
#include "f3init.h"
#include "mem.h"
#include "vfs.h"

/* 0x4273d0 */
void evt_Error(EVTSCRIPT *sc, char *pos, char *msg)
{
    char buf[512];
    int line = 0;

    if (!sc) {
        sc = evt_Cur;
        pos = sc->pc;
    }
    if (evt_Cur) {
        if (sc->source) {
            for (line = 0; line < sc->nlines; line++)
                if (sc->source + sc->lines[line] > pos)
                    break;
            if (line) {
                sprintf(buf, "Line %i:%s", line, msg);
                goto log;
            }
        }
        sprintf(buf, "%s", msg);
log:
        strncpy(evt_Log[evt_LogPos].text, buf, 0x7f);
        evt_Log[evt_LogPos].line = line;
        if (++evt_LogPos > 30)
            evt_LogPos = 0;
        sprintf(buf, "\"%s\" %s in Line: %i", evt_Cur->name, msg, line);
    } else {
        sprintf(buf, "%s", msg);
        strncpy(evt_Log[evt_LogPos].text, buf, 0x7f);
        evt_Log[evt_LogPos].line = 0;
        if (++evt_LogPos > 30)
            evt_LogPos = 0;
        sprintf(buf, "%s in Line: %i", msg, 0);
    }
}
/* 0x427570 */

EVTVAR *evt_AddGlobal(char *name, void *data, int type)
{
    EVTVAR *v;

    if (evt_MaxGlobals < evt_NGlobals + 0x30U) {
        v = mem_Alloc((evt_NGlobals + 16) * sizeof(EVTVAR), "evt:t");
        if (evt_Globals) {
            memcpy(v, evt_Globals, evt_NGlobals * sizeof(EVTVAR));
            mem_Free(evt_Globals);
        }
        evt_MaxGlobals += 0x300;
        evt_Globals = v;
    }
    strcpy(evt_Globals[evt_NGlobals].name, name);
    evt_Globals[evt_NGlobals].type = type;
    evt_Globals[evt_NGlobals].local = 0;
    evt_Globals[evt_NGlobals].data.p = data;
    evt_Globals[evt_NGlobals].count = 1;
    evt_NGlobals++;
    return &evt_Globals[evt_NGlobals - 1];
}
/* 0x4276a0 */

EVTVAR *evt_AddLocal(EVTSCRIPT *sc, int type, int count, char *name)
{
    int size = 0;
    EVTVAR *v = 0;
    int i;

    for (i = 0; i < 8; i++) {
        if (!sc->frame->locals[i]) {
            if (type == 1)
                size = 4;
            else if (type == 2)
                size = 1;
            else if (type == 7)
                size = 4;
            else if (type == 6)
                size = 0x60;
            else
                __asm int 3
            v = (EVTVAR *)sc->sp;
            v->count = count;
            v->local = 1;
            v->type = type;
            v->data.p = v + 1;
            strcpy(v->name, name);
            sc->sp += size * count + sizeof(EVTVAR);
            if (sc->sp > sc->stack + 0x400) {
                evt_Error(sc, sc->pc, "Stack Overflow!");
                evt_Stop(sc);
            }
            sc->frame->locals[i] = v;
            break;
        }
    }
    if (!v)
        evt_Error(sc, sc->pc - 1, "Too many locals!");
    return v;
}
/* 0x4277c0 @deep B: hunks code 93, fill 8, slots 4. Read every hunk in asmdiff: original keeps count=1 in ebx and the error return 0 in edi (mov ebx,1; xor edi,edi; mov eax,edi), ours uses edi for count and xor eax,eax; original merges both error exits into one shared evt_Error/evt_Stop tail (add esp,0xc before the pushes). Every constant (2, 0x1b) and every statement is present - only allocation and block layout differ. Nested-if / goto-tail / message-variable variants did not change the layout (best 206 B). Same operations spelled differently. */

int evt_DeclareLocal(EVTSCRIPT *sc, int type)
{
    EVTTOKEN t;
    char name[32];
    int count = 1;

    evt_GetToken(sc->pc, &t);
    if (t.type != EVT_IDENT) {
        if (t.type == EVT_VAR) {
            if (((EVTVAR *)t.u.p)->local) {
                evt_Error(sc, sc->pc, "Syntax error: Propably duplicate symbol...");
                evt_Stop(sc);
                return 0;
            }
        } else {
            evt_Error(sc, sc->pc, "Syntax error: unknown");
            evt_Stop(sc);
            return 0;
        }
    }
    if (t.type == EVT_IDENT)
        strcpy(name, t.u.s);
    else
        strcpy(name, ((EVTVAR *)t.u.p)->name);
    evt_GetToken(sc->pc, &t);
    if (t.type == EVT_OP && t.u.i == EVT_OP_LBRACKET) {
        evt_GetToken(sc->pc, &t);
        if (t.type == EVT_INT)
            count = t.u.i;
    }
    return (int)evt_AddLocal(sc, type, count, name);
}
/* 0x4278d0 */

EVTVAR *evt_FindVar(EVTSCRIPT *sc, char *name)
{
    EVTSCRIPT *s = evt_Scope ? evt_Scope : sc;
    EVTFRAME *f = sc->frame;
    int i;

    for (i = 0; f && i < 8; i++)
        if (f->locals[i] && !strcmp(f->locals[i]->name, name))
            return f->locals[i];
    for (i = 0; i < s->nvars; i++)
        if (!strcmp(s->vars[i].name, name))
            return &s->vars[i];
    for (i = 0; i < evt_NGlobals; i++)
        if (!strcmp(evt_Globals[i].name, name))
            return &evt_Globals[i];
    return 0;
}
/* 0x427a20 */

EVTSCRIPT *evt_FindInclude(EVTSCRIPT *sc, char *name)
{
    int i;

    for (i = 0; i < 8; i++)
        if (sc->includes[i] && !strcmp(sc->includes[i]->name, name))
            return sc->includes[i];
    return 0;
}
/* 0x427a90 */

EVTFUNC *evt_FindFunc(EVTSCRIPT *sc, char *name)
{
    EVTSCRIPT *s = evt_Scope;
    int i;

    if (!s)
        s = sc;
    if (sc->funcs)
        for (i = 0; i < s->nfuncs; i++)
            if (!strcmp(s->funcs[i].name, name))
                return &s->funcs[i];
    return 0;
}
/* 0x427b20 */

int evt_SkipStatement(void)
{
    while (evt_Cur->pc < evt_Cur->source + evt_Cur->sourcelen) {
        if (*evt_Cur->pc == ';') {
            evt_Cur->pc++;
            return 1;
        }
        evt_Cur->pc++;
    }
    return 0;
}
/* 0x427b90 */

int evt_SkipBlock(char dir)
{
    int depth = 0;

    if (dir == 1) {
        while (evt_Cur->pc < evt_Cur->source + evt_Cur->sourcelen) {
            if (*evt_Cur->pc == '{') {
                depth++;
            } else if (*evt_Cur->pc == '}' && --depth <= 0) {
                evt_Cur->pc++;
                return 1;
            }
            evt_Cur->pc++;
        }
    } else if (dir == 2) {
        while (evt_Cur->pc >= evt_Cur->source) {
            if (*evt_Cur->pc == '{') {
                if (--depth <= 0)
                    return 1;
                depth--;
            } else if (*evt_Cur->pc == '}') {
                depth++;
            }
            evt_Cur->pc--;
        }
    }
    return 0;
}
/* 0x427c70 */

int evt_TypeOf(char *name)
{
    char buf[512];
    char *p;

    strcpy(buf, name);
    if ((p = strchr(buf, ' ')) || (p = strchr(buf, ')')))
        *p = 0;
    if (!strcmp(buf, "int"))
        return 1;
    if (!strcmp(buf, "float"))
        return 7;
    if (!strcmp(buf, "void"))
        return 5;
    if (!strcmp(buf, "string"))
        return 6;
    if (!strcmp(buf, "{"))
        return 8;
    if (!strcmp(buf, "}"))
        return 9;
    if (!strcmp(buf, "byte") || !strcmp(buf, "char"))
        return 2;
    return 0;
}
/* 0x427dd0 */

int evt_Match(char *s, char *word)
{
    int n = 0;

    for (; n < 0x80 && word[n]; n++)
        if (s[n] != word[n])
            return 0;
    return n;
}
/* 0x427e00 @deep B: hunks regs 12, code 4. asmdiff shows a pure esi/edi swap of n and end (xor esi,esi vs xor edi,edi; mov edi,[esp+0x14] vs mov esi; lea eax,[esi+edx] vs [edi+edx]); every instruction pair is identical after renaming esi/edi, only the xor sits between the two pushes. No meaning error; declaration order / p forms / &s[n] / ++n tried. */

char *evt_FindLogic(char *s, char *op, char *end)
{
    char *p = s;
    int n = 0;

    if (*s != '&' && s[1] != '&' && *s != '|' && s[1] != '|')
        for (; p < end && *p != ')' && *p != ';' && *p != '(' && *p != '{'; p++) {
            if ((*p == '&' && p[1] == '&') || (*p == '|' && p[1] == '|'))
                return s + n;
            n++;
        }
    return 0;
}
/* 0x427e60 */

void evt_GetToken(char *s, EVTTOKEN *t)
{
    char word[256];
    char found = 0;
    unsigned int start;
    int k, i, n, len;
    char *p;

    if (!*evt_Cur->pc || !evt_Cur->pc[1]) {
        t->type = EVT_END;
        return;
    }
    t->type = EVT_IDENT;
    start = 0;
    while (s[start] == ' ' && start < strlen(s))
        start++;
    for (k = start, len = strlen(s); !found && k < len; k++) {
        for (i = 1; i < 30; i++) {
            if (s[k] == evt_Ops[i][0]) {
                if (i == EVT_OP_DOT && isdigit(s[k - 1]))
                    continue;
                if (!(k - start))
                    for (i = 1; i < 30; i++)
                        if (evt_Match(s + k, evt_Ops[i])) {
                            k += strlen(evt_Ops[i]);
                            break;
                        }
                n = k - start;
                memcpy(word, s + start, n);
                word[n] = 0;
                evt_Cur->pc = s + strlen(word) + start;
                found = 1;
                break;
            }
        }
    }
    for (i = 1; i < 9; i++)
        if (word[0] == evt_Keywords[i][0] && evt_Match(word, evt_Keywords[i])) {
            t->type = EVT_KEYWORD;
            t->u.i = i;
            return;
        }
    for (i = 1; i < 30; i++)
        if (word[0] == evt_Ops[i][0] && evt_Match(word, evt_Ops[i])) {
            t->type = EVT_OP;
            t->u.i = i;
            if (i == EVT_OP_QUOTE) {
                p = strchr(evt_Cur->pc, '"');
                if (p) {
                    memcpy(t->u.s, evt_Cur->pc, p - evt_Cur->pc);
                    t->u.s[p - evt_Cur->pc] = 0;
                }
                t->type = EVT_STRING;
                evt_Cur->pc = p + 1;
            }
            evt_LastOp = i;
            if (i == EVT_OP_LPAREN)
                evt_Parens++;
            else if (i == EVT_OP_RPAREN)
                evt_Parens--;
            return;
        }
    if (isdigit(word[0])) {
        if (strchr(word, '.')) {
            t->type = EVT_FLOAT;
            t->u.f = (float)atof(word);
        } else {
            t->type = EVT_INT;
            t->u.i = atoi(word);
        }
    } else {
        if ((p = (char *)evt_FindVar(evt_Cur, word))) {
            t->type = EVT_VAR;
            t->u.p = p;
            return;
        }
        if ((n = evt_TypeOf(word))) {
            t->type = EVT_TYPE;
            t->u.i = n;
            return;
        }
        if ((p = (char *)evt_FindFunc(evt_Cur, word))) {
            t->type = EVT_FUNC;
            t->u.p = p;
            return;
        }
        if ((p = (char *)evt_FindBuiltin(word))) {
            t->type = EVT_BUILTIN;
            t->u.p = p;
            return;
        }
        if ((p = (char *)evt_FindInclude(evt_Cur, word))) {
            t->type = EVT_SCRIPT;
            t->u.p = p;
            return;
        }
    }
    if (t->type == EVT_IDENT)
        strcpy(t->u.s, word);
}
/* 0x4282a0 */

int evt_Preprocess(char *src, char *dst, int *lines)
{
    unsigned int i = 0;
    int nl = 0;
    int n, j;
    char *e;

    strcpy(dst, src);
    if (lines)
        lines[0] = 0;
    for (; i < strlen(dst); i++) {
        switch (dst[i]) {
        case '\n':
        case '\r':
        case 0x10:
            if (lines && dst[i] == '\n')
                lines[++nl] = i + 1;
        case '\t':
            dst[i] = ' ';
            break;
        case '/':
            if (dst[i + 1] == '*' && (e = strstr(dst + i + 1, "*/")))
                memset(dst + i, ' ', e - (dst + i) + 2);
            break;
        }
    }
    for (i = 0; i < strlen(dst); i++) {
        if (dst[i] == ' ') {
            n = 1;
            while (dst[i + n] == ' ')
                n++;
            if (lines && n > 1) {
                memmove(dst + i + 1, dst + i + n, strlen(dst + i + n) + 1);
                for (j = 0; j < nl; j++)
                    if (lines[j] >= (int)i) {
                        lines[j] = lines[j] - n + 1;
                        if (lines[j] < (int)i)
                            lines[j] = i;
                        if (lines[j] < 0)
                            lines[j] = 0;
                    }
            }
        }
    }
    lines[nl] = -1;
    return strlen(dst);
}
/* 0x4284a0, the script with this id among the 128 slots of evt_Scripts, 0 if none (-1 = no id) */

EVTSCRIPT *evt_FindById(int id)
{
    int i;

    if (id == -1)
        return 0;
    for (i = 0; i < 128; i++)
        if (evt_Scripts[i].id == id)
            return &evt_Scripts[i];
    return 0;
}
/* 0x4284e0 */

EVTSCRIPT *evt_FindByName(char *name)
{
    int i;

    for (i = 0; i < 128; i++)
        if (!_strcmpi(evt_Scripts[i].name, name))
            return &evt_Scripts[i];
    return 0;
}
/* 0x428530 */

EVTSCRIPT *evt_LoadScript(char *path)
{
    char msg[512];
    void *f;
    int i, len;
    EVTSCRIPT *sc;
    char *tmp, *out;

    f = vfs_Tfopen(path, "rb", vfs_Root);
    if (!f) {
        sprintf(msg, "evt_LoadScript: Could not open t_vfs_file: %s!", path);
        err_Message(msg);
        return 0;
    }
    for (i = 0; i < 128; i++)
        if (!evt_Scripts[i].name[0])
            break;
    if (i >= 128) {
        sprintf(msg, "evt_LoadScript: Could not load script %s, because too many scripts are currently in memory!", path);
        vfs_fclose(f);
        return 0;
    }
    evt_Loaded = i;
    sc = &evt_Scripts[i];
    tmp = mem_Alloc(64001, "ect_LoadScript:tmp");
    out = mem_Alloc(64001, "ect_LoadScript:output");
    memset(out, 0, 64001);
    memset(tmp, 0, 64001);
    vfs_fread(tmp, 64000, 1, f);
    vfs_fclose(f);
    len = evt_Preprocess(tmp, out, evt_Lines);
    for (sc->nlines = 0; sc->nlines < 0x800; sc->nlines++)
        if (evt_Lines[sc->nlines] == -1)
            break;
    sc->lines = mem_Alloc(sc->nlines * 4, "evt:debuginfo");
    memcpy(sc->lines, evt_Lines, sc->nlines * 4);
    sc->source = mem_Alloc(len + 1, "evt_LoadScript:sc->SourceCode");
    sc->sourcelen = len + 1;
    sc->nvars = 0;
    sc->nfuncs = 0;
    sc->frame = 0;
    strcpy(sc->source, out);
    strcpy(sc->name, path + strlen(evt_Path));
    memset(sc->frames, 0, sizeof(sc->frames));
    if (tmp)
        mem_Free(tmp);
    if (out)
        mem_Free(out);
    sc->sp = sc->stack = mem_Alloc(0x400, "evt:stack");
    sc->scene = d3_CurrentScene;
    sc->group = evt_NoGroup;
    sc->waitfn = 0;
    sc->call = 0;
    sc->id = evt_NextId++;
    return sc;
}
/* 0x4287b0 */

EVTSCRIPT *evt_Load(char *name)
{
    char path[256];

    sprintf(path, "%s%s", evt_Path, name);
    return evt_LoadScript(path);
}
/* 0x4287e0 */

int evt_While(void)
{
    EVTTOKEN t;
    char *start, *pos;
    int cond;

    start = evt_Cur->pc - strlen(evt_Keywords[1]);
    evt_GetToken(evt_Cur->pc, &t);
    if (t.type == EVT_OP && t.u.i == EVT_OP_LPAREN) {
        cond = evt_Eval(0);
        evt_Cur->frame->loops[evt_Cur->frame->level].cond = cond;
        if (!cond) {
            evt_GetToken(evt_Cur->pc, &t);
            if (t.type == EVT_OP) {
                if (t.u.i == EVT_OP_LBRACE) {
                    evt_Cur->pc = start;
                    evt_SkipBlock(1);
                }
            } else if (!evt_SkipStatement()) {
                evt_Error(evt_Cur, evt_Cur->pc, "Syntax error: ';' missing!");
            }
        } else {
            memset(&evt_Cur->frame->loops[evt_Cur->frame->level], 0, sizeof(EVTLOOP));
            evt_Cur->frame->loops[evt_Cur->frame->level].type = 1;
            evt_Cur->frame->loops[evt_Cur->frame->level].start = start;
            pos = evt_Cur->pc;
            evt_GetToken(pos, &t);
            if (t.type == EVT_OP && t.u.i == EVT_OP_LBRACE) {
                evt_Cur->frame->loops[evt_Cur->frame->level].braced = 0;
            } else {
                evt_Cur->frame->loops[evt_Cur->frame->level].braced = 1;
                evt_Cur->pc = pos;
            }
            evt_Cur->frame->level++;
            if (evt_Cur->frame->level > 8) {
                evt_Error(evt_Cur, evt_Cur->pc, "Too many looplevel...");
                evt_Cur->frame->level--;
            }
        }
    } else {
        evt_Error(evt_Cur, evt_Cur->pc, "Syntax error: '(' missing!");
    }
    return 0;
}
/* 0x4289f0 */

int evt_If(void)
{
    EVTTOKEN t;
    char *start, *pos;
    int cond;

    pos = evt_Cur->pc;
    start = pos - strlen(evt_Keywords[2]);
    evt_GetToken(pos, &t);
    if (t.type == EVT_OP && t.u.i == EVT_OP_LPAREN) {
        cond = evt_Eval(0);
        memset(&evt_Cur->frame->loops[evt_Cur->frame->level], 0, sizeof(EVTLOOP));
        evt_Cur->frame->loops[evt_Cur->frame->level].type = 2;
        evt_Cur->frame->loops[evt_Cur->frame->level].start = start;
        evt_Cur->frame->loops[evt_Cur->frame->level].cond = cond;
        pos = evt_Cur->pc;
        evt_GetToken(pos, &t);
        if (t.type == EVT_OP && t.u.i == EVT_OP_LBRACE) {
            evt_Cur->frame->loops[evt_Cur->frame->level].braced = 0;
        } else {
            evt_Cur->frame->loops[evt_Cur->frame->level].braced = 1;
            evt_Cur->pc = pos;
        }
        evt_Cur->frame->level++;
        if (evt_Cur->frame->level > 8) {
            evt_Error(evt_Cur, evt_Cur->pc, "Too many looplevel...");
            evt_Cur->frame->level--;
        }
        if (!cond) {
            if (t.type == EVT_OP) {
                if (t.u.i != EVT_OP_LBRACE)
                    return 0;
                evt_Cur->pc = pos;
                evt_SkipBlock(1);
                evt_Cur->pc--;
                return 0;
            }
            if (!evt_SkipStatement()) {
                evt_Error(evt_Cur, evt_Cur->pc, "Syntax error: ';' missing!");
                return 0;
            }
            evt_Cur->pc--;
            return 0;
        }
    } else {
        evt_Error(evt_Cur, evt_Cur->pc, "Syntax error: '(' missing!");
    }
    return 0;
}
/* 0x428bf0 */

void evt_Include(void)
{
    EVTTOKEN t;
    EVTSCRIPT *sc;
    char *msg;
    int i;

    evt_GetToken(evt_Cur->pc, &t);
    if (t.type == EVT_STRING) {
        sc = evt_Load(t.u.s);
        if (sc) {
            evt_Compile(sc);
            for (i = 0; i < 8; i++)
                if (!evt_Cur->includes[i]) {
                    evt_Cur->includes[i] = sc;
                    return;
                }
            return;
        }
        evt_Error(evt_Cur, evt_Cur->pc, "Error including the script!");
        evt_Stop(evt_Cur);
    } else {
        evt_Error(evt_Cur, evt_Cur->pc, "Wrong include parameter!");
        evt_Stop(evt_Cur);
    }
}
/* 0x428ca0 */

void evt_Return(void)
{
    int r = evt_Eval(0);

    if (!evt_LeaveFunction(evt_Cur))
        evt_Stop(evt_Cur);
    evt_ReturnValue = r;
}
/* 0x428ce0 */

void evt_Mode1(void)
{
    evt_Cur->mode = 1;
}
/* 0x428cf0 */

void evt_Mode2(void)
{
    evt_Cur->mode = 2;
}
/* 0x428d00 */

void evt_Mode0(void)
{
    evt_Cur->mode = 0;
}
/* 0x428d10 */

int evt_Else(void)
{
    EVTTOKEN t;

    if (evt_Cur->frame->loops[evt_Cur->frame->level + 1].cond) {
        evt_GetToken(evt_Cur->pc, &t);
        if (t.type == EVT_KEYWORD && t.u.i == 4) {
            evt_SkipBlock(1);
            return 0;
        }
        evt_SkipStatement();
    }
    return 0;
}
/* 0x428d70 */

int evt_DefineFunc(EVTSCRIPT *sc, int unk)
{
    EVTTOKEN t;
    EVTFUNC *f = &sc->funcs[sc->nfuncs];

    f->nargs = 0;
    memset(&t, 0, sizeof(t));
    do {
        evt_GetToken(evt_Cur->pc, &t);
        if (t.type == EVT_TYPE && ((t.u.i >= 1 && t.u.i <= 2) || t.u.i == 6)) {
            f->argtype[f->nargs] = t.u.i;
            evt_GetToken(evt_Cur->pc, &t);
            if (t.type != EVT_IDENT) {
                if (t.type == EVT_VAR) {
                    evt_Error(sc, sc->pc, "Dublicate symbolname!");
                    evt_Stop(sc);
                } else {
                    evt_Error(sc, sc->pc, "Missing operand!");
                    evt_Stop(sc);
                }
                return 0;
            }
            strcpy(f->argname[f->nargs], t.u.s);
            f->nargs++;
        }
        evt_GetToken(evt_Cur->pc, &t);
    } while (t.type != EVT_OP || t.u.i != EVT_OP_RPAREN);
    sc->nfuncs++;
    return unk;
}
/* 0x428eb0 */

EVTVAR *evt_NewVar(EVTSCRIPT *sc, int type, int count)
{
    int size = 0;

    if (type == 1)
        size = 4;
    else if (type == 2)
        size = 1;
    else if (type == 6)
        size = 0x60;
    else if (type == 7)
        size = 4;
    else
        __asm int 3
    sc->vars[sc->nvars].type = type;
    sc->vars[sc->nvars].data.p = mem_Alloc(size * count, "evt:var");
    memset(sc->vars[sc->nvars].data.p, 0, size * count);
    sc->vars[sc->nvars].count = count;
    return &sc->vars[sc->nvars++];
}
/* 0x428fa0 */

int evt_Declare(EVTSCRIPT *sc, int type)
{
    char name[32];
    EVTTOKEN t;
    EVTVAR *v;
    char *pos;
    int n;

    evt_GetToken(evt_Cur->pc, &t);
    if (t.type == EVT_IDENT) {
        strcpy(name, t.u.s);
        evt_GetToken(evt_Cur->pc, &t);
        if (t.type == EVT_OP) {
            if (t.u.i == EVT_OP_SEMI) {
                if (!evt_FindVar(sc, name)) {
                    v = evt_NewVar(sc, type, 1);
                    strcpy(v->name, name);
                    return 1;
                }
            } else if (t.u.i == EVT_OP_ASSIGN) {
                if (!evt_FindVar(sc, name)) {
                    v = evt_NewVar(sc, type, 1);
                    strcpy(v->name, name);
                } else
                    __asm int 3
                evt_GetToken(evt_Cur->pc, &t);
                if (t.type == EVT_INT) {
                    *v->data.i = t.u.i;
                    return 1;
                }
            } else if (t.u.i == EVT_OP_LPAREN) {
                pos = evt_Cur->pc;
                if (!evt_DefineFunc(sc, (int)&t.u))
                    return 0;
                sc->funcs[sc->nfuncs - 1].body = strchr(pos, '{');
                strcpy(sc->funcs[sc->nfuncs - 1].name, name);
                evt_Cur->pc = pos;
                return 1;
            } else if (t.u.i == EVT_OP_LBRACKET) {
                evt_GetToken(evt_Cur->pc, &t);
                if (t.type == EVT_INT) {
                    n = t.u.i;
                } else {
                    n = 1;
                    evt_Error(sc, sc->pc, "Illegal array size...");
                }
                if (!evt_FindVar(sc, name)) {
                    v = evt_NewVar(sc, type, n);
                    strcpy(v->name, name);
                }
                evt_GetToken(evt_Cur->pc, &t);
                if (t.u.i != EVT_OP_RBRACKET)
                    evt_Error(sc, sc->pc, "Missing ']'!");
                evt_GetToken(evt_Cur->pc, &t);
                if (t.u.i != EVT_OP_SEMI)
                    evt_Error(sc, sc->pc, "Expecting ';'");
            }
        }
    }
    return 1;
}
/* 0x4292b0 */

int evt_LeaveFunction(EVTSCRIPT *sc)
{
    int i, j;

    if (strcmp(sc->frame->func->name, "exit")) {
        for (i = 15; i >= 0; i--)
            if (sc->frames[i].func)
                break;
        if (i >= 0) {
            if (i >= 1) {
                for (j = 0; j < 8; j++)
                    sc->frame->locals[j] = 0;
                sc->sp = sc->frame->sp;
                sc->frame = &sc->frames[i - 1];
                sc->pc = sc->frame->ret;
                sc->frames[i].func = 0;
                sc->frames[i].ret = 0;
                return 1;
            }
            for (j = 0; j < 8; j++)
                sc->frame->locals[j] = 0;
            sc->sp = sc->frame->sp;
            sc->frame = 0;
            sc->flags &= ~1;
            sc->frames[i].func = 0;
            sc->frames[i].ret = 0;
        }
    }
    return 0;
}
/* 0x4293b0 */

void evt_EnterFunction(EVTSCRIPT *sc, EVTFUNC *f)
{
    int vals[8];
    char strs[8][0x60];
    EVTVAR *v;
    char *s;
    int i = 0;
    int k;

    if (!f) {
        evt_Error(sc, sc->pc, "evt_EnterFunction: Unknown function...");
        return;
    }
    for (; i < 16; i++)
        if (!sc->frames[i].func)
            break;
    if (i >= 16) {
        evt_Error(sc, sc->pc, "evt_EnterFunction: Callstack overflow...");
        return;
    }
    if (i > 0) {
        sc->pc++;
        for (k = 0; k < f->nargs; k++) {
            s = (char *)evt_Eval(0);
            if (f->argtype[k] == 6)
                strcpy(strs[k], s);
            else
                vals[k] = (int)s;
        }
        if (f->nargs == 0)
            sc->pc++;
        sc->frame->ret = sc->pc;
    } else {
        for (k = 0; k < f->nargs; k++)
            if (f->argtype[k] == 6)
                strcpy(strs[k], evt_StrArgs[k]);
            else
                vals[k] = evt_Args[k];
    }
    sc->frame = &sc->frames[i];
    sc->frames[i].sp = sc->sp;
    sc->frames[i].func = f;
    for (k = 0; k < f->nargs; k++) {
        v = evt_AddLocal(sc, f->argtype[k], 1, f->argname[k]);
        if (!v) {
            evt_Error(sc, sc->pc, "Could not allocate symbol...out of memory!");
            evt_Stop(sc);
            return;
        }
        switch (v->type) {
        case 2:
            *v->data.b = vals[k];
            break;
        case 1:
            *v->data.i = vals[k];
            break;
        case 7:
            *v->data.f = (float)vals[k];
            break;
        case 6:
            strcpy(v->data.p, strs[k]);
            break;
        }
    }
    sc->pc = f->body;
    sc->frame->ret = f->body;
    sc->frame->level = 0;
}
/* 0x429670 */

int evt_Compile(EVTSCRIPT *sc)
{
    EVTTOKEN t;
    int depth = 0;
    EVTFUNC *funcs;
    EVTVAR *vars;
    EVTSCRIPT *old;

    funcs = mem_Alloc(0x5300, "evt_t:funcs");
    vars = mem_Alloc(0x1800, "evt_t:vars");
    funcs->nameWord = 0;
    vars->typeWord = 0;
    sc->vars = mem_Alloc(0x1800, "evt_t:vars");
    sc->funcs = mem_Alloc(0x5300, "evt_t:funcs");
    old = evt_Cur;
    evt_Cur = sc;
    sc->pc = sc->source;
    do {
        evt_GetToken(evt_Cur->pc, &t);
        if (t.type == EVT_TYPE && depth == 0) {
            if (!evt_Declare(sc, t.u.i)) {
                mem_Free(funcs);
                mem_Free(vars);
                mem_Free(sc->vars);
                sc->vars = 0;
                mem_Free(sc->funcs);
                sc->funcs = 0;
                return 0;
            }
        } else {
            if (t.type == EVT_OP) {
                if (t.u.i == EVT_OP_LBRACE)
                    depth++;
                else if (t.u.i == EVT_OP_RBRACE)
                    depth--;
            }
            if (depth < 0) {
                evt_Error(sc, sc->pc, "Syntax Error: too many '}'...");
                mem_Free(funcs);
                mem_Free(vars);
                mem_Free(sc->vars);
                sc->vars = 0;
                mem_Free(sc->funcs);
                sc->funcs = 0;
                return 0;
            }
            if (t.type == EVT_KEYWORD && t.u.i == 5)
                evt_Include();
        }
    } while (t.type != EVT_END);
    if (depth != 0) {
        evt_Error(sc, 0, "Syntax Error: Wrong braces-count...");
        mem_Free(funcs);
        mem_Free(vars);
        mem_Free(sc->vars);
        sc->vars = 0;
        mem_Free(sc->funcs);
        sc->funcs = 0;
        return 0;
    }
    if (sc->nvars != 0) {
        memcpy(vars, sc->vars, sc->nvars * sizeof(EVTVAR));
        mem_Free(sc->vars);
        sc->vars = 0;
        sc->vars = mem_Alloc(sc->nvars * sizeof(EVTVAR), "evt:sc->Vars");
        memcpy(sc->vars, vars, sc->nvars * sizeof(EVTVAR));
    } else {
        mem_Free(sc->vars);
        sc->vars = 0;
    }
    if (sc->nfuncs != 0) {
        memcpy(funcs, sc->funcs, sc->nfuncs * sizeof(EVTFUNC));
        mem_Free(sc->funcs);
        sc->funcs = 0;
        sc->funcs = mem_Alloc(sc->nfuncs * sizeof(EVTFUNC), "evt:sc->Functions");
        memcpy(sc->funcs, funcs, sc->nfuncs * sizeof(EVTFUNC));
    } else {
        mem_Free(sc->funcs);
        sc->funcs = 0;
    }
    evt_Cur = old;
    mem_Free(funcs);
    mem_Free(vars);
    return 1;
}
/* 0x4299a0 */

int evt_RunScript(EVTSCRIPT *sc)
{
    char buf[512];
    EVTFUNC *f;

    if (!sc)
        return 0;
    if (!evt_Compile(sc)) {
        evt_Unload(sc);
        return 0;
    }
    f = evt_FindFunc(sc, "main");
    if (!f) {
        evt_Error(sc, 0, "evt_RunScript:No entrypoint(main) found in script...");
        return 0;
    }
    evt_EnterFunction(sc, f);
    sc->flags |= 1;
    sc->mode = 0;
    sc->frame = &sc->frames[0];
    sprintf(buf, "Run script: %s", sc->name);
    strncpy(evt_Log[evt_LogPos].text, buf, 0x7f);
    evt_Log[evt_LogPos].line = 0;
    if (++evt_LogPos > 30)
        evt_LogPos = 0;
    return 1;
}
/* 0x429ab0 */

int evt_RunScriptArgs(EVTSCRIPT *sc, int n, ...)
{
    EVTFUNC *f;
    int i;

    if (!sc)
        return 0;
    if (!evt_Compile(sc))
        return 0;
    f = evt_FindFunc(sc, "main");
    if (!f) {
        evt_Error(sc, 0, "evt_RunScript:No entrypoint(main) found in script...");
        return 0;
    }
    if (n != f->nargs) {
        evt_Error(sc, 0, "evt_RunScript:Wrong parameter count...");
        evt_Stop(sc);
        return 0;
    }
    {
        va_list ap;
        va_start(ap, n);
        for (i = 0; i < f->nargs; i++) {
            if (f->argtype[i] == 6)
                strcpy(evt_StrArgs[i], va_arg(ap, char *));
            else if (f->argtype[i] == 1)
                evt_Args[i] = va_arg(ap, int);
            else if (f->argtype[i] == 2)
                evt_Args[i] = va_arg(ap, char);
        }
    }
    evt_EnterFunction(sc, f);
    sc->flags |= 1;
    sc->frame = &sc->frames[0];
    sprintf(evt_Log[evt_LogPos].text, "Run script: %s", sc->name);
    evt_Log[evt_LogPos].line = 0;
    if (++evt_LogPos > 30)
        evt_LogPos = 0;
    return 1;
}
/* 0x429c40 */

void evt_RunCommand(EVTSCRIPT *sc, char *cmd)
{
    char sep[] = " ";
    int args[16];
    char buf[256];
    char words[16][0x40];
    int n = 0;
    int i;
    char *p;

    strcpy(buf, cmd);
    for (p = strtok(buf, sep); p; p = strtok(0, sep))
        strcpy(words[n++], p);
    for (i = 0; i < n; i++)
        if (isdigit(words[i][0]))
            args[i] = atoi(words[i]);
        else
            args[i] = (int)words[i];
    switch (n) {
    case 0:
        evt_RunScript(sc);
        return;
    case 1:
        evt_RunScriptArgs(sc, 1, args[0]);
        return;
    case 2:
        evt_RunScriptArgs(sc, 2, args[0], args[1]);
        return;
    case 3:
        evt_RunScriptArgs(sc, 3, args[0], args[1], args[2]);
        return;
    case 4:
        evt_RunScriptArgs(sc, 4, args[0], args[1], args[2], args[3]);
        return;
    case 5:
        evt_RunScriptArgs(sc, 5, args[0], args[1], args[2], args[3], args[4]);
        return;
    case 6:
        evt_RunScriptArgs(sc, 6, args[0], args[1], args[2], args[3], args[4], args[5]);
        return;
    case 7:
        evt_RunScriptArgs(sc, 7, args[0], args[1], args[2], args[3], args[4], args[5], args[6]);
        return;
    }
}
/* 0x429e90 */

int evt_Stop(EVTSCRIPT *sc)
{
    unsigned int flags;
    EVTFUNC *f;

    if (!sc)
        return 0;
    flags = sc->flags;
    sc->call = 0;
    sc->waitfn = 0;
    if (!(flags & 2)) {
        f = evt_FindFunc(sc, "exit");
        if (f) {
            sc->flags = flags | 3;
            evt_EnterFunction(sc, f);
            sc->mode = 2;
            evt_Step(sc);
            return 0;
        }
    }
    sc->id = 0;
    sc->flags = 0;
    if (sc->character)
        sc->character->scriptId = -1;
    evt_Error(sc, 0, "Script done...");
    evt_Unload(sc);
    return 1;
}
/* 0x429f30 @deep B: hunks regs 180, code 162. Read asmdiff: evt_Cur and val hold the opposite callee-saved registers (ours edi/esi, original esi/edi) throughout; the string element copy is an inline two-pointer loop in the original vs relative-addressing strcpy in ours; one jump-table index is in ecx vs edx. No missing statement, no wrong constant. The val split / declaration orders / strcpy forms were tried; c2prio puts val above the first evt_Cur web. */

int evt_Eval(char *end)
{
    EVTTOKEN t;
    char buf[512];
    int val = 0;
    int next = EVT_OP_ASSIGN;
    int op = EVT_OP_ASSIGN;
    int result = 0;
    int idx;
    char *p, *pos;
    EVTVAR *v;
    EVTFRAME *f;
    int l;

    if (evt_Cur->flags & 1) {
        while ((evt_Cur->pc < end - 1 || !end) && next != 11) {
            if (next == EVT_OP_SEMI)
                goto semi;
            if (end) {
                p = evt_FindLogic(evt_Cur->pc, "&&", end);
                if (p && p < end)
                    result = val = evt_Eval(p);
            } else {
                p = evt_FindLogic(evt_Cur->pc, "&&", evt_Cur->source + evt_Cur->sourcelen);
                if (p)
                    result = val = evt_Eval(p);
            }
            evt_GetToken(evt_Cur->pc, &t);
            next = 0;
            if (t.type == EVT_OP) {
                evt_Scope = 0;
                next = t.u.i;
            } else {
            if (t.type == EVT_BUILTIN)
                val = evt_CallBuiltin(t.u.p);
            if (t.type == EVT_VAR) {
                v = t.u.p;
                pos = evt_Cur->pc;
                evt_GetToken(pos, &t);
                if (t.type == EVT_OP && t.u.i == EVT_OP_LBRACKET) {
                    idx = evt_Eval(0);
                } else {
                    idx = 0;
                    evt_Cur->pc = pos;
                }
                if (idx > v->count - 1 || idx < 0) {
                    sprintf(buf, "Array index out of bounce: '%s'", v->name);
                    evt_Error(evt_Cur, evt_Cur->pc, buf);
                    evt_Stop(evt_Cur);
                    return 0;
                }
                switch (v->type) {
                case 1:
                case 7:
                    val = v->data.i[idx];
                    break;
                case 2:
                    val = v->data.b[idx];
                    break;
                case 6:
                    strcpy(evt_StrResult, (char *)(val = (int)(v->data.s + idx * 0x60)));
                    break;
                }
            } else if (t.type == EVT_INT) {
                val = t.u.i;
            } else if (t.type == EVT_FLOAT) {
            } else if (t.type == EVT_STRING) {
                strcpy(evt_StrResult, t.u.s);
                val = (int)evt_StrResult;
            } else if (t.type == EVT_IDENT) {
                sprintf(buf, "Unknown symbol: '%s'", t.u.s);
                evt_Error(evt_Cur, evt_Cur->pc, buf);
                evt_Stop(evt_Cur);
                return 0;
            } else if (t.type == EVT_SCRIPT) {
                evt_Scope = t.u.p;
                next = EVT_OP_ASSIGN;
                op = EVT_OP_ASSIGN;
                evt_Cur->pc++;
            }
            }
            switch (next) {
            case 17:
                return result < evt_Eval(end);
            case 19:
                return result > evt_Eval(end);
            case 1:
                return result == evt_Eval(end);
            case 7:
                return result != evt_Eval(end);
            case 12:
                val = evt_Eval(end);
                break;
            case 13:
            case 28:
                return result;
            case 16:
                return result <= evt_Eval(end);
            case 18:
                return result >= evt_Eval(end);
            case 23:
                return evt_Eval(end) & result;
            case 22:
                return evt_Eval(end) | result;
            }
            switch (op) {
            case 2:
                result = val;
                break;
            case 4:
                result += val;
                break;
            case 6:
                result -= val;
                break;
            case 20:
                result /= val;
                break;
            case 21:
                result *= val;
                break;
            case 24:
                result |= val;
                break;
            case 25:
                result &= val;
                break;
            }
            op = next;
            if (t.type == EVT_END || !(evt_Cur->flags & 1))
                break;
        }
        if (next == EVT_OP_SEMI) {
semi:
            f = evt_Cur->frame;
            l = f->level;
            if (l > 1 && f->loops[l - 1].start && f->loops[l - 1].braced) {
                f->level = l - 1;
                f = evt_Cur->frame;
                if (f->loops[f->level].type == 1)
                    evt_Cur->pc = f->loops[f->level].start;
            }
        }
    }
    evt_Scope = 0;
    return result;
}
/* 0x42a480 */

int evt_Assign(EVTVAR *v)
{
    EVTTOKEN t;
    int idx = 0;
    int r;

    evt_GetToken(evt_Cur->pc, &t);
    if (t.type == EVT_OP) {
        if (t.u.i == EVT_OP_LBRACKET) {
            idx = evt_Eval(0);
            evt_GetToken(evt_Cur->pc, &t);
        }
        switch (t.u.i) {
        case EVT_OP_ASSIGN:
            if (v->type == 1) {
                v->data.i[idx] = evt_Eval(0);
                return 0;
            }
            if (v->type == 2) {
                v->data.b[idx] = evt_Eval(0);
                return 0;
            }
            if (v->type == 7) {
                v->data.f[idx] = (float)evt_Eval(0);
                return 0;
            }
            if (v->type == 6) {
                r = evt_Eval(0);
                if (r)
                    strcpy(v->data.s + idx * 0x60, evt_StrResult);
            }
            break;
        case EVT_OP_INC:
            if (v->type == 1) {
                v->data.i[idx]++;
                return 0;
            }
            if (v->type == 2) {
                v->data.b[idx]++;
                return 0;
            }
            break;
        case EVT_OP_DEC:
            if (v->type == 1) {
                v->data.i[idx]--;
                return 0;
            }
            if (v->type == 2) {
                v->data.b[idx]--;
                return 0;
            }
            break;
        }
    }
    return 0;
}
/* 0x42a620 */

int evt_CallBuiltin(EVTBUILTIN *f)
{
    char strs[8][0x60];
    int args[8];
    int r;
    int i;
    union {
        char b;                     /* the byte argument as stored */
        int word;                   /* the same slot read as the int the builtin gets */
    } c;

    evt_Cur->pc++;
    r = -99;
    for (i = 0; i < f->nargs; i++) {
        r = evt_Eval(0);
        if (f->argtype[i] == 7) {
            args[i] = r;
        } else if (f->argtype[i] == 1) {
            args[i] = r;
        } else if (f->argtype[i] == 2) {
            c.b = r;
            args[i] = c.word;
        } else if (f->argtype[i] == 6) {
            strcpy(strs[i], evt_StrResult);
            args[i] = (int)strs[i];
        }
    }
    if (f->nargs && evt_LastOp != EVT_OP_RPAREN) {
        evt_Error(evt_Cur, evt_Cur->pc, "Expecting ')'...possible wrong parameter-count");
        evt_Stop(evt_Cur);
        return 0;
    }
    if (!f->nargs)
        evt_Cur->pc++;
    switch (f->nargs) {
    case 0:
        r = f->fn0();
        break;
    case 1:
        r = f->fn1(&args[0]);
        break;
    case 2:
        r = f->fn2(&args[0], &args[1]);
        break;
    case 3:
        r = f->fn3(&args[0], &args[1], &args[2]);
        break;
    case 4:
        r = f->fn4(&args[0], &args[1], &args[2], &args[3]);
        break;
    case 5:
        r = f->fn5(&args[0], &args[1], &args[2], &args[3], &args[4]);
        break;
    case 6:
        r = f->fn6(&args[0], &args[1], &args[2], &args[3], &args[4], &args[5]);
        break;
    case 7:
        r = f->fn7(&args[0], &args[1], &args[2], &args[3], &args[4], &args[5], &args[6]);
        break;
    }
    if (f->rettype == 7)
        r = evt_FloatResult;
    if ((evt_Cur->mode == 1 && evt_Cur->waitfn) || evt_Cur->waitfn == (void *)evtc_Wait) {
        for (i = 0; i < f->nargs; i++) {
            evt_Cur->args[i] = args[i];
            if (f->argtype[i] == 6) {
                evt_Cur->args[i] = (int)malloc(0x60);
                strcpy((char *)evt_Cur->args[i], strs[i]);
            }
        }
        evt_Cur->call = f;
    }
    return r;
}
/* 0x42a8f0 */

int evt_Statement(EVTSCRIPT *sc)
{
    EVTTOKEN t;
    char buf2[256];
    char buf[256];
    EVTFRAME *f;
    char *p;
    int l;

    evt_Parens = 0;
    do {
        evt_GetToken(sc->pc, &t);
        if (t.type == EVT_OP) {
            evt_Scope = 0;
            if (t.u.i == EVT_OP_RBRACE) {
                l = sc->frame->level;
                if (l > 0) {
                    sc->frame->level = l - 1;
                    f = sc->frame;
                    p = f->loops[f->level].start;
                    if (p && f->loops[f->level].type == 1)
                        sc->pc = p;
                } else if (!evt_LeaveFunction(sc)) {
                    evt_Stop(sc);
                }
                return 0;
            }
        } else if (t.type == EVT_KEYWORD) {
            evt_Scope = 0;
            switch (t.u.i) {
            case 1:
                evt_While();
                break;
            case 2:
                evt_If();
                break;
            case 4:
                evt_Else();
                break;
            case 5:
                evt_Include();
                break;
            case 6:
                evt_Mode1();
                break;
            case 8:
                evt_Mode2();
                break;
            case 7:
                evt_Mode0();
                break;
            case 3:
                evt_Return();
                break;
            default:
                sprintf(buf, "Keyword is not surported!");
                evt_Error(sc, sc->pc, buf);
                evt_Stop(sc);
                return 0;
            }
        } else if (t.type == EVT_BUILTIN) {
            evt_CallBuiltin(t.u.p);
        } else if (t.type == EVT_VAR) {
            evt_Assign(t.u.p);
        } else if (t.type == EVT_FUNC) {
            evt_EnterFunction(sc, t.u.p);
        } else if (t.type == EVT_TYPE) {
            evt_DeclareLocal(sc, t.u.i);
        } else if (t.type == EVT_SCRIPT) {
            evt_Scope = t.u.p;
            sc->pc++;
        } else if (t.type != EVT_END) {
            sprintf(buf2, "Unknown symbol : '%s'", t.u.s);
            evt_Error(sc, sc->pc, buf2);
            evt_Stop(sc);
        }
    } while ((sc->flags & 1) && t.type != EVT_END && (t.type != EVT_OP || t.u.i != EVT_OP_SEMI));
    if (t.type == EVT_OP && t.u.i == EVT_OP_SEMI) {
        f = sc->frame;
        l = f->level;
        if (l > 1 && f->loops[l - 1].start && f->loops[l - 1].braced) {
            f->level = l - 1;
            f = sc->frame;
            if (f->loops[f->level].type == 1)
                sc->pc = f->loops[f->level].start;
        } else {
            return 0;
        }
    }
    if (t.type == EVT_END)
        sc->flags &= ~1;
    return 0;
}
/* 0x42abc0 */

int evt_ResumeCall(EVTSCRIPT *sc)
{
    EVTBUILTIN *f;
    int r;
    int i;

    f = evt_CurCall = sc->call;
    sc->call = 0;
    evt_Cur->waitfn = 0;
    switch (f->nargs) {
    case 0:
        r = f->fn0();
        break;
    case 1:
        r = f->fn1(&sc->args[0]);
        break;
    case 2:
        r = f->fn2(&sc->args[0], &sc->args[1]);
        break;
    case 3:
        r = f->fn3(&sc->args[0], &sc->args[1], &sc->args[2]);
        break;
    case 4:
        r = f->fn4(&sc->args[0], &sc->args[1], &sc->args[2], &sc->args[3]);
        break;
    case 5:
        r = f->fn5(&sc->args[0], &sc->args[1], &sc->args[2], &sc->args[3], &sc->args[4]);
        break;
    case 6:
        r = f->fn6(&sc->args[0], &sc->args[1], &sc->args[2], &sc->args[3], &sc->args[4], &sc->args[5]);
        break;
    case 7:
        r = f->fn7(&sc->args[0], &sc->args[1], &sc->args[2], &sc->args[3], &sc->args[4], &sc->args[5],
                   &sc->args[6]);
        break;
    }
    if (!sc->waitfn) {
        sc->call = 0;
        evt_CurCall = 0;
        for (i = 0; i < f->nargs; i++) {
            if (f->argtype[i] == 6)
                free((void *)sc->args[i]);
            sc->args[i] = 0;
        }
    } else {
        sc->call = f;
    }
    evt_CurCall = 0;
    return r;
}
/* 0x42ada0 */

void evt_Step(EVTSCRIPT *sc)
{
    int scene = -1;

    evt_LastId = -1;
    if (evt_Depth < 32)
        evt_Stack[evt_Depth++] = evt_Cur;
    evt_Cur = sc;
    if (sc->group == -1 || sc->group == evt_NoGroup || sc->ownScene || (sc->flags & 2) || !sc->pc) {
        if (sc->character && sc->character->scriptGroup != sc->group && sc->character->group != sc->group) {
            if (evt_Depth > 0) {
                evt_Cur = evt_Stack[evt_Depth - 1];
                evt_Depth--;
                evt_Stack[evt_Depth] = 0;
            }
            return;
        }
        if (sc->ownScene || (sc->flags & 2)) {
            scene = d3_CurrentScene;
            d3if_SelectScene(sc->scene, 1);
        }
        if (sc->waitfn)
            evt_ResumeCall(sc);
        if (!sc->waitfn) {
            do
                evt_Statement(sc);
            while (sc->mode == 2 && (sc->flags & 1));
        }
        if (scene != -1)
            d3if_SelectScene(scene, 1);
    }
    if (evt_Depth > 0) {
        evt_Depth--;
        evt_Cur = evt_Stack[evt_Depth];
        evt_Stack[evt_Depth] = 0;
    }
}
/* 0x42aef0 */

void evt_StepAll(void)
{
    int i;

    for (i = 0; i < 128; i++) {
        if (evt_Scripts[i].flags & 1) {
            evt_Cur = &evt_Scripts[i];
            evt_Step(evt_Cur);
        }
    }
    evt_Cur = 0;
}
/* 0x42af30 */

void evt_Shutdown(void)
{
    int i;

    for (i = 0; i < 128; i++)
        if (evt_Scripts[i].name[0])
            evt_Unload(&evt_Scripts[i]);
    for (i = 0; i < 256; i++)
        evt_Builtins[i].name[0] = 0;
    if (evt_Globals) {
        mem_Free(evt_Globals);
        evt_Globals = 0;
    }
    if (evt_Builtins) {
        mem_Free(evt_Builtins);
        evt_Builtins = 0;
    }
    if (evt_Args) {
        mem_Free(evt_Args);
        evt_Args = 0;
    }
    if (evt_Console) {
        mem_Free(evt_Console);
        evt_Console = 0;
    }
    if (evt_Log) {
        mem_Free(evt_Log);
        evt_Log = 0;
    }
    if (evt_Scripts) {
        mem_Free(evt_Scripts);
        evt_Scripts = 0;
    }
}
/* 0x42b010 */

void evt_UnloadAll(void)
{
    int i;

    for (i = 0; i < 128; i++)
        if (evt_Scripts[i].name[0] || evt_Scripts[i].id != -1)
            evt_Unload(&evt_Scripts[i]);
}
/* 0x42b050 */

int evt_Init(void)
{
    int i;

    evt_Builtins = mem_Alloc(256 * sizeof(EVTBUILTIN), "evt:command_entry");
    evt_Args = mem_Alloc(0x20, "evt:commandLine");
    evt_Console = mem_Alloc(0x400, "evt:console");
    evt_Log = mem_Alloc(0x1080, "evt:errorline");
    evt_Scripts = mem_Alloc(128 * sizeof(EVTSCRIPT), "evt:script");
    memset(evt_Scripts, 0, 128 * sizeof(EVTSCRIPT));
    for (i = 0; i < 128; i++)
        evt_Scripts[i].id = -1;
    evt_Builtins->nameWord = 0;
    strcpy(evt_Ops[26], " ");
    strcpy(evt_Ops[29], ".");
    strcpy(evt_Ops[6], "-");
    strcpy(evt_Ops[4], "+");
    strcpy(evt_Ops[5], "--");
    strcpy(evt_Ops[3], "++");
    strcpy(evt_Ops[2], "=");
    strcpy(evt_Ops[1], "==");
    strcpy(evt_Ops[7], "!=");
    strcpy(evt_Ops[8], "{");
    strcpy(evt_Ops[9], "}");
    strcpy(evt_Ops[27], "[");
    strcpy(evt_Ops[28], "]");
    strcpy(evt_Ops[12], "(");
    strcpy(evt_Ops[13], ")");
    strcpy(evt_Ops[14], "\"");
    strcpy(evt_Ops[10], ";");
    strcpy(evt_Ops[11], ",");
    strcpy(evt_Ops[15], "//");
    strcpy(evt_Ops[17], "<");
    strcpy(evt_Ops[24], "|");
    strcpy(evt_Ops[25], "&");
    strcpy(evt_Ops[22], "||");
    strcpy(evt_Ops[23], "&&");
    strcpy(evt_Ops[16], "<=");
    strcpy(evt_Ops[19], ">");
    strcpy(evt_Ops[18], ">=");
    strcpy(evt_Ops[20], "/");
    strcpy(evt_Ops[21], "*");
    strcpy(evt_Keywords[1], "while");
    strcpy(evt_Keywords[0], "do");
    strcpy(evt_Keywords[2], "if");
    strcpy(evt_Keywords[3], "return");
    strcpy(evt_Keywords[4], "else");
    strcpy(evt_Keywords[5], "#include");
    strcpy(evt_Keywords[6], "#singlestep");
    strcpy(evt_Keywords[7], "#normalstep");
    strcpy(evt_Keywords[8], "#multistep");
    memset(&evt_G, 0, sizeof(evt_G));
    return 1;
}
/* 0x42b410 */

int evt_Unload(EVTSCRIPT *sc)
{
    int i;

    if (!sc)
        return 0;
    if (sc->vars) {
        for (i = 0; i < sc->nvars; i++) {
            if (sc->vars[i].data.p) {
                mem_Free(sc->vars[i].data.p);
                sc->vars[i].data.p = 0;
                sc->vars[i].data.p = 0;
            }
        }
    }
    for (i = 0; i < 8; i++) {
        if (sc->includes[i]) {
            evt_Unload(sc->includes[i]);
            sc->includes[i] = 0;
        }
    }
    if (sc->vars) {
        mem_Free(sc->vars);
        sc->vars = 0;
    }
    if (sc->funcs) {
        mem_Free(sc->funcs);
        sc->funcs = 0;
    }
    if (sc->source) {
        mem_Free(sc->source);
        sc->source = 0;
    }
    if (sc->lines) {
        mem_Free(sc->lines);
        sc->lines = 0;
    }
    if (sc->stack) {
        mem_Free(sc->stack);
        sc->stack = 0;
    }
    memset(sc, 0, sizeof(EVTSCRIPT));
    sc->id = -1;
    return 1;
}
/* 0x42b540 */

EVTBUILTIN *evt_FindBuiltin(char *name)
{
    int i;

    for (i = 0; i < 256; i++)
        if (!strcmp(evt_Builtins[i].name, name))
            break;
    if (i < 256)
        return &evt_Builtins[i];
    return 0;
}
/* 0x42b5b0 */

int evt_ImportCommand(char *name, int (*fn)(), int rettype, int nargs, ...)
{
    int i = 0;
    int k;
    char *s;
    char *d;
    char c;

    if (strlen(name) > 0x1f) {
        err_Message("evt_ImportCommand: Commandnamelength is invalid...");
        return 0;
    }
    if (evt_FindBuiltin(name)) {
        err_Message("evt_ImportCommand: Commandname already exists...");
        return 0;
    }
    for (; i < 256; i++)
        if (!evt_Builtins[i].fn)
            break;
    if (i >= 256) {
        err_Message("evt_ImportCommand: Too many commands!");
        return 0;
    }
    evt_Builtins[i].fn = fn;
    evt_Builtins[i].nargs = nargs;
    evt_Builtins[i].rettype = rettype;
    {
        va_list ap;
        va_start(ap, nargs);
        for (k = 0; k < nargs; k++)
            evt_Builtins[i].argtype[k] = va_arg(ap, int);
        va_end(ap);
    }
    s = name;
    d = evt_Builtins[i].name;
    do {
        c = *s++;
        *d++ = c;
    } while (c);
    return 1;
}
/* 0x42b6c0 */

int evt_StartScript(int unused1, int unused2, char *name)
{
    EVTSCRIPT *sc;

    sc = evt_Load(name);
    if (sc) {
        evt_RunScript(sc);
        return evt_LastId = sc->id;
    }
    return evt_LastId = -1;
}
/* 0x42b700 */

void *evt_GetStartScript(void)
{
    return evt_StartScript;
}
/* 0x42b710 */

void evt_ResetNoGroup(void)
{
    evt_NoGroup = -1;
}

EVTSCRIPT * evt_Cur = 0;  /* 0x6332b8 */
EVTBUILTIN * evt_Builtins = 0;  /* 0x6332bc, [256] */
int * evt_Args = 0;  /* 0x6332c0 */
void * evt_Console = 0;  /* 0x6332c4 */
int evt_LogPos = 0;  /* 0x6332d0 */
int evt_LastOp = 0;  /* 0x6332d4 */
EVTSCRIPT * evt_Scope = 0;  /* 0x6332d8 */
EVTBUILTIN * evt_CurCall = 0;  /* 0x6332dc */
int evt_ReturnValue = 0;  /* 0x6332e0, the value of the last script "return" */
int evt_Parens = 0;  /* 0x6332e4 */
int evt_Depth = 0;  /* 0x6332e8, entries in evt_Stack */
EVTSCRIPT * evt_Stack[32] = {0};  /* 0x145e1e0, evt_Cur of the callers of evt_Step */
int evt_Lines[0x800] = {0};  /* 0x145e260 */
char evt_Keywords[9][0x10] = {0};  /* 0x1460260, [0] unused: "while", "if", ... */
int evt_Loaded = 0;  /* 0x14602f0 */
char evt_StrResult[0x100] = {0};  /* 0x1460300 */
char evt_StrArgs[8][0x60] = {0};  /* 0x1460400 */
char evt_Ops[30][5] = {0};  /* 0x1460700, [0] unused */
EVTGLOBALS evt_G = { 0 };  /* 0x14607a0, evt_Globals / evt_NGlobals / evt_MaxGlobals (evt.h) */
int evt_FloatResult = 0;  /* 0x14607ac */
EVTLOG * evt_Log = 0;  /* 0x14607b0 */
char evt_Path[] = "x:\\engine\\gfx\\scripts\\";  /* 0x5f8330, the scripts directory */
int evt_NextId = 1;  /* 0x5f8434 */

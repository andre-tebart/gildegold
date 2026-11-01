/* evt.c -- event script interpreter ("evt_LoadScript", "evt_RunScript", "evt_EnterFunction"),
   original file name unknown. Starts at 0x4273d0 right after the sample library; continues past
   0x42abc0 (at least FUN_0042ada0, FUN_0042b410, FUN_0042b540). Names are ours. */
#ifndef UNK_4273D0_H
#define UNK_4273D0_H

/* token types (EVTTOKEN.type) */
#define EVT_IDENT     0     /* unknown identifier, name in s */
#define EVT_OP        1     /* operator, index into evt_Ops */
#define EVT_VAR       2     /* variable, EVTVAR * */
#define EVT_FUNC      3     /* script function, EVTFUNC * */
#define EVT_BUILTIN   4     /* engine function, EVTBUILTIN * */
#define EVT_INT       5
#define EVT_FLOAT     6     /* value stored as (int)(float) */
#define EVT_STRING    7     /* string literal in s */
#define EVT_TYPE      8     /* type keyword, see evt_TypeOf */
#define EVT_KEYWORD   10    /* index into evt_Keywords */
#define EVT_SCRIPT    11    /* name of an included script */
#define EVT_END       12

/* operators (evt_Ops index) */
#define EVT_OP_EQ       1
#define EVT_OP_ASSIGN   2
#define EVT_OP_INC      3
#define EVT_OP_ADD      4
#define EVT_OP_DEC      5
#define EVT_OP_SUB      6
#define EVT_OP_NE       7
#define EVT_OP_LBRACE   8
#define EVT_OP_RBRACE   9
#define EVT_OP_SEMI     10
#define EVT_OP_LPAREN   12
#define EVT_OP_RPAREN   13
#define EVT_OP_QUOTE    14
#define EVT_OP_LBRACKET 27
#define EVT_OP_RBRACKET 28
#define EVT_OP_DOT      29

typedef struct EVTTOKEN {           /* 0x64 bytes */
    int type;
    union {
        int i;
        float f;
        void *p;
        char s[0x60];
    } u;                            /* 0x04 */
} EVTTOKEN;

typedef union EVTVARDATA {          /* what EVTVAR.data points to, one view per type */
    void *p;                        /* the allocation itself */
    int *i;                         /* type 1 */
    char *b;                        /* type 2 */
    char *s;                        /* type 6: count strings of 0x60 chars each */
    float *f;                       /* type 7 */
} EVTVARDATA;

typedef struct EVTVAR {             /* 0x30 bytes */
    union {
        int type : 4;               /* 1 int, 2 char/byte, 6 string (0x60 chars), 7 float */
        int typeWord;               /* the bitfield with its padding, cleared as one word */
    };
    char name[0x20];                /* 0x04 */
    int count;                      /* 0x24, array size */
    int local;                      /* 0x28, 1 = on the script stack */
    EVTVARDATA data;                /* 0x2c */
} EVTVAR;

typedef struct EVTFUNC {            /* 0x14c bytes, function defined in a script */
    union {
        char name[0x20];
        int nameWord;               /* name[0..3] as the one 4-byte scratch clear of evt_Compile */
    };
    char *body;                     /* 0x20, at its '{' */
    char nargs;                     /* 0x24 */
    int argtype[8];                 /* 0x28 */
    char argname[8][0x20];          /* 0x48 */
    int pad148;
} EVTFUNC;

typedef struct EVTBUILTIN {         /* engine function callable from scripts */
    union {
        char name[0x20];
        int nameWord;               /* name[0..3] as the one 4-byte scratch clear of evt_Init */
    };
    int nargs;                      /* 0x20 */
    int argtype[8];                 /* 0x24 */
    union {                         /* 0x44, one pointer called with a pointer to each of the nargs arguments */
        int (*fn)();                /* as imported (evt_ImportCommand takes K&R pointers) */
        int (*fn0)(void);
        int (*fn1)(int *);
        int (*fn2)(int *, int *);
        int (*fn3)(int *, int *, int *);
        int (*fn4)(int *, int *, int *, int *);
        int (*fn5)(int *, int *, int *, int *, int *);
        int (*fn6)(int *, int *, int *, int *, int *, int *);
        int (*fn7)(int *, int *, int *, int *, int *, int *, int *);
    };
    int rettype;                    /* 0x48 */
} EVTBUILTIN;

typedef struct EVTLOOP {            /* 0x10 bytes */
    char *start;                    /* loop head (while) */
    int cond;                       /* 0x04 */
    int type;                       /* 0x08, 1 while, 2 if */
    char braced;                    /* 0x0c */
} EVTLOOP;

typedef struct EVTFRAME {           /* 0xb0 bytes */
    EVTFUNC *func;
    char *ret;                      /* 0x04 */
    int level;                      /* 0x08, nesting depth of loops */
    char *sp;                       /* 0x0c, stack pointer at entry */
    EVTVAR *locals[8];              /* 0x10 */
    EVTLOOP loops[8];               /* 0x30 */
} EVTFRAME;

struct CHARACTER;                   /* ch.h, the character a script was started for */

typedef struct EVTSCRIPT {          /* 0xc98 bytes, 128 at evt_Scripts */
    char name[0x100];
    int id;                         /* 0x100, 0 = free */
    int group;                      /* 0x104, the group the script runs for (CHC_GROUP); evt_NoGroup = none */
    EVTVAR *vars;                   /* 0x108 */
    EVTFUNC *funcs;                 /* 0x10c */
    int nfuncs;                     /* 0x110 */
    int nvars;                      /* 0x114 */
    char *pc;                       /* 0x118 */
    char *source;                   /* 0x11c */
    int sourcelen;                  /* 0x120 */
    unsigned int flags;             /* 0x124, 1 running, 2 exiting */
    EVTFRAME frames[16];            /* 0x128 */
    EVTFRAME *frame;                /* 0xc28 */
    int nlines;                     /* 0xc2c */
    int *lines;                     /* 0xc30, offset of each line in source */
    char *stack;                    /* 0xc34 */
    char *sp;                       /* 0xc38 */
    struct EVTSCRIPT *includes[8];  /* 0xc3c */
    EVTBUILTIN *call;               /* 0xc5c */
    void *waitfn;                   /* 0xc60, the wait command the script is suspended in (0 = running) */
    int args[8];                    /* 0xc64 */
    int mode;                       /* 0xc84 */
    int waitStart;                  /* 0xc88, d3_Time tick at which the current wait started */
    int ownScene;                   /* 0xc8c, run the script in its own scene (evt_Step selects it, then restores) */
    int scene;                      /* 0xc90, the scene the script was loaded in / runs in */
    struct CHARACTER *character;    /* 0xc94, the character the script was started for (ch.h; f3init.c / sv.c set it), or 0 */
} EVTSCRIPT;

typedef struct EVTLOG {             /* 0x84 bytes, ring of 31 */
    int line;
    char text[0x80];
} EVTLOG;

extern EVTSCRIPT *evt_Scripts;      /* 0x6332b4, [128] */
extern EVTSCRIPT *evt_Cur;          /* 0x6332b8 */
extern int *evt_Args;               /* 0x6332c0 */
extern int evt_LogPos;              /* 0x6332d0 */
extern int evt_LastOp;              /* 0x6332d4 */
extern EVTSCRIPT *evt_Scope;        /* 0x6332d8 */
extern int evt_ReturnValue;         /* 0x6332e0, the value of the last script "return" */
extern int evt_Parens;              /* 0x6332e4 */
extern int evt_NextId;              /* 0x5f8434 */
extern char evt_Path[];            /* 0x5f8330, the scripts directory */
extern int evt_Lines[0x800];        /* 0x145e260 */
extern char evt_Keywords[9][0x10];  /* 0x1460260, [0] unused: "while", "if", ... */
extern int evt_Loaded;              /* 0x14602f0 */
extern char evt_StrResult[0x100];   /* 0x1460300 */
extern char evt_StrArgs[8][0x60];   /* 0x1460400 */
extern char evt_Ops[30][5];         /* 0x1460700, [0] unused */
extern EVTVAR *evt_Globals;         /* 0x14607a0 */
extern int evt_NGlobals;            /* 0x14607a4 */
extern unsigned int evt_MaxGlobals;         /* 0x14607a8 */
extern int evt_FloatResult;         /* 0x14607ac */
extern EVTLOG *evt_Log;             /* 0x14607b0 */
extern EVTSCRIPT *evt_Stack[32];    /* 0x145e1e0, evt_Cur of the callers of evt_Step */
extern EVTBUILTIN *evt_Builtins;    /* 0x6332bc, [256] */
extern void *evt_Console;           /* 0x6332c4, 0x400 bytes */
extern EVTBUILTIN *evt_CurCall;     /* 0x6332dc */
extern int evt_Depth;               /* 0x6332e8, entries in evt_Stack */
extern int evt_LastId;              /* 0x5f8438, id of the last started script, -1 */
char d3if_SelectScene(int index, char keep);                                                /* 0x4478e0 */

extern int d3_CurrentScene;         /* 0x633444 */
extern struct VFSDIR *vfs_Root;              /* 0x6333fc */

void err_Message(char *msg);                                                            /* 0x4222b0 */
void * mem_Alloc(int size, char *tag);                                                   /* 0x4223c0 */
void mem_Free(void *p);                                                                 /* 0x422620 */
struct VFSFILE * vfs_Tfopen(char *name, char *mode, int unk);                                    /* 0x430470 */
void evt_Error(EVTSCRIPT *sc, char *pos, char *msg);                                    /* 0x4273d0, reports a script error: prefixes "Line N:" from the line table, appends text and line to the 31-entry evt_Log ring (evt_LogPos wraps at 30) and formats the console message; no return */
EVTVAR *evt_AddGlobal(char *name, void *data, int type);                                /* 0x427570, registers the global variable name (type, data, count 1), growing evt_Globals by 0x300 entries when fewer than 0x30 are free; returns it */
EVTVAR *evt_AddLocal(EVTSCRIPT *sc, int type, int count, char *name);                   /* 0x4276a0, allocates count values of type (1 int 4, 2 byte 1, 6 string 0x60, 7 float 4) on the script stack in the first free of the frame's 8 locals; reports "Stack Overflow!" (and stops) above 0x400 and "Too many locals!" when all 8 are taken; returns the entry or 0 */
int evt_DeclareLocal(EVTSCRIPT *sc, int type);                                          /* 0x4277c0, parses the name and optional [count] of a local declaration at sc->pc, rejects a duplicate local and a non-identifier with a syntax error; returns the evt_AddLocal entry as int, 0 on error */
EVTVAR *evt_FindVar(EVTSCRIPT *sc, char *name);                                         /* 0x4278d0, the variable name: the current frame's 8 locals, then the scope/script vars, then the globals; 0 if none */
EVTSCRIPT *evt_FindInclude(EVTSCRIPT *sc, char *name);                                  /* 0x427a20, the included script with this name among the 8 includes of sc; 0 if none */
EVTFUNC *evt_FindFunc(EVTSCRIPT *sc, char *name);                                       /* 0x427a90, the script function name in the current scope (or sc); 0 if none or sc has no functions */
int evt_SkipStatement(void);                                                            /* 0x427b20, advances evt_Cur->pc past the next ';' of the source; 1 when found, 0 at the end */
int evt_SkipBlock(char dir);                                                            /* 0x427b90, moves evt_Cur->pc over a brace block: dir 1 forward past the matching '}', dir 2 backward to the matching '{' (pc left before it); 1 when found, 0 at the source end */
int evt_TypeOf(char *name);                                                             /* 0x427c70, maps a type keyword up to the first space or ')': int 1, byte/char 2, void 5, string 6, float 7, { 8, } 9; 0 = not a type */
int evt_Match(char *s, char *word);                                                     /* 0x427dd0, length of the prefix of s equal to word (up to 0x80); 0 when they differ */
char *evt_FindLogic(char *s, char *op, char *end);                                      /* 0x427e00, the "&&" / "||" at this nesting level of s up to end (stops at ')' ';' '(' '{'); 0 if none; op unused */
void evt_GetToken(char *s, EVTTOKEN *t);                                                /* 0x427e60, scans one token at s (normally evt_Cur->pc) into t and leaves evt_Cur->pc after it: operators (also evt_LastOp, evt_Parens), keywords, quoted strings, numbers, names as variable/type/function/builtin/include or EVT_IDENT; EVT_END at the source end */
int evt_Preprocess(char *src, char *dst, int *lines);                                   /* 0x4282a0, copies src into dst (the caller's 64001 bytes) with tab, CR, LF and 0x10 as spaces and C comments blanked, collapses space runs, and fills lines[] (offset per line, -1 terminated, adjusted for the collapsed runs); returns the new length */
EVTSCRIPT *evt_FindById(int id);                                                        /* 0x4284a0, the script with this id among the 128 slots of evt_Scripts, 0 if none (-1 = no id) */
EVTSCRIPT *evt_FindByName(char *name);                                                  /* 0x4284e0, the script with this name (case-insensitive) among the 128 slots of evt_Scripts; 0 if none */
EVTSCRIPT *evt_LoadScript(char *path);                                                  /* 0x428530, loads and preprocesses the script file path into the first free of the 128 slots (id evt_NextId++, scene d3_CurrentScene, group evt_NoGroup, 0x400-byte stack); reports and returns 0 when the file or a slot is missing; sets evt_Loaded */
EVTSCRIPT *evt_Load(char *name);                                                        /* 0x4287b0, evt_LoadScript of evt_Path + name */
int evt_While(void);                                                                    /* 0x4287e0, a "while" statement: evaluates the condition, pushes an EVTLOOP type 1 at the loop level (braced 0 when a '{' follows, else 1 with pc back at the body); a false condition skips the block or statement; reports a missing '('; returns 0 */
int evt_If(void);                                                                       /* 0x4289f0, an "if" statement: evaluates the condition, pushes an EVTLOOP type 2 and skips the block or statement when it is false (pc ends before the matching '}'); reports a missing '('; returns 0 */
void evt_Include(void);                                                                 /* 0x428bf0, a #include "file": loads and compiles the script and stores it in the first free of the 8 includes of evt_Cur; errors and stops on a wrong parameter or a failed load */
void evt_Return(void);                                                                  /* 0x428ca0, a "return": evaluates the expression, leaves the script function (evt_LeaveFunction, else evt_Stop) and stores the value in the script return value */
void evt_Mode1(void);                                                                   /* 0x428ce0, #singlestep: sets the script mode to 1 */
void evt_Mode2(void);                                                                   /* 0x428cf0, #multistep: sets the script mode to 2 */
void evt_Mode0(void);                                                                   /* 0x428d00, #normalstep: sets the script mode to 0 */
int evt_Else(void);                                                                     /* 0x428d10, an "else": when the matching if was taken, skips the else body (a block when the next token is the else keyword, else the statement up to ';'); returns 0 */
int evt_DefineFunc(EVTSCRIPT *sc, int unk);                                             /* 0x428d70, parses the parameter list "type name, ..." of a script function definition into sc->funcs[sc->nfuncs] (8 argument types and names); errors and stops on a duplicate or missing name; returns its argument (the nonzero token address from evt_Declare = ok) */
EVTVAR *evt_NewVar(EVTSCRIPT *sc, int type, int count);                                 /* 0x428eb0, creates the script variable (zeroed data of 1 int 4, 2 byte 1, 6 string 0x60 or 7 float 4 bytes times count) in sc->vars[sc->nvars]; returns the entry and advances nvars */
int evt_Declare(EVTSCRIPT *sc, int type);                                               /* 0x428fa0, parses a global declaration "type name;", "type name = int;", "type name(args)" or "type name[count];" at evt_Cur->pc and creates it unless the name exists; reports "Illegal array size...", "Missing ']'!" and "Expecting ';'"; returns 1, 0 when evt_DefineFunc fails */
int evt_LeaveFunction(EVTSCRIPT *sc);                                                   /* 0x4292b0, pops the current script function frame (locals, stack pointer, pc = the caller's ret) and clears the running flag and frame at the outermost call; the "exit" function or no frame returns 0, else 1 */
void evt_EnterFunction(EVTSCRIPT *sc, EVTFUNC *f);                                      /* 0x4293b0, enters script function f in the first free of the 16 frames: evaluates the call arguments (or takes evt_Args/evt_StrArgs for the outermost call) into new locals by argtype and sets pc to the body; reports "Unknown function...", "Callstack overflow..." and out-of-memory (evt_Stop) */
int evt_Compile(EVTSCRIPT *sc);                                                         /* 0x429670, parses a loaded script into its variable and function tables (declarations at brace depth 0, #include scripts, brace count check) and shrinks both to the used size; reports syntax errors and returns 0 with the tables freed, else 1 */
int evt_RunScript(EVTSCRIPT *sc);                                                       /* 0x4299a0, compiles sc, enters its "main" function, marks the script running (mode 0, frame 0) and logs "Run script: name"; returns 0 for no script, a failed compile (unloads) or a missing main, else 1 */
int evt_RunScriptArgs(EVTSCRIPT *sc, int n, ...);                                       /* 0x429ab0, compiles sc, checks n against main's argument count, stores the arguments in evt_Args/evt_StrArgs by type, enters main and logs "Run script: name"; returns 0 on failure, 1 when started */
void evt_RunCommand(EVTSCRIPT *sc, char *cmd);                                          /* 0x429c40, runs a console command: splits cmd at spaces into at most 16 words, digit-leading words become ints (else string pointers) and starts the script with 0..7 arguments by the word count; no return */
int evt_Stop(EVTSCRIPT *sc);                                                            /* 0x429e90, stops script sc: unless it is already exiting (flags & 2) it runs the "exit" function (flags |= 3, mode 2, evt_Step) and returns 0; else clears id and flags and the character's script id, logs "Script done...", unloads sc and returns 1 */
int evt_Eval(char *end);                                                                /* 0x429f30, evaluates the expression at evt_Cur->pc up to end (or the source end): operators with a precedence recursion for &&, || and the comparisons, builtin calls, variable reads with an array bounds check; ends at ',' ';' (a ';' rewinds a braced while); returns the value */
int evt_Assign(EVTVAR *v);                                                              /* 0x42a480, applies =, ++ or -- (with an optional [index]) to the script variable v, storing int, byte, float or string by v->type; returns 0 */
int evt_CallBuiltin(EVTBUILTIN *f);                                                     /* 0x42a620, calls the engine function f from a script: evaluates its arguments by type, checks the closing ')', dispatches by the argument count and uses evt_FloatResult for a float return; when the script waits (mode 1 with a waitfn, or the wait builtin 0x40b710) it stores the arguments in sc->args and f in sc->call; returns the result */
int evt_Statement(EVTSCRIPT *sc);                                                       /* 0x42a8f0, executes statements of sc until ';' or the end: '}' pops the loop or function, keywords dispatch (while, if, else, #include, the step modes, return), builtin/var/func/type/include names are handled; a ';' rewinds a braced while; clears the running flag at the source end; returns 0 */
int evt_ResumeCall(EVTSCRIPT *sc);                                                      /* 0x42abc0, resumes the builtin call sc is suspended in: calls f with sc->args and, unless the wait repeated, frees the string arguments and clears the call; returns its result */
void evt_Step(EVTSCRIPT *sc);                                                           /* 0x42ada0, one step of script sc: pushes evt_Cur on evt_Stack (max 32), makes sc current, resumes a waiting builtin and runs its statements while the character's groups match sc->group (selecting sc->scene when ownScene or exiting, then restoring it); pops evt_Cur */
void evt_StepAll(void);                                                                 /* 0x42aef0, steps every running script of the 128 slots once; clears evt_Cur */
void evt_Shutdown(void);                                                                /* 0x42af30, unloads every loaded script and frees the builtin, command-line, console, log and script tables; no return */
void evt_UnloadAll(void);                                                               /* 0x42b010, unloads every script slot that has a name or an id; no return */
int evt_Init(void);                                                                     /* 0x42b050, allocates the builtin (256), command-line, console, log (31 entries) and script (128) tables, sets every script id to -1, fills evt_Ops and evt_Keywords and clears the globals; returns 1 */
int evt_Unload(EVTSCRIPT *sc);                                                          /* 0x42b410, frees sc's variable data, includes, variable and function tables, source, lines and stack, zeroes the slot and sets id -1; returns 0 for a null script, else 1 */
EVTBUILTIN *evt_FindBuiltin(char *name);                                                /* 0x42b540, the engine function with this name among the 256 builtins; 0 if none */
int evt_ImportCommand(char *name, int (*fn)(), int rettype, int nargs, ...);            /* 0x42b5b0, registers the engine function fn under name (at most 0x1f chars) with its return type and argument types from the varargs; reports a too long or duplicate name and a full table; returns 1 when registered, 0 on error */
int evt_StartScript(int unused1, int unused2, char *name);                              /* 0x42b6c0, loads and runs the script name and stores its id (or -1) in evt_LastId, which it returns; the two ints are unused */
void *evt_GetStartScript(void);                                                         /* 0x42b700, the start-script function (evt_StartScript), for the engine to run scripts */
void evt_ResetNoGroup(void);                                                            /* 0x42b710, resets evt_NoGroup to -1 */

#endif

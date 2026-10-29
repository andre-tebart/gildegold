
EVTSCRIPT * evt_Stack[32];  /* 0x145e1e0, evt_Cur of the callers of evt_Step */
int evt_Lines[0x800];  /* 0x145e260 */
char evt_Keywords[9][0x10];  /* 0x1460260, [0] unused: "while", "if", ... */
int evt_Loaded;  /* 0x14602f0 */
char evt_StrResult[0x100];  /* 0x1460300 */
char evt_StrArgs[8][0x60];  /* 0x1460400 */
char evt_Ops[30][5];  /* 0x1460700, [0] unused */
EVTVAR * evt_Globals;  /* 0x14607a0 */
int evt_NGlobals;  /* 0x14607a4 */
unsigned int evt_MaxGlobals;  /* 0x14607a8 */
int evt_FloatResult;  /* 0x14607ac */
EVTLOG * evt_Log;  /* 0x14607b0 */
char evt_Path[] = "x:\\engine\\gfx\\scripts\\";  /* 0x5f8330, the scripts directory */
int evt_NextId = 1;  /* 0x5f8434 */
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

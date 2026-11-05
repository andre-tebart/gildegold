/* evtc.c -- original file name unknown: script commands ("CallUserFunction(): Invalid userfunction").
   Extent in GildeGold.exe (inferred): starts at 0x40b580 right after the character code (ch.c)
   and ends at 0x40ba08 with the command registration evtc_Register. Functions are kept in their original
   order. Names are ours.
   unit of rea 8cef7d5b (src/game/evtc.c); boundaries in decl/text.dcl */
#include <stdlib.h>
#include <string.h>
#include "evtc.h"
#include "sv.h"
#include "tb.h"
#include "poker.h"

/* 0x40b580, random number 0 .. *n - 1 */
int evtc_Random(int *n)
{
    if (!*n)
        return 0;
    return rand() % *n;
}
/* 0x40b5a0, script command GetTime: the running time in milliseconds (d3_Time ticks * tb_Delay) */

int evtc_GetTime(void)
{
    return d3_Time * tb_Delay;
}
/* 0x40b5b0, starts a script in the caller's context */

int evtc_RunScript(char **name)
{
    EVTSCRIPT *sc = evt_Load(*name);

    if (!sc)
        return 0;
    sc->group = evt_Cur->group;
    sc->ownScene = evt_Cur->ownScene;
    sc->scene = evt_Cur->scene;
    evt_RunScript(sc);
    return (int)sc;
}
/* 0x40b610, the same with two arguments */

int evtc_RunScript2(char **name, int *a, int *b)
{
    EVTSCRIPT *sc = evt_Load(*name);

    if (!sc)
        return 0;
    sc->group = evt_Cur->group;
    sc->ownScene = evt_Cur->ownScene;
    sc->scene = evt_Cur->scene;
    evt_RunScriptArgs(sc, 2, *a, *b);
    return (int)sc;
}
/* 0x40b680, the same with one argument */

int evtc_RunScript1(char **name, int *a)
{
    EVTSCRIPT *sc = evt_Load(*name);

    if (!sc)
        return 0;
    sc->group = evt_Cur->group;
    sc->ownScene = evt_Cur->ownScene;
    sc->scene = evt_Cur->scene;
    evt_RunScriptArgs(sc, 1, *a);
    return (int)sc;
}
/* 0x40b6f0, script command StopScript: stops the script whose handle (evt_Load result) was passed; always
   returns 0 */

int evtc_StopScript(int *sc)
{
    evt_Stop((EVTSCRIPT *)*sc);
    return 0;
}
/* 0x40b710, script command Sleep: suspends the script for *t milliseconds (*t / 14 ticks, -1: forever); the
   waitStart tick is taken on the first call, the resumed calls re-arm the wait until it has passed */

int evtc_Wait(int *t)
{
    EVTSCRIPT *sc;

    if (evt_CurCall && evt_CurCall->fn == evtc_Wait) {
        sc = evt_Cur;
        if (*t != -1 && d3_Time >= (unsigned long)(*t / 14 + sc->waitStart))
            return 0;
        sc->waitfn = evtc_Wait;
        return 0;
    }
    evt_Cur->waitfn = evtc_Wait;
    evt_Cur->waitStart = d3_Time;
    return 1;
}
/* 0x40b7a0, script command FindScript: returns the script record of that name (evt_FindByName), 0 if no
   script of the name is registered */

int evtc_FindScript(char **name)
{
    return (int)evt_FindByName(*name);
}
/* 0x40b7b0, stops the other running scripts of the caller's group */

int evtc_StopOthers(void)
{
    int i;

    for (i = 0; i < 128; i++) {
        if (evt_Scripts[i].id != -1 && (evt_Scripts[i].flags & 1) && evt_Scripts[i].group == evt_Cur->group
            && evt_Scripts[i].id != evt_Cur->id)
            evt_Stop(&evt_Scripts[i]);
    }
    return 0;
}
/* 0x40b820, script command CallUserFunction: calls the given user function with one argument; a null
   function is a script error ("CallUserFunction(): Invalid userfunction") and returns 0, else 1 */

int evtc_CallUserFunction(EVTUSERFUNC1 *f, int *a)
{
    if (!*f) {
        evt_Error(evt_Cur, evt_Cur->pc, "CallUserFunction(): Invalid userfunction");
        return 0;
    }
    (*f)(*a);
    return 1;
}
/* 0x40b860, script command CallUserFunctionExtended: calls the given user function with two arguments; a
   null function is a script error ("CallUserFunctionExtended(): Invalid userfunction") and returns 0, else 1 */

int evtc_CallUserFunctionExt(EVTUSERFUNC2 *f, int *a, int *b)
{
    if (!*f) {
        evt_Error(evt_Cur, evt_Cur->pc, "CallUserFunctionExtended(): Invalid userfunction");
        return 0;
    }
    (*f)(*a, *b);
    return 1;
}
/* 0x40b8a0, registers the commands above (the last function of this file) */

void evtc_Register(void)
{
    evt_ImportCommand("ecmd_Dummy", (int (*)())ai_AlwaysTrue, 5, 0);
    evt_ImportCommand("Print", (int (*)())sv_ReturnZero, 5, 1, 6);
    evt_ImportCommand("PrintInt", (int (*)())sv_ReturnZero, 5, 1, 1);
    evt_ImportCommand("PrintFloat", (int (*)())sv_ReturnZero, 5, 1, 7);
    evt_ImportCommand("rnd", evtc_Random, 1, 1, 1);
    evt_ImportCommand("GetTime", evtc_GetTime, 1, 0);
    evt_ImportCommand("RunScript", evtc_RunScript, 1, 1, 6);
    evt_ImportCommand("RunScriptInt", evtc_RunScript1, 1, 2, 6, 1);
    evt_ImportCommand("RunScriptInt2", evtc_RunScript2, 1, 3, 6, 1, 1);
    evt_ImportCommand("RunScriptString", evtc_RunScript1, 1, 2, 6, 6);
    evt_ImportCommand("StopScript", evtc_StopScript, 5, 1, 1);
    evt_ImportCommand("Sleep", evtc_Wait, 5, 1, 1);
    evt_ImportCommand("FindScript", evtc_FindScript, 1, 1, 6);
    evt_ImportCommand("KillLocalScripts", evtc_StopOthers, 1, 0);
    evt_ImportCommand("CallUserFunction", evtc_CallUserFunction, 1, 2, 1, 1);
    evt_ImportCommand("CallUserFunctionExtended", evtc_CallUserFunctionExt, 1, 3, 1, 1, 1);
}

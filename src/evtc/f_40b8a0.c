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

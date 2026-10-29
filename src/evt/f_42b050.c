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
    memset(&evt_Globals, 0, 12);   /* evt_Globals, evt_NGlobals, evt_MaxGlobals: one struct? 3 stores late from xor eax,eax */
    return 1;
}

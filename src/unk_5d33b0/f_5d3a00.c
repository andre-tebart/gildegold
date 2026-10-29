/* 0x5d3a00, writes a save slot (161 B): param_1 unused, param_2 the two slot records (0x220 bytes each),
   param_3 the slot (-1 -> -1; 0 = AUTOSAVE, 1 = QUICKSAVE, any other value writes without a name
   check), param_4 the request record (the name at +9). The name must equal the slot's string
   ("QUICKSAVE" 0x624528 / "AUTOSAVE" 0x62451c, case-insensitive). A free slot (+8 == -1) gets the byte
   +0 = the slot and the 0x84 dwords of param_4 copied to +0x10. Returns the slot, -1 when taken / the
   name does not match.
   @deep meaning matches the original/Ghidra: the QUICKSAVE/AUTOSAVE name rules over param_4+9, param_3 0/1/-1 handling, the second case-insensitive compare, the slot = param_2 + param_3*0x220 free check at +8 == -1, the byte store and the 0x84-dword copy from param_4; the 100 code hunks are control-flow layout (the two strcmpi paths with the inline -1 returns at 0x5d3a46/0x5d3a56 and 0x5d3a6d) and register allocation (param_4 in edi vs esi, param_3 in ebx) only */

int ai_RegisterSaveSlot(int param_1, int param_2, int param_3, void *param_4)
{
    char *name;
    char *want;
    char *slot;
    int i;

    (void)param_1;
    name = (char *)param_4 + 9;
    if (_strcmpi(name, "QUICKSAVE") == 0) {
        param_3 = 1;
        want = "QUICKSAVE";
    } else if (_strcmpi(name, "AUTOSAVE") == 0) {
        param_3 = 0;
        want = "AUTOSAVE";
    } else if (param_3 == 0) {
        want = "AUTOSAVE";
    } else if (param_3 == 1) {
        want = "QUICKSAVE";
    } else if (param_3 == -1) {
        return -1;
    } else {
        goto write;
    }
    if (_strcmpi(name, want) != 0)
        return -1;
write:
    slot = (char *)(param_2 + param_3 * 0x220);
    if (*(int *)(slot + 8) != -1)
        return -1;
    *slot = (char)param_3;
    for (i = 0; i < 0x84; i++)
        ((int *)(slot + 0x10))[i] = ((int *)param_4)[i];
    return param_3;
}

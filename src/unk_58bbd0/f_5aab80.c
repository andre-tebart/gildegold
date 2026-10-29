/* 0x5aab80, the main window of the Domizil module, ROUGH SKELETON (5328 B): a huge frame (0x5de9c0 is the
   stack probe); the building record arg (the byte *in_stack_00001a50 of Ghidra) with the +1 field and the +0x3d
   index, the kind record 0x7e3f30 + kind * 0x24d (pcVar14) and the character record 0x770b80 + 0x6f25b8 * 0x21c;
   the entry array at r + 0x22c (0xc entries of 0x5c bytes, the count +0x231 + +0x232), up to three rows
   0x5aa0d0 of six entries; 0x5aa700 builds the worker list; the pump 0x5309a0 loop refreshes 0x5aa9a0, the
   keys 0x1468410 -0x33 / -0x35, the selected record (0x509e60 / 0x5aa1f0 / 0x5aa520), the AIMSG messages 7, 6,
   0x31 and 0x32 of the click dispatch (0x4c4b70, the clock 0x770ae0, the character 0x770b84) and the exit
   0x52d7a0. ROUGH: the Ghidra pseudocode lost most call arguments (a 0x1a50-byte frame); the arguments below are
   the ones the pseudocode shows, the nested dispatch is condensed to its calls */


int ai_PersonnelBook(int param_1, char *param_2)
{
    /* ROUGH: not written yet - see the comment above (the Ghidra pseudocode lost the call arguments) */
    return 0;
}

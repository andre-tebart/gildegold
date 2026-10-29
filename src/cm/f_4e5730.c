/* 0x4e5730, registers the scene kind: the run function (+0), the callbacks of the members of the kind 6 (+4) and of the others (+8), the check (+0x10) and
   the flag +0xc (the members answer one after the other) */

void ai_Unk4e5730(int kind, int run, int cb4, int cb8, int check, char flag)
{
    ai_Unk92c500[kind * 5] = run;
    ai_Unk92c504[kind * 5] = cb4;
    ai_Unk92c508[kind * 5] = cb8;
    ai_Unk92c510[kind * 5] = check;
    ai_Unk92c50c[kind * 20] = flag;
}

/* 0x5d0a40, a message callback (108 B), the twin of 0x5d09d0 with the trial step index 3: type 6 ->
   the text 0x5176d0 (0x9d) through 0x5bea00; other types -> the target 0x505df0 (param_2 + 4); no
   target -> 0; else 0x5d08c0 (the target, 3) and 1. */

int ai_Unk5d0a40(int param_1, int param_2)
{
    char text[0x100];
    int target;

    if (*(char *)(param_1 + 2) == 6) {
        txt_Format(text, (char *)0x9d);
        target = ai_Unk5bea00(param_1, text);
    } else {
        target = gm_FindChar(*(int *)(param_2 + 4));
    }
    if (target == 0)
        return 0;
    ai_Unk5d08c0(target, 3);
    return 1;
}

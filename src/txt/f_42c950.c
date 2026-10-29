/* 0x42c950, reads a "{rN}" random-text escape from p: the chosen variant (N + 9) is queued in
   txt_Random; on a syntax error it reports and queues the pointer itself.
   @deep behaviour only - grade.py code 18, fill 8; unitdiff 25 B / 18 asm lines (2026-10-26): the four stores and the branch structure are identical, the hunks differ in that the original reloads p from its home slot ([esp+0xc]/[esp+8]) where ours keeps it in esi, i.e. register allocation; readable met; reversed if polarity (2026-10-26, the store-p arm first) is 52 B / 24 lines, WORSE */

void txt_ParseRandomText(char *p)
{
    int n;

    if (!strncmp(p, "{r", 2)) {
        n = p[2] - '1';
        if (n >= 0 && n <= 9) {
            txt_Random[txt_Count] = n + 9;
            return;
        }
        err_Message("txt_ParseRandomText(): Syntax error...");
        txt_Random[txt_Count] = (int)p;
    } else
        txt_Random[txt_Count] = (int)p;
}

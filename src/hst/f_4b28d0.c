/* 0x4b28d0, leaked name (its message): replaces in s (in place) every word "_NAME-...-n" (two dashes, then
   digits) by its replacement (hst_ParseContext), with the list of the "[_SET_n" / "[_USE_n" prefix of text k
   @deep only allocation / scheduling left (347/343 instructions, 266 differing: 304 code + 14 regs + 6 fill hunks; no Q rules fail). Every hunk read against the disassembly: the frame is equal (both 'mov eax,0x1de0; call __chkstk'), the two AI_ZERO clears (buf 0x17d0, word 0x100), the G12_COPY_P copy of s into buf, row = hst_ParseCommandline_1st_Pass(k), the word loop ('_' opens a word with word[0] = *p and w = word + 1, a space inside a word is the syntax error, '-' stores and increments dashes with > 2 the error, dashes < 2 stores, the digits branch isdigit + the digit copy + AI_ZERO(rep) + *w = 0 + hst_ParseContext + the rep copy + *s++ = *p), the two tails (in && dashes < 2 error, in == 1 && dashes == 2 replacement) and *s = 0 are all present with the same constants, string formats and callees. The diff is the register rotation eax/ecx/edx (diffclass: regs 14, the rest code because the rotation is not consistent inside a hunk) plus the source-pointer spill (the original keeps s in [ebp+0x8] with a reload, ours holds it in a register across the loop) - vargen ren / named-local work. */

/* G12_COPY_P: G12_COPY from a pointer variable */
#define G12_COPY_P(dst, src, size) {                                \
    int _cn = (size); void *_cp = (dst);                            \
    __asm mov edi, _cp                                              \
    __asm mov esi, src                                              \
    __asm mov ebx, _cn                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle AI_ZERO_LABEL(__LINE__)                               \
    __asm rep movsb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep movsd                                                 \
    __asm AI_ZERO_LABEL(__LINE__): add ecx, ebx                     \
    __asm rep movsb                                                 \
}

int hst_ParseText_Fake_2nd_Pass(char *s, int k)
{
    char buf[0x17d0];
    char err[0x400];
    char rep[0x100];
    char word[0x100];
    G12ROW *row;
    int in, dashes;
    char *p, *w, *q;

    in = 0;
    dashes = 0;
    AI_ZERO(buf, 0x17d0);
    G12_COPY_P(buf, s, strlen(s));
    row = hst_ParseCommandline_1st_Pass(k);
    p = buf;
    while (*p != 0) {
        if (!in) {
            if (*p == '_') {
                AI_ZERO(word, 0x100);
                word[0] = *p;
                w = word + 1;
                in = 1;
                dashes = 0;
            } else
                *s++ = *p;
        } else if (*p == ' ') {
            sprintf(err, "hst_ParseText_Fake_2nd_Pass() failed: Syntax Error in Label %i < %s >",
                    k - hst_TextFirst - 1, word);
            return 0;
        } else if (*p == '-') {
            *w++ = '-';
            p++;
            if (++dashes > 2) {
                sprintf(err, "hst_ParseText_Fake_2nd_Pass() failed: Syntax Error in Label %i < %s >",
                        k - hst_TextFirst - 1, word);
                return 0;
            }
            continue;
        } else if (dashes < 2) {
            *w++ = *p;
        } else {
            if (dashes != 2)
                continue;
            if (!isdigit(*p)) {
                sprintf(err, "hst_ParseText_Fake_2nd_Pass() failed: Syntax Error in Label %i < %s >",
                        k - hst_TextFirst - 1, word);
                return 0;
            }
            while (isdigit(*p) && *p != 0)
                *w++ = *p++;
            AI_ZERO(rep, 0x100);
            *w = 0;
            if (!hst_ParseContext(word, rep, (G12ARG *)row)) {
                if (row != 0)
                    row->inUse = 0;
                return 0;
            }
            for (q = rep; *q != 0; q++)
                *s++ = *q;
            in = 0;
            dashes = 0;
            *s++ = *p;
        }
        p++;
    }
    if (in && dashes < 2) {
        sprintf(err, "hst_ParseText_Fake_2nd_Pass() failed: Syntax Error in Label %i < %s >",
                k - hst_TextFirst - 1, word);
        return 0;
    }
    if (in == 1 && dashes == 2) {
        AI_ZERO(rep, 0x100);
        *w = 0;
        if (!hst_ParseContext(word, rep, (G12ARG *)row)) {
            if (row != 0)
                row->inUse = 0;
            return 0;
        }
        for (q = rep; *q != 0; q++)
            *s++ = *q;
    }
    *s = 0;
    return 1;
}

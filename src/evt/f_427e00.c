/* 0x427e00 @deep B: hunks regs 12, code 4. asmdiff shows a pure esi/edi swap of n and end (xor esi,esi vs xor edi,edi; mov edi,[esp+0x14] vs mov esi; lea eax,[esi+edx] vs [edi+edx]); every instruction pair is identical after renaming esi/edi, only the xor sits between the two pushes. No meaning error; declaration order / p forms / &s[n] / ++n tried (notes/units/evt.md). */

char *evt_FindLogic(char *s, char *op, char *end)
{
    char *p = s;
    int n = 0;

    if (*s != '&' && s[1] != '&' && *s != '|' && s[1] != '|')
        for (; p < end && *p != ')' && *p != ';' && *p != '(' && *p != '{'; p++) {
            if ((*p == '&' && p[1] == '&') || (*p == '|' && p[1] == '|'))
                return s + n;
            n++;
        }
    return 0;
}

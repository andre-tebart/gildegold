/* 0x4ae160, the BUERGERMEISTER token of a history text (callback 0x601b10[0], name 0x601730[0]): the mayor
   (officeKind 0x0f); mode 1 checks args[k].id, otherwise the first living one is taken (mode 0 stores it in
   args[k]); out = fmt (0x4adf40) with its id; 0 if there is none
   @deep behaviour: one code hunk at 0x4ae1f7 - our 'if (c == 0) return 0;' after the mode==0 store is folded to the first return-0 tail (je back to 0x4ae19c), the original keeps its own inline copy (test esi,esi / jne 0x4ae201 / pop edi / xor eax,eax / pop esi / mov esp,ebp / pop ebp / ret); every other instruction (58 + 50) is equal. The meaning is read and right: the statement order is store-then-null-test (the swap puts c in esi / i in edx and breaks 30 instructions). Forms tried, one build each: if (c==0), if (!c), braces, if/else with empty then, 'if (c!=0) ; else return 0', while (c==0) return 0, do{}while(0), nested if (1), switch, check before the store, and dead statements between store and check (k=0, c=c, i=0) - all give the folded je; scratch reproductions in build/scratch show VC6 folds any later identical return block into the first, so the missing copy is original block layout/scheduling (deep's toolbox). Q1 name now has evidence (callback 0x601b10[0] = 0x4ae160, name 0x601730[0] BUERGERMEISTER), Q2 macro txt_FormatString -> txt_FormatString, description written */

/* txt_Format takes a text id; these callers pass a format string */
#define txt_FormatString ((void (*)(char *, char *, ...))txt_Format)

int ai_TokenMayor(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    AICHAR *c;
    int i, n1, n2;

    if (mode == 1) {
        c = gm_FindChar(args[k].id);
        if (c == 0 || c->health == 0 || c->officeKind != 0x0f)
            return 0;
    } else {
        i = 0;
        while (!(ai_IsLivingCharacter((unsigned short)i) && ai_CharsT[i].officeKind == 0x0f)) {
            if (++i >= 768)
                return 0;
        }
        c = &ai_CharsT[i];
        if (mode == 0)
            args[k].id = c->netId;
        if (c == 0)
            return 0;
    }
    n1 = (int)buf;
    AI_ZERO((char *)n1, 0x200);
    ai_SplitTokenFormat(fmt, buf, &n1, &n2);
    txt_FormatString(out, buf, c->id);
    return 1;
}

/* 0x4b21c0, leaked name (its messages): one replacement "[_NEWn_NAME...]": mode _NEW / _USE / _REL with
   slot n of ctx (ctx[n + 1].unk00 holds the replacement, ctx[0].unk00 the valid flag), then the callback of
   NAME; the error messages are written to a local buffer only
   @deep only allocation left plus two Q blocks. Meaning read equal hunk by hunk against the disassembly: the 3-mode loop over hst_Modes (0x601af0, 5-byte stride, 4-byte compare), AI_ZERO(buf, 0x100) (the original's rep stos alignment dance is exactly am.h's AI_ZERO), buf[0] = s[5] / atoi / s += 6 / the 0..7 slot check, the mode == 1 path (k = ctx[slot + 1].unk00), the 15-token loop over hst_TokenNames (0x601730, 0x40 stride, strlen + memcmp), s += strlen, the mode == 0 store ctx[slot + 1].unk00 = k, the two 'Unknown Replacement' error exits and the tail call hst_TokenCallbacks[k](mode, ctx, slot, s, out) + ctx[0].unk00 = 0 on failure. The whole 116 asm comes from ONE 4-byte frame difference: sub esp,0x10c vs 0x110 - ours gives AI_ZERO's _zn the dead parameter slot [ebp+0x8] and _zp [ebp-0xc], the original gives _zn [ebp-0xc] and _zp [ebp-0x10] (its s spill stays in [ebp+0x8]); that shifts buf from [ebp-0x10c] to [ebp-0x110] and every lea/push with it. Same AI_ZERO temp-slot problem as the ai_Token* @deep reasons. Also slot order: slot/mode at [ebp-0x8]/[ebp-0x4] here vs [ebp-0x4]/[ebp-0x8] in the original (diffclass: 2 slots hunks). Q2 block: G12ARG.unk00 (hst.h) - slot 0 holds the valid flag and slots 1..8 the token index, no reader names it, so a name would be invented. Q4 block: the 2 '*(int *)s == *(int *)hst_Modes[i]' 4-byte compares (a strncmp/memcmp spelling adds a call). */

int hst_KeyFirst = -1;  /* 0x601064, first entry of the key list (-1: none) */
int ai_Unk601068 = -1;  /* 0x601068, end of the key list */
int hst_KeyNext = -1;  /* 0x60106c, next entry to look at */
char hst_CommandNames[27][0x40] = { "FEST", "BELAGERUNG_START", "AUFSTAND", "BRAND", "WIRBELSTURM", "STADTKASSE", "VERMOEGEN_STEUER", "VERMOEGEN", "ANSEHEN_BEI_AMTSTRAEGERN", "KILL_PLAYER", "STRAFE_HINRICHTUNG", "STRAFE_KERKER", "AMTSTRAEGER_INVENTAR_PLUS", "GESETZ", "GESETZ_REL", "GLAUBENSWECHSEL", "AUFRUHR", "GILDENSITZE_BRACH", "NACHFRAGE", "SOELDNER_PLUENDERN", "SOELDNER_MARODIEREN", "FERNHANDEL_RAUBRITTER", "SPENDE_ANSEHEN", "BETEILIGUNG", "KOMMENTARE", "PEST", "INVENTAR_PLUS" };  /* 0x601070, the text command names ("FEST" .. "INVENTAR_PLUS") */
char hst_TokenNames[15][0x40] = { "BUERGERMEISTER", "BISCHOF", "RND_GILDENMEISTER", "RND_AMTSTRAEGERIN", "RND_AMTSTRAEGER", "RND_AMTSPERSON", "REICHSTER_EINWOHNER", "BESTES_WIRTSHAUS", "GELD", "STADTKASSE", "RND_KIRCHENBERUF", "RND_REICH", "RND_NPC_EINWOHNER", "RND_HANDELSHERR", "RND_SPIELER" };  /* 0x601730, the names of the text tokens (callback */
char hst_Modes[3][5] = { "_NEW", "_USE", "_REL" };  /* 0x601af0, the modes of a context reference: "_NEW", "_USE", "_REL" */
extern int (*hst_TokenCallbacks[15])(int mode, G12ARG *args, int k, char *fmt, char *out);

int hst_ParseContext(char *s, char *out, G12ARG *ctx)
{
    char buf[0x100];
    int slot, mode, k, i, r;

    slot = -1;
    mode = 4;
    if (s == 0 || out == 0)
        return 0;
    for (i = 0; i < 3; i++) {
        if (*(int *)s == *(int *)hst_Modes[i])
            mode = i;
    }
    AI_ZERO(buf, 0x100);
    if (mode != 4) {
        if (ctx == 0 || ctx[0].unk00 == 0) {
            sprintf(buf, "hst_ParseContext() failed: Wrong group reference");
            return 0;
        }
        buf[0] = s[5];
        slot = atoi(buf);
        s += 6;
        if (slot < 0 || slot >= 8) {
            sprintf(buf, "hst_ParseContext() failed: Wrong slot reference %i", slot);
            return 0;
        }
    }
    if (mode == 1) {
        k = ctx[slot + 1].unk00;
    } else {
        s++;
        for (k = 0; k < 15; k++) {
            if (memcmp(s, hst_TokenNames[k], strlen(hst_TokenNames[k])) == 0)
                break;
        }
        if (k >= 15) {
            sprintf(buf, "hst_ParseContext() failed: Unknown Replacement");
            return 0;
        }
        s += strlen(hst_TokenNames[k]);
        if (mode == 0)
            ctx[slot + 1].unk00 = k;
    }
    if (k >= 15) {
        sprintf(buf, "hst_ParseContext() failed: Unknown Replacement");
        return 0;
    }
    r = hst_TokenCallbacks[k](mode, ctx, slot, s, out);
    if (r == 0 && ctx != 0)
        ctx[0].unk00 = 0;
    return r;
}

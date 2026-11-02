/* hst.c -- the history texts (0x4adf40..0x4b39f0, after hmt.c): character predicates, the text tokens and text
   commands of a history text, its parser (the hst_* names are leaked by its error texts), the date labels and the
   yearly history dialogs. Its .data block starts at 0x601058. Names are ours except the hst_* ones.
   Open: the ai_Token* siblings still take AI_ZERO with its own _zp slot (frame 0x20c vs 0x208); AI_ZERO_AT plus
   the ai_CharInts pointer walk fixed this in ai_TokenRandomNpcInhabitant and is untried in the others.
   Open: G12OBJ repeats hmt.h's HMTOBJ (0x4e6ec0 / 0x4e6680 declared in both files); G12_B reads AICHAR bytes.
   Open: gm_FirstBuilding (0x50b010) returns unsigned char * (am.h, f3gm.c) while the callers here use AIBLD *.
   unit of rea 8cef7d5b (src/game/hst.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hst.h"
#include "txt.h"
typedef unsigned int SOCKET;            /* <winsock.h>, for net.h (as ai.c) */
#include "net.h"
#include "d2_types.h"                 /* D2WIN, d2_LastWindowPtr's record */
#include "data_views.h"
#include "ahm.h"
#include "amt.h"
#include "anm.h"
#include "cm.h"
#include "f3gm.h"
#include "gv.h"
#include "io_text.h"
#include "sv.h"
#include "tr.h"
#include "d2.h"
#include "gfx.h"
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, ai_Unk4d88c0 0x4d88c0 move it as a word / movzx) */
#define ai_GoodsT (*(AIGOOD **)&ai_Goods)  /* 0x91aaf0, ai_Goods (f3gm.c: char *) as the AIGOOD records it points to */
#define ai_BldsT (*(AIBLD **)&ai_Blds)  /* 0x889f74, ai_Blds (f3gm.c: char *) as the AIBLD records it points to */
#define ai_BTypesT (*(AIBTYPE **)&ai_BTypes)  /* 0x7e3f30, ai_BTypes (ahm.c: char *) as the AIBTYPE records it points to */
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
#define ai_TimeT (*(AITIME *)&ai_Time)  /* 0x770ae0, ai_Time (anm.c: char[14]) as the AITIME it is: this unit reads its fields */
#define gm_AddHealthS ((float (*)(unsigned short, int))gm_AddHealth)  /* 0x4fca40 (f3gm.c: unsigned id): @original this unit pushes the id as 16 bits (byte-matched ai_CommandKillPlayer / ai_Unk4b86c0) */
/* 0x4adf40, copies src to dst up to the second '-', '%' becomes " %" after a non-blank; *n1 = the characters
   read, *n2 = the length of the rest */
void ai_SplitTokenFormat(char *src, char *dst, int *n1, int *n2)
{
    char *p;
    char *d;
    char prev;
    int n;

    d = dst;
    n = 0;
    prev = ' ';
    for (p = src; *p != 0 && n < 2; p++) {
        if (*p == '-') {
            n++;
        } else {
            if (*p == '%') {
                if (prev == ' ') {
                    *d = '%';
                } else {
                    *d = ' ';
                    prev = *p;
                    d++;
                    *d = prev;
                }
            } else {
                prev = *p;
                *d = *p;
            }
            d++;
        }
    }
    *n1 = p - src;
    *n2 = strlen(p);
}
/* 0x4adfb0, 1 if id is in none of the four lists of hst_Lists */

int ai_NotInLists(int id)
{
    int i, j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 8; j++) {
            if (hst_Lists[i].e[j].id == id)
                return 0;
        }
    }
    return 1;
}
/* 0x4adff0, character id exists, lives and has kind < 10 */

int ai_IsLivingCharacter(unsigned short id)
{
    if (ai_CharsT[id].id != 0xffff && ai_CharsT[id].health > 0 && ai_CharsT[id].kind < 10)
        return 1;
    return 0;
}
/* 0x4ae030, ... and is a player (kind 6 / 7 / 5) */

int ai_IsLivingPlayer(unsigned short id)
{
    if (ai_CharsT[id].id != 0xffff && ai_CharsT[id].health > 0
        && (ai_CharsT[id].kind == 6 || ai_CharsT[id].kind == 7 || ai_CharsT[id].kind == 5))
        return 1;
    return 0;
}
/* 0x4ae080, ... kind < 10 and not a human player (6 / 7) */

int ai_IsLivingNonPlayer(unsigned short id)
{
    if (ai_CharsT[id].id != 0xffff && ai_CharsT[id].health > 0 && ai_CharsT[id].kind < 10
        && ai_CharsT[id].kind != 6 && ai_CharsT[id].kind != 7)
        return 1;
    return 0;
}
/* 0x4ae0d0, ... a human player (kind 6 / 7) */

int ai_IsLivingHuman(unsigned short id)
{
    if (ai_CharsT[id].id != 0xffff && ai_CharsT[id].health > 0
        && (ai_CharsT[id].kind == 6 || ai_CharsT[id].kind == 7))
        return 1;
    return 0;
}
/* 0x4ae120, ... a plain inhabitant (kind 0): the predicate of the RND_NPC_EINWOHNER token (0x4af8d0) */

int ai_IsLivingInhabitant(unsigned short id)
{
    if (ai_CharsT[id].id != 0xffff && ai_CharsT[id].health > 0 && ai_CharsT[id].kind == 0)
        return 1;
    return 0;
}

/* txt_Format takes a text id; these callers pass a format string */
#define txt_FormatString ((void (*)(char *, char *, ...))txt_Format)

/* 0x4ae160, the BUERGERMEISTER token of a history text (callback 0x601b10[0], name 0x601730[0]): the mayor
   (officeKind 0x0f); mode 1 checks args[k].id, otherwise the first living one is taken (mode 0 stores it in
   args[k]); out = fmt (0x4adf40) with its id; 0 if there is none
   @deep behaviour: one code hunk at 0x4ae1f7 - our 'if (c == 0) return 0;' after the mode==0 store is folded to the first return-0 tail (je back to 0x4ae19c), the original keeps its own inline copy (test esi,esi / jne 0x4ae201 / pop edi / xor eax,eax / pop esi / mov esp,ebp / pop ebp / ret); every other instruction (58 + 50) is equal. The meaning is read and right: the statement order is store-then-null-test (the swap puts c in esi / i in edx and breaks 30 instructions). Forms tried, one build each: if (c==0), if (!c), braces, if/else with empty then, 'if (c!=0) ; else return 0', while (c==0) return 0, do{}while(0), nested if (1), switch, check before the store, and dead statements between store and check (k=0, c=c, i=0) - all give the folded je; scratch reproductions in build/scratch show VC6 folds any later identical return block into the first, so the missing copy is original block layout/scheduling (deep's toolbox). Q1 name now has evidence (callback 0x601b10[0] = 0x4ae160, name 0x601730[0] BUERGERMEISTER), Q2 macro txt_FormatString -> txt_FormatString, description written */
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
/* 0x4ae280, the BISCHOF token of a history text (callback 0x601b10[1], name 0x601730[1]): a bishop (officeKind
   0x15 or 0x1a); otherwise as ai_TokenMayor */

int ai_TokenBishop(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    AICHAR *c;
    int i, n1, n2;

    if (mode == 1) {
        c = gm_FindChar(args[k].id);
        if (c == 0 || c->health == 0 || c->officeKind != 0x15)
            return 0;
    } else {
        i = 0;
        while (!(ai_IsLivingCharacter((unsigned short)i) && ai_CharsT[i].officeKind == 0x1a
                 || ai_CharsT[i].officeKind == 0x15)) {
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
/* 0x4ae3a0, the RND_GILDENMEISTER token of a history text (callback 0x601b10[2], name 0x601730[2]): a random
   character holding a guild master office (secondOffice 0x1e..0x21); fmt "...-n" selects 0 = anybody living,
   1 = no human player (default), 2 = a computer one, else a human one; mode 1 checks args[k].id, mode 0 stores
   the pick
   @deep behaviour: 65 code hunks left (was 175). Meaning fixes applied this session (all kept): c->health <= 0 in the mode 1 arm (the original jbe, not je), secondOffice >= 0x1e && <= 0x21 in all four predicates (the original compares 0x1e / 0x21, not > 0x1d / < 0x22), the sel 0 / sel 1 search loops written as 'for (n = 0x300; n != 0; n--) { if (found) break; i = (i + 1) % 0x300; } if (n == 0) return 0; pick = &ai_Chars[i];' (that gives the original's bottom test 'dec esi; test esi,esi; jne' and the inline return 0), and n2 no longer initialised to 0x200 so VC6 puts it in the dead fmt parameter slot (ai_SplitTokenFormat(fmt, buf, &n1, &n2) pushes lea ecx,[ebp+0x14] like the original) with AI_ZERO((char *)n1, 0x200) giving the 0x200 store to [_zn]. Remaining: (1) frame 0x20c vs 0x208 - VC6 materialises the AI_ZERO _zp temp in its own slot (-0xc) and reads it for edi, the original reads n1's slot directly (mov edi,[ebp-0x4]); tried AI_ZERO(n1,..), AI_ZERO(buf,..), AI_ZERO((char *)n1,..) - all give the temp slot, so the dst form/coalescing is a slot-allocation decision; (2) the sel == 2 branch: ours pick in esi / n in edi, the original pick in edi / n in esi (30 regs hunks); (3) the tail after 'c = pick; if (c == 0) return 0;' - ours folds it to an earlier return-0 block (lea esi; test esi,esi; mov eax,esi) where the original keeps lea eax; test eax,eax / jne / mov eax,edi; test eax,eax (the same folded-return-tail effect as ai_TokenMayor's @deep reason). Q1 name has evidence (callback 0x601b10[2] = 0x4ae3a0, name 0x601730[2] RND_GILDENMEISTER), description written, Q2 clean */

int ai_TokenRandomGuildMaster(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    int n1, n2;
    int sel;
    int i, n;
    AICHAR *c, *pick;

    n1 = (int)buf;
    AI_ZERO((char *)n1, 0x200);
    ai_SplitTokenFormat(fmt, buf, &n1, &n2);
    if (n2 != 1 || (sel = atoi(fmt + n1)) < 0 || sel > 2)
        sel = 1;
    if (mode == 1) {
        c = gm_FindChar(args[k].id);
        if (c == 0 || c->health <= 0 || c->secondOffice == 0)
            return 0;
    } else {
        if (sel == 0) {
            i = util_RandomU(0x300);
            for (n = 0x300; n != 0; n--) {
                if (ai_IsLivingCharacter((unsigned short)i) && ai_CharsT[i].secondOffice >= 0x1e
                    && ai_CharsT[i].secondOffice <= 0x21)
                    break;
                i = (i + 1) % 0x300;
            }
            if (n == 0)
                return 0;
            pick = &ai_CharsT[i];
        } else if (sel == 1) {
            i = util_RandomU(0x300);
            for (n = 0x300; n != 0; n--) {
                if (ai_IsLivingNonPlayer((unsigned short)i) && ai_CharsT[i].secondOffice >= 0x1e
                    && ai_CharsT[i].secondOffice <= 0x21)
                    break;
                i = (i + 1) % 0x300;
            }
            if (n == 0)
                return 0;
            pick = &ai_CharsT[i];
        } else {
            pick = 0;
            i = util_RandomU(0x300);
            for (n = 0x300; n != 0; n--) {
                if (pick == 0 && ai_IsLivingNonPlayer((unsigned short)i) && ai_CharsT[i].secondOffice >= 0x1e
                    && ai_CharsT[i].secondOffice <= 0x21) {
                    pick = &ai_CharsT[i];
                } else if (ai_IsLivingHuman((unsigned short)i) && ai_CharsT[i].secondOffice >= 0x1e
                           && ai_CharsT[i].secondOffice <= 0x21) {
                    pick = &ai_CharsT[i];
                    break;
                }
                i = (i + 1) % 0x300;
            }
        }
        c = pick;
        if (c == 0)
            return 0;
        if (mode == 0)
            args[k].id = c->netId;
    }
    txt_FormatString(out, buf, c->id);
    return 1;
}

#define G12_B(i, o) (((unsigned char *)&ai_CharsT[i])[o])
#define G12_TITLED(i) (ai_CharsT[i].officeKind != 0 && ai_CharsT[i].officeKind != 0x0f && ai_CharsT[i].officeKind != 0x1a \
                       && ai_CharsT[i].officeKind != 0x15)

/* 0x4ae600, the RND_AMTSTRAEGERIN token of a history text (callback 0x601b10[3], name 0x601730[3]): a character
   with byte 9 == 1, an ordinary title (G12_TITLED) and in none of the lists of 0x4adfb0; the selection as in
   ai_TokenRandomGuildMaster
   @deep behaviour: far draft, 73 code hunks left (was 100; meaning fixes of this session applied: n2 no longer initialised to 0x200, so VC6 puts it in the dead fmt parameter slot like the original, AI_ZERO((char *)n1, 0x200), and the sel 0 / sel 1 search loops as 'for (n = 0x300; n != 0; n--) { if (found) break; i = (i + 1) % 0x300; } if (n == 0) return 0; pick = &ai_Chars[i];' - the same fixes that took ai_TokenRandomGuildMaster from 175 to 65 code hunks). Remaining: the same three issues as that function's @deep reason - (1) frame 0x20c vs 0x208: our AI_ZERO _zp temp gets its own slot (-0xc) and is read for edi where the original reads n1's slot (-0x4) (tried AI_ZERO(n1,..), AI_ZERO(buf,..), AI_ZERO((char *)n1,..)); (2) the register/layout work of the three selection loops and the tail after 'c = pick; if (c == 0) return 0;' (the folded return tail); (3) the G12_B/G12_TITLED macro expansions vs the original's order. Q1 name has evidence (callback 0x601b10[3] = 0x4ae600, name 0x601730[3] RND_AMTSTRAEGERIN), description written */
int ai_TokenRandomOfficeHolderFemale(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    int n1, n2;
    int sel;
    int i, n;
    AICHAR *c, *pick;

    n1 = (int)buf;
    AI_ZERO((char *)n1, 0x200);
    ai_SplitTokenFormat(fmt, buf, &n1, &n2);
    if (n2 != 1 || (sel = atoi(fmt + n1)) < 0 || sel > 2)
        sel = 1;
    if (mode == 1) {
        c = gm_FindChar(args[k].id);
        if (c == 0 || c->health == 0 || c->officeKind == 0)
            return 0;
    } else {
        if (sel == 0) {
            i = util_RandomU(0x300);
            for (n = 0x300; n != 0; n--) {
                if (G12_B(i, 9) == 1 && ai_IsLivingCharacter((unsigned short)i) && ai_NotInLists(ai_CharsT[i].netId)
                    && G12_TITLED(i))
                    break;
                i = (i + 1) % 0x300;
            }
            if (n == 0)
                return 0;
            pick = &ai_CharsT[i];
        } else if (sel == 1) {
            i = util_RandomU(0x300);
            for (n = 0x300; n != 0; n--) {
                if (G12_B(i, 9) == 1 && ai_IsLivingNonPlayer((unsigned short)i) && ai_NotInLists(ai_CharsT[i].netId)
                    && G12_TITLED(i))
                    break;
                i = (i + 1) % 0x300;
            }
            if (n == 0)
                return 0;
            pick = &ai_CharsT[i];
        } else {
            pick = 0;
            i = util_RandomU(0x300);
            for (n = 0x300; n != 0; n--) {
                if (G12_B(i, 9) == 1) {
                    if (pick == 0 && ai_IsLivingNonPlayer((unsigned short)i) && G12_TITLED(i)) {
                        if (ai_NotInLists(ai_CharsT[i].netId))
                            pick = &ai_CharsT[i];
                    } else if (ai_IsLivingHuman((unsigned short)i) && ai_NotInLists(ai_CharsT[i].netId)
                               && G12_TITLED(i)) {
                        pick = &ai_CharsT[i];
                        break;
                    }
                }
                i = (i + 1) % 0x300;
            }
        }
        c = pick;
        if (c == 0)
            return 0;
        if (mode == 0)
            args[k].id = c->netId;
    }
    txt_FormatString(out, buf, c->id);
    return 1;
}
/* 0x4ae900, the same as 0x4ae600 for a woman (byte 9 == 0) @deep behaviour: far draft, 95 code hunks left (was more; this session's unit-wide fixes applied: n2 no longer initialised so it takes the dead fmt parameter slot, AI_ZERO((char *)n1, 0x200), the sel 0/1 loops as 'for (n = 0x300; n != 0; n--) { if (found) break; ... } if (n == 0) return 0; pick = &ai_Chars[i];' - the same idiom that took ai_TokenRandomGuildMaster 175 -> 65 hunks). Remaining: the frame 0x20c vs 0x208 (_zp temp materialised in its own slot instead of reading n1's -0x4; tried AI_ZERO(n1,..), AI_ZERO(buf,..), AI_ZERO((char *)n1,..)), the G12_B/G12_TITLED/ai_NotInLists evaluation order in the three predicates, and the c = pick return-tail fold - all as written in ai_TokenRandomGuildMaster's @deep reason. Q1 name has evidence (callback 0x601b10[4] = 0x4ae900, name 0x601730[4] RND_AMTSTRAEGER) */

int ai_TokenRandomOfficeHolder(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    int n1, n2;
    int sel;
    int i, n;
    AICHAR *c, *pick;

    n1 = (int)buf;
    AI_ZERO((char *)n1, 0x200);
    ai_SplitTokenFormat(fmt, buf, &n1, &n2);
    if (n2 != 1 || (sel = atoi(fmt + n1)) < 0 || sel > 2)
        sel = 1;
    if (mode == 1) {
        c = gm_FindChar(args[k].id);
        if (c == 0 || c->health == 0 || c->officeKind == 0)
            return 0;
    } else {
        if (sel == 0) {
            i = util_RandomU(0x300);
            for (n = 0x300; n != 0; n--) {
                if (G12_B(i, 9) == 0 && ai_IsLivingCharacter((unsigned short)i) && G12_TITLED(i)
                    && ai_NotInLists(ai_CharsT[i].netId))
                    break;
                i = (i + 1) % 0x300;
            }
            if (n == 0)
                return 0;
            pick = &ai_CharsT[i];
        } else if (sel == 1) {
            i = util_RandomU(0x300);
            for (n = 0x300; n != 0; n--) {
                if (G12_B(i, 9) == 0 && ai_IsLivingNonPlayer((unsigned short)i) && G12_TITLED(i)
                    && ai_NotInLists(ai_CharsT[i].netId))
                    break;
                i = (i + 1) % 0x300;
            }
            if (n == 0)
                return 0;
            pick = &ai_CharsT[i];
        } else {
            pick = 0;
            i = util_RandomU(0x300);
            for (n = 0x300; n != 0; n--) {
                if (G12_B(i, 9) == 0) {
                    if (pick == 0 && ai_IsLivingNonPlayer((unsigned short)i) && G12_TITLED(i)) {
                        if (ai_NotInLists(ai_CharsT[i].netId))
                            pick = &ai_CharsT[i];
                    } else if (ai_IsLivingHuman((unsigned short)i) && ai_NotInLists(ai_CharsT[i].netId)
                               && G12_TITLED(i)) {
                        pick = &ai_CharsT[i];
                        break;
                    }
                }
                i = (i + 1) % 0x300;
            }
        }
        c = pick;
        if (c == 0)
            return 0;
        if (mode == 0)
            args[k].id = c->netId;
    }
    txt_FormatString(out, buf, c->id);
    return 1;
}
/* 0x4aec00, the same as 0x4ae600 for anybody (no sex test); mode 2 gives no token @deep behaviour: far draft, 77 code hunks left (unit-wide frame and search-loop fixes of this session applied, as in ai_TokenRandomGuildMaster's @deep reason). Remaining: the AI_ZERO _zp temp slot (frame 0x20c vs 0x208), the predicate evaluation order (G12_TITLED / ai_NotInLists / the living helpers) and the c = pick return-tail fold. Q1 name has evidence (callback 0x601b10[5] = 0x4aec00, name 0x601730[5] RND_AMTSPERSON) */

int ai_TokenRandomOfficePerson(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    int n1, n2;
    int sel;
    int i, n;
    AICHAR *c, *pick;

    n1 = (int)buf;
    AI_ZERO((char *)n1, 0x200);
    ai_SplitTokenFormat(fmt, buf, &n1, &n2);
    if (n2 != 1 || (sel = atoi(fmt + n1)) < 0 || sel > 2)
        sel = 1;
    if (mode == 2)
        return 0;
    if (mode == 1) {
        c = gm_FindChar(args[k].id);
        if (c == 0 || c->health == 0 || c->officeKind == 0)
            return 0;
    } else {
        if (sel == 0) {
            i = util_RandomU(0x300);
            for (n = 0x300; n != 0; n--) {
                if (ai_IsLivingCharacter((unsigned short)i) && G12_TITLED(i) && ai_NotInLists(ai_CharsT[i].netId))
                    break;
                i = (i + 1) % 0x300;
            }
            if (n == 0)
                return 0;
            pick = &ai_CharsT[i];
        } else if (sel == 1) {
            i = util_RandomU(0x300);
            for (n = 0x300; n != 0; n--) {
                if (ai_IsLivingNonPlayer((unsigned short)i) && G12_TITLED(i) && ai_NotInLists(ai_CharsT[i].netId))
                    break;
                i = (i + 1) % 0x300;
            }
            if (n == 0)
                return 0;
            pick = &ai_CharsT[i];
        } else {
            pick = 0;
            i = util_RandomU(0x300);
            for (n = 0x300; n != 0; n--) {
                if (pick == 0 && ai_IsLivingNonPlayer((unsigned short)i) && G12_TITLED(i)) {
                    if (ai_NotInLists(ai_CharsT[i].netId))
                        pick = &ai_CharsT[i];
                } else if (ai_IsLivingHuman((unsigned short)i) && G12_TITLED(i)
                           && ai_NotInLists(ai_CharsT[i].netId)) {
                    pick = &ai_CharsT[i];
                    break;
                }
                i = (i + 1) % 0x300;
            }
        }
        c = pick;
        if (c == 0)
            return 0;
        if (mode == 0)
            args[k].id = c->netId;
    }
    txt_FormatString(out, buf, c->id);
    return 1;
}

/* below every character's worth (gm_GetWorth): the "no candidate yet" start value of the richest searches */
#define HST_NO_WORTH (-10000000)

/* 0x4aef10, the REICHSTER_EINWOHNER token of a history text (callback 0x601b10[6], name 0x601730[6]): the
   richest character (0x50a640) who passes 0x4adff0 / 0x4adfb0; fmt "...-n" selects 0 = anybody, 1 = men
   (byte 9 == 0 .. sic: byte 9 == (n != 1)), 2 = the other sex
   @deep behaviour: far draft, 177 code hunks left; read: the sel parse (sel 0..2, sel != 0 sets sex/filt) and the mode 1/0/2 paths match the reading, the difference is the frame and slot layout (ours 0x214, original 0x210: our locals best/sex/filt/n1/n2 get separate slots, the original keeps filt in [ebp-4]=0x200, sex in [ebp-0x10], n1 in [ebp-8] and the second out in the dead fmt slot [ebp+0x14]) plus the loop and the worth-comparison instruction order; the AI_ZERO dst here is filt (a variable) and still materialises. Q3 fixed this session by naming the -10000000 start value HST_NO_WORTH; Q1 name has evidence (callback 0x601b10[6] = 0x4aef10, name 0x601730[6] REICHSTER_EINWOHNER). Leads left for deep: the slot allocation of filt/sex/best (a slot-allocation problem like ai_TokenRandomGuildMaster's _zp temp) and the loop form */
int ai_TokenRichestInhabitant(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    int n1, n2;
    int sel, i, v;
    int best = HST_NO_WORTH;
    unsigned int sex = 0;
    unsigned int filt = 0x200;
    AICHAR *c;

    n1 = (int)buf;
    AI_ZERO((char *)n1, filt);
    ai_SplitTokenFormat(fmt, buf, &n1, &n2);
    if (n2 == 1) {
        sel = atoi(fmt + n1);
        if (sel >= 0 && sel < 3) {
            if (sel != 0)
                sex = sel != 1;
            filt = sel != 0;
            if (mode != 2) {
                c = 0;
                if (mode == 1) {
                    c = gm_FindChar(args[k].id);
                    if (c != 0 && c->health != 0) {
                        txt_FormatString(out, buf, c->id);
                        return 1;
                    }
                } else {
                    for (i = 0; i < 0x300; i++) {
                        if (ai_IsLivingCharacter((unsigned short)i) && (filt == 0 || G12_B(i, 9) == sex)
                            && (v = gm_GetWorth((unsigned short)i)) > best && ai_NotInLists(ai_CharsT[i].netId)) {
                            c = &ai_CharsT[i];
                            best = v;
                        }
                    }
                    if (c != 0) {
                        if (mode == 0)
                            args[k].id = c->netId;
                        txt_FormatString(out, buf, c->id);
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
/* 0x4af090, text token for the building of type 0x16 with the highest 0x509b10 rating */

int ai_TokenBestInn(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    int n1, n2;
    int best = -1, v;
    AIBLD *b, *pick = 0;

    if (mode == 2)
        return 0;
    if (mode == 1) {
        AI_4FEA50_4((int *)&pick, 0, 0, args[k].id);
        if (pick == 0)
            return 0;
    } else {
        for (b = gm_BuildQuery(1, 5, 0x16); b != 0; b = gm_BuildNext()) {
            v = gm_EquipmentValue((void *)b, b->level);
            if (v > best) {
                pick = b;
                best = v;
            }
        }
        if (pick == 0)
            return 0;
        if (mode == 0)
            args[k].id = pick->id;
    }
    n1 = (int)buf;
    AI_ZERO((char *)n1, 0x200);
    ai_SplitTokenFormat(fmt, buf, &n1, &n2);
    txt_FormatString(out, buf, pick);
    return 1;
}
/* 0x4af1b0, text token (mode 2 only): fmt "...-n" gives n percent of the wealth (0x50a640) of the character
   args[k], as a money text (0x5085b0)
   @deep behaviour: 49 code hunks. Read: the guard (mode == 2, args != 0, k != 8, gm_FindChar, ai_IsLivingCharacter) matches the original branch for branch, and so does v = gm_GetWorth(c->id). The differences are (1) the frame 0x204 vs 0x200 - ours spills c to [ebp-4], the original keeps it in eax; (2) the ai_SplitTokenFormat prologue slots: the original keeps n1 = 0x200 in the mode parameter slot [ebp+8], n2 = (int)buf in the args slot [ebp+0xc], v in the k slot [ebp+0x10], and AI_ZERO's _zp reads the n2 slot directly - ours allocates [_zn]/[ebp-4] and writes 0x200 twice; (3) the money tail: the original does fild n1 / fmul 0.01f (0x5eb7f0) / fimul v with al = ai_OwnPlayer, mov cl,al, then pushes 3 dwords (ecx = zero-extended ai_OwnPlayer, eax = the atoi result, then the __ftol (0x5de518) result) before gm_ConvertMoney with 'add esp,8' - a 3-push/2-pop shape (the leaked dword is dropped by mov esp,ebp) that a plain 2-arg gm_ConvertMoney((int)(n1*0.01f*v), ai_OwnPlayer) does not produce; the source form that yields it is unknown (the argument evaluation order and the __ftol call site). Q2 fixed this session: gm_ConvertMoney/gm_ConvertMoney -> gm_ConvertMoney (project-wide, 60 references, no byte change); Q1 name has evidence (callback 0x601b10[8] = 0x4af1b0, name 0x601730[8] GELD) */

int ai_TokenMoney(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    int n1, n2;
    int v;
    AICHAR *c;

    if (mode == 2 && args != 0 && k != 8 && (c = gm_FindChar(args[k].id)) != 0
        && ai_IsLivingCharacter(c->id)) {
        v = gm_GetWorth(c->id);
        n1 = 0x200;
        n2 = (int)buf;
        AI_ZERO(buf, n1);
        ai_SplitTokenFormat(fmt, buf, &n1, &n2);
        n1 = atoi(fmt + n1);
        txt_FormatString(out, buf, gm_ConvertMoney((int)(n1 * 0.01f * v), ai_OwnPlayer));
        return 1;
    }
    return 0;
}
/* 0x4af2c0, text token: n percent of the count of the first goods of type 9 in the stock (object 0x115) of
   building 0x50b010(1), as a money text
   @deep behaviour: 31 code hunks; the reading is right (building 1 = class 1 via 0x50b010(1), the object 0x115 of its stock, the first goods of type 9, its count in unk0e, then n1 percent of it as money) and the money tail is the same unsolved gm_ConvertMoney call shape as ai_TokenMoney's @deep reason. Q2: gm_FirstBuilding (0x50b010, f3gm.c) has a name with evidence ('the first building of the class n', class 1 = the type 0xf) but its canonical definition returns unsigned char * while all its callers (am.c 5, cm.c, hst.c, this unit) use it as AIBLD *: the rename needs that return type unified first (type debt noted in f3gm.c's layout section), so it is a blocked shared-header/type decision; AIOBJ.unk0e (the count field) and AIOBJ.unk14 (the same list field gm_Query walks) have no evidence yet. Q1 name has evidence (callback 0x601b10[9] = 0x4af2c0, name 0x601730[9] STADTKASSE) */

int ai_TokenCityTreasury(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    int n1, n2, cnt;
    int v = 0;
    AIBLD *b;
    AIOBJ *o;

    b = gm_FirstBuilding(1);
    if (b == 0)
        return 0;
    o = gm_Query(b->objList, 2, 6, 0, 0x115);
    if (o == 0)
        return 0;
    for (o = gm_Query(o->unk14, 1, 5); o != 0; o = gm_QueryNext()) {
        if (ai_GoodsT[o->id].type == 9) {
            v = o->unk0e;
            break;
        }
    }
    cnt = 0x200;
    n2 = (int)buf;
    AI_ZERO((char *)n2, cnt);
    ai_SplitTokenFormat(fmt, buf, &n1, &n2);
    n1 = atoi(fmt + n1);
    txt_FormatString(out, buf, gm_ConvertMoney((int)(n1 * 0.01f * v), ai_OwnPlayer));
    return 1;
}

/* 0x4af3f0, the same as 0x4aec00 for a character whose byte 0x164 is 13..18 */
#define G12_AGED(i) (G12_B(i, 0x164) > 12 && G12_B(i, 0x164) < 19)

/* 0x4af3f0 @deep behaviour: far draft, 105 code hunks, 20 slot hunks (the RND_KIRCHENBERUF token, callback 0x601b10[10] = 0x4af3f0, name 0x601730[10]): the reading matches (a random character with a church profession) but the frame/slot layout, the loops and the folded return tails remain as in ai_TokenRandomGuildMaster's @deep reason */
int ai_TokenRandomChurchProfession(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    int n1, n2;
    int sel;
    int i, n;
    AICHAR *c, *pick;

    n1 = (int)buf;
    AI_ZERO((char *)n1, 0x200);
    ai_SplitTokenFormat(fmt, buf, &n1, &n2);
    if (n2 != 1 || (sel = atoi(fmt + n1)) < 0 || sel > 2)
        sel = 1;
    if (mode == 1) {
        c = gm_FindChar(args[k].id);
        if (c == 0 || c->health == 0)
            return 0;
    } else {
        if (sel == 0) {
            i = util_RandomU(0x300);
            for (n = 0x300; n != 0; n--) {
                if (ai_IsLivingCharacter((unsigned short)i) && G12_AGED(i) && ai_NotInLists(ai_CharsT[i].netId))
                    break;
                i = (i + 1) % 0x300;
            }
            if (n == 0)
                return 0;
            pick = &ai_CharsT[i];
        } else if (sel == 1) {
            i = util_RandomU(0x300);
            for (n = 0x300; n != 0; n--) {
                if (ai_IsLivingNonPlayer((unsigned short)i) && G12_AGED(i) && ai_NotInLists(ai_CharsT[i].netId))
                    break;
                i = (i + 1) % 0x300;
            }
            if (n == 0)
                return 0;
            pick = &ai_CharsT[i];
        } else {
            pick = 0;
            i = util_RandomU(0x300);
            for (n = 0x300; n != 0; n--) {
                if (pick == 0 && ai_IsLivingNonPlayer((unsigned short)i) && G12_AGED(i)) {
                    if (ai_NotInLists(ai_CharsT[i].netId))
                        pick = &ai_CharsT[i];
                } else if (ai_IsLivingHuman((unsigned short)i) && G12_AGED(i)
                           && ai_NotInLists(ai_CharsT[i].netId)) {
                    pick = &ai_CharsT[i];
                    break;
                }
                i = (i + 1) % 0x300;
            }
        }
        c = pick;
        if (c == 0)
            return 0;
        if (mode == 0)
            args[k].id = c->netId;
    }
    txt_FormatString(out, buf, c->id);
    return 1;
}
/* 0x4af6c0, text token for one of the five richest characters (0x50a640) passing a predicate: fmt "...-n"
   0 = 0x4adff0, 2 = 0x4ae0d0, else 0x4ae080
   @deep behaviour: far draft, 173 code hunks (the RND_REICH token, callback 0x601b10[11] = 0x4af6c0, name 0x601730[11]): the reading matches (the top-5 worth lists of the unit note) but the frame/slot layout, the loop form and the c = pick return-tail fold remain - the same three issues as ai_TokenRandomGuildMaster's @deep reason; the -10000000 list start values are HST_NO_WORTH now (Q3) */

int ai_TokenRandomRich(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    int n1, n2;
    int score[5];
    int found;
    unsigned short ids[5];
    int (*pred)(unsigned short);
    int i, j, v, t;
    unsigned short u;
    AICHAR *c;

    score[0] = HST_NO_WORTH;
    score[1] = HST_NO_WORTH;
    score[2] = HST_NO_WORTH;
    score[3] = HST_NO_WORTH;
    score[4] = HST_NO_WORTH;
    found = 0;
    ids[0] = 0xffff;
    ids[1] = 0xffff;
    ids[2] = 0xffff;
    ids[3] = 0xffff;
    ids[4] = 0xffff;
    if (mode == 2)
        return 0;
    n1 = (int)buf;
    AI_ZERO((char *)n1, 0x200);
    ai_SplitTokenFormat(fmt, buf, &n1, &n2);
    pred = ai_IsLivingNonPlayer;
    if (n2 == 1 && (v = atoi(fmt + n1)) >= 0 && v < 3) {
        if (v == 0)
            pred = ai_IsLivingCharacter;
        else if (v != 1)
            pred = ai_IsLivingHuman;
    }
    if (mode == 1) {
        c = gm_FindChar(args[k].id);
        if (c == 0 || c->health == 0)
            return 0;
    } else {
        for (i = 0; i < 0x300; i++) {
            if (pred((unsigned short)i) && (v = gm_GetWorth((unsigned short)i)) > score[4]
                && ai_NotInLists(ai_CharsT[i].netId)) {
                score[4] = v;
                found |= 1;
                ids[4] = ai_CharsT[i].id;
                for (j = 3; j >= 0; j--) {
                    t = score[j];
                    if (t < score[j + 1]) {
                        u = ids[j];
                        score[j] = score[j + 1];
                        score[j + 1] = t;
                        ids[j] = ids[j + 1];
                        ids[j + 1] = u;
                    }
                }
            }
        }
        if (found == 0)
            return 0;
        i = (unsigned short)util_RandomU(5);
        c = &ai_CharsT[ids[i]];
        if (mode == 0)
            args[k].id = ai_CharsT[ids[i]].netId;
    }
    txt_FormatString(out, buf, c->id);
    return 1;
}
/* 0x4af8d0, text token for the first character in the lists of 0x4adfb0 who passes 0x4ae120; fmt "...-n" as in
   0x4aef10 (sex filter) */

int ai_TokenRandomNpcInhabitant(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    int n1, n2;
    int sel, i;
    unsigned int sex = 0;
    unsigned int filt;
    AICHAR *c = 0;
    int *q;

    n1 = (int)buf;
    AI_ZERO_AT(n1, 0x200);
    ai_SplitTokenFormat(fmt, buf, &n1, &n2);
    if (n2 == 1) {
        sel = atoi(fmt + n1);
        if (sel >= 0 && sel <= 2) {
            filt = 1;
            if (sel == 0)
                filt = 0;
            else
                sex = sel != 1;
            if (mode == 1) {
                c = gm_FindChar(args[k].id);
                if (c == 0)
                    return 0;
                if (c->health <= 0)
                    return 0;
            } else {
                i = 0;
                for (q = ai_CharInts[0]; (int)q < (int)((char *)ai_CharInts + 768 * 0x21c); q += 0x87) {
                    if (ai_NotInLists(*q)) {
                        c = (AICHAR *)(q - 1);
                        if (filt == 0 || ((unsigned char *)q)[5] == sex) {
                            if (ai_IsLivingInhabitant((unsigned short)i))
                                break;
                            c = 0;
                        }
                    }
                    i++;
                }
                if (c == 0)
                    return 0;
                if (mode == 0)
                    args[k].id = c->netId;
            }
            txt_FormatString(out, buf, c->id);
            return 1;
        }
    }
    return 0;
}
/* 0x4afa20, one of the five richest characters (0x50a640) passing 0x4adff0 whose byte 0x164 is
   19..69 but not 31..33, 40..45, 52..57
   @deep behaviour: far draft, 82 code hunks (the RND_HANDELSHERR token, callback 0x601b10[13] = 0x4afa20, name 0x601730[13]): the selection reads (richness of the merchants) but the frame/slot layout and the folded return tails remain as in ai_TokenRandomGuildMaster's @deep reason; the -10000000 start values are HST_NO_WORTH now (Q3) */

int ai_TokenRandomMerchant(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    int n1, n2;
    int score[5];
    int found;
    unsigned short ids[5];
    unsigned char g;
    int i, j, v, t;
    unsigned short u;
    AICHAR *c;

    score[0] = HST_NO_WORTH;
    score[1] = HST_NO_WORTH;
    score[2] = HST_NO_WORTH;
    score[3] = HST_NO_WORTH;
    score[4] = HST_NO_WORTH;
    found = 0;
    ids[0] = 0xffff;
    ids[1] = 0xffff;
    ids[2] = 0xffff;
    ids[3] = 0xffff;
    ids[4] = 0xffff;
    if (mode == 2)
        return 0;
    n1 = (int)buf;
    AI_ZERO((char *)n1, 0x200);
    ai_SplitTokenFormat(fmt, buf, &n1, &n2);
    if (mode == 1) {
        c = gm_FindChar(args[k].id);
        if (c == 0 || c->health == 0)
            return 0;
    } else {
        for (i = 0; i < 0x300; i++) {
            g = G12_B(i, 0x164);
            if (g > 18 && g < 70 && (g < 31 || g > 33) && (g < 40 || g > 45) && (g < 52 || g > 57)
                && ai_IsLivingCharacter((unsigned short)i) && (v = gm_GetWorth((unsigned short)i)) > score[4]
                && ai_NotInLists(ai_CharsT[i].netId)) {
                score[4] = v;
                found |= 1;
                ids[4] = ai_CharsT[i].id;
                for (j = 3; j >= 0; j--) {
                    t = score[j];
                    if (t < score[j + 1]) {
                        u = ids[j];
                        score[j] = score[j + 1];
                        score[j + 1] = t;
                        ids[j] = ids[j + 1];
                        ids[j + 1] = u;
                    }
                }
            }
        }
        if (found == 0)
            return 0;
        i = (unsigned short)util_RandomU(5);
        c = &ai_CharsT[ids[i]];
        if (mode == 0)
            args[k].id = ai_CharsT[ids[i]].netId;
    }
    txt_FormatString(out, buf, c->id);
    return 1;
}
/* 0x4afc30, text token for a random character passing 0x4ae0d0 / 0x4adfb0, searched with a random step
   (3, 5, 7 or 11)
   @deep behaviour: far draft, 70 code hunks (the RND_SPIELER token, callback 0x601b10[14] = 0x4afc30, name 0x601730[14]): the reading matches (a random living player 6/7) but the frame/slot layout, the loop form and the folded return tails remain as in ai_TokenRandomGuildMaster's @deep reason */

int ai_TokenRandomPlayer(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    int n1, n2;
    int steps[4];
    int step, i, n;
    AICHAR *c, *pick = 0;

    if (mode == 2)
        return 0;
    n1 = (int)buf;
    AI_ZERO((char *)n1, 0x200);
    ai_SplitTokenFormat(fmt, buf, &n1, &n2);
    if (mode == 1) {
        c = gm_FindChar(args[k].id);
        if (c == 0 || c->health == 0)
            return 0;
    } else {
        steps[0] = 3;
        steps[1] = 5;
        steps[2] = 7;
        steps[3] = 11;
        do {
            step = steps[(unsigned short)util_RandomU(4)];
            i = (unsigned short)util_RandomU(0x300);
            for (n = 0x300; n != 0; n--) {
                if (ai_IsLivingHuman((unsigned short)i) && ai_NotInLists(ai_CharsT[i].netId)) {
                    pick = &ai_CharsT[i];
                    break;
                }
                i = (step + i) % 0x300;
            }
        } while (i + 1 < 0x300);
        if (pick == 0)
            return 0;
        c = pick;
        if (mode == 0)
            args[k].id = pick->netId;
    }
    txt_FormatString(out, buf, c->id);
    return 1;
}
/* 0x4afdd0, the FEST text command (callback 0x601b50[0], name 0x601070[0]) "-n" (n > 0): sends message 0x53
   for the character ai_PlayerChar, today 12:00
   @deep B: 38 code hunks / 23 differing instructions of 54 vs 53 - one scheduling region, meaning read and equal (type 0x53, to = ((AICHAR *)ai_PlayerChar)->netId, TMX(msg) = NOWX, fromId -1, flags 1, byte 0 at 0x58, hour 12, min 0, cm_SendLongMessage): the original hoists ai_Time's first dword above the msg zeroing loop and stores the TMX copy a,c,b,d with the '&msg' push before fromId/flags/car/hour/min, ours stores b,c,a,d and pushes after car/min. Tried, one build each: source order (TMX before msg.to moves the hoist but stores a,b,d,c, 29 differing - reverted), msg.car as int (no change), typed netId (byte-neutral). spell: width hint at 0x4afdf6 is the hoisted dword read of the TMX slot. Needs vargen perm/mask rounds (store order + hoisted load). Q4 left: (1) the TMX/NOWX macro (am.h deref cast) - a typed view needs an AITIMEX member on AITIME in ai.h; both spellings tried (struct AITIME{union{...;AITIMEX x;};} and union AITIME{AITIMEX x; struct{...};}) regress ai_Unk483a60 in ai.c from byte to the [esp+0x10]/[esp+0x18] slot swap (the same regression the ai_StartBuildingWork note records for an AIMSG char view) - reverted, shared-header decision; (2) *(char *)&msg.car = 0: the byte at AIMSG+0x58 has no evidenced name (G6MSG/G8MSG call it unk58, a Q2 placeholder; every other sender writes the int car there), so a typed view would invent a name. Q1 fixed by the rename (callback 0x601b50[0], name 0x601070[0] FEST), Q2/Q3/Q5/Q6/Q7 pass. */

int ai_CommandFest(int a, int b, char *fmt)
{
    AIMSG msg;

    if (*fmt != '-')
        return 0;
    if (atoi(fmt + 1) > 0) {
        memset(&msg, 0, sizeof(msg));
        msg.type = 0x53;
        msg.to = ((AICHAR *)ai_PlayerChar)->netId;
        TMX(msg) = NOWX;
        msg.fromId = -1;
        msg.flags = 1;
        *(char *)&msg.car = 0;
        msg.time.hour = 12;
        msg.time.min = 0;
        cm_SendLongMessage(&msg);
    }
    return 1;
}
/* 0x4afe80, text command: sends message 0x7f to every councillor (kind 6 / 7), today 8:00 */

int ai_Unk4afe80(void)
{
    AIMSG msg;
    int i;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x7f;
    TMX(msg) = NOWX;
    msg.fromId = -1;
    msg.time.hour = 8;
    msg.time.min = 0;
    msg.flags = 1;
    for (i = 0; i < 0x300; i++) {
        if (ai_CharsT[i].kind == 6 || ai_CharsT[i].kind == 7) {
            msg.to = ai_CharsT[i].netId;
            msg.car = ai_CharsT[i].netId;
            cm_SendLongMessage(&msg);
        }
    }
    return 1;
}
/* 0x4aff30, text command: sends message 0x7d (arg 1) for the character ai_PlayerChar, today 8:00 */

int ai_Unk4aff30(void)
{
    AIMSG msg;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x7d;
    msg.to = *(int *)((char *)ai_PlayerChar + 4);
    TMX(msg) = NOWX;
    msg.fromId = -1;
    msg.flags = 1;
    msg.car = 1;
    msg.time.hour = 8;
    msg.time.min = 0;
    cm_SendLongMessage(&msg);
    return 1;
}

/* the 0x4e / 0x50 event messages (the BRAND / WIRBELSTURM commands): an AIMSG (0xf8 bytes) with the event's
   value at 0x9c (1000 in both; the same dword is a name / a flag byte in the other long-message kinds) */
typedef struct G12EVENTMSG {
    char pad00[0x9c];
    int value;                          /* 0x9c */
    char padA0[0x58];
} G12EVENTMSG;

/* 0x4affc0, the BRAND text command (callback 0x601b50[3], name 0x601070[3]): sends message 0x4e (value 1000) for
   the character ai_PlayerChar, today 7:00
   @deep byte already (the event value at 0x9c via the G12EVENTMSG view, fixed this session; AIMSG.unka0 at 0xa0 was the wrong offset). Q4 only left: the TMX/NOWX macro of am.h is a deref-cast macro; a typed view needs an AITIMEX member on AITIME in ai.h and both spellings tried (struct AITIME{union{...;AITIMEX x;};} and union AITIME{AITIMEX x; struct{...};}) regress ai_Unk483a60 in ai.c from byte to the [esp+0x10]/[esp+0x18] slot swap (same regression as ai_StartBuildingWork's note for an AIMSG char view) - reverted, shared-header decision. Q1 evidence: callback 0x601b50[3], name 0x601070[3] BRAND; Q2/Q3/Q5/Q6/Q7 pass. */
int ai_CommandFire(void)
{
    AIMSG msg;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x4e;
    msg.to = *(int *)((char *)ai_PlayerChar + 4);
    msg.fromId = -1;
    msg.car = -1;
    msg.flags = 1;
    ((G12EVENTMSG *)&msg)->value = 1000;
    TMX(msg) = NOWX;
    msg.time.hour = 7;
    cm_SendLongMessage(&msg);
    return 1;
}
/* 0x4b0050, the WIRBELSTURM text command (callback 0x601b50[4], name 0x601070[4]), the same as ai_CommandFire:
   message 0x50 (value 1000, flags 2), today 16:00
   @deep byte already (the event value at 0x9c via the G12EVENTMSG view, as ai_CommandFire). Q4 only left: the TMX/NOWX macro (am.h deref cast); the typed AITIMEX view on AITIME in ai.h regresses ai_Unk483a60 from byte to the [esp+0x10]/[esp+0x18] slot swap (documented in ai_CommandFire's reason and ai_StartBuildingWork's note) - shared-header decision. Q1 has evidence (callback 0x601b50[4], name 0x601070[4] WIRBELSTURM), Q2/Q3/Q5/Q6/Q7 pass. */

int ai_CommandTornado(void)
{
    AIMSG msg;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x50;
    msg.to = *(int *)((char *)ai_PlayerChar + 4);
    msg.fromId = -1;
    msg.car = -1;
    msg.flags = 2;
    ((G12EVENTMSG *)&msg)->value = 1000;
    TMX(msg) = NOWX;
    msg.time.hour = 16;
    cm_SendLongMessage(&msg);
    return 1;
}
/* 0x4b00e0, the STADTKASSE text command (callback 0x601b50[5], name 0x601070[5]) "-PLUS_n" / "-MINUS_n":
   moves n percent of the count of the first goods of type 9 in the stock (object 0x115) of building 0x50b010(1)
   to or from that stock (0x4c4830)
   @deep B near: 184/184 instructions, was 96 code hunks (ours 416 vs 445 bytes). The big win this session: the guard as an early return 'if (*fmt != '-') return 0;' plus a block for the sign/name tables (the original puts its return-0 block inline at the top; the if(...) {body} return 0; spelling puts it after and shifted everything). Left, 3 hunks, 8 differing instructions: (1) slots at 0x4b01a6: the original keeps n (the atoi result) in the dead fmt parameter slot [esp+0x30] and v (g->unk0e) in sign's dead slot [esp+0x10], ours swaps them; tried int i,s,v,n; v declared first; v declared inside the 'if (n > 0)' block - all byte-identical to the current form; (2) code at 0x4b0238 and (3) order at 0x4b0269 are the same swap in the fpu pair (original fild n / fmul 0.01f / fimul v, ours fild v / fimul n / fmul 0.01f). Slot/fpu allocation -> vargen perm rounds. Q2 blocked: gm_FirstBuilding's canonical definition returns unsigned char * while every caller (am.c, cm.c, hst.c, f3gm.c) uses AIBLD * (the type debt noted in ai_TokenCityTreasury's @deep reason) - a shared-header/type decision; AIOBJ.unk0e (the stock count) and AIOBJ.unk14 (the object list gm_Query walks) have no role evidence in the sources. Q1 has evidence (callback 0x601b50[5], name 0x601070[5] STADTKASSE), Q3/Q5/Q6/Q7 pass. */

int ai_CommandCityTreasury(int a, int b, char *fmt)
{
    if (*fmt != '-')
        return 0;
    {
        int sign[2] = { -1, 1 };
        char name[2][6] = { "MINUS", "PLUS" };
        int i, s, n, v;
        AIBLD *bld;
        AIOBJ *o, *g;

        fmt++;
        for (i = 0; i < 2; i++) {
            if (memcmp(fmt, name[i], strlen(name[i])) == 0)
                break;
        }
        if (i >= 2)
            return 0;
        s = sign[i];
        fmt += strlen(name[i]);
        if (*fmt != '_')
            return 0;
        n = atoi(fmt + 1);
        if (n > 0) {
            bld = gm_FirstBuilding(1);
            if (bld == 0)
                return 0;
            o = gm_Query(bld->objList, 2, 6, 0, 0x115);
            if (o == 0)
                return 0;
            for (g = gm_Query(o->unk14, 1, 5); g != 0; g = gm_QueryNext()) {
                if (ai_GoodsT[g->id].type == 9)
                    break;
            }
            if (g == 0)
                return 1;
            v = g->unk0e;
            if (v > 0) {
                if (s == -1)
                    cm_SendMoveGoodsUnchecked(s, o->objId, 0, (int)(n * 0.01f * v));
                else if (s == 1)
                    cm_SendMoveGoodsUnchecked(o->objId, -1, 0, (int)(n * 0.01f * v));
            }
        }
        return 1;
    }
}
/* 0x4b02a0, the VERMOEGEN_STEUER text command (callback 0x601b50[6], name 0x601070[6]) "-n": every character
   passing ai_IsLivingCharacter pays n percent of his wealth (0x50a640) into the stock (object 0x115) of building
   0x50b010(1)
   @deep B: 31 code hunks / 80 instructions, all shape, meaning read and equal (bld = gm_FirstBuilding(1), the stock object 0x115 of gm_Query, the '-n' guard, b = atoi, f = b*0.01f, the 0..0x300 loop over ai_IsLivingCharacter with gm_GetWorth and cm_SendMoveGoodsUnchecked(o->objId, netId, 0, (int)(b*f))). The first hunk is the frame: the original opens with push ecx (a 4-byte local before the loop, popped at every return), ours with push ebx only, so every slot after it is shifted by 4 (4b02cf / 4b02e6 code hunks are the same statements at [esp+0x10]/[esp+0x14] vs [esp+0x14]/[esp+0x18]) and 4b0319 is the same rotation (slots). Tried this session: nothing source-level yet - the float f and int b are the only locals; the 4-byte push-ecx slot is a VC6 allocation choice (vargen / slot building), not a meaning error. Q2 blocked: gm_FirstBuilding's canonical definition returns unsigned char * while every caller uses AIBLD * (type debt in f3gm.c, as in ai_CommandCityTreasury's reason) - shared-header decision. Q1 has evidence (callback 0x601b50[6], name 0x601070[6] VERMOEGEN_STEUER), Q3/Q4/Q5/Q6/Q7 pass. */

int ai_CommandWealthTax(int a, int b, char *fmt)
{
    AIBLD *bld;
    AIOBJ *o;
    float f;
    int i;

    bld = gm_FirstBuilding(1);
    if (bld == 0)
        return 0;
    o = gm_Query(bld->objList, 2, 6, 0, 0x115);
    if (o == 0)
        return 0;
    if (*fmt != '-')
        return 0;
    b = atoi(fmt + 1);
    f = b * 0.01f;
    for (i = 0; i < 0x300; i++) {
        if (ai_IsLivingCharacter((unsigned short)i)) {
            b = gm_GetWorth(ai_CharsT[i].id);
            cm_SendMoveGoodsUnchecked(o->objId, ai_CharsT[i].netId, 0, (int)(b * f));
        }
    }
    return 1;
}
/* 0x4b0360, the VERMOEGEN text command (callback 0x601b50[7], name 0x601070[7]) "-PLUS_n" / "-MINUS_n" for the
   character args[k]: gives or takes n percent of 0x509e60 (0x4c4830) */

int ai_CommandWealth(G12ARG *args, int k, char *fmt)
{
    int sign[2] = { -1, 1 };
    char name[2][6] = { "MINUS", "PLUS" };
    int i, n, v;
    float f;
    AICHAR *c;

    if (args != 0 && k < 8 && (c = gm_FindChar(args[k].id)) != 0 && *fmt == '-') {
        fmt++;
        for (i = 0; i < 2; i++) {
            if (memcmp(fmt, name[i], strlen(name[i])) == 0)
                break;
        }
        if (i < 2) {
            fmt += strlen(name[i]);
            if (*fmt == '_') {
                n = atoi(fmt + 1);
                f = n * 0.01f;
                v = (int)(gm_GetGoodsValue(c) * f);
                if (sign[i] == -1)
                    cm_SendMoveGoodsUnchecked(-1, c->netId, 0, v);
                else
                    cm_SendMoveGoodsUnchecked(c->netId, -1, 0, v);
                return 1;
            }
        }
    }
    return 0;
}
/* 0x4b04a0, the ANSEHEN_BEI_AMTSTRAEGERN text command (callback 0x601b50[8], name 0x601070[8])
   "-PLUS_n" / "-MINUS_n": changes the relation (0x4c4b00) of every character with a title (byte 0x166 or 0x169)
   to the character args[k] by 2n */

int ai_CommandOfficeStanding(G12ARG *args, int k, char *fmt)
{
    AICHAR *c;

    if (args != 0 && k < 8 && (c = gm_FindChar(args[k].id)) != 0 && *fmt == '-') {
        int sign[2] = { -1, 1 };
        char name[2][6] = { "MINUS", "PLUS" };
        int i, n;

        fmt++;
        for (i = 0; i < 2; i++) {
            if (memcmp(fmt, name[i], strlen(name[i])) == 0)
                break;
        }
        if (i < 2) {
            fmt += strlen(name[i]);
            if (*fmt == '_') {
                n = atoi(fmt + 1) * sign[i] * 2;
                for (i = 0; i < 0x300; i++) {
                    if (ai_IsLivingCharacter((unsigned short)i) && (G12_B(i, 0x166) != 0 || G12_B(i, 0x169) != 0))
                        cm_SendRelations(c->netId, ai_CharsT[i].netId, n, 1);
                }
                return 1;
            }
        }
    }
    return 0;
}
/* 0x4b05e0, the KILL_PLAYER text command (callback 0x601b50[9], name 0x601070[9]): lowers the health
   (0x4fc820) of the character args[k] below zero (gm_AddHealth with -(x + 1)) */

int ai_CommandKillPlayer(G12ARG *args, int k)
{
    AICHAR *c;

    if (args != 0 && k < 8 && (c = gm_FindChar(args[k].id)) != 0) {
        gm_AddHealthS(c->id, (int)-(gm_CharacterHealth((short *)c) + 1.0f));
        return 1;
    }
    return 0;
}

#pragma pack(push, 1)
typedef struct G12XMSG {                /* 0x100 bytes, given to 0x4c4fa0 (a vote / an execution) */
    int unk00;                          /* 0x00 */
    int unk04;                          /* 0x04 */
    unsigned char type;                 /* 0x08 */
    char pad09[3];
    int unk0c;                          /* 0x0c, the player's character */
    int unk10;                          /* 0x10 */
    int unk14;                          /* 0x14 */
    AITIME time;                        /* 0x18 */
    unsigned char unk26;                /* 0x26 */
    char pad27[9];
    unsigned char n;                    /* 0x30, entries of ids */
    char pad31[3];
    int ids[16];                        /* 0x34 */
    char pad74[8];
    unsigned char kind;                 /* 0x7c */
    unsigned char unk7d;                /* 0x7d */
    char pad7e[0x82];
} G12XMSG;
#pragma pack(pop)

int cm_SendNewParty(void *p);                                                   /* 0x4c4fa0 */

/* 0x4b0640, the STRAFE_HINRICHTUNG text command (callback 0x601b50[10], name 0x601070[10])
   "-KANONE" / "-GALGEN" / "-KOEPFEN" / "-SHAUFEN": starts the execution (kind 3, 0, 2, 4) of the character args[k]
   in 24 hours at 10:00, with the judge, ai_JudgeCatholic / ai_JudgeProtestant and up to 12 councillors
   @deep B: 153 code hunks, 189 vs 187 instructions, frame 0x158 vs 0x16c (20 B): the nested blocks name[4][8] / kinds[4] / list[16] get other slots in ours, so every [esp+N] after the prologue is shifted (first hunk 4b0640: sub esp,0x158 vs 0x16c, args at [esp+0x160]/[esp+0x15c] vs [esp+0x174]/[esp+0x170]); the constants, callees and statements were read against the original (the KANONE/GALGEN/KOEPFEN/SHAUFEN table and kinds 3/0/2/4, the kind list, msg.type 5 with unk14 -1 / unk26 0 / unk7d 1 and the judge + councillor netIds, the call to 0x4c4fa0) and match. Block-scope slot reuse -> match-deep's slotmap/vargen work. Q2 blocked: ai_Unk4c4fa0 (0x4c4fa0, cm.c) has no evidenced name in its unit (the note calls it only 'the vote / execution message' sender); G12XMSG.unk0c / unk14 / unk26 / unk7d are written here only, no reader in any source names them. Q4 blocked: the TMX/NOWX macro (the AITIMEX view on AITIME regresses ai_Unk483a60 - see ai_CommandFire's reason). Q1 done (callback 0x601b50[10], name 0x601070[10] STRAFE_HINRICHTUNG), Q3/Q5/Q6/Q7 pass. */
int ai_CommandExecution(G12ARG *args, int k, char *fmt)
{
    G12XMSG msg;
    AICHAR *c;
    int kinds[4];
    int i, n, id;
    unsigned char kind;

    c = gm_FindChar(args[k].id);
    if (c == 0 || !ai_IsLivingCharacter(c->id))
        return 0;
    {
        char name[4][8] = { "KANONE", "GALGEN", "KOEPFEN", "SHAUFEN" };

        n = 4;
        kinds[3] = 4;
        kinds[0] = 3;
        kinds[1] = 0;
        kinds[2] = 2;
        if (*fmt != '-')
            return 0;
        fmt++;
        for (i = 0; i < 4; i++) {
            if (memcmp(fmt, name[i], strlen(name[i])) == 0)
                break;
        }
        if (i >= 4)
            return 0;
    }
    kind = (unsigned char)kinds[i];
    {
        int list[16];

        msg.unk14 = -1;
        list[1] = -1;
        msg.type = 5;
        msg.unk26 = 0;
        msg.kind = kind;
        msg.unk7d = 1;
        msg.unk0c = ai_CharsT[ai_PlayerId].netId;
        list[0] = c->netId;
        list[2] = ai_JudgeCatholic->netId;
        list[3] = ai_JudgeProtestant->netId;
        for (i = 0; i < 0x300; i++) {
            id = ai_CharsT[i].netId;
            if (id != c->netId && ai_IsLivingNonPlayer((unsigned short)i)) {
                if (G12_B(i, 0x166) != 0)
                    list[1] = id;
            } else if (n < 16 && ai_IsLivingHuman((unsigned short)i)) {
                list[n++] = id;
            }
        }
        if (list[1] == -1)
            list[1] = list[n-- - 1];
        if (n > 0)
            memcpy(msg.ids, list, n * 4);
    }
    for (i = n; i < 16; i++)
        msg.ids[i] = -1;
    msg.n = (unsigned char)n;
    TMX(msg) = NOWX;
    gm_AddTime(&msg.time, 0x18, 0, 0);
    msg.unk26 = 0x22;
    msg.time.hour = 10;
    cm_SendNewParty(&msg);
    return 1;
}
/* 0x4b08e0, the STRAFE_KERKER text command (callback 0x601b50[11], name 0x601070[11]) "-n": sets the byte field
   0x1b1 of the character args[k] to n (message 0x4c3ba0) */

int ai_CommandPrison(G12ARG *args, int k, char *fmt)
{
    char v = 0;
    AICHAR *c;

    if (*fmt == '-') {
        v = (char)atoi(fmt + 1);
        if (args != 0 && k < 8 && (c = gm_FindChar(args[k].id)) != 0) {
            cm_BeginFieldBlock((void *)c, c->netId);
            cm_AddFieldValues(1, 1, 0x1b1, &v);
            cm_SendFieldDeltas();
            return 1;
        }
    }
    return 0;
}

int gm_FindObjectProto(char *s);                                                       /* 0x4fe900 */
void cm_SendExSellObject(int a, int b, short c, int d, unsigned char e, int f);           /* 0x4c4880 */

/* 0x4b0950, the AMTSTRAEGER_INVENTAR_PLUS text command (callback 0x601b50[12], name 0x601070[12]) "-xxxNAME":
   0x4c4880 from every titled character (byte 0x166 or 0x169) with the value of 0x4fe900(fmt + 4) */
int ai_CommandOfficeInventoryPlus(int a, int b, char *fmt)
{
    char *s;
    int v, i;

    if (*fmt != '-')
        return 0;
    s = fmt + 1;
    if (strlen(s) < 4)
        return 0;
    s += 3;
    tolower((int)(s + 1));      /* sic: a pointer */
    v = gm_FindObjectProto(s);
    if (v == 0)
        return 0;
    for (i = 0; i < 0x300; i++) {
        if (ai_IsLivingCharacter((unsigned short)i) && (G12_B(i, 0x166) != 0 || G12_B(i, 0x169) != 0))
            cm_SendExSellObject(ai_CharsT[i].netId, -1, v, 1, ai_OwnPlayer, 0);
    }
    return 1;
}


/* 0x4b09f0, the GESETZ text command (callback 0x601b50[13], name 0x601070[13])
   "-RECHTSPRECHUNG_HAERTE_n" / "-AEMTEREINKOMMEN_n": sets the AI decision 0 / 15 to n, clamped to its range
   (refused out of range when its byte 1 is set) */
int ai_CommandLaw(int a, int b, char *fmt)
{
    if (*fmt != '-')
        return 0;
    {
        char name[2][32] = { "RECHTSPRECHUNG_HAERTE", "AEMTEREINKOMMEN" };
        int kinds[2];
        int i, n, k;
        AIDEC d;

        kinds[0] = 0;
        kinds[1] = 0xf;
        fmt++;
        for (i = 0; i < 2; i++) {
            if (memcmp(fmt, name[i], strlen(name[i])) == 0)
                break;
        }
        if (i >= 2)
            return 0;
        fmt += strlen(name[i]);
        if (*fmt != '_')
            return 0;
        n = atoi(fmt + 1);
        k = kinds[i];
        if (!gv_GetTableRecord(k, &d))
            return 0;
        if ((n < d.min || n > d.max) && ((unsigned char *)&d)[1] != 0)
            return 0;
        gv_SetTableRecord(0, k, max(min(d.max, n), d.min));
        return 1;
    }
}
/* 0x4b0b50, the GESETZ_REL text command (callback 0x601b50[14], name 0x601070[14])
   "-PLUS_<decision>_n" / "-MINUS_...": moves the AI decision 0 / 15 (names of ai_CommandLaw) by n, clamped to its
   range. Sic: after the sign word it skips the length of the sign name with its own index.
   @deep B near: 204/204 instructions, only the kinds[2] slot left - the original keeps the array at [esp+0x70]/[esp+0x74], ours at [esp+0x68]/[esp+0x6c] (the 8-byte shift also moves the kinds[1] = 0xf store from before the byte store to after it: code hunk 4b0be7; the imul reads [esp+0x74] vs [esp+0x6c], slots hunk 4b0c48). Everything else is equal, including the max(min(d.max, i), d.min) operand order fixed this session and the early-return guard restructure (was a draft with the whole body in an if-block). Tried: int kinds[2] moved after AIDEC d - byte-identical, reverted. Slot allocation of a local array -> vargen / slotmap work. Q1 has evidence (callback 0x601b50[14], name 0x601070[14] GESETZ_REL), Q2/Q3/Q4/Q5/Q6/Q7 pass. */

int ai_CommandLawChange(int s, int b, char *fmt)
{
    if (*fmt != '-')
        return 0;
    {
        char sname[2][6] = { "MINUS", "PLUS" };
        char name[2][32] = { "RECHTSPRECHUNG_HAERTE", "AEMTEREINKOMMEN" };
        int kinds[2];
        int i, k;
        AIDEC d;

        s = -1;
        kinds[0] = 0;
        kinds[1] = 0xf;
        fmt++;
        for (i = 0; i < 2; i++) {
            if (memcmp(fmt, sname[i], strlen(sname[i])) == 0)
                break;
        }
        if (i >= 2)
            return 0;
        if (i == 1)
            s = i;
        fmt += strlen(name[i]);
        if (*fmt != '_')
            return 0;
        fmt++;
        for (i = 0; i < 2; i++) {
            if (memcmp(fmt, name[i], strlen(name[i])) == 0)
                break;
        }
        if (i >= 2)
            return 0;
        fmt += strlen(name[i]);
        if (*fmt != '_')
            return 0;
        k = kinds[i];
        if (!gv_GetTableRecord(k, &d))
            return 0;
        i = atoi(fmt + 1) * s + d.cur;
        gv_SetTableRecord(0, k, max(min(d.max, i), d.min));
        return 1;
    }
}
/* 0x4b0d40, the GLAUBENSWECHSEL text command (callback 0x601b50[15], name 0x601070[15])
   "-KATHOLISCH_pp_m" / "-EVANGELISCH_pp_m": message 0x81 for pp percent (1..80) of the characters of the religion
   byte 0x0c that the sign word names (KATHOLISCH = 1 protestant, EVANGELISCH = 0 catholic, the two words of
   f3init.c's INIT_FIELD_RELIGION) in m groups (1..8, at most a third of them), today 8:00; the message's ch is the
   religion of the counted characters.
   Match-base session: the name array order {KATHOLISCH, EVANGELISCH} (the original copies KATHOLISCH from
   ds:0x601c20 into the lower slot [esp+0x14], so the KATHOLISCH word counts the 1s), the float product
   (float)cnt * p (the original's fild + fimul, an integer imul has no fimul and grows the frame by 8), the
   d1-before-d2 declaration order, the two zero stores as memset of a 4-byte buffer, and msg.time = ai_Time
   (byte-identical to am.h's TMX(msg) = NOWX, which is Q4-blocked) are the fixes; 165 -> 6 differing
   instructions. Q1 has evidence, Q2..Q7 pass.
   @deep only scheduling left: 3 order hunks of 226 instructions (6 differing) - the for-init i = 0 of both digit loops sits after fmt++ (4b0e0e) / after the *fmt == 0 test (4b0e52) in ours and before them in the original (4b0e05, 4b0e4d), and the TMX word store (msg.ids[0], [esp+0x7c]) is kept until after fromId/flags/car/n in the original (4b0fa9) but emitted right after the TMX dwords in ours (4b0f82). Meaning read equal against the disassembly: the name array is {KATHOLISCH, EVANGELISCH} (ds:0x601c20 into [esp+0x14]), the product is float (fild + fimul p), the d1/d2 slot pair and the msg fields match. Store-order / hoist work: vargen perm rounds or a source shape that keeps the word store late. */

int ai_CommandFaithChange(int a, int b, char *fmt)
{
    char name[2][16] = { "KATHOLISCH", "EVANGELISCH" };
    char d1[4];
    char d2[4];
    int rel, i, p, m, n, cnt;
    AIMSG msg;

    i = 0;
    if (*fmt != '-')
        return 0;
    fmt++;
    for (; i < 2; i++) {
        if (memcmp(fmt, name[i], strlen(name[i])) == 0)
            break;
    }
    if (i == 0)
        rel = 1;
    else if (i == 1)
        rel = 0;
    else
        return 0;
    fmt += strlen(name[i]);
    if (*fmt != '_')
        return 0;
    fmt++;
    memset(d1, 0, sizeof(d1));
    memset(d2, 0, sizeof(d2));
    for (i = 0; ; i++, fmt++) {
        if (*fmt == 0)
            return 0;
        if (*fmt == '_' || i >= 3)
            break;
        if (*fmt < '0' || *fmt > '9')
            return 0;
        d1[i] = *fmt;
    }
    if (*fmt == 0 || i == 0)
        return 0;
    fmt++;
    if (*fmt == 0)
        return 0;
    for (i = 0; *fmt != 0 && i < 3; i++, fmt++) {
        if (*fmt < '0' || *fmt > '9')
            return 0;
        d2[i] = *fmt;
    }
    if (i == 0)
        return 0;
    p = atoi(d1);
    m = atoi(d2);
    if (p < 1 || p > 80 || m < 1 || m > 8)
        return 0;
    cnt = 0;
    for (i = 0; i < 0x300; i++) {
        if (ai_CharsT[i].id != 0xffff && ai_CharsT[i].kind <= 4 && G12_B(i, 0x0c) == rel)
            cnt++;
    }
    n = (int)((float)cnt * p * 0.01f);
    if (n == 0)
        return 1;
    m = max(min(m, n / 3), 1);
    memset(&msg, 0, sizeof(msg));
    msg.type = 0x81;
    msg.to = ((AICHAR *)ai_PlayerChar)->netId;
    msg.ch = rel;
    msg.time = ai_TimeT;
    msg.fromId = -1;
    msg.flags = 2;
    msg.car = n;
    msg.n = m;
    msg.time.hour = 8;
    msg.time.min = 0;
    cm_SendLongMessage(&msg);
    return 1;
}
/* 0x4b0fd0, the AUFRUHR text command (callback 0x601b50[16], name 0x601070[16]), the same message as 0x4aff30
   (AUFSTAND) with car 0: message 0x7d for the character ai_PlayerChar, today 8:00
   @deep only scheduling left (readable, 23 asm / 91 B): the original hoists ai_Time's first dword out of the TMX copy above the msg zeroing loop (mov edx,ds:0x770ae0 at 4b0fd6, before push edi / rep stos) and then stores the copy a,c,b,d (year 4b0ff7, +8 4b100b, +4 4b100f, word 4b1032) interleaved with the netId load, while ours keeps source order (netId load 4b0fe4, year 4b0fe9) and stores b,c,a,d (4b1006, 4b100c, 4b1018); car 0 and time.min 0 also come earlier here than in the original. diffclass: one 1-instruction code hunk (the hoist) + one code hunk of the same 8-instruction store permutation; the multiset test fails only because the offsets 0x2c/0x30/0x34 attach to different registers. Tried and reverted: TMX before msg.to (29 asm, worse). The same blocked pattern as the @deep 0x4afdd0 ai_CommandFest (hoisted ai_Time dword + a,c,b,d store order), so one vargen store-order pass should settle both. */

int ai_CommandRiot(void)
{
    AIMSG msg;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x7d;
    msg.to = ((AICHAR *)ai_PlayerChar)->netId;
    msg.time = ai_TimeT;
    msg.fromId = -1;
    msg.flags = 1;
    msg.car = 0;
    msg.time.hour = 8;
    msg.time.min = 0;
    cm_SendLongMessage(&msg);
    return 1;
}

typedef struct G12OBJ {                 /* 0x18 bytes, filled by 0x4e6ec0 (hmt.h: HMTOBJ) */
    int obj;                            /* 0x00 */
    int id;                             /* 0x04, a character id or -1 */
    char pad08[0x10];
} G12OBJ;

int amt_GetFirstOffice(unsigned int k, struct AMTREC *out);                                         /* 0x4e6ec0, defined elsewhere */
int amt_SendOfficeMessage(unsigned char a, char *b, char *c, unsigned char d, unsigned char e);                /* 0x4e6680, defined elsewhere */

/* 0x4b1060, text command "-n": frees the holders of the offices 0x1e..0x21 (0x4e6680) and sends message 0x80
   (arg 4), today 8:00 */
int ai_Unk4b1060(int a, int b, char *fmt)
{
    G12OBJ o;
    AIMSG msg;

    if (*fmt != '-')
        return 0;
    if (amt_GetFirstOffice(0x1e, &o) && o.id != -1)
        amt_SendOfficeMessage(o.obj, 0, 0, 4, 0xff);
    if (amt_GetFirstOffice(0x1f, &o) && o.id != -1)
        amt_SendOfficeMessage(o.obj, 0, 0, 4, 0xff);
    if (amt_GetFirstOffice(0x20, &o) && o.id != -1)
        amt_SendOfficeMessage(o.obj, 0, 0, 4, 0xff);
    if (amt_GetFirstOffice(0x21, &o) && o.id != -1)
        amt_SendOfficeMessage(o.obj, 0, 0, 4, 0xff);
    atoi(fmt + 1);
    memset(&msg, 0, sizeof(msg));
    msg.type = 0x80;
    msg.to = *(int *)((char *)ai_PlayerChar + 4);
    TMX(msg) = NOWX;
    msg.fromId = -1;
    msg.flags = 2;
    msg.car = 4;
    msg.time.hour = 8;
    msg.time.min = 0;
    cm_SendLongMessage(&msg);
    return 1;
}

typedef struct G12STOCK {               /* 0x80 bytes, a player's goods record (f3init.c's INIT_STOCK, f3gm.c's GM_GOODS) */
    unsigned short item;                /* 0x00, the goods prototype (an ai_GoodsT index) */
    char pad02[2];
    int amount;                         /* 0x04, the amount the player holds */
    char pad08[0x18];                   /* 0x08 .. 0x1f */
    float demand;                       /* 0x20, the amount the market wants */
    float mean;                         /* 0x24 */
    char pad28[0x10];                   /* 0x28 .. 0x37 */
    float price;                        /* 0x38, the stored price */
    char pad3c[0x44];                   /* 0x3c .. 0x7f */
} G12STOCK;

typedef struct G12STOCKBLOCK {          /* the 0x55 goods records of one player; the next player's records start 0x2a90
                                           bytes on (f3init.c's INIT_STOCKBLOCK: four price-index ints precede each block) */
    G12STOCK rec[0x55];                 /* 0x00, at 0x7d6a70 + player * 0x2a90 (f3gm.c's GM_GOODS) */
    char pad2a80[0x10];                 /* 0x2a80 */
} G12STOCKBLOCK;

extern char gm_StockBlocks[];  /* 0x7d6a60, the five 0x2a90-byte stock blocks (f3gm.c's definition) */
#define init_StockBlocks ((G12STOCKBLOCK *)(gm_StockBlocks + 0x10))  /* 0x7d6a70, the records of block 0; a
                                       view into the one 0xd4d0-byte object, not a separate COMMON symbol
                                       (a separate definition let the block writer/reader overwrite the
                                       globals the linker placed inside the stock area) */

typedef struct G12REC10 {               /* 0x5c bytes, one of the ten records 0x4cd9d0 searches (ai_Unk60c7c0) */
    int id;                             /* 0x00, the id 0x4cd9d0 matches */
    char pad04[0x58];
} G12REC10;
void cm_SendGoodsRecord(void *p, unsigned char k);                                /* 0x4c56d0, defined elsewhere */

/* 0x4b11e0, text command "-PLUS_<kind>_n" / "-MINUS_<kind>_n" (kind WAFFEN / BUECHER / ENDPRODUKTE): scales
   the prices of the player's stock of that kind by 1 +- n% (0x4c56d0)
   @deep meaning read equal against the disassembly (all 14 hunks), only allocation / scheduling left plus a Q2 block. This session: the stock block stride (0x7d6a70 + player * 0x2a90, f3gm.c's GM_GOODS / f3init.c's INIT_STOCKBLOCK; was G12STOCK[][0x55], stride 0x2a80, wrong records for every ai_OwnPlayer > 0) and &e instead of e.unk08 to 0x4c56d0 (that callee memcpy's 0x80 bytes; the original's lea [esp+0x54] with 8 pushed bytes is e+0) are fixed: 332 -> 275 differing instructions, frame 0xbc now equal, every slot equal (sign 0x14, name 0x1c, kind 0x28, e 0x4c). What is left: the register permutation fmt/i/cursor = edx/ebx/ebp here vs ebp/edx/ebx in the original (the original keeps fmt in ebp as ai_CommandLaw does - ours loads it into edx before sub esp and spills i to [esp+0x10] inside the name loop, one extra store), the FPU kept-alive pattern in the three loop tails (original fst g; fmul e.demand, ours fstp g; fld e.demand; fmul g) and the 167-instruction prologue store order (the string copies are verified against .rdata: name MINUS/PLUS at 0x601bc4/0x601bbc, kind WAFFEN/BUECHER/ENDPRODUKTE at 0x601c40/0x601c38/0x601c2c, sign -1/1). Tried and reverted: a per-loop G12STOCK *s pointer local (336 asm, code 48 bytes smaller) - allocation, not meaning. Q2 block: cm_SendGoodsRecord (0x4c56d0 = message 0x3f, k and 0x80 bytes of p, cm.c; cm_SendGoodsRecord is the evidenced name) and ai_FindWeaponRecord (0x4cd9d0, the record of ten with id, cm.c) - both need their own units' evidence before the rename. */
int ai_CommandDemand(int a, int b, char *fmt)
{
    G12STOCK e;
    float f, g;
    int i, j;

    if (*fmt != '-')
        return 0;
    {
    int sign[2] = { -1, 1 };
    char name[2][6] = { "MINUS", "PLUS" };
    char kind[3][12] = { "WAFFEN", "BUECHER", "ENDPRODUKTE" };

    fmt++;
    for (i = 0; i < 2; i++) {
        if (memcmp(fmt, name[i], strlen(name[i])) == 0)
            break;
    }
    if (i >= 2)
        return 0;
    fmt += strlen(name[i]);
    if (*fmt != '_')
        return 0;
    fmt++;
    for (j = 0; j < 3; j++) {
        if (memcmp(fmt, kind[j], strlen(kind[j])) == 0)
            break;
    }
    if (j >= 3)
        return 0;
    fmt += strlen(kind[j]);
    if (*fmt != '_')
        return 0;
    f = (float)(atoi(fmt + 1) * sign[i]) * 0.01;
    switch (j) {
    case 0:
        for (i = 0; i < 0x55; i++) {
            if (init_StockBlocks[ai_OwnPlayer].rec[i].item != 0 && init_StockBlocks[ai_OwnPlayer].rec[i].amount != 0) {
                G12REC10 *r = (G12REC10 *)ai_FindWeaponRecord(init_StockBlocks[ai_OwnPlayer].rec[i].item);

                if (r != 0 && r->id != 0) {
                    g = f + 1.0f;
                    e = init_StockBlocks[ai_OwnPlayer].rec[i];
                    e.mean = g * e.demand;
                    e.price = gm_ItemPrice(init_StockBlocks[ai_OwnPlayer].rec[i].item, 100) * g;
                    cm_SendGoodsRecord(&e, ai_OwnPlayer);
                }
            }
        }
        break;
    case 1:
        for (i = 0; i < 0x55; i++) {
            if (init_StockBlocks[ai_OwnPlayer].rec[i].item != 0 && init_StockBlocks[ai_OwnPlayer].rec[i].amount != 0
                && ai_GoodsT[init_StockBlocks[ai_OwnPlayer].rec[i].item].type == 8) {
                g = f + 1.0f;
                e = init_StockBlocks[ai_OwnPlayer].rec[i];
                e.mean = g * e.demand;
                e.price = gm_ItemPrice(init_StockBlocks[ai_OwnPlayer].rec[i].item, 100) * g;
                cm_SendGoodsRecord(&e, ai_OwnPlayer);
            }
        }
        break;
    case 2:
        for (i = 0; i < 0x55; i++) {
            if (init_StockBlocks[ai_OwnPlayer].rec[i].item != 0 && init_StockBlocks[ai_OwnPlayer].rec[i].amount != 0
                && ai_GoodsT[init_StockBlocks[ai_OwnPlayer].rec[i].item].type == 0x20) {
                g = f + 1.0f;
                e = init_StockBlocks[ai_OwnPlayer].rec[i];
                e.mean = g * e.demand;
                e.price = gm_ItemPrice(init_StockBlocks[ai_OwnPlayer].rec[i].item, 100) * g;
                cm_SendGoodsRecord(&e, ai_OwnPlayer);
            }
        }
        break;
    }
    }
    return 1;
}
/* 0x4b1600, sends message 0x57 to every player (0x4ae030), today at 10..13 o'clock (by id) */

int ai_Unk4b1600(void)
{
    AITIME t;
    AIMSG msg;
    int i;

    *(AITIMEX *)&t = NOWX;
    memset(&msg, 0, sizeof(msg));
    t.min = 0;
    for (i = 0; i < 0x300; i++) {
        if (ai_IsLivingPlayer((unsigned short)i)) {
            memset(&msg, 0, sizeof(msg));
            t.hour = i % 4 + 10;
            msg.type = 0x57;
            msg.to = ai_CharsT[i].netId;
            msg.fromId = ai_CharsT[i].netId;
            msg.flags = 1;
            TMX(msg) = *(AITIMEX *)&t;
            cm_SendLongMessage(&msg);
        }
    }
    return 1;
}
/* 0x4b16e0, the same with message 0x58 at 13..16 o'clock */

int ai_Unk4b16e0(void)
{
    AITIME t;
    AIMSG msg;
    int i;

    *(AITIMEX *)&t = NOWX;
    memset(&msg, 0, sizeof(msg));
    t.min = 0;
    for (i = 0; i < 0x300; i++) {
        if (ai_IsLivingPlayer((unsigned short)i)) {
            memset(&msg, 0, sizeof(msg));
            t.hour = i % 4 + 13;
            msg.type = 0x58;
            msg.to = ai_CharsT[i].netId;
            msg.fromId = ai_CharsT[i].netId;
            msg.flags = 1;
            TMX(msg) = *(AITIMEX *)&t;
            cm_SendLongMessage(&msg);
        }
    }
    return 1;
}
/* 0x4b17c0, text command "-<d>..." (d one of N S O W A): message 0x51 with n = atoi(fmt + 1) (sic: starts at d)
   and d, if n > 0 */

int ai_Unk4b17c0(int a, int b, char *fmt)
{
    AIMSG msg;
    char *p;
    char d;
    int n;

    if (*fmt != '-')
        return 0;
    p = fmt + 1;
    if (strlen(p) < 2)
        return 0;
    d = *p;
    if (d != 'N' && d != 'S' && d != 'O' && d != 'W' && d != 'A')
        return 0;
    n = atoi(p);
    if (n > 0) {
        memset(&msg, 0, sizeof(msg));
        msg.type = 0x51;
        msg.to = *(int *)((char *)ai_PlayerChar + 4);
        msg.fromId = -1;
        msg.flags = 0x12;
        *((char *)&msg + 0x58) = (char)n;
        *((char *)&msg + 0x59) = d;
        cm_SendLongMessage(&msg);
    }
    return 1;
}
/* 0x4b1880, sends message 0x54 to the first 8 characters of 0x4ae0d0, today 12:00 */

int ai_Unk4b1880(void)
{
    AITIME t;
    AIMSG msg;
    int i, n;

    *(AITIMEX *)&t = NOWX;
    t.hour = 12;
    t.min = 0;
    n = 0;
    for (i = 0; i < 0x300 && n < 8; i++) {
        if (ai_IsLivingHuman((unsigned short)i)) {
            memset(&msg, 0, sizeof(msg));
            msg.type = 0x54;
            msg.to = ai_CharsT[i].netId;
            msg.fromId = ai_CharsT[i].netId;
            msg.flags = 9;
            TMX(msg) = *(AITIMEX *)&t;
            cm_SendLongMessage(&msg);
            n++;
        }
    }
    return 1;
}
/* 0x4b1950, the same with message 0x55 at 10:00 */

int ai_Unk4b1950(void)
{
    AITIME t;
    AIMSG msg;
    int i, n;

    *(AITIMEX *)&t = NOWX;
    t.hour = 10;
    t.min = 0;
    n = 0;
    for (i = 0; i < 0x300 && n < 8; i++) {
        if (ai_IsLivingHuman((unsigned short)i)) {
            memset(&msg, 0, sizeof(msg));
            msg.type = 0x55;
            msg.to = ai_CharsT[i].netId;
            msg.fromId = ai_CharsT[i].netId;
            msg.flags = 9;
            TMX(msg) = *(AITIMEX *)&t;
            cm_SendLongMessage(&msg);
            n++;
        }
    }
    return 1;
}

/* G12_COPY: AI_ZERO's copy twin (inline rep movsb / movsd from a global) */
#define G12_COPY(dst, src, size) {                                  \
    int _cn = (size); void *_cp = (dst);                            \
    __asm mov edi, _cp                                              \
    __asm mov esi, OFFSET src                                       \
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

extern int hst_KeyFirst;
extern int ai_Unk601068;
extern int hst_KeyNext;
extern char hst_Key[0x20];
int hst_ParseDate(int k, unsigned char mode, int *year, char *name);              /* 0x4b2ed0 */

/* 0x4b1a20, looks for the key hst_Key in the list from hst_KeyNext on (entries of 6, up to today); found:
   sends message 0x52 with its index */
int ai_Unk4b1a20(void)
{
    AIMSG msg;
    char key[16];
    int day[4];
    char name[16];
    int pos;

    pos = hst_KeyNext;
    if (hst_KeyFirst <= -1 && (hst_KeyNext < hst_KeyFirst || hst_KeyNext >= ai_Unk601068))
        return 0;
    AI_ZERO(key, 16);
    G12_COPY(key, hst_Key, strlen(hst_Key));
    AI_ZERO(name, 16);
    if (!hst_ParseDate(pos, 4, day, name))
        return 0;
    do {
        if (memcmp(key, name, strlen(name)) == 0) {
            hst_KeyNext = pos + 6;
            memset(&msg, 0, sizeof(msg));
            msg.type = 0x52;
            msg.to = *(int *)((char *)ai_PlayerChar + 4);
            msg.car = pos - hst_KeyFirst;
            msg.fromId = -1;
            msg.flags = 0x12;
            cm_SendLongMessage(&msg);
            return 1;
        }
        if (day[0] > ai_TimeT.year)
            break;
        pos += 6;
        if (!hst_ParseDate(pos, 4, day, name))
            break;
    } while (hst_KeyNext < ai_Unk601068);
    return 0;
}
/* 0x4b1bd0, text command "-n": message 0x59 with n clamped to 1..10, today 11:00 */

int ai_Unk4b1bd0(int a, int b, char *fmt)
{
    AIMSG msg;
    int n;

    if (*fmt != '-')
        return 0;
    n = atoi(fmt + 1);
    if (n <= 10) {
        if (n <= 1)
            n = 1;
    } else
        n = 10;
    memset(&msg, 0, sizeof(msg));
    msg.type = 0x59;
    msg.to = *(int *)((char *)ai_PlayerChar + 4);
    msg.car = n;
    msg.fromId = -1;
    msg.flags = 2;
    TMX(msg) = NOWX;
    msg.time.hour = 11;
    cm_SendLongMessage(&msg);
    return 1;
}
/* 0x4b1c90, the INVENTAR_PLUS text command (callback 0x601b50[26], name 0x601070[26]) "-xxxNAME": 0x4c4880 from
   the character args[k] with the value of 0x4fe900(fmt + 4) (cf. ai_CommandOfficeInventoryPlus) */

int ai_CommandInventoryPlus(G12ARG *args, int k, char *fmt)
{
    AICHAR *c;
    char *s;
    int v;

    c = gm_FindChar(args[k].id);
    if (c != 0 && ai_IsLivingCharacter(c->id) && *fmt == '-') {
        s = fmt + 1;
        if (strlen(s) >= 4) {
            s += 3;
            tolower((int)(s + 1));      /* sic: a pointer */
            v = gm_FindObjectProto(s);
            if (v != 0) {
                cm_SendExSellObject(c->netId, -1, v, 1, ai_OwnPlayer, 0);
                return 1;
            }
        }
    }
    return 0;
}

/* G12_ZERO_G: AI_ZERO on a global (its address as an immediate) */
#define G12_ZERO_G(dst, size) {                                         int _zn = (size);                                                   __asm mov edi, OFFSET dst                                           __asm mov ebx, _zn                                                  __asm xor eax, eax                                                  __asm lea ecx, [edi+edi*2]                                          __asm and ecx, 3                                                    __asm sub ebx, ecx                                                  __asm jle AI_ZERO_LABEL(__LINE__)                                   __asm rep stosb                                                     __asm mov ecx, ebx                                                  __asm and ebx, 3                                                    __asm shr ecx, 2                                                    __asm rep stosd                                                     __asm AI_ZERO_LABEL(__LINE__): add ecx, ebx                         __asm rep stosb                                                 }


int hst_TextFirst = -1;  /* 0x601058, first text of the history file */
int hst_TextLast = -1;  /* 0x60105c, last text of the history file */
int hst_TextCurrent = -1;  /* 0x601060, the current text */
extern char ai_PlayerSlots[4][0x2f4];
unsigned char hst_TextsOn = 0;  /* 0x6f0724, history texts on */

/* 0x4b1d20, resets the lists hst_Lists and opens the player's history texts "historie\text_H_<name>"
   (txt.c); none: hst_TextsOn = 0 */
void ai_Unk4b1d20(void)
{
    char path[0x40];
    int i, j, k;

    for (i = 0; i < 4; i++) {
        hst_Lists[i].inUse = 0;
        for (j = 0; j < 4; j++) {
            hst_Lists[i].e[j].id = -1;
            hst_Lists[i].e[j].v = 0xffff;
        }
    }
    hst_TextFirst = -1;
    hst_TextLast = -1;
    sprintf(path, "historie\\text_H_%s", ai_PlayerSlots[ai_OwnPlayer]);
    if (txt_Files[txt_FindFile(path)].buf == 0 && !txt_ReloadTextFile(path)) {
        hst_TextsOn = 0;
        return;
    }
    k = txt_FindFile(path);
    hst_TextFirst = txt_Files[k].first;
    hst_TextLast = txt_Files[k].last;
    hst_TextCurrent = txt_Files[k].first;
    G12_ZERO_G(hst_Key, 16);
    hst_KeyFirst = -1;
    ai_Unk601068 = -1;
}
/* 0x4b1e40, marks the lists hst_Lists used and empty, frees the player's history texts, resets the text
   positions */

void hst_ResetPlayerTexts(void)
{
    char path[0x40];
    int i, j;

    for (i = 0; i < 4; i++) {
        hst_Lists[i].inUse = 1;
        for (j = 0; j < 4; j++) {
            hst_Lists[i].e[j].id = -1;
            hst_Lists[i].e[j].v = 0xffff;
        }
    }
    sprintf(path, "historie\\text_H_%s", ai_PlayerSlots[ai_OwnPlayer]);
    txt_FreeFile(path);
    hst_TextFirst = -1;
    hst_TextLast = -1;
    hst_TextCurrent = -1;
    hst_KeyFirst = -1;
    ai_Unk601068 = -1;
    hst_KeyNext = -1;
}
/* 0x4b1ee0, marks list i of hst_Lists used and empty */

int ai_Unk4b1ee0(unsigned short i)
{
    int j;

    if (i >= 4)
        return 0;
    hst_Lists[i].inUse = 1;
    for (j = 0; j < 4; j++) {
        hst_Lists[i].e[j].id = -1;
        hst_Lists[i].e[j].v = 0xffff;
    }
    return 1;
}
/* 0x4b1f30, marks all lists of hst_Lists used and empty and frees every text file under "historie\" */

void hst_ResetLists(void)
{
    int i, j;

    for (i = 0; i < 4; i++) {
        hst_Lists[i].inUse = 1;
        for (j = 0; j < 4; j++) {
            hst_Lists[i].e[j].id = -1;
            hst_Lists[i].e[j].v = 0xffff;
        }
    }
    for (i = 0; i < 0x80; i++) {
        if (_strnicmp(txt_Files[i].name, "historie\\", 9) == 0)
            txt_FreeFile(txt_Files[i].name);
    }
}
/* 0x4b1f90, switches the history texts on / off */

void ai_Unk4b1f90(unsigned char on)
{
    hst_TextsOn = on;
}

extern int txt_Loaded;                  /* 0xfe7c20 */
extern char *txt_Ptrs[0x4800];          /* 0x143a1c0 */

/* 0x4b1fa0, leaked name: copies text k (a text number, or a string above 0x4800) to out with every label
   "[name#text]" replaced by its text; the error messages are written to a local buffer only */
int hst_ParseText_Fake_1st_Pass(char *out, int k)
{
    char buf[0x17d0];
    char err[0x100];
    char *open, *hash, *p;

    open = 0;
    hash = 0;
    if (!txt_Loaded)
        return 0;
    AI_ZERO(buf, 0x17d0);
    if (k < 0x4800) {
        if (txt_Ptrs[k] == 0)
            return 0;
        if (strlen(txt_Ptrs[k]) > 0x17c0) {
            sprintf(err, "hst_ParseText_Fake_1st_Pass() failed: Label %i is too long", k);
            return 0;
        }
        strncpy(buf, txt_Ptrs[k], 0x17c0);
    } else {
        char *s = (char *)k, *d = buf;

        while ((*d++ = *s++) != 0)
            ;
    }
    p = buf;
    while (*p != 0) {
        if (*p == '[') {
            if (open != 0) {
                sprintf(err, "hst_ParseText_Fake_1st_Pass() failed: Syntax Error in Label %i", k - hst_TextFirst);
                return 0;
            }
            open = p;
        } else if (*p == '#') {
            if (open == 0 || hash != 0) {
                sprintf(err, "hst_ParseText_Fake_1st_Pass() failed: Syntax Error in Label %i", k - hst_TextFirst);
                return 0;
            }
            hash = p;
        } else if (*p == ']') {
            if (open == 0 || hash == 0) {
                sprintf(err, "hst_ParseText_Fake_1st_Pass() failed: Syntax Error in Label %i", k - hst_TextFirst);
                return 0;
            }
            memmove(p, p + 1, strlen(p + 1) + 1);
            memmove(open, hash + 1, strlen(hash + 1) + 1);
            p += open - hash - 1;
            open = 0;
            hash = 0;
            continue;
        }
        p++;
    }
    if (open != 0 || hash != 0) {
        sprintf(err, "hst_ParseText_Fake_1st_Pass() failed: Syntax Error in Label %i", k - hst_TextFirst);
        return 0;
    }
    sprintf(out, "%s", buf);
    return 1;
}

int hst_KeyFirst = -1;  /* 0x601064, first entry of the key list (-1: none) */
int ai_Unk601068 = -1;  /* 0x601068, end of the key list */
int hst_KeyNext = -1;  /* 0x60106c, next entry to look at */
char hst_CommandNames[27][0x40] = { "FEST", "BELAGERUNG_START", "AUFSTAND", "BRAND", "WIRBELSTURM", "STADTKASSE", "VERMOEGEN_STEUER", "VERMOEGEN", "ANSEHEN_BEI_AMTSTRAEGERN", "KILL_PLAYER", "STRAFE_HINRICHTUNG", "STRAFE_KERKER", "AMTSTRAEGER_INVENTAR_PLUS", "GESETZ", "GESETZ_REL", "GLAUBENSWECHSEL", "AUFRUHR", "GILDENSITZE_BRACH", "NACHFRAGE", "SOELDNER_PLUENDERN", "SOELDNER_MARODIEREN", "FERNHANDEL_RAUBRITTER", "SPENDE_ANSEHEN", "BETEILIGUNG", "KOMMENTARE", "PEST", "INVENTAR_PLUS" };  /* 0x601070, the text command names ("FEST" .. "INVENTAR_PLUS") */
char hst_TokenNames[15][0x40] = { "BUERGERMEISTER", "BISCHOF", "RND_GILDENMEISTER", "RND_AMTSTRAEGERIN", "RND_AMTSTRAEGER", "RND_AMTSPERSON", "REICHSTER_EINWOHNER", "BESTES_WIRTSHAUS", "GELD", "STADTKASSE", "RND_KIRCHENBERUF", "RND_REICH", "RND_NPC_EINWOHNER", "RND_HANDELSHERR", "RND_SPIELER" };  /* 0x601730, the names of the text tokens (callback */
char hst_Modes[3][5] = { "_NEW", "_USE", "_REL" };  /* 0x601af0, the modes of a context reference: "_NEW", "_USE", "_REL" */
extern int (*hst_TokenCallbacks[15])(int mode, G12ARG *args, int k, char *fmt, char *out);

/* 0x4b21c0, leaked name (its messages): one replacement "[_NEWn_NAME...]": mode _NEW / _USE / _REL with
   slot n of ctx (ctx[n + 1].unk00 holds the replacement, ctx[0].unk00 the valid flag), then the callback of
   NAME; the error messages are written to a local buffer only
   @deep only allocation left plus two Q blocks. Meaning read equal hunk by hunk against the disassembly: the 3-mode loop over hst_Modes (0x601af0, 5-byte stride, 4-byte compare), AI_ZERO(buf, 0x100) (the original's rep stos alignment dance is exactly am.h's AI_ZERO), buf[0] = s[5] / atoi / s += 6 / the 0..7 slot check, the mode == 1 path (k = ctx[slot + 1].unk00), the 15-token loop over hst_TokenNames (0x601730, 0x40 stride, strlen + memcmp), s += strlen, the mode == 0 store ctx[slot + 1].unk00 = k, the two 'Unknown Replacement' error exits and the tail call hst_TokenCallbacks[k](mode, ctx, slot, s, out) + ctx[0].unk00 = 0 on failure. The whole 116 asm comes from ONE 4-byte frame difference: sub esp,0x10c vs 0x110 - ours gives AI_ZERO's _zn the dead parameter slot [ebp+0x8] and _zp [ebp-0xc], the original gives _zn [ebp-0xc] and _zp [ebp-0x10] (its s spill stays in [ebp+0x8]); that shifts buf from [ebp-0x10c] to [ebp-0x110] and every lea/push with it. Same AI_ZERO temp-slot problem as the ai_Token* @deep reasons. Also slot order: slot/mode at [ebp-0x8]/[ebp-0x4] here vs [ebp-0x4]/[ebp-0x8] in the original (diffclass: 2 slots hunks). Q2 block: G12ARG.unk00 (hst.h) - slot 0 holds the valid flag and slots 1..8 the token index, no reader names it, so a name would be invented. Q4 block: the 2 '*(int *)s == *(int *)hst_Modes[i]' 4-byte compares (a strncmp/memcmp spelling adds a call). */
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

char hst_ListModes[2][5] = { "_SET", "_USE" };  /* 0x601b00, the modes of a list reference: "_SET", "_USE" */

/* 0x4b2380, leaked name (its messages): text k (or a string above 0x4800) "[_SET_n..." / "[_USE_n...":
   returns list n of hst_Lists (_SET empties it first) */
G12ROW *hst_ParseCommandline_1st_Pass(int k)
{
    char buf[0x17d0];
    char err[0x100];
    int set, n;

    set = 0;
    if (!txt_Loaded)
        return 0;
    AI_ZERO(buf, 0x17d0);
    AI_ZERO(err, 0x100);
    if (k < 0x4800) {
        if (txt_Ptrs[k] == 0)
            return 0;
        if (strlen(txt_Ptrs[k]) > 0x17c0) {
            sprintf(err, "hst_ParseCommandline_1st_Pass() failed: Label %i is too long", k);
            return 0;
        }
        strncpy(buf, txt_Ptrs[k], 0x17c0);
    } else {
        char *s = (char *)k, *d = buf;

        while ((*d++ = *s++) != 0)
            ;
    }
    if (*(int *)(buf + 1) == *(int *)hst_ListModes[0]) {
        err[0] = buf[6];
        n = atoi(err);
        set = 1;
    } else if (*(int *)(buf + 1) == *(int *)hst_ListModes[1]) {
        err[0] = buf[6];
        n = atoi(err);
    } else
        return 0;
    if (n > -1 && n < 4) {
        G12ROW *row = &hst_Lists[n];

        if (set)
            ai_Unk4b1ee0((unsigned short)n);
        return row;
    }
    if (n >= 4)
        sprintf(err, "hst_ParseCommandline_1st_Pass() failed: Wrong group index in Label %i", k - hst_TextFirst);
    return 0;
}

int (*hst_TokenCallbacks[15])(int mode, G12ARG *args, int k, char *fmt, char *out) = { ai_TokenMayor, ai_TokenBishop, ai_TokenRandomGuildMaster, ai_TokenRandomOfficeHolderFemale, ai_TokenRandomOfficeHolder, ai_TokenRandomOfficePerson, ai_TokenRichestInhabitant, ai_TokenBestInn, ai_TokenMoney, ai_TokenCityTreasury, ai_TokenRandomChurchProfession, ai_TokenRandomRich, ai_TokenRandomNpcInhabitant, ai_TokenRandomMerchant, ai_TokenRandomPlayer };  /* 0x601b10 */
int (*hst_CommandCallbacks[27])(G12ROW *row, int slot, char *fmt) = { ai_CommandFest, ai_Unk4afe80, ai_Unk4aff30, ai_CommandFire, ai_CommandTornado, ai_CommandCityTreasury, ai_CommandWealthTax, ai_CommandWealth, ai_CommandOfficeStanding, ai_CommandKillPlayer, ai_CommandExecution, ai_CommandPrison, ai_CommandOfficeInventoryPlus, ai_CommandLaw, ai_CommandLawChange, ai_CommandFaithChange, ai_CommandRiot, ai_Unk4b1060, ai_CommandDemand, ai_Unk4b1600, ai_Unk4b16e0, ai_Unk4b17c0, ai_Unk4b1880, ai_Unk4b1950, ai_Unk4b1a20, ai_Unk4b1bd0, ai_CommandInventoryPlus };  /* 0x601b50 */

/* 0x4b2530, leaked name (its messages): runs every word of text k (or a string above 0x4800) through the text
   commands of hst_CommandNames / hst_CommandCallbacks; a leading "[_SET_n" / "[_USE_n" selects list n of hst_Lists (it
   must be in use)
   @deep only allocation left (317/317 instructions, 82 differing: 30 slots + 120 code hunks) plus a Q4 block. Meaning read equal: the two AI_ZERO clears (buf 0x17d0, word 0x100), the k < 0x4800 / strlen 0x17c0 / strncpy path and the string-copy else, the '[_SET' / '[_USE' prefix compare (hst_ListModes 0x601b00), word[0] = buf[6] + atoi + the 0..3 range check, row = &hst_Lists[n] + row->inUse, p = buf + 7 / p = buf, the word loop (space starts a command, slot 8 or atoi for the _USE prefix, q = word + 8 / word + 2, the 27-name loop over hst_CommandNames and the hst_CommandCallbacks call with q + strlen), the per-word re-clear and the tail command. What is left is the AI_ZERO temp allocation (ours gives _zn the dead parameter slot [ebp+0x8] and _zp [ebp-0x8], the original gives both their own slots) and the variable slot pair [-0x4]/[-0x8] swapped (slot / n / i order) - the same AI_ZERO problem as hst_ParseContext and the ai_Token* @deep reasons. Q4 block: the 8 '*(int *)(p + 1) == *(int *)hst_ListModes[n]' 4-byte prefix compares; a memcmp(p + 1, hst_ListModes[n], 4) spelling adds a call and was measured WORSE (it also lost the byte match of hst_ParseCommandline_1st_Pass), so it needs a typed 4-byte view of the mode string (shared-header decision). */
int hst_ParseCommandline_2nd_Pass(int k)
{
    char buf[0x17d0];
    char word[0x100];
    G12ROW *row;
    char *p, *w, *q;
    int started, slot, n, i;

    row = 0;
    if (!txt_Loaded)
        return 0;
    AI_ZERO(buf, 0x17d0);
    AI_ZERO(word, 0x100);
    if (k < 0x4800) {
        if (txt_Ptrs[k] == 0)
            return 0;
        if (strlen(txt_Ptrs[k]) > 0x17c0) {
            sprintf(word, "hst_ParseCommandline_2nd_Pass() failed: Label %i is too long", k);
            return 0;
        }
        strncpy(buf, txt_Ptrs[k], 0x17c0);
    } else {
        char *s = (char *)k, *d = buf;

        while ((*d++ = *s++) != 0)
            ;
    }
    if (*(int *)(buf + 1) == *(int *)hst_ListModes[0] || *(int *)(buf + 1) == *(int *)hst_ListModes[1]) {
        word[0] = buf[6];
        n = atoi(word);
        if (n <= -1 || n >= 4) {
            sprintf(word, "hst_ParseCommandline_2nd_Pass() failed: Invalid GROUP reference in Label %i",
                    k - hst_TextFirst);
            return 0;
        }
        row = &hst_Lists[n];
        if (row->inUse == 0)
            return 0;
        p = buf + 7;
    } else
        p = buf;
    AI_ZERO(word, 0x100);
    started = 0;
    w = word;
    while (*p != 0) {
        if (*p == ' ') {
            if (started) {
                slot = 8;
                if (*(int *)(word + 1) == *(int *)hst_ListModes[1]) {
                    slot = atoi(word);
                    q = word + 8;
                } else
                    q = word + 2;
                for (i = 0; i < 27; i++) {
                    if (memcmp(q, hst_CommandNames[i], strlen(hst_CommandNames[i])) == 0)
                        break;
                }
                if (i < 27)
                    hst_CommandCallbacks[i](row, slot, q + strlen(hst_CommandNames[i]));
                AI_ZERO(word, 0x100);
                started = 0;
                w = word;
                continue;
            }
        } else {
            if (!started)
                started = 1;
            *w++ = *p;
        }
        p++;
    }
    if (started) {
        slot = 8;
        if (*(int *)(word + 1) == *(int *)hst_ListModes[1]) {
            slot = atoi(word);
            q = word + 8;
        } else
            q = word + 2;
        for (i = 0; i < 27; i++) {
            if (memcmp(q, hst_CommandNames[i], strlen(hst_CommandNames[i])) == 0)
                break;
        }
        if (i < 27)
            hst_CommandCallbacks[i](row, slot, q + strlen(hst_CommandNames[i]));
    }
    return 1;
}

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

/* 0x4b28d0, leaked name (its message): replaces in s (in place) every word "_NAME-...-n" (two dashes, then
   digits) by its replacement (hst_ParseContext), with the list of the "[_SET_n" / "[_USE_n" prefix of text k
   @deep only allocation / scheduling left (347/343 instructions, 266 differing: 304 code + 14 regs + 6 fill hunks; no Q rules fail). Every hunk read against the disassembly: the frame is equal (both 'mov eax,0x1de0; call __chkstk'), the two AI_ZERO clears (buf 0x17d0, word 0x100), the G12_COPY_P copy of s into buf, row = hst_ParseCommandline_1st_Pass(k), the word loop ('_' opens a word with word[0] = *p and w = word + 1, a space inside a word is the syntax error, '-' stores and increments dashes with > 2 the error, dashes < 2 stores, the digits branch isdigit + the digit copy + AI_ZERO(rep) + *w = 0 + hst_ParseContext + the rep copy + *s++ = *p), the two tails (in && dashes < 2 error, in == 1 && dashes == 2 replacement) and *s = 0 are all present with the same constants, string formats and callees. The diff is the register rotation eax/ecx/edx (diffclass: regs 14, the rest code because the rotation is not consistent inside a hunk) plus the source-pointer spill (the original keeps s in [ebp+0x8] with a reload, ours holds it in a register across the loop) - vargen ren / named-local work. */
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
/* 0x4b2c90, the text after label k: both passes into out (text k + 1, then k + 2), then the commands of text
   k + 2 if cmd */

int hst_ParseEventText(char *out, int k, int cmd)
{
    if (!hst_ParseText_Fake_1st_Pass(out, k + 1))
        return 0;
    k += 2;
    if (!hst_ParseText_Fake_2nd_Pass(out, k))
        return 0;
    if (cmd)
        hst_ParseCommandline_2nd_Pass(k);
    return 1;
}
/* 0x4b2ce0, leaked name (its message): writes text k (or a string above 0x4800) to out (0x5176d0) with every
   label "[text#name]" replaced by its text */

void hst_ParseText_Real(char *out, int k)
{
    char buf[0x17d0];
    char err[0x100];
    char *open, *hash, *p;

    open = 0;
    hash = 0;
    if (!txt_Loaded)
        return;
    AI_ZERO(buf, 0x17d0);
    if (k < 0x4800) {
        if (txt_Ptrs[k] == 0)
            return;
        if (strlen(txt_Ptrs[k]) > 0x17c0)
            return;
        strncpy(buf, txt_Ptrs[k], 0x17c0);
    } else {
        char *s = (char *)k, *d = buf;

        while ((*d++ = *s++) != 0)
            ;
    }
    p = buf;
    while (*p != 0) {
        if (*p == '[') {
            if (open != 0) {
                sprintf(err, "hst_ParseText_Real() failed: Syntax Error in Label %i", k - hst_TextFirst);
                return;
            }
            open = p;
        } else if (*p == '#') {
            if (open == 0 || hash != 0) {
                sprintf(err, "hst_ParseText_Real() failed: Syntax Error in Label %i", k - hst_TextFirst);
                return;
            }
            hash = p;
        } else if (*p == ']') {
            if (open == 0 || hash == 0) {
                sprintf(err, "hst_ParseText_Real() failed: Syntax Error in Label %i", k - hst_TextFirst);
                return;
            }
            memmove(hash, p + 1, strlen(p + 1) + 1);
            memmove(open, open + 1, strlen(open + 1) + 1);
            p = hash - 1;
            open = 0;
            hash = 0;
            continue;
        }
        p++;
    }
    if (open != 0 || hash != 0)
        return;
    txt_FormatString(out, "%s", buf);
}

/* G12_COPY2: G12_COPY with destination and source in variables */
#define G12_COPY2(dst, src, size) {                                 \
    int _cn = (size); void *_cs = (src); void *_cp = (dst);         \
    __asm mov edi, _cp                                              \
    __asm mov esi, _cs                                              \
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

/* 0x4b2ed0, leaked name (its message): text k is a date "DD.MM.YYYY..." (DD / MM 00: unknown); *year = YYYY -
   1400; mode 4: name = the text, mode 2: name = "<town>, d <month> y" (as far as known); mode 1: the first 10
   characters become the key hst_Key
   @deep read the original in full: fixed strlen(buf) != 10 (was < 10; original cmp 0xa/je, longer texts take the error path) and moved *year after the d/m normalization (187 -> 183 asm). Types tried: short y 194, m+0x4f 191, both 196, block-scoped d/m worse; all 120 declaration orders and s/t at function scope leave the slots unchanged. Remaining (allocation): frame 0x17f4 vs 0x17f8 because d/m sit in dead slots ([ebp+0xb]/[ebp-0x4]) where the original gives them -0x28/-0x27, shifting known/tmp by 4 and rotating the G12 temp/name slots */
int hst_ParseDate(int k, unsigned char mode, int *year, char *name)
{
    char buf[0x17d0];
    char tmp[16];
    char d, m;
    unsigned short y;
    int known;

    known = 0;
    if (!txt_Loaded || hst_TextFirst == -1)
        return 0;
    AI_ZERO(buf, 0x17d0);
    AI_ZERO(tmp, 16);
    if (k < 0x4800) {
        if (txt_Ptrs[k] == 0)
            return 0;
        if (strlen(txt_Ptrs[k]) > 0x17c0)
            return 0;
        strncpy(buf, txt_Ptrs[k], 0x17c0);
    } else {
        char *s = (char *)k, *t = buf;

        while ((*t++ = *s++) != 0)
            ;
    }
    if (strlen(buf) != 10) {
        sprintf(buf, "hst_ParseDate() failed: Label %i is not a <<DATE>> ", k - hst_TextFirst);
        return 0;
    }
    G12_COPY2(tmp, buf, 2);
    d = (char)atoi(tmp);
    G12_COPY2(tmp, buf + 3, 2);
    m = (char)atoi(tmp);
    G12_COPY2(tmp, buf + 6, 4);
    y = (unsigned short)atoi(tmp);
    if (d == 0) {
        d = 1;
        known = 1;
    }
    if (m == 0) {
        m = 1;
        known = 2;
    }
    *year = atoi(tmp) - 1400;
    if (mode & 4) {
        G12_COPY2(name, buf, strlen(buf));
    } else if (mode & 2) {
        switch (known) {
        case 0:
            txt_FormatString(name, "%s, %i %s %i", ai_PlayerSlots[ai_OwnPlayer] + 0x20, d, (unsigned char)m + 0x4f, y);
            break;
        case 1:
            txt_FormatString(name, "%s, %s %i", ai_PlayerSlots[ai_OwnPlayer] + 0x20, (unsigned char)m + 0x4f, y);
            break;
        case 2:
            txt_FormatString(name, "%s, %i", ai_PlayerSlots[ai_OwnPlayer] + 0x20, y);
            break;
        }
    }
    if (mode & 1) {
        G12_ZERO_G(hst_Key, 16);
        G12_COPY2(hst_Key, buf, 10);
    }
    return 1;
}
/* 0x4b3280, the next history event (3 texts per entry from hst_TextCurrent: date, text, ..) of last year: 1 with
   its date in name and its text in out; 2: not yet; 3 / 4: history off / ended */

int hst_NextEvent(char *name, char *out)
{
    int year[4];
    int r, d;

    do {
        if (!hst_TextsOn)
            return 3;
        if (!hst_ParseDate(hst_TextCurrent, 3, year, name)) {
            hst_TextsOn = 0;
            return 4;
        }
        d = ai_TimeT.year - year[0];
        if (d == 1) {
            r = d;
            hst_ParseText_Real(out, hst_TextCurrent + 1);
            if (*out == 0)
                r = 6;
        } else if (d > 1)
            r = 0;
        else {
            r = 2;
            break;
        }
        hst_TextCurrent += 3;
        if (hst_TextLast < hst_TextCurrent)
            hst_TextsOn = 0;
    } while (r == 0 || r == 6);
    return r;
}


extern D2WIN d2_Wins[];            /* 0xf8f120 */
extern int d2_LastWindow;                /* 0x6ed938 */
extern struct D2WIN *d2_LastWindowPtr;            /* 0x6ed980 */
extern int d2_ClickedId;                /* 0xf8a668 */
void d2_SetPointerSprite(int a, int pic);                                               /* 0x476f80 */
int d2_SetForm(int view, int n);                                                 /* 0x475f70 */
void d2_CreateScrollButtons(int x, int y, int win, int obj, int pic);                      /* 0x472270 */
int sv_CreateScrollForm(void);                                                          /* 0x52fa10, defined elsewhere */
void sv_Unk52fad0(void);                                                         /* 0x52fad0, defined elsewhere */
void sv_DestroyScrollForm(void);                                                         /* 0x52fa90, defined elsewhere */
int io_PrintText(char *fmt, ...);                                               /* 0x515960, defined elsewhere */
int sv_RunFrame(int cb, unsigned mask);                                      /* 0x5309a0, defined elsewhere */
extern int sv_PopRequest;                /* 0x6f22a0 */

/* 0x4b3350, the history dialog: shows the events of last year (0x4b3280) one after the other */
void ai_Unk4b3350(void)
{
    char text[0x1fb8];
    char name[0x100];
    int view, win;
    int *vis;

    AI_ZERO(name, 0x100);
    AI_ZERO(text, 0x1fb8);
    if (hst_NextEvent(name, text) != 1)
        return;
    view = sv_CreateScrollForm();
    d2_SetPointerSprite(0, 0);
    d2_SetForm(view, 1);
    win = d2_LastWindow;
    d2_SetForm(view, 2);
    io_PrintText((char *)0x10b0);
    d2_CreateScrollButtons(-1, -1, d2_LastWindow, win, 0x767);
    vis = &d2_Wins[win].visible;
    *vis = 0x18;
    d2_SetForm(view, 1);
    io_PrintText("$C%s", text);
    d2_SetForm(view, 3);
    io_PrintText("$C$Z$[%s$]", name);
    d2_ClickedId = -1;
    do {
        sv_Unk52fad0();
        if (d2_ClickedId == 0) {
            AI_ZERO(text, 0x1fb8);
            AI_ZERO(name, 0x100);
            if (hst_NextEvent(name, text) != 1) {
                sv_PopRequest = 1;
            } else {
                d2_SetForm(view, 2);
                io_PrintText((char *)0x10b0);
                d2_CreateScrollButtons(-1, -1, d2_LastWindow, win, 0x767);
                *vis = 0x18;
                d2_SetForm(view, 1);
                io_PrintText("$C%s", text);
                d2_LastWindowPtr->scrollY = 0;
                d2_LastWindowPtr->scrollStep = 2;
                d2_SetForm(view, 3);
                io_PrintText("$C$Z$[%s$]", name);
            }
        }
    } while (sv_RunFrame(ai_Unk4b3350, 0x66087));
    sv_DestroyScrollForm();
}
/* 0x4b35e0, 0x4b3280 with both passes (0x4b2c90, commands if cmd): 5 when they fail */

int hst_NextEventCmd(char *name, char *out, int cmd)
{
    int year[4];
    int r, d;

    do {
        if (!hst_TextsOn)
            return 3;
        if (!hst_ParseDate(hst_TextCurrent, 3, year, name)) {
            hst_TextsOn = 0;
            return 4;
        }
        d = ai_TimeT.year - year[0];
        if (d == 1) {
            r = d;
            if (!hst_ParseEventText(out, hst_TextCurrent, cmd))
                r = 5;
            else if (*out == 0)
                r = 6;
        } else if (d > 1)
            r = 0;
        else {
            r = 2;
            break;
        }
        hst_TextCurrent += 3;
        if (hst_TextLast < hst_TextCurrent)
            hst_TextsOn = 0;
    } while (r == 0 || r == 6 || r == 5);
    return r;
}
/* 0x4b36d0, the same with 0x4b35e0 (commands if cmd) */

void ai_Unk4b36d0(int cmd)
{
    char text[0x1fb8];
    char name[0x100];
    int view, win;
    int *vis;

    AI_ZERO(name, 0x100);
    AI_ZERO(text, 0x1fb8);
    if (hst_NextEventCmd(name, text, cmd) != 1)
        return;
    view = sv_CreateScrollForm();
    d2_SetPointerSprite(0, 0);
    d2_SetForm(view, 1);
    win = d2_LastWindow;
    d2_SetForm(view, 2);
    io_PrintText((char *)0x10b0);
    d2_CreateScrollButtons(-1, -1, d2_LastWindow, win, 0x767);
    vis = &d2_Wins[win].visible;
    *vis = 0x18;
    d2_SetForm(view, 1);
    io_PrintText("$C%s", text);
    d2_SetForm(view, 3);
    io_PrintText("$C$Z$[%s$]", name);
    d2_ClickedId = -1;
    do {
        sv_Unk52fad0();
        if (d2_ClickedId == 0) {
            AI_ZERO(text, 0x1fb8);
            AI_ZERO(name, 0x100);
            if (hst_NextEventCmd(name, text, cmd) != 1) {
                sv_PopRequest = 1;
            } else {
                d2_SetForm(view, 2);
                io_PrintText((char *)0x10b0);
                d2_CreateScrollButtons(-1, -1, d2_LastWindow, win, 0x767);
                *vis = 0x18;
                d2_SetForm(view, 1);
                io_PrintText("$C%s", text);
                d2_LastWindowPtr->scrollY = 0;
                d2_LastWindowPtr->scrollStep = 2;
                d2_SetForm(view, 3);
                io_PrintText("$C$Z$[%s$]", name);
            }
        }
    } while (sv_RunFrame((void (*)(void))ai_Unk4b36d0, 0x66087));
    sv_DestroyScrollForm();
}

extern int cm_NetFlags;      /* 0x6f2594, game flags (8: network game) */
extern int hst_SeedPending;
int cm_SendExSysMessage(unsigned char id, void *msg);                                            /* 0x4c4db0, defined elsewhere */
void sv_PumpNet(void);                                                         /* 0x52fe00, defined elsewhere */
__declspec(dllimport) unsigned long __stdcall GetTickCount(void);

/* 0x4b3960, the yearly history: tells the others (message 5 with the tick count) in a network game, waits
   while hst_SeedPending, then shows the history dialog of hst_TextsOn (1: texts, 2: texts and commands) */
void hst_YearlyHistory(int cmd)
{
    int msg[0x1f];

    if ((cm_NetFlags & 8) || net_Conn.s == -1) {
        msg[0] = GetTickCount();
        cm_SendExSysMessage(5, msg);
    }
    while (hst_SeedPending != 0) {
        if (net_Conn.closed != 0)
            return;
        sv_PumpNet();
    }
    if (net_Conn.closed == 0 && hst_TextsOn != 0) {
        if (hst_TextsOn == 1)
            ai_Unk4b3350();
        else if (hst_TextsOn == 2)
            ai_Unk4b36d0(cmd);
    }
}

char ai_PlayerSlots[4][0x2f4] = {0};  /* 0x7e3f40, per player: starts with the name of its history file */
char hst_Key[0x20] = {0};  /* 0xf83520, the key to look for */
G12ROW hst_Lists[4] = {0};  /* 0xf83540 */
int hst_SeedPending = 0;  /* 0xf83650 */

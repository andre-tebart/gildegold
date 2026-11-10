/* hst.c -- the history texts (0x4adf40..0x4b39f0, after hmt.c): character predicates, the text tokens and text
   commands of a history text, its parser (the hst_* names are leaked by its error texts), the date labels and the
   yearly history dialogs. Its .data block starts at 0x601058; the text commands' callbacks are 0x601b50 and their
   names (0x40-byte keys such as "FEST" / "BELAGERUNG_START") 0x601070, which is what the ai_Command* names rest on.
   Names are ours except the hst_* ones.
   Open: the ai_Token* siblings still take AI_ZERO with its own _zp slot (frame 0x20c vs 0x208); AI_ZERO_AT(n1, ..)
   fixes the frame (ai_TokenRandomNpcInhabitant, ai_TokenRandomChurchProfession) and is untried in the others.
   Open: G12OBJ repeats hmt.h's HMTOBJ (0x4e6ec0 / 0x4e6680 declared in both files); G12_B reads AICHAR bytes.
   Open: gm_FirstBuilding (0x50b010) returns unsigned char * (am.h, f3gm.c) while the callers here use AIBLD *.
   Open: hst_KeyFirst / hst_KeyLast are only ever set -1 (0x520200 reads hst_KeyFirst for hst_KeyNext = hst_KeyFirst + n),
   so ai_CommandComments returns 0 at every call: is the key list ever filled?
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
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, cm_FightLoop 0x4d88c0 move it as a word / movzx) */
#define ai_GoodsT (*(AIGOOD **)&ai_Goods)  /* 0x91aaf0, ai_Goods (f3gm.c: char *) as the AIGOOD records it points to */
#define ai_BldsT (*(AIBLD **)&ai_Blds)  /* 0x889f74, ai_Blds (f3gm.c: char *) as the AIBLD records it points to */
#define ai_BTypesT (*(AIBTYPE **)&ai_BTypes)  /* 0x7e3f30, ai_BTypes (ahm.c: char *) as the AIBTYPE records it points to */
#define ai_CharsT (*(AICHAR (*)[768])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) as the AICHAR records they are */
#define gm_AddHealthS ((float (*)(unsigned short, int))gm_AddHealth)  /* 0x4fca40 (f3gm.c: unsigned id): @original this unit pushes the id as 16 bits (byte-matched ai_CommandKillPlayer / ai_MsgWorkerOwnHealthLossType1f) */
/* AI_ZERO_AT with the destination local assigned inside: @original the token callbacks store the size temp
   (0x200) before n1 = buf and read both slots into edi / ebx (ai_TokenRandomChurchProfession, byte) */
#define AI_ZERO_BUF(var, dst, size) {                               \
    int _zn = (size);                                               \
    var = (int)(dst);                                               \
    __asm mov edi, var                                              \
    __asm mov ebx, _zn                                              \
    __asm xor eax, eax                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle AI_ZERO_LABEL(__LINE__)                               \
    __asm rep stosb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep stosd                                                 \
    __asm AI_ZERO_LABEL(__LINE__): add ecx, ebx                     \
    __asm rep stosb                                                 \
}
/* a list reference's mode ("_SET" / "_USE") is its four characters, which the parser compares as one dword */
typedef union HST_MODE {
    char text[4];                       /* the characters, without the terminator */
    int  tag;                           /* the same four bytes as the dword VC6 compares */
} HST_MODE;
#pragma pack(push, 1)
typedef struct HST_LABEL {              /* the start of a text label: "[_SET..." / "[_USE..." */
    char     lead;                      /* +0, the '[' */
    HST_MODE mode;                      /* +1, the mode's four characters */
} HST_LABEL;
#pragma pack(pop)
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
   the pick */

int ai_TokenRandomGuildMaster(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    int n1, n2;
    int sel;
    int i, n;
    AICHAR *c, *pick;

    AI_ZERO_BUF(n1, buf, 0x200);
    ai_SplitTokenFormat(fmt, buf, &n1, &n2);
    if (n2 != 1 || (sel = atoi(fmt + n1)) < 0 || sel > 2)
        sel = 1;
    if (mode == 1) {
        c = gm_FindChar(args[k].id);
        if (c == 0 || c->health <= 0 || c->secondOffice == 0)
            return 0;
    } else {
        c = 0;
        if (sel == 0) {
            i = util_RandomU(0x300);
            n = 0x300;
            while (n != 0) {
                if (ai_IsLivingCharacter((unsigned short)i) && ai_CharsT[i].secondOffice >= 0x1e
                    && ai_CharsT[i].secondOffice <= 0x21) {
                    c = &ai_CharsT[i];
                    break;
                }
                n--;
                i = (i + 1) % 0x300;
            }
        } else if (sel == 1) {
            i = util_RandomU(0x300);
            n = 0x300;
            while (n != 0) {
                if (ai_IsLivingNonPlayer((unsigned short)i) && ai_CharsT[i].secondOffice >= 0x1e
                    && ai_CharsT[i].secondOffice <= 0x21) {
                    c = &ai_CharsT[i];
                    break;
                }
                n--;
                i = (i + 1) % 0x300;
            }
        } else {
            pick = 0;
            i = util_RandomU(0x300);
            n = 0x300;
            while (n != 0) {
                if (pick == 0 && ai_IsLivingNonPlayer((unsigned short)i) && ai_CharsT[i].secondOffice >= 0x1e
                    && ai_CharsT[i].secondOffice <= 0x21) {
                    pick = &ai_CharsT[i];
                } else if (ai_IsLivingHuman((unsigned short)i) && ai_CharsT[i].secondOffice >= 0x1e
                           && ai_CharsT[i].secondOffice <= 0x21) {
                    c = &ai_CharsT[i];
                    break;
                }
                n--;
                i = (i + 1) % 0x300;
            }
            if (c == 0)
                c = pick;
        }
        if (c == 0)
            return 0;
        if (mode == 0)
            args[k].id = c->netId;
    }
    txt_FormatString(out, buf, c->id);
    return 1;
}

#define G12_B(i, o) (((unsigned char *)&ai_CharsT[i])[o])
#define G12_TITLED(i) (ai_CharsT[i].officeKind > 0 && ai_CharsT[i].officeKind != 0x0f && ai_CharsT[i].officeKind != 0x1a \
                       && ai_CharsT[i].officeKind != 0x15)

/* 0x4ae600, the RND_AMTSTRAEGERIN token of a history text (callback 0x601b10[3], name 0x601730[3]): a character
   with byte 9 == 1, an ordinary title (G12_TITLED) and in none of the lists of 0x4adfb0; the selection as in
   ai_TokenRandomGuildMaster */
int ai_TokenRandomOfficeHolderFemale(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    int n1, n2;
    int sel;
    int i, n;
    AICHAR *c, *pick;

    AI_ZERO_BUF(n1, buf, 0x200);
    ai_SplitTokenFormat(fmt, buf, &n1, &n2);
    if (n2 != 1 || (sel = atoi(fmt + n1)) < 0 || sel > 2)
        sel = 1;
    if (mode == 1) {
        c = gm_FindChar(args[k].id);
        if (c == 0 || c->health <= 0 || c->officeKind == 0)
            return 0;
    } else {
        c = 0;
        if (sel == 0) {
            i = util_RandomU(0x300);
            n = 0x300;
            while (n != 0) {
                if (G12_B(i, 9) == 1 && ai_IsLivingCharacter((unsigned short)i) && ai_NotInLists(ai_CharsT[i].netId)
                    && G12_TITLED(i)) {
                    c = &ai_CharsT[i];
                    break;
                }
                n--;
                i = (i + 1) % 0x300;
            }
        } else if (sel == 1) {
            i = util_RandomU(0x300);
            n = 0x300;
            while (n != 0) {
                if (G12_B(i, 9) == 1 && ai_IsLivingNonPlayer((unsigned short)i) && ai_NotInLists(ai_CharsT[i].netId)
                    && G12_TITLED(i)) {
                    c = &ai_CharsT[i];
                    break;
                }
                n--;
                i = (i + 1) % 0x300;
            }
        } else {
            pick = 0;
            i = util_RandomU(0x300);
            n = 0x300;
            while (n != 0) {
                if (G12_B(i, 9) == 1) {
                    if (pick == 0 && ai_IsLivingNonPlayer((unsigned short)i) && G12_TITLED(i)) {
                        if (ai_NotInLists(ai_CharsT[i].netId))
                            pick = &ai_CharsT[i];
                    } else if (ai_IsLivingHuman((unsigned short)i) && ai_NotInLists(ai_CharsT[i].netId)
                               && G12_TITLED(i)) {
                        c = &ai_CharsT[i];
                        break;
                    }
                }
                n--;
                i = (i + 1) % 0x300;
            }
            if (c == 0)
                c = pick;
        }
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
   name: callback 0x601b10[8] = 0x4af1b0, token name 0x601730[8] GELD */

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
        /* @original the engine's clear (AI_ZERO) with both operands named in the asm: the original reads
           edi and ebx straight from the n2 / n1 slots, no _zp / _zn temporaries */
        __asm {
            mov edi, n2
            mov ebx, n1
            xor eax, eax
            lea ecx, [edi+edi*2]
            and ecx, 3
            sub ebx, ecx
            jle tail
            rep stosb
            mov ecx, ebx
            and ebx, 3
            shr ecx, 2
            rep stosd
        tail:
            add ecx, ebx
            rep stosb
        }
        ai_SplitTokenFormat(fmt, buf, &n1, &n2);
        n1 = atoi(fmt + n1);
        txt_FormatString(out, buf, gm_ConvertMoney((int)(v * (n1 * 0.01f)), ai_OwnPlayer), ai_OwnPlayer);
        return 1;
    }
    return 0;
}
/* 0x4af2c0, text token: n percent of the count of the first goods of type 9 in the stock (object 0x115) of
   building 0x50b010(1), as a money text: txt_Format (out, the split format, gm_ConvertMoney (amount,
   ai_OwnPlayer), ai_OwnPlayer) */

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
    for (o = gm_Query(o->list, 1, 5); o != 0; o = gm_QueryNext()) {
        if (ai_GoodsT[o->id].type == 9) {
            v = o->count;
            break;
        }
    }
    cnt = 0x200;
    n2 = (int)buf;
    AI_ZERO((char *)n2, cnt);
    ai_SplitTokenFormat(fmt, buf, &n1, &n2);
    n1 = atoi(fmt + n1);
    txt_FormatString(out, buf, gm_ConvertMoney((int)(v * (n1 * 0.01f)), ai_OwnPlayer), ai_OwnPlayer);
    return 1;
}

/* 0x4af3f0, the same as 0x4aec00 for a character whose byte 0x164 is 13..18 */
/* jobs 13 .. 18 are the church offices (the token's name, RND_ ... church profession) */
#define G12_CHURCHJOB(i) (ai_CharsT[i].jobId >= 13 && ai_CharsT[i].jobId <= 18)

/* 0x4af3f0, text token for a living character holding a church office (job 13 .. 18) and in none of the lists of
   0x4adfb0: fmt "...-n", n 0 any character, 1 (the default) a non-player, 2 a non-player first, else a human taking
   precedence; mode 1 formats args[k]'s character, mode 0 also stores the pick's net id there */
int ai_TokenRandomChurchProfession(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    int n1, n2;
    int sel;
    int i, n;
    AICHAR *c, *pick;

    AI_ZERO_BUF(n1, buf, 0x200);
    ai_SplitTokenFormat(fmt, buf, &n1, &n2);
    if (n2 != 1 || (sel = atoi(fmt + n1)) < 0 || sel > 2)
        sel = 1;
    if (mode == 1) {
        c = gm_FindChar(args[k].id);
        if (c == 0 || c->health <= 0)
            return 0;
    } else {
        c = 0;
        if (sel == 0) {
            i = util_RandomU(0x300);
            n = 0x300;
            while (n != 0) {
                if (ai_IsLivingCharacter((unsigned short)i) && G12_CHURCHJOB(i) && ai_NotInLists(ai_CharsT[i].netId)) {
                    c = &ai_CharsT[i];
                    break;
                }
                n--;
                i = (i + 1) % 0x300;
            }
        } else if (sel == 1) {
            i = util_RandomU(0x300);
            n = 0x300;
            while (n != 0) {
                if (ai_IsLivingNonPlayer((unsigned short)i) && G12_CHURCHJOB(i) && ai_NotInLists(ai_CharsT[i].netId)) {
                    c = &ai_CharsT[i];
                    break;
                }
                n--;
                i = (i + 1) % 0x300;
            }
        } else {
            pick = 0;
            i = util_RandomU(0x300);
            n = 0x300;
            while (n != 0) {
                if (pick == 0 && ai_IsLivingNonPlayer((unsigned short)i) && G12_CHURCHJOB(i)) {
                    if (ai_NotInLists(ai_CharsT[i].netId))
                        pick = &ai_CharsT[i];
                } else if (ai_IsLivingHuman((unsigned short)i) && G12_CHURCHJOB(i)
                           && ai_NotInLists(ai_CharsT[i].netId)) {
                    c = &ai_CharsT[i];
                    break;
                }
                n--;
                i = (i + 1) % 0x300;
            }
            if (c == 0)
                c = pick;
        }
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
                for (q = ai_CharInts[0]; (int)q < (int)ai_CharInts[768]; q += 0x87) {
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
   (3, 5, 7 or 11); the RND_SPIELER token (callback 0x601b10[14] = 0x4afc30, name 0x601730[14]) */

int ai_TokenRandomPlayer(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    int n1, n2;
    int steps[4];
    int step, i, n;
    AICHAR *c = 0;

    if (mode == 2)
        return 0;
    n1 = (int)buf;
    AI_ZERO_AT(n1, 0x200);
    ai_SplitTokenFormat(fmt, buf, &n1, &n2);
    if (mode == 1) {
        c = gm_FindChar(args[k].id);
        if (c == 0 || c->health <= 0)
            return 0;
    } else {
        steps[0] = 3;
        steps[1] = 5;
        steps[2] = 7;
        steps[3] = 11;
        /* @original the outer loop counts i, which the search reuses: it repeats the search until a
           search ends on 0x2ff (the original's inc / cmp 0x300 after each search) */
        for (i = 0; i < 0x300; i++) {
            step = steps[(unsigned short)util_RandomU(4)];
            i = (unsigned short)util_RandomU(0x300);
            n = 0x300;
            while (n != 0) {
                if (ai_IsLivingHuman((unsigned short)i) && ai_NotInLists(ai_CharsT[i].netId)) {
                    c = &ai_CharsT[i];
                    break;
                }
                n--;
                i = (step + i) % 0x300;
            }
        }
        if (c == 0)
            return 0;
        if (mode == 0)
            args[k].id = c->netId;
    }
    txt_FormatString(out, buf, c->id);
    return 1;
}
/* 0x4afdd0, the FEST text command (callback 0x601b50[0], name 0x601070[0]) "-n" (n > 0): sends message 0x53
   for the character ai_PlayerChar, today 12:00 */

int ai_CommandFest(int a, int b, char *fmt)
{
    AIMSG msg;

    if (*fmt != '-')
        return 0;
    if (atoi(fmt + 1) > 0) {
        memset(&msg, 0, sizeof(msg));
        msg.type = 0x53;
        msg.to = ((AICHAR *)ai_PlayerChar)->netId;
        msg.fromId = -1;
        msg.flags = 1;
        msg.thiefClass = 0;     /* @original a byte store at 0x58 (the field's name is the thief message's) */
        msg.time = ai_Time;
        msg.time.hour = 12;
        msg.time.min = 0;
        cm_SendLongMessage(&msg);
    }
    return 1;
}
/* 0x4afe80, the BELAGERUNG_START text command (callback 0x601b50[1]): sends message 0x7f to every councillor
   (kind 6 / 7), today 8:00 */

int ai_CommandSiegeStart(void)
{
    AIMSG msg;
    int i;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x7f;
    msg.time = ai_Time;
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
/* 0x4aff30, the AUFSTAND text command (callback 0x601b50[2]): sends message 0x7d (arg 1) for the character
   ai_PlayerChar, today 8:00 */

int ai_CommandUprising(void)
{
    AIMSG msg;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x7d;
    msg.to = ((AICHAR *)ai_PlayerChar)->netId;
    msg.time = ai_Time;
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
   the character ai_PlayerChar, today 7:00 */
int ai_CommandFire(void)
{
    AIMSG msg;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x4e;
    msg.to = ((AICHAR *)ai_PlayerChar)->netId;
    msg.fromId = -1;
    msg.car = -1;
    msg.flags = 1;
    ((G12EVENTMSG *)&msg)->value = 1000;
    msg.time = ai_Time;
    msg.time.hour = 7;
    cm_SendLongMessage(&msg);
    return 1;
}
/* 0x4b0050, the WIRBELSTURM text command (callback 0x601b50[4], name 0x601070[4]), the same as ai_CommandFire:
   message 0x50 (value 1000, flags 2), today 16:00 */

int ai_CommandTornado(void)
{
    AIMSG msg;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x50;
    msg.to = ((AICHAR *)ai_PlayerChar)->netId;
    msg.fromId = -1;
    msg.car = -1;
    msg.flags = 2;
    ((G12EVENTMSG *)&msg)->value = 1000;
    msg.time = ai_Time;
    msg.time.hour = 16;
    cm_SendLongMessage(&msg);
    return 1;
}
/* 0x4b00e0, the STADTKASSE text command (callback 0x601b50[5], name 0x601070[5]) "-PLUS_n" / "-MINUS_n":
   moves n percent of the count of the first goods of type 9 in the stock (object 0x115) of building 0x50b010(1)
   to or from that stock (0x4c4830) */

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
            for (g = gm_Query(o->list, 1, 5); g != 0; g = gm_QueryNext()) {
                if (ai_GoodsT[g->id].type == 9)
                    break;
            }
            if (g == 0)
                return 1;
            v = g->count;
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

/* 0x4b0640, the STRAFE_HINRICHTUNG text command (callback 0x601b50[10], name 0x601070[10])
   "-SHAUFEN" / "-KOEPFEN" / "-GALGEN" / "-KANONE": posts the execution party (G28REC kind 5, variant 3, 0, 2, 4) of
   the character args[k] for 10:00 in 24 hours, with the judges ai_JudgeCatholic / ai_JudgeProtestant, the
   living humans (at most 16 members) and a non-player office holder in the second place (the last member when
   there is none); quiet: the aftermath sends no notes
   @deep read the whole function against the original 0x4b0640..0x4b08da and fixed: the record is cm's
   G28REC (0x114 bytes: frame 0x16c now equal; was the local 0x100-byte G12XMSG with unk fields), kind 5 /
   thing -1 / host / flags 0 then 0x22 before gm_AddTime / variant / quiet (G28REC.zero7d renamed quiet:
   cm_ExecutionAftermath skips its notes when set), the name table in the original's slot order (name[0] =
   SHAUFEN .. name[3] = KANONE, the copies read 0x601be4 into [esp+0x18]; kinds 3, 0, 2, 4 go with that
   order), the member loop as an index loop (the original's jl end test) with the id read once, the -1 fill
   as the loop VC6 turns into 'mov ecx,0x10 / sub ecx,edx / rep stosd' (memset gave the shl/shr/and-3
   tail). ours 180 vs 181. Left: (1) the loop's induction pointer - the original biases it to the netId
   word ('+ mov edx,ai_CharInts / + mov esi,[edx] / + mov al,[edx+0x162] / + cmp edx,0x7d5f84'), ours to
   officeKind ('- mov edx,ai_Chars+0x166 / - mov esi,[edx-0x162] / - cmp byte ptr [edx],0 / - cmp
   edx,ai_Chars+0x65566'): the same records and fields (tried the id through ai_CharInts[i][0] and the byte
   through the same view: no change); (2) the scheduling of the 10 literal copies of the name table and of
   fmt ('- mov ebx,[esp+0x188] / - inc ebx' vs '+ mov eax,[esp+0x188] / + lea ebx,[eax+1]'), and kind/-1
   stores around the player's netId load (order hunks). (references: M9 at +0x7a: our literal "KOEPFEN"
   where the original's field refers to "GALGEN" at 0x601bd4 (and 2 more), explained: the M8 refs are the
   induction pointer's bias (ai_Chars+0x166 vs the original's ai_Chars+4 base, the loop end 0x7d60e6 vs
   0x7d5f84 = the same 0x300 records), the M9 literal is the copy order of the same four strings into the
   same slots (asmdiff: ours loads KOEPFEN where the original loads GALGEN on that line, both copies are
   present on both sides)) */
int ai_CommandExecution(G12ARG *args, int k, char *fmt)
{
    G28REC party;
    AICHAR *c;
    int kinds[4];
    int i, n, id;
    unsigned char variant;

    c = gm_FindChar(args[k].id);
    if (c == 0 || !ai_IsLivingCharacter(c->id))
        return 0;
    {
        char name[4][8] = { "SHAUFEN", "KOEPFEN", "GALGEN", "KANONE" };

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
    variant = (unsigned char)kinds[i];
    {
        int list[16];

        party.thing = -1;
        list[1] = -1;
        party.kind = 5;
        party.flags = 0;
        party.variant = variant;
        party.quiet = 1;
        party.host = ai_CharsT[ai_PlayerId].netId;
        list[0] = c->netId;
        list[2] = ai_JudgeCatholic->netId;
        list[3] = ai_JudgeProtestant->netId;
        for (i = 0; i < 0x300; i++) {
            id = ai_CharInts[i][0];
            if (id != c->netId && ai_IsLivingNonPlayer((unsigned short)i)) {
                if (ai_CharsT[i].officeKind != 0)
                    list[1] = id;
            } else if (n < 16 && ai_IsLivingHuman((unsigned short)i)) {
                list[n++] = id;
            }
        }
        if (list[1] == -1)
            list[1] = list[n-- - 1];
        if (n > 0)
            memcpy(party.members, list, n * 4);
    }
    for (i = n; i < 16; i++)
        party.members[i] = -1;
    party.used = (unsigned char)n;
    party.time = ai_Time;
    party.flags = 0x22;
    gm_AddTime(&party.time, 0x18, 0, 0);
    party.time.hour = 10;
    cm_SendNewParty(&party);
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
void cm_SendExSellObject(int a, int b, int c, int d, unsigned char e, int f);           /* 0x4c4880 */

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
   (byte-identical to msg.time = ai_Time) are the fixes; 165 -> 6 differing
   instructions. Q1 has evidence, Q2..Q7 pass.
   @deep only scheduling left: 3 order hunks of 226 instructions (6 differing) - the for-init i = 0 of both digit loops sits after fmt++ (4b0e0e) / after the *fmt == 0 test (4b0e52) in ours and before them in the original (4b0e05, 4b0e4d), and the clock word store (msg.ids[0], [esp+0x7c]) is kept until after fromId/flags/car/n in the original (4b0fa9) but emitted right after the clock copy's dwords in ours (4b0f82). Meaning read equal against the disassembly: the name array is {KATHOLISCH, EVANGELISCH} (ds:0x601c20 into [esp+0x14]), the product is float (fild + fimul p), the d1/d2 slot pair and the msg fields match. Store-order / hoist work: vargen perm rounds or a source shape that keeps the word store late. */

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
    msg.time = ai_Time;
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
   (AUFSTAND) with car 0: message 0x7d for the character ai_PlayerChar, today 8:00 */

int ai_CommandRiot(void)
{
    AIMSG msg;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x7d;
    msg.to = ((AICHAR *)ai_PlayerChar)->netId;
    msg.fromId = -1;
    msg.flags = 1;
    msg.car = 0;
    msg.time = ai_Time;
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
int amt_SendOfficeMessage(unsigned char a, char *b, char *c, int d, unsigned char e);                /* 0x4e6680, defined elsewhere */

/* 0x4b1060, the GILDENSITZE_BRACH text command (callback 0x601b50[17]): frees the holders of the offices
   0x1e..0x21 (0x4e6680) and sends message 0x80 (arg 4), today 8:00 */
int ai_CommandBreakGuildSeats(int a, int b, char *fmt)
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
    msg.to = ((AICHAR *)ai_PlayerChar)->netId;
    msg.time = ai_Time;
    msg.fromId = -1;
    msg.flags = 2;
    msg.car = 4;
    msg.time.hour = 8;
    msg.time.min = 0;
    cm_SendLongMessage(&msg);
    return 1;
}


typedef struct G12REC10 {               /* 0x5c bytes, one of the ten records 0x4cd9d0 searches (ai_WeaponRecordData) */
    int id;                             /* 0x00, the id 0x4cd9d0 matches */
    char pad04[0x58];
} G12REC10;
void cm_SendGoodsRecord(void *p, unsigned char k);                                /* 0x4c56d0, defined elsewhere */

/* 0x4b11e0, text command "-PLUS_<kind>_n" / "-MINUS_<kind>_n" (kind WAFFEN / BUECHER / ENDPRODUKTE): scales
   the prices of the player's stock of that kind by 1 +- n% (0x4c56d0)
   @deep meaning read equal against the disassembly (all 14 hunks), only allocation / scheduling left plus a Q2 block. This session: the stock block stride (0x7d6a70 + player * 0x2a90, gm_StockBlocks[p].goods; was a [][0x55] record view, stride 0x2a80, wrong records for every ai_OwnPlayer > 0) and &e instead of e.unk08 to 0x4c56d0 (that callee memcpy's 0x80 bytes; the original's lea [esp+0x54] with 8 pushed bytes is e+0) are fixed: 332 -> 275 differing instructions, frame 0xbc now equal, every slot equal (sign 0x14, name 0x1c, kind 0x28, e 0x4c). What is left: the register permutation fmt/i/cursor = edx/ebx/ebp here vs ebp/edx/ebx in the original (the original keeps fmt in ebp as ai_CommandLaw does - ours loads it into edx before sub esp and spills i to [esp+0x10] inside the name loop, one extra store), the FPU kept-alive pattern in the three loop tails (original fst g; fmul e.demand, ours fstp g; fld e.demand; fmul g) and the 167-instruction prologue store order (the string copies are verified against .rdata: name MINUS/PLUS at 0x601bc4/0x601bbc, kind WAFFEN/BUECHER/ENDPRODUKTE at 0x601c40/0x601c38/0x601c2c, sign -1/1). Tried and reverted: a per-loop stock record pointer local (336 asm, code 48 bytes smaller) - allocation, not meaning. Q2 block: cm_SendGoodsRecord (0x4c56d0 = message 0x3f, k and 0x80 bytes of p, cm.c; cm_SendGoodsRecord is the evidenced name) and ai_FindWeaponRecord (0x4cd9d0, the record of ten with id, cm.c) - both need their own units' evidence before the rename. */
int ai_CommandDemand(int a, int b, char *fmt)
{
    GMSTOCK e;
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
            if (gm_StockBlocks[ai_OwnPlayer].goods[i].item != 0 && gm_StockBlocks[ai_OwnPlayer].goods[i].amount != 0) {
                G12REC10 *r = (G12REC10 *)ai_FindWeaponRecord(gm_StockBlocks[ai_OwnPlayer].goods[i].item);

                if (r != 0 && r->id != 0) {
                    g = f + 1.0f;
                    e = gm_StockBlocks[ai_OwnPlayer].goods[i];
                    e.mean = g * e.demand;
                    e.price = gm_ItemPrice(gm_StockBlocks[ai_OwnPlayer].goods[i].item, 100) * g;
                    cm_SendGoodsRecord(&e, ai_OwnPlayer);
                }
            }
        }
        break;
    case 1:
        for (i = 0; i < 0x55; i++) {
            if (gm_StockBlocks[ai_OwnPlayer].goods[i].item != 0 && gm_StockBlocks[ai_OwnPlayer].goods[i].amount != 0
                && ai_GoodsT[gm_StockBlocks[ai_OwnPlayer].goods[i].item].type == 8) {
                g = f + 1.0f;
                e = gm_StockBlocks[ai_OwnPlayer].goods[i];
                e.mean = g * e.demand;
                e.price = gm_ItemPrice(gm_StockBlocks[ai_OwnPlayer].goods[i].item, 100) * g;
                cm_SendGoodsRecord(&e, ai_OwnPlayer);
            }
        }
        break;
    case 2:
        for (i = 0; i < 0x55; i++) {
            if (gm_StockBlocks[ai_OwnPlayer].goods[i].item != 0 && gm_StockBlocks[ai_OwnPlayer].goods[i].amount != 0
                && ai_GoodsT[gm_StockBlocks[ai_OwnPlayer].goods[i].item].type == 0x20) {
                g = f + 1.0f;
                e = gm_StockBlocks[ai_OwnPlayer].goods[i];
                e.mean = g * e.demand;
                e.price = gm_ItemPrice(gm_StockBlocks[ai_OwnPlayer].goods[i].item, 100) * g;
                cm_SendGoodsRecord(&e, ai_OwnPlayer);
            }
        }
        break;
    }
    }
    return 1;
}
/* 0x4b1600, the SOELDNER_PLUENDERN text command (callback 0x601b50[19]): sends message 0x57 to every living
   player, today at 10..13 o'clock (by id) */

int ai_CommandMercenariesPlunder(void)
{
    AITIME t;
    AIMSG msg;
    int i;

    t = ai_Time;
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
            msg.time = t;
            cm_SendLongMessage(&msg);
        }
    }
    return 1;
}
/* 0x4b16e0, the SOELDNER_MARODIEREN text command (callback 0x601b50[20]): the same as
   ai_CommandMercenariesPlunder with message 0x58 at 13..16 o'clock */

int ai_CommandMercenariesMaraud(void)
{
    AITIME t;
    AIMSG msg;
    int i;

    t = ai_Time;
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
            msg.time = t;
            cm_SendLongMessage(&msg);
        }
    }
    return 1;
}
/* 0x4b17c0, the FERNHANDEL_RAUBRITTER text command (callback 0x601b50[21], name 0x601070[21]) "-<d>..." (d one of
   N S O W A, the direction): message 0x51 for the player character with n = atoi(fmt + 1) (sic: starts at the letter,
   so n is 0) in the first byte of arg[0] and d in its second, when n > 0 */

int ai_CommandRemoteTradeRaider(int a, int b, char *fmt)
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
        msg.to = ((AICHAR *)ai_PlayerChar)->netId;
        msg.fromId = -1;
        msg.flags = 0x12;
        ((unsigned char *)&msg.arg[0])[0] = (unsigned char)n;
        ((unsigned char *)&msg.arg[0])[1] = d;
        cm_SendLongMessage(&msg);
    }
    return 1;
}
/* 0x4b1880, the SPENDE_ANSEHEN text command (callback 0x601b50[22], name 0x601070[22]): sends message 0x54, flags 9,
   from the character itself, to the first 8 living characters (ai_IsLivingHuman), today 12:00 */

int ai_CommandViewDonation(void)
{
    AITIME t;
    AIMSG msg;
    int i, n;

    t = ai_Time;
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
            msg.time = t;
            cm_SendLongMessage(&msg);
            n++;
        }
    }
    return 1;
}
/* 0x4b1950, the BETEILIGUNG text command (callback 0x601b50[23], name 0x601070[23]), the same as ai_CommandViewDonation
   with message 0x55 at 10:00 */

int ai_CommandParticipation(void)
{
    AITIME t;
    AIMSG msg;
    int i, n;

    t = ai_Time;
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
            msg.time = t;
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
extern int hst_KeyLast;
extern int hst_KeyNext;
extern char hst_Key[0x20];
int hst_ParseDate(int k, unsigned char mode, int *year, char *name);              /* 0x4b2ed0 */

/* 0x4b1a20, the KOMMENTARE text command (callback 0x601b50[24], name 0x601070[24]): looks for the key hst_Key in
   the list from hst_KeyNext on (entries of 6, up to today); found: sends message 0x52 with its index */
int ai_CommandComments(void)
{
    AIMSG msg;
    char key[16];
    int day[4];
    char name[16];
    int pos;

    pos = hst_KeyNext;
    if (hst_KeyFirst <= -1 && (hst_KeyNext < hst_KeyFirst || hst_KeyNext >= hst_KeyLast))
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
            msg.to = ((AICHAR *)ai_PlayerChar)->netId;
            msg.car = pos - hst_KeyFirst;
            msg.fromId = -1;
            msg.flags = 0x12;
            cm_SendLongMessage(&msg);
            return 1;
        }
        if (day[0] > ai_Time.year)
            break;
        pos += 6;
        if (!hst_ParseDate(pos, 4, day, name))
            break;
    } while (hst_KeyNext < hst_KeyLast);
    return 0;
}
/* 0x4b1bd0, the PEST text command (callback 0x601b50[25], name 0x601070[25]) "-n": message 0x59 with n clamped to
   1..10, today 11:00 */

int ai_CommandPlague(int a, int b, char *fmt)
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
    msg.to = ((AICHAR *)ai_PlayerChar)->netId;
    msg.car = n;
    msg.fromId = -1;
    msg.flags = 2;
    msg.time = ai_Time;
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
void hst_LoadPlayerTexts(void)
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
    hst_KeyLast = -1;
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
    hst_KeyLast = -1;
    hst_KeyNext = -1;
}
/* 0x4b1ee0, marks list i of hst_Lists used and empty (the [_SET_n command of a history text) */

int hst_ResetList(unsigned short i)
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

void hst_SetTextsOn(unsigned char on)
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
int hst_KeyLast = -1;  /* 0x601068, end of the key list (-1: none) */
int hst_KeyNext = -1;  /* 0x60106c, next entry to look at */
char hst_CommandNames[27][0x40] = { "FEST", "BELAGERUNG_START", "AUFSTAND", "BRAND", "WIRBELSTURM", "STADTKASSE", "VERMOEGEN_STEUER", "VERMOEGEN", "ANSEHEN_BEI_AMTSTRAEGERN", "KILL_PLAYER", "STRAFE_HINRICHTUNG", "STRAFE_KERKER", "AMTSTRAEGER_INVENTAR_PLUS", "GESETZ", "GESETZ_REL", "GLAUBENSWECHSEL", "AUFRUHR", "GILDENSITZE_BRACH", "NACHFRAGE", "SOELDNER_PLUENDERN", "SOELDNER_MARODIEREN", "FERNHANDEL_RAUBRITTER", "SPENDE_ANSEHEN", "BETEILIGUNG", "KOMMENTARE", "PEST", "INVENTAR_PLUS" };  /* 0x601070, the text command names ("FEST" .. "INVENTAR_PLUS") */
char hst_TokenNames[15][0x40] = { "BUERGERMEISTER", "BISCHOF", "RND_GILDENMEISTER", "RND_AMTSTRAEGERIN", "RND_AMTSTRAEGER", "RND_AMTSPERSON", "REICHSTER_EINWOHNER", "BESTES_WIRTSHAUS", "GELD", "STADTKASSE", "RND_KIRCHENBERUF", "RND_REICH", "RND_NPC_EINWOHNER", "RND_HANDELSHERR", "RND_SPIELER" };  /* 0x601730, the names of the text tokens (callback */
char hst_Modes[3][5] = { "_NEW", "_USE", "_REL" };  /* 0x601af0, the modes of a context reference: "_NEW", "_USE", "_REL" */
extern int (*hst_TokenCallbacks[15])(int mode, G12ARG *args, int k, char *fmt, char *out);

/* 0x4b21c0, leaked name (its messages): one replacement "[_NEWn_NAME...]": mode _NEW / _USE / _REL with
   slot n of ctx (ctx[n + 1].token holds the replacement, ctx[0].token the valid flag), then the callback of
   NAME; the error messages are written to a local buffer only
   @deep read the whole function against the original 0x4b21c0..0x4b2373 and fixed: s is a local copy of
   the src parameter living in the dead parameter slot (the original's 'mov [ebp+8],esi' at the entry; this
   gave the AI_ZERO temps their own slots: frame 0x110 now equal, 371 -> 112 differing bytes); readable:
   G12ARG.token (args[n+1].token is slot n's replacement token, args[0].token the valid flag), the mode
   compare through HST_MODE.tag. ours 170 vs 159: the 11 extra are one error tail. Left: (1) the token
   search's exits - the original threads both the exhausted loop ('+ push "...Unknown Replacement" / + jmp
   0x4b22a1' straight after the 'cmp ebx,hst_Modes; jl') and the found path (je straight to the strlen)
   past the 'k >= 15' test, where ours re-tests after the break ('- cmp ebx,0xf / - jge 0x4b2335') and
   keeps its own sprintf/return-0 copy (0x4b2335..0x4b2351) instead of the original's push-and-jmp into the
   'Wrong group reference' block's 'lea ecx,[ebp-0x110] / push ecx / call sprintf' (ours emits that lea
   before the push, so the tails do not merge); (2) k in ebx (ours) vs edx and the token pointer in edx vs
   ebx (regs hunks 0x4b22c2..0x4b2363). Tried: the error inside the loop ('if (++k >= 15)', 478 bytes) and
   the strlen/store under 'if (k < 15)' with only the final check (484 bytes). */
int hst_ParseContext(char *src, char *out, G12ARG *ctx)
{
    char buf[0x100];
    char *s;
    int slot, mode, k, i, r;

    s = src;
    slot = -1;
    mode = 4;
    if (s == 0 || out == 0)
        return 0;
    for (i = 0; i < 3; i++) {
        if (((HST_MODE *)s)->tag == ((HST_MODE *)hst_Modes[i])->tag)
            mode = i;
    }
    AI_ZERO(buf, 0x100);
    if (mode != 4) {
        if (ctx == 0 || ctx[0].token == 0) {
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
        k = ctx[slot + 1].token;
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
            ctx[slot + 1].token = k;
    }
    if (k >= 15) {
        sprintf(buf, "hst_ParseContext() failed: Unknown Replacement");
        return 0;
    }
    r = hst_TokenCallbacks[k](mode, ctx, slot, s, out);
    if (r == 0 && ctx != 0)
        ctx[0].token = 0;
    return r;
}

char hst_ListModes[2][5] = { "_SET", "_USE" };  /* 0x601b00, the modes of a list reference: "_SET", "_USE" */
#define hst_ListModeSet (*(int *)((char *)&hst_ListModes + 0x0))  /* 0x601b00, hst_ListModes[0] ("_SET") as one dword */
#define hst_ListModeUse (*(int *)((char *)&hst_ListModes + 0x5))  /* 0x601b05, hst_ListModes[1] ("_USE") as one dword */

/* 0x4b2380, leaked name (its messages): text k (or a string above 0x4800) "[_SET_n..." / "[_USE_n...":
   returns list n of hst_Lists (_SET empties it first) */
G12ROW *hst_ParseCommandline_1st_Pass(int k)
{
    union {
        char      text[0x17d0];
        HST_LABEL label;
    } buf;
    char err[0x100];
    int set, n;

    set = 0;
    if (!txt_Loaded)
        return 0;
    AI_ZERO(buf.text, 0x17d0);
    AI_ZERO(err, 0x100);
    if (k < 0x4800) {
        if (txt_Ptrs[k] == 0)
            return 0;
        if (strlen(txt_Ptrs[k]) > 0x17c0) {
            sprintf(err, "hst_ParseCommandline_1st_Pass() failed: Label %i is too long", k);
            return 0;
        }
        strncpy(buf.text, txt_Ptrs[k], 0x17c0);
    } else {
        char *s = (char *)k, *d = buf.text;

        while ((*d++ = *s++) != 0)
            ;
    }
    if (buf.label.mode.tag == hst_ListModeSet) {
        err[0] = buf.text[6];
        n = atoi(err);
        set = 1;
    } else if (buf.label.mode.tag == hst_ListModeUse) {
        err[0] = buf.text[6];
        n = atoi(err);
    } else
        return 0;
    if (n > -1 && n < 4) {
        G12ROW *row = &hst_Lists[n];

        if (set)
            hst_ResetList((unsigned short)n);
        return row;
    }
    if (n >= 4)
        sprintf(err, "hst_ParseCommandline_1st_Pass() failed: Wrong group index in Label %i", k - hst_TextFirst);
    return 0;
}

int (*hst_TokenCallbacks[15])(int mode, G12ARG *args, int k, char *fmt, char *out) = { ai_TokenMayor, ai_TokenBishop, ai_TokenRandomGuildMaster, ai_TokenRandomOfficeHolderFemale, ai_TokenRandomOfficeHolder, ai_TokenRandomOfficePerson, ai_TokenRichestInhabitant, ai_TokenBestInn, ai_TokenMoney, ai_TokenCityTreasury, ai_TokenRandomChurchProfession, ai_TokenRandomRich, ai_TokenRandomNpcInhabitant, ai_TokenRandomMerchant, ai_TokenRandomPlayer };  /* 0x601b10 */
int (*hst_CommandCallbacks[27])(G12ROW *row, int slot, char *fmt) = { ai_CommandFest, ai_CommandSiegeStart, ai_CommandUprising, ai_CommandFire, ai_CommandTornado, ai_CommandCityTreasury, ai_CommandWealthTax, ai_CommandWealth, ai_CommandOfficeStanding, ai_CommandKillPlayer, ai_CommandExecution, ai_CommandPrison, ai_CommandOfficeInventoryPlus, ai_CommandLaw, ai_CommandLawChange, ai_CommandFaithChange, ai_CommandRiot, ai_CommandBreakGuildSeats, ai_CommandDemand, ai_CommandMercenariesPlunder, ai_CommandMercenariesMaraud, ai_CommandRemoteTradeRaider, ai_CommandViewDonation, ai_CommandParticipation, ai_CommandComments, ai_CommandPlague, ai_CommandInventoryPlus };  /* 0x601b50 */

/* 0x4b2530, leaked name (its messages): runs every word of text k (or a string above 0x4800) through the text
   commands of hst_CommandNames / hst_CommandCallbacks; a leading "[_SET_n" / "[_USE_n" selects list n of hst_Lists (it
   must be in use) */
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
    if (((HST_LABEL *)buf)->mode.tag == hst_ListModeSet || ((HST_LABEL *)buf)->mode.tag == hst_ListModeUse) {
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
    started = 0;
    AI_ZERO(word, 0x100);
    w = word;
    while (*p != 0) {
        if (*p == ' ') {
            if (started) {
                slot = 8;
                if (((HST_LABEL *)word)->mode.tag == hst_ListModeUse) {
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
                started = 0;
                AI_ZERO(word, 0x100);
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
        if (((HST_LABEL *)word)->mode.tag == hst_ListModeUse) {
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
        d = ai_Time.year - year[0];
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
void sv_ScrollFormAnimate(void);                                                         /* 0x52fad0, defined elsewhere */
void sv_DestroyScrollForm(void);                                                         /* 0x52fa90, defined elsewhere */
int io_PrintText(char *fmt, ...);                                               /* 0x515960, defined elsewhere */
int sv_RunFrame(int cb, unsigned mask);                                      /* 0x5309a0, defined elsewhere */
extern int sv_PopRequest;                /* 0x6f22a0 */

/* 0x4b3350, the history dialog: shows the events of last year (0x4b3280) one after the other */
void hst_LastYearDialog(void)
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
        sv_ScrollFormAnimate();
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
    } while (sv_RunFrame(hst_LastYearDialog, 0x66087));
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
        d = ai_Time.year - year[0];
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

void hst_LastYearCmdDialog(int cmd)
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
        sv_ScrollFormAnimate();
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
    } while (sv_RunFrame((void (*)(void))hst_LastYearCmdDialog, 0x66087));
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
            hst_LastYearDialog();
        else if (hst_TextsOn == 2)
            hst_LastYearCmdDialog(cmd);
    }
}

char ai_PlayerSlots[4][0x2f4] = {0};  /* 0x7e3f40, per player: starts with the name of its history file */
char hst_Key[0x20] = {0};  /* 0xf83520, the key to look for */
G12ROW hst_Lists[4] = {0};  /* 0xf83540 */
int hst_SeedPending = 0;  /* 0xf83650 */

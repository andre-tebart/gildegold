/* 0x4e88a0, elects the two deputy offices (the kinds 0x1c and 0x1d): the candidates are the owner characters (AIBLD.owner) of the buildings of type 4
   with the rank 1 .. 6 and of the buildings of type 0x10 with the rank 0x28 .. 0x2d, each character once, found through the build query 0x501190; when
   at least 3 were seen, the richest (gm_GetWorth 0x50a640) replaces the holder of the 0x1c office (its rank 1 .. 6) or, when the rank distance to the
   holder is above 5, gets the 0x1d office; a free office of the two is filled directly. The old holder gets the lost-office news (amt_SendOfficeLost)
   and the new one the office news (amt_SendOfficeNews). No return value, messages only.
   @deep B2 draft (255 code / 48 regs / 2 fill hunks, 417/896 differing bytes, 233 lines): the whole original 0x4e88a0..0x4e8c20 was read against the source (build/ghidra/4e88a0.c + tools/disasm) and every statement now matches - the two build queries (1,5,4) / (1,5,0x10) with the owner / 0xffff / player / flags5a guard, total and the 16-candidate dedupe, the rank ranges (>= 1 && <= 6, >= 0x28 && <= 0x2d), total >= 3 && n != 0, the 36..0 scan with the rep movs into a (0x1d) / b (0x1c), v = b.owner then a.owner, cur = 0 / gm_FindChar / gm_GetWorth, the best for-loop and the four send arms (d > 5 with the rank 1..6 -> b arm, the rank test after SendOfficeLost/News, the cur == 0 arm with b <- pick first). Left is the register allocation: original n=edi / best=ebx / j=ebp and -1 in ebx, ours n=ebx / best=ebp and -1 in eax (the permutation inverts the vote loop and the prologue from the first push; n's home slot [esp+0x10] already matches). spell.py dup-tail 4e8a33 / 4e8baa / 4e8c1b all describe that allocation; the structural leads were applied (the n > 0 wrapper pulled into the for - single test like the original; the init order a.owner/b.owner/j/n/total; ch = ai_Chars[id] hoisted before the rank compare). Q1-Q7 pass; readability done (AMTBLDVIEW pack(1) added to amt.h). Left for match-deep: c2prio register priority and the slot/register permutation. */

void amt_ElectDeputies(void)
{
    AMTREC a, b;
    char *cand[16];
    char *it, *cur, *pick, *cp, *ch;
    int n, total, i, j, left, best, s, rb, rc, d, v;
    unsigned short id;
    unsigned char k;

    memset(&a, 0, sizeof(a));
    memset(&b, 0, sizeof(b));
    memset(cand, 0, sizeof(cand));
    a.owner = -1;
    b.owner = -1;
    j = 0;
    n = 0;
    total = 0;
    it = gm_BuildQuery(1, 5, 4);
    while (it != 0 && n < 16) {
        id = ((AMTBLDVIEW *)it)->owner;
        if (id != 0xffff && id != *ai_PlayerChar && (((AMTBLDVIEW *)it)->flags5a & 1) == 0) {
            total++;
            ch = ai_Chars[id];
            k = ((AMTCHARVIEW *)ch)->rank;
            if (k >= 1 && k <= 6) {
                for (i = 0; i < 16; i++) {
                    if (cand[i] == ch)
                        break;
                }
                if (i == 16) {
                    cand[n] = ch;
                    n++;
                }
            }
        }
        it = gm_BuildNext();
    }
    it = gm_BuildQuery(1, 5, 0x10);
    while (it != 0 && n < 16) {
        id = ((AMTBLDVIEW *)it)->owner;
        if (id != 0xffff && id != *ai_PlayerChar && (((AMTBLDVIEW *)it)->flags5a & 1) == 0) {
            total++;
            ch = ai_Chars[id];
            k = ((AMTCHARVIEW *)ch)->rank;
            if (k >= 0x28 && k <= 0x2d) {
                for (i = 0; i < 16; i++) {
                    if (cand[i] == ch)
                        break;
                }
                if (i == 16) {
                    cand[n] = ch;
                    n++;
                }
            }
        }
        it = gm_BuildNext();
    }
    if (total >= 3 && n != 0) {
        left = 2;
        i = 36;
        do {
            if (left == 0)
                break;
            if (amt_Table[i].kind == 0x1d) {
                a = amt_Table[i];
                left--;
            } else if (amt_Table[i].kind == 0x1c) {
                b = amt_Table[i];
                left--;
            }
            i--;
        } while (i >= 0);
        cur = 0;
        v = b.owner;
        if (v == -1)
            v = a.owner;
        if (v == -1) {
            best = 0;
        } else {
            cur = gm_FindChar(v);
            if (cur == 0)
                best = 0;
            else
                best = gm_GetWorth(((AMTCHARVIEW *)cur)->id);
        }
        pick = 0;
        for (j = 0; j < n; j++) {
            cp = cand[j];
            s = gm_GetWorth(((AMTCHARVIEW *)cp)->id);
            if (s > best) {
                pick = cp;
                best = s;
            }
        }
        if (pick != 0 && pick != cur) {
            if (cur != 0) {
                rb = ((AMTCHARVIEW *)pick)->rank;
                rc = ((AMTCHARVIEW *)cur)->rank;
                d = rc - rb;
                if (d < 0)
                    d = -d;
                if (d > 5) {
                    if (rb == 0 || rb > 6) {
                        amt_SendOfficeMessage((char)b.idx, 0, 0, 1, 0xff);
                        amt_SendOfficeMessage((char)a.idx, pick, 0, 1, 0xff);
                        v = 0x1d;
                    } else {
                        amt_SendOfficeMessage((char)b.idx, pick, 0, 1, 0xff);
                        amt_SendOfficeMessage((char)a.idx, 0, 0, 1, 0xff);
                        v = 0x1c;
                    }
                    amt_SendOfficeLost((unsigned short *)cur, ((AMTCHARVIEW *)cur)->secondOffice);
                    amt_SendOfficeNews((unsigned short *)pick, v);
                    return;
                }
                amt_SendOfficeLost((unsigned short *)cur, ((AMTCHARVIEW *)cur)->secondOffice);
                amt_SendOfficeNews((unsigned short *)pick, ((AMTCHARVIEW *)cur)->secondOffice);
                if (((AMTCHARVIEW *)pick)->rank != 0 && ((AMTCHARVIEW *)pick)->rank < 7) {
                    amt_SendOfficeMessage((char)b.idx, pick, 0, 1, 0xff);
                    return;
                }
                amt_SendOfficeMessage((char)a.idx, pick, 0, 1, 0xff);
                return;
            }
            if (((AMTCHARVIEW *)pick)->rank >= 1 && ((AMTCHARVIEW *)pick)->rank <= 6) {
                amt_SendOfficeMessage((char)b.idx, pick, 0, 1, 0xff);
                v = 0x1c;
            } else {
                amt_SendOfficeMessage((char)a.idx, pick, 0, 1, 0xff);
                v = 0x1d;
            }
            amt_SendOfficeNews((unsigned short *)pick, v);
        }
    }
}

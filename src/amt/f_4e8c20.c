/* 0x4e8c20, elects the office of the kind 0x22: the candidates are the owner characters (AIBLD.owner) of the buildings of type 7 with the rank 13 .. 18
   (the job group 7 of gm_TitleGroup), each character once, found through the build query 0x501190; when at least 3 were seen, the richest (gm_GetWorth
   0x50a640) replaces the holder of the office. The old holder gets the lost-office news (amt_SendOfficeLost), the new one the office news 0x22. No return
   value, messages only. */

void amt_ElectKind22(void)
{
    AMTREC a;
    char *cand[16];
    char *it, *cur, *pick, *cp, *ch;
    int n, total, i, j, left, best, s;
    unsigned short id;
    unsigned char k;

    memset(&a, 0, sizeof(a));
    memset(cand, 0, sizeof(cand));
    a.owner = -1;
    j = 0;
    n = 0;
    total = 0;
    it = gm_BuildQuery(1, 5, 7);
    if (it == 0)
        return;
    do {
        if (n >= 16)
            break;
        id = ((AMTBLDVIEW *)it)->owner;
        if (id != 0xffff && (((AMTBLDVIEW *)it)->flags5a & 1) == 0) {
            total++;
            ch = ai_Chars[id];
            k = ((AMTCHARVIEW *)ch)->rank;
            if (k >= 0xd && k <= 0x12) {
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
    } while (it != 0);
    if (total < 3)
        return;
    if (n == 0)
        return;
    left = 1;
    i = 36;
    do {
        if (left == 0)
            break;
        if (amt_Table[i].kind == 0x22) {
            left--;
            a = amt_Table[i];
        }
        i--;
    } while (i >= 0);
    cur = 0;
    if (a.owner == -1 || (cur = gm_FindChar(a.owner)) == 0)
        best = 0;
    else
        best = gm_GetWorth(((AMTCHARVIEW *)cur)->id);
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
            amt_SendOfficeMessage((char)a.idx, pick, 0, 1, 0xff);
            amt_SendOfficeLost((unsigned short *)cur, 0x22);
            amt_SendOfficeNews((unsigned short *)pick, 0x22);
            return;
        }
        amt_SendOfficeMessage((char)a.idx, pick, 0, 1, 0xff);
        amt_SendOfficeNews((unsigned short *)pick, 0x22);
    }
}

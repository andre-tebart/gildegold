/* 0x4ad1c0, the yearly election (years 1..3 of 4, ai_Time.year % 4): the four best candidates (score 0x4ad3e0)
   among the characters of the right group (hmt_ElectionGroupBits) without the excluded one (hmt_ElectionExcludedBits) get the
   office hmt_ElectionOfficeBits; human players are told
   @deep B: 175 code hunks read against the original: the frame is 8 bytes smaller here (sub esp,0x138 vs 0x140), the candidate scan keeps the character in edi where the original holds it in esi and the best[] base in ebp where the original holds it in edi, the score compare is our and eax,0x4100/jne against the original's test ah,0x41/jnp (the same fcom outcome), the insertion copy runs 4-esi against 3-ecx and copies [eax-8]->[eax] against [eax-4]->[eax+4], and cm_SendSetField's 4 is a literal here but the register ebp in the original (same value, the n clamp): every hunk is the same operation spelled differently, no missing statement/constant/callee found. Q2 also needs the 0x5176d0/0x53dda0 renames (blocked like hmt_Update) and amt_GetSuperiorOfficeHolder */

/* bytes in AICHAR's pad009 (a view in ai.h changed ai_Unk483a60's code) */
#define HMT_B(c, o) (((unsigned char *)(c))[o])

void hmt_Election(void)
{
    HMTCAND best[4];
    char text[260];
    unsigned int mask, excl;
    int type, office;
    int m, n, i, j, k;
    void *obj;
    AICHAR *c;
    float s;

    if (ai_TimeT.year >= 4) {
        m = ai_TimeT.year % 4;
        if (m != 0) {
            mask = hmt_ElectionGroupBits[m - 1];
            type = hmt_ElectionObjTypes[m - 1];
            excl = hmt_ElectionExcludedBits[m - 1];
            office = hmt_ElectionOfficeBits[m - 1];
            hmt_SendElectionMessage(m - 1);
            obj = amt_GetSuperiorOfficeHolder(type);
            if (obj != 0) {
                n = 0;
                memset(best, 0, sizeof(best));
                for (i = 0; i < 768; i++) {
                    c = &ai_CharsT[i];
                    if ((excl & c->flags1cc) == 0 && (mask & c->flags1cc) != 0 && HMT_B(c, 0x0d) >= 7
                        && (c->kind == 6 || c->kind == 7 || c->kind == 5 || c->kind == 4 || c->kind == 3
                            || c->kind == 2)) {
                        s = hmt_Score(c, obj, type, 0);
                        for (j = 4; j != 0; j--) {
                            if (s <= best[j - 1].score)
                                break;
                        }
                        if (j < 4) {
                            for (k = 3; k > j; k--) {
                                best[k].c = best[k - 1].c;
                                best[k].score = best[k - 1].score;
                            }
                            best[j].score = s;
                            n++;
                            best[j].c = c;
                        }
                    }
                }
                if (n >= 4)
                    n = 4;
                for (i = 0; i < n; i++) {
                    c = best[i].c;
                    if (c == 0)
                        return;
                    cm_SendSetField(c->netId, 0x1cc, 4, office, 0);
                    if (c->kind == 6 || c->kind == 7) {
                        txt_Format(text, 0x42b1, (HMT_B(c, 0x09) ? 0x28 : 0) + 0x253 + type);
                        he_ShowNews(c->netId, -1, text, 0, 0x5fd, "_NACHRICHTEN_NEU_14");
                    }
                }
            }
        }
    }
}

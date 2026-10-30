/* 0x551d70 (the Ghidra start; ROUGH), the run handler of the event 0x551d00 (six figure groups at +0xb0, state 0xc = the announcement): state 0xc tells every player character (kind 6 / 7: 0x53dda0 (id, -1, text 0x10bc / 0x10be by +0xac, 0x20, 0x600, 0x631090)) and goes to state 0; states 0 .. 5 take the group +0xb0 + state * 4: for every member (0x4e6fc0, 0x18-byte records) that has a person (0x505df0) and a job (0x4e6f80) a random roll may raise its mood (0x4e6680), unless the person has kind 3 every other character in the same building (+0x168 / +0x27) gets a random opinion change (0x4c4b00), and a player character gets the message 0x10bd / 0x10bf (0x5fd); the state is then incremented and the event posted 1 later; other states free the record */

int amt_GetColumnOffices(int col, int max, struct AMTREC *out);
int amt_GetKind(int k, struct AMTKIND *out);
int amt_SendOfficeMessage(unsigned char a, char *b, char *c, unsigned char d, unsigned char e);
void cm_SendRelations(int a, int b, int c, int d);

void he_Unk551d70(int ev)
{
    char text[0x200];
    int st = *(int *)(ev + 0x70);
    unsigned char arr[0x18 * 6];
    int n, i, k, c, p, hit;
    int job[8];

    if (st == 0xc) {
        txt_Format(text, *(int *)(ev + 0xac) == 0 ? 0x10bc : 0x10be, (char *)((unsigned)&ai_PlayerSlots + 0x20) + *(unsigned char *)(unsigned)&ai_OwnPlayer * 0x2f4);
        for (c = (unsigned)&ai_Chars; c < (unsigned)((unsigned)ai_Chars + 0x65400); c += 0x21c) {
            if (*(unsigned char *)(c + 2) == 6 || *(unsigned char *)(c + 2) == 7)
                he_ShowNews(*(int *)(c + 4), -1, text, 0x20, 0x600, (unsigned)&ch_EmptyName);
        }
        *(int *)(ev + 0x70) = 0;
        gm_AddTime(ev + 0x52, 1, 0, 0);
        return;
    }
    if (st < 0 || st >= 6) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    n = ((int (*)(int, int, struct AMTREC *, int))amt_GetColumnOffices)(*(int *)(ev + 0xb0 + st * 4), arr, 6, 0);
    if (n > 0) {
        for (i = 0; i < n; i++) {
            unsigned char *m = arr + i * 0x18;

            p = gm_FindChar(*(int *)(m + 0));
            if (p == 0)
                continue;
            if (amt_GetKind(m[4], job) == 0)
                continue;
            if (*(int *)(ev + 0xac) == 1 && ((unsigned char)job[2] == 9 || (unsigned char)job[2] == 6)) {
                if ((unsigned short)util_Random(3) != 0)
                    amt_SendOfficeMessage(m[0], 0, 0, 4, 0xff);
            }
            if (*(unsigned char *)(p + 2) == 3)
                continue;
            hit = 0;
            for (c = (unsigned)&ai_CharInts; c < ((unsigned)((unsigned)ai_CharInts + 0x653fe) + 0x2); c += 0x21c) {
                if (*(int *)(c + 0x168) != 0 && *(int *)c != *(int *)(p + 4) && *(unsigned short *)(*(int *)(c + 0x168) + 0x27) == *(unsigned short *)p) {
                    hit++;
                    k = (unsigned short)util_Random(1);
                    ((void (*)(int, int, int))cm_SendRelations)(*(int *)(p + 4), *(int *)c, -k - job[2] * 3);
                }
            }
            if (hit != 0 && (*(unsigned char *)(p + 2) == 6 || *(unsigned char *)(p + 2) == 7)) {
                txt_Format(text, *(int *)(ev + 0xac) == 0 ? 0x10bd : 0x10bf, *(unsigned short *)p);
                he_ShowNews(*(int *)(p + 4), -1, text, 1, 0x5fd, (unsigned)&ch_EmptyName);
            }
        }
    }
    *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
    gm_AddTime(ev + 0x52, 1, 0, 0);
}

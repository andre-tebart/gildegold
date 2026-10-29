/* 0x4cd3c0, appends a line (text) of the player id (with x) to the ring ai_Unk935da0[15] */

void ai_Unk4cd3c0(int id, int x, char *text)
{
    char *p;
    int n;

    p = ai_Unk4cd5e0(id);
    if (p != 0) {
        sprintf(ai_Unk935da0[ai_Unk9363f4].name, "%s %s", p + 0x12, p + 0x22);
        n = ai_Unk9363f4;
    } else {
        n = ai_Unk9363f4;
        *(int *)ai_Unk935da0[n].name = ai_Unk60c7b0[0];
        *(int *)(ai_Unk935da0[n].name + 4) = ai_Unk60c7b0[1];
    }
    ai_Unk935da0[n].id = id;
    ai_Unk935da0[n].x = x;
    strcpy(ai_Unk935da0[n].text, text);
    n++;
    ai_Unk9363f4 = n;
    if (n >= 15)
        ai_Unk9363f4 = 0;
    if (ai_Unk6f0cd8 < 15)
        ai_Unk6f0cd8++;
}

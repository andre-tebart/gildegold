/* 0x4cec00, closes the 32 windows of ai_Unk935d20 */

void ai_Unk4cec00()
{
    int *p;

    for (p = ai_Unk935d20; (int)p < (int)(ai_Unk935d20 + 0x20); p++) {
        if (*p != -1) {
            d2_RemoveWindow(*p);
            *p = -1;
        }
    }
}

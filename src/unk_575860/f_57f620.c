/* 0x57f620, the fire check (74 B): sc + 0x7c clear and the zoom sc + 0x20 <= 0.01: the fire button sc + 0xb8
   clicked (0xf8a668 / 0x5fe710) or the shot pending 0x14681f2 -> 0x14681f2 = 0, the timer sc + 0x78 = sc + 0x6c,
   the recoil sc + 0x7c = 1; 1, else 0 */

int ai_Unk57f620(char *sc)
{
    if (*(int *)(sc + 0x7c) == 0 && *(float *)(sc + 0x20) <= 0.01f) {
        if (U_I((unsigned)&d2_ClickedId) != -1 && U_I((unsigned)&ai_Unk5fe710) == *(int *)(sc + 0xb8)) {
            *(char *)((unsigned)&kb_Down + 0x32) = 0;
            *(int *)(sc + 0x78) = *(int *)(sc + 0x6c);
            *(int *)(sc + 0x7c) = 1;
            return 1;
        }
        if (*(char *)((unsigned)&kb_Down + 0x32) != 0) {
            *(char *)((unsigned)&kb_Down + 0x32) = 0;
            *(int *)(sc + 0x78) = *(int *)(sc + 0x6c);
            *(int *)(sc + 0x7c) = 1;
            return 1;
        }
    }
    return 0;
}

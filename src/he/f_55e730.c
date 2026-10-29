/* 0x55e730, the run handler of the exam repeat event ("Berufe\Messagebox_Meisterpruefung"; ROUGH skeleton, 815 bytes): opens the window for the player (0x53d6f0, d2_SetForm), shows the offer (0x515960, the news 0x53dda0 via 0x5176d0), the answer pays / refuses (0x4c5fa0, 0x50b290, 0x4c4b00 relation) and closes (0x53d800, free) */

void he_Unk55e730(int ev)
{
    int st = *(int *)(ev + 0x70);

    if (st < 0 || st > 1) {
        if (*(int *)(ev + 0x74) != 0)
            he_Unk53d800(ev);
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (st == 0) {
        he_Unk53d6f0(ev, 0, 0, (int)"Berufe\\Messagebox_Meisterpruefung", 0x5fd);
        if (*(int *)(ev + 0x74) == 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        d2_SetForm(*(int *)(*(int *)(ev + 0x74) + 8), 1);
        *(int *)(ev + 0x70) = 1;
        return;
    }
    if (U_I((unsigned)&d2_ClickedId) == -1)
        return;
    ((void (*)(int))ai_Unk4c5fa0)(*(int *)(ev + 0xac));
    he_Unk53d800(ev);
    he_Unk53e1f0((int *)ev);
}

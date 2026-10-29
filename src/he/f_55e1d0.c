/* 0x55e1d0, the run handler "Entlassen" of the dismissal with news (ROUGH skeleton, 464 bytes): the figure +0xac (0x505df0), 0x50c520, the job end 0x4c5970 / 0x4c5b90, the marks 0x4c3ba0 / 0x4c3d90 / 0x4c49e0, the walk away (0x4fdcb0 contact, 0x4c52e0 "Entlassen"), the relation 0x4c4b00 and the news "_NACHRICHTEN_HS_01" (0x53dee0, with the building main object) to the owner; the record is freed */

void he_Unk55e1d0(int ev)
{
    int p;
    char text[0x100];

    p = gm_FindChar(*(int *)(ev + 0xac));
    if (p == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    ((void (*)(int, int))ai_Unk4c5970)(0, 0);
    ai_Unk4c5b90(*(int *)(p + 4));
    ai_Unk4c52e0(*(int *)(p + 4), -1, -1, 0, 0, "Entlassen");
    txt_Format(text, 0x1bba, p);
    he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, *(int *)(p + 4), -1, "_NACHRICHTEN_HS_01", 0);
    he_Unk53e1f0((int *)ev);
}

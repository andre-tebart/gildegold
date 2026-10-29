/* 0x490830, the execute callback of "Graveyard Owner" */

int ai_Unk490830(AGCH *p, AGARG *s1, AGARG *s2)
{
    AGARG *a = s1;

    gm_FindById((int)&s1, 0, 0, a->unk0c);
    if (s1 != 0) {
        switch (a->kind) {
        case 0:
            return ai_Unk4900c0((AIPX *)p, (AIBLD *)s1, (int)a, s2);
        case 1:
            return ai_Unk4902b0((AIPX *)p, (AIBLD *)s1, (int)a, s2);
        case 2:
            return ai_Unk4904c0((AIPX *)p, (AIBLD *)s1, (int)a, s2);
        case 3:
            return ai_Unk490600((AIPX *)p, (AIBLD *)s1, (int)a, s2);
        }
    }
    return 0;
}

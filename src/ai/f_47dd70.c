/* 0x47dd70, a character kind that works for a master (2..5) */

int ai_WorksForMaster(AICHAR *c)
{
    char k;

    k = c->kind;
    if (k != 2 && k != 5 && k != 4 && k != 3)
        return 0;
    return 1;
}

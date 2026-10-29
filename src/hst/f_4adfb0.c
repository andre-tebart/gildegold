/* 0x4adfb0, 1 if id is in none of the four lists of hst_Lists */

int ai_NotInLists(int id)
{
    int i, j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 8; j++) {
            if (hst_Lists[i].e[j].id == id)
                return 0;
        }
    }
    return 1;
}

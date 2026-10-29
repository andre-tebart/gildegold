/* 0x402ac0 */

CHARACTER *ch_FindByObject(D3OBJECT *obj)
{
    int i;

    for (i = 0; i < CH_MAX; i++)
        if (ch_List[i] && ch_List[i]->obj == obj)
            return ch_List[i];
    return 0;
}

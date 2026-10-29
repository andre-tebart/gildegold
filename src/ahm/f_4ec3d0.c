/* 0x4ec3d0, the value of the alchemist object p (the same formula as ahm_StepValue, for a whole building): the four shares of its record squared and added,
   plus the recipe value of each of its six places (ahm_FindRecipe) added to the place's value, both sums times 500 (0x5ed098), as an int shifted by 5. Used
   by the alchemist's windows (0x57cf90, 0x57de60) as the value they build their texts from (0x5176d0). -1 when p has no owner character (ahm_OwnerChar).
   Matching notes: the two products are separate statements with the recipe sum first (`v = sum2 * 500.0f; v = v + sum * 500.0f;`) - one expression made
   VC6 factor them into (sum + sum2) * 500 and left an extra fxch; sum2 = 0.0f is assigned before the ahm_OwnerChar call, as the original clears the slot
   before the call. */

int ahm_ObjectValue(unsigned char *p)
{
    AHMOBJ *o = (AHMOBJ *)p;
    AHMREC *rec;
    char *c;
    float s, sum, sum2, v;
    AHMGRID *g;
    int i;

    sum2 = 0.0f;
    c = ahm_OwnerChar(p);
    if (c == 0)
        return -1;
    rec = o->rec;
    s = ahm_Share(c, 0);
    sum = s * s;
    s = ahm_Share(c, 1);
    sum = s * s + sum;
    s = ahm_Share(c, 2);
    sum = s * s + sum;
    s = ahm_Share(c, 3);
    sum = s * s + sum;
    for (i = 0; i < 6; i++) {
        g = ahm_FindRecipe(rec->slots[i].id);
        if (g != 0)
            sum2 = g->rcp.a + rec->slots[i].v + sum2;
    }
    v = sum2 * 500.0f;
    v = v + sum * 500.0f;
    return (int)v << 5;
}

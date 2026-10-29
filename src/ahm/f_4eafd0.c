/* 0x4eafd0, the progress the character c makes researching the recipe g in *cap minutes (*cap gets the minutes used): ahm_ResearchTime * (1 - share 2 / 2) * (1 - the
   value of the character's place of g) is the time t the rest of the place needs; min(*cap, t) are used and the progress they are worth ((used / t) * (1 - value)) is
   returned; 0.0 when the place is full / not there / nothing is left */

float ahm_ResearchProgress(char *c, AHMGRID *g, int *cap)
{
    float s2;
    AHMSLOT *slot;
    int n, b, t, m;

    s2 = ahm_Share(c, 2);
    slot = ahm_SlotOf(c, g->rcp.id);
    if (slot == 0)
        return 0.0f;
    if (slot->v >= 1.0f)
        return 0.0f;
    n = *cap;
    if (n <= 0)
        return 0.0f;
    b = ahm_ResearchTime(c, g);
    t = (int)(b * (1.0f - s2 * 0.5f) * (1.0f - slot->v));
    if (t <= 0)
        return 0.0f;
    m = n < t ? n : t;
    *cap = m;
    return (float)m / t * (1.0f - slot->v);
}

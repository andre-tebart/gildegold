/* 0x4ec490, posts the type 99 message (0x4c6170) that makes the alchemist building o (kind 0x1e) create its owner character from the walk step s: o's id, the
   step's prototype entry, the step's name indexes x / y and the +8 of the recipe entry whose id is the prototype's place id (ahm_FindRecipe); -1 when there is no
   such recipe entry. The prototype is copied with its union assignment: that is what makes VC6 hold the entry's address in a register (lea [s+4]). */

int ahm_SendCreateAlchemist(unsigned char *o, AHMSTEP *s)
{
    AHMOBJ *obj = (AHMOBJ *)o;
    AHMGRID *g;
    AHMCREATEMSG m;

    g = ahm_FindRecipe(s->grid.plr.id);
    if (g == 0)
        return -1;
    m.building = obj->id;
    m.proto = s->grid;
    m.x = s->x;
    m.y = s->y;
    m.value = g->w[2];
    return ((int (*)(void *))cm_SendCreateAlchemist)(&m);
}

/* 0x44eb10, number of materials */

void agi_Materials(void *unused, AGICTX *c)
{
    int n = atoi(AGI_TOK(" "));

    c->nmats = n;
    c->cur = 0;
    if (n > 0)
        c->mats = (AGIMAT *)mem_Alloc(n * sizeof(AGIMAT), "d3_io_a:AGIMaterialBuffer");
}

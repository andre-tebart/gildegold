/* 0x50f570, init_Betrieb (the error text "init_Betrieb(): costs are zero" 0x622fe0): stocks a producer
   building b of the classes 1 / 4 (gm_BuildingClass). The first object 0xff of the building that is not
   a kind 0x25 good opens the recipe list: its object list (gm_Query kind 5) is walked while the good's
   prototype name starts with '%'; the four (weight, recipe) pairs at +0x26 / +0x2e of that prototype give the
   cost sum - the price of each recipe (gm_ItemPrice 0x5087f0) times its weight, truncated - and the amount is
   16000 / sum (a float division for the prototype kind 0x15, an integer one otherwise), at least 1. Each
   recipe is then sent into the stall (the object kind 4) in the amount times its weight, capped at the
   stall's stock (gm_ListStock 0x50ab50); 0xffff recipe slots are skipped, a 0 slot ends the list.
   The error text fires when the sum is zero.
   @deep B3 draft, 177/177 instructions, 38 differing: the rest is the sum / i register swap (ours sum=ebx i=ebp, the original sum=ebp i=ebx - spell.py's only menu entry, 50f6a6 regs ebp->ebx ebx->ebp) plus the error tail's block placement (the original's 'costs are zero' tail sits at the function end 0x50f73a behind a je, ours right after the test) and the add/lea fold of the recipe base (50f616 / 50f61a). All hunks read against 0x50f570..0x50f74e: no meaning error left - the two meaning errors found are fixed (the sum weights the price by rw[-4] through fimul, the stall clamp is if (amount < stock) n = amount; else n = stock) and took 152 -> 38 differing instructions. Tried, no change: the local declaration order, the (short*)+0x17 scaled recipe base, the while/for spelling of the recipe walk, -1 vs 0xffff (the latter fixed cmp ax,0xffff). Readability done: Q1 init_Betrieb (the error string 'init_Betrieb(): costs are zero', 0x622fe0, is the function's own), Q2 (gm_BuildingClass -> gm_BuildingClass project-wide, err_Message call gone, gm_ListStock prototype), Q4 (INIT_BLD parameter, INIT_OBJ objId / list), Q6. */


void init_Betrieb(INIT_BLD *b)
{
    int cls, i, sum, n, v;
    char *p;
    INIT_OBJ *o1, *st;
    unsigned short *rw, *rw0;

    cls = gm_BuildingClass(b->type);
    p = ai_BTypes + (unsigned)b->type * 0x24d;
    if (cls != 1 && cls != 4)
        return;
    o1 = (INIT_OBJ *)gm_Query((void *)b->objList, 1, 0, 0xff);
    if (o1 == 0)
        return;
    st = (INIT_OBJ *)gm_Query((void *)o1->list, 1, 5);
    if (st == 0)
        return;
    while (st != 0 && ai_Goods[st->proto * 0x41] == '%')
        st = (INIT_OBJ *)gm_QueryNext();
    if (st == 0)
        return;
    sum = 0;
    i = 0;
    rw0 = (unsigned short *)(ai_Goods + st->proto * 0x41 + 0x2e);
    rw = rw0;
    for (i = 0; i < 4; i++) {
        if (*rw == 0)
            break;
        if (*rw != 0xffff)
            sum += (int)(gm_ItemPrice(*rw, 100) * rw[-4]);
        rw++;
    }
    if (sum == 0) {
        err_Message("init_Betrieb(): costs are zero");
        return;
    }
    if (*p == 0x15)
        n = (int)(16000.0f / sum);
    else
        n = 16000 / sum;
    if (n <= 1)
        n = 1;
    st = (INIT_OBJ *)gm_Query((void *)b->objList, 1, 4, 5);
    if (st == 0)
        return;
    for (i = 0; i < 4; i++) {
        if (*rw0 == 0)
            return;
        if (*rw0 != 0xffff) {
            v = rw0[-4] * n;
            if (v < gm_ListStock((short *)st, *rw0))
                n = v;
            else
                n = gm_ListStock((short *)st, *rw0);
            cm_SendExSellObject(st->objId, -1, *rw0, n, ai_OwnPlayer, 0);
        }
        rw0++;
    }
}

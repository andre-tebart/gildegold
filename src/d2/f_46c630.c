/* 0x46c630, selects button sel of a radio button group */

void d2_RadioSelect(int g, int sel)
{
    int i;

    for (i = 0; i < d2_Radios[g].n; i++)
        if (i != sel)
            d2_SetObjValue(d2_Radios[g].objs[i], 0);  /* @original: this call site pushes 2 arguments (0x46c661: push 0; push eax) against the 5-parameter definition */
        else
            d2_SetObjValue(d2_Radios[g].objs[i], 1);  /* @original: this call site pushes 2 arguments (0x46c66a: push 1; push ecx) against the 5-parameter definition */
    d2_Radios[g].sel = sel;
}

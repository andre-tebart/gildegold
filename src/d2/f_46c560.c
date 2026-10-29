/* 0x46c560, adds an object to a radio button group (the first one gets selected) */

void d2_RadioAdd(int g, int obj)
{
    d2_Radios[g].objs[d2_Radios[g].n] = obj;
    d2_Objs[obj].checkable = 1;
    d2_Objs[obj].style &= ~2;
    d2_Radios[g].n++;
    if (d2_Radios[g].sel == -1)
        d2_RadioSelect(g, 0);
}

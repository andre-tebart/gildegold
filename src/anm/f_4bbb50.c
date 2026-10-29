/* 0x4bbb50, removes animal a (ch_Delete of its object) */

void anm_RemoveAnimal(ANMREC *a)
{
    ch_Delete(a->obj);
    memset(a, 0, sizeof(ANMREC));
    anm_NAnimals--;
}

/* 0x4ea990, the yearly pay of all the owned offices: the salary of each office's kind (AMTKIND.salary, times 32) times
   the pay rate, summed over the 42 records. The rate is the factor amt_Pay[buf[6]] of the decision record 0xf that
   gv_GetTableRecord fills (9 ints) */

int amt_GetOwnedOfficePay(void)
{
    int buf[9];
    int pay, sum, i;

    gv_GetTableRecord(0xf, buf);
    pay = (int)amt_Pay[buf[6]];
    sum = 0;
    for (i = 0; i < 42; i++) {
        if (amt_Table[i].owner != -1)
            sum += (int)((double)(amt_Kinds[amt_Table[i].kind].salary << 5) * pay);
    }
    return sum;
}

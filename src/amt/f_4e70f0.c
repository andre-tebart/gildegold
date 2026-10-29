/* 0x4e70f0, collects up to max of the column-7 offices (the table records 30 .. 36: the deputies 0x1c / 0x1d, the town mayors 0x1e .. 0x21 and 0x22) into out,
   forward; a record is taken when its kind's column is 7 and, with `all` 0, when it has an owner (-1 = free; the yearly check 0x4e9260 passes all 1). Writes the
   records and returns their number (0 .. max); the scan stops at max */

int amt_GetColumn7Offices(int all, int max, AMTREC *out)
{
    int n, i;

    n = 0;
    i = 30;
    do {
        if (n >= max)
            return n;
        if (amt_Kinds[amt_Table[i].kind].col == 7 && (all != 0 || amt_Table[i].owner != -1)) {
            n++;
            *out = amt_Table[i];
            out++;
        }
        i++;
    } while (i < 37);
    return n;
}

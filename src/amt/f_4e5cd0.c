/* 0x4e5cd0, clears the office table (42 records of 0x18 bytes at 0x925120) and numbers them: +0 the index, +4 and +0x14 -1, +8 the kind of the office
   (0 .. 1 the first two, then 2, 3, 4, 4, 5, 6, 7, 7, and i - 2 for the records 10 .. 41); the table loader 0x4ea7a0 calls it first */

void amt_InitTable(void)
{
    int i;

    AI_ZERO_AT(offset amt_Table, 0x3f0);
    for (i = 0; i < 42; i++) {
        amt_Table[i].idx = i;
        amt_Table[i].owner = -1;
        amt_Table[i].x14 = -1;
    }
    amt_Table[2].kind = 2;
    amt_Table[0].kind = 1;
    amt_Table[1].kind = 1;
    amt_Table[3].kind = 3;
    amt_Table[4].kind = 4;
    amt_Table[5].kind = 4;
    amt_Table[6].kind = 5;
    amt_Table[7].kind = 6;
    amt_Table[8].kind = 7;
    amt_Table[9].kind = 7;
    amt_Table[10].kind = 8;
    amt_Table[11].kind = 9;
    amt_Table[12].kind = 10;
    amt_Table[13].kind = 11;
    amt_Table[14].kind = 12;
    amt_Table[15].kind = 13;
    amt_Table[16].kind = 14;
    amt_Table[17].kind = 15;
    amt_Table[18].kind = 16;
    amt_Table[19].kind = 17;
    amt_Table[20].kind = 18;
    amt_Table[21].kind = 19;
    amt_Table[22].kind = 20;
    amt_Table[23].kind = 21;
    amt_Table[24].kind = 22;
    amt_Table[25].kind = 23;
    amt_Table[26].kind = 24;
    amt_Table[27].kind = 25;
    amt_Table[28].kind = 26;
    amt_Table[29].kind = 27;
    amt_Table[30].kind = 28;
    amt_Table[31].kind = 29;
    amt_Table[32].kind = 30;
    amt_Table[33].kind = 31;
    amt_Table[34].kind = 32;
    amt_Table[35].kind = 33;
    amt_Table[36].kind = 34;
    amt_Table[37].kind = 35;
    amt_Table[38].kind = 36;
    amt_Table[39].kind = 37;
    amt_Table[40].kind = 38;
    amt_Table[41].kind = 39;
}

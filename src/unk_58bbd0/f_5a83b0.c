/* 0x5a83b0, the goods weights of a record (256 B): the first ten ushort ids of 0x5a84d0 (a character) are added
   to the seven {id, weight} slots at +0xc (stride 8, the weight 0xe), the weights then adjusted by 1 to sum 100
   (0x100 / 0x65 per slot) */

void ai_Unk5a83b0(int param_1, char *tbl)
{
    unsigned short *list;
    unsigned char *w;
    int i;
    int j;
    int n;
    int sum;
    int count;
    int slots;
    int v;
    int step;
    int found;

    list = (unsigned short *)ai_Unk5a84d0((char *)param_1);
    if (list == 0)
        return;
    list = (unsigned short *)((char *)list + 1);
    for (n = 0; n < 10; n++) {
        if (list[n] != 0) {
            found = 0;
            for (j = 0; j < 7; j++)
                if (*(unsigned int *)(tbl + 0xc + j * 8) == list[n])
                    found = 1;
            if (!found) {
                for (j = 0; j < 7; j++) {
                    if (*(unsigned int *)(tbl + 0xc + j * 8) == 0) {
                        *(unsigned int *)(tbl + 0xc + j * 8) = list[n];
                        *(unsigned char *)(tbl + 0x10 + j * 8) = 0xe;
                        break;
                    }
                }
            }
        }
    }
    sum = 0;
    count = 0;
    for (j = 0; j < 7; j++) {
        if (*(unsigned int *)(tbl + 0xc + j * 8) != 0) {
            count++;
            sum += *(unsigned char *)(tbl + 0x10 + j * 8);
        }
    }
    if (sum != 100) {
        if (sum < 100) {
            step = 1;
            n = 100 - sum;
        } else {
            n = sum - 100;
            step = -1;
        }
        j = 0;
        while (n != 0 && count != 0) {
            if (*(unsigned int *)(tbl + 0xc + j * 8) != 0) {
                w = (unsigned char *)(tbl + 0x10 + j * 8);
                v = (int)*w + step;
                if (v > -1 && v < 100) {
                    n--;
                    *w = (unsigned char)(*w + step);
                }
            }
            j = (j + 1) % 7;
        }
    }
}

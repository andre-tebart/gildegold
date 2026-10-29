/* 0x4e6f10, the column (rank class) of the kind k (0 for k > 39) */

int amt_KindColumn(int k)
{
    if (k >= 40)
        return 0;
    return amt_Kinds[k].col;
}

/* 0x4e6f80, copies the kind record k to *out (the first one for k > 39); 1 when k is valid */

int amt_GetKind(int k, AMTKIND *out)
{
    if (k >= 40) {
        *out = amt_Kinds[0];
        return 0;
    }
    *out = amt_Kinds[k];
    return 1;
}

/* 0x5051d0, the record n (3 ints; kinds above 9: the first one) of the table 0x617850 into out; 1 for n <= 9, else 0 */

int gm_Unk5051d0(int n, int *out)
{
    if (9 < n) {
        out[0] = ai_Unk617850[0];
        out[1] = ai_Unk617850[1];
        out[2] = ai_Unk617850[2];
        return 0;
    }
    out[0] = ai_Unk617850[n * 3];
    out[1] = ai_Unk617850[n * 3 + 1];
    out[2] = ai_Unk617850[n * 3 + 2];
    return 1;
}

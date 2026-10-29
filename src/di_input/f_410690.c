/* 0x410690, clear the right-button event fields (rup / rdown / rhold / rclick) of dm_L, dm_P and dm_S */

void di_ClearAxes(void)
{
    di_A[7] = 0;
    di_B[7] = 0;
    di_C[7] = 0;
    di_A[4] = 0;
    di_B[4] = 0;
    di_C[4] = 0;
    di_A[5] = 0;
    di_B[5] = 0;
    di_C[5] = 0;
    di_A[6] = 0;
    di_B[6] = 0;
    di_C[6] = 0;
}

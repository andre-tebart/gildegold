/* 0x410620 */

/* 0x410620 */
#define di_A (&dm_L.ldown)              /* 0x1467390 */
#define di_B (&dm_P.ldown)              /* 0x14683d0 */
#define di_C (&dm_S.ldown)              /* 0x14673f0 */

void di_ClearButtons(void)
{
    di_A[2] = 0;
    di_B[2] = 0;
    di_C[2] = 0;
    di_A[0] = 0;
    di_B[0] = 0;
    di_C[0] = 0;
    di_A[1] = 0;
    di_B[1] = 0;
    di_C[1] = 0;
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

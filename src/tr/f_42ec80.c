/* 0x42ec80, seeds the C runtime random generator with the current time (timeGetTime, ms) */

void util_Randomize(void)
{
    srand(timeGetTime());
}

/* 0x427b20 */

int evt_SkipStatement(void)
{
    while (evt_Cur->pc < evt_Cur->source + evt_Cur->sourcelen) {
        if (*evt_Cur->pc == ';') {
            evt_Cur->pc++;
            return 1;
        }
        evt_Cur->pc++;
    }
    return 0;
}

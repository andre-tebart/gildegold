/* 0x427b90 */

int evt_SkipBlock(char dir)
{
    int depth = 0;

    if (dir == 1) {
        while (evt_Cur->pc < evt_Cur->source + evt_Cur->sourcelen) {
            if (*evt_Cur->pc == '{') {
                depth++;
            } else if (*evt_Cur->pc == '}' && --depth <= 0) {
                evt_Cur->pc++;
                return 1;
            }
            evt_Cur->pc++;
        }
    } else if (dir == 2) {
        while (evt_Cur->pc >= evt_Cur->source) {
            if (*evt_Cur->pc == '{') {
                if (--depth <= 0)
                    return 1;
                depth--;
            } else if (*evt_Cur->pc == '}') {
                depth++;
            }
            evt_Cur->pc--;
        }
    }
    return 0;
}

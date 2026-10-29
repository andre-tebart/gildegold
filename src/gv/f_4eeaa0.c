/* 0x4eeaa0, reserves the effect slot of the id: 1 when it has one already or a free one (id -1) is found (the first free one is taken); 0 for id -1 or no slot */

int gv_ReserveEffect(int id)
{
    int *p;
    int first, i;

    first = -1;
    if (id != -1) {
        i = 0;
        p = (int *)gv_Effects;
        do {
            if (*p == -1) {
                if (first == -1)
                    first = i;
            } else if (*p == id)
                return 1;
            p += 0x5c;
            i++;
        } while ((int)p < (int)(gv_Effects + 8 * 0x170));
        if (first >= -2) {
            p = (int *)gv_Effects;
            p[first * 0x5c] = id;
            return 1;
        }
    }
    return 0;
}

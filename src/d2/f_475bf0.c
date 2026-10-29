/* 0x475bf0, step of a counter by its range */

int d2_InputStep(D2INPUT *in)
{
    int r = 1;
    int d = in->maxval - in->minval;

    if (d >= 125000)
        return 10000;
    if (d >= 100000)
        return 5000;
    if (d >= 75000)
        return 4000;
    if (d >= 50000)
        return 3000;
    if (d >= 37500)
        return 2000;
    if (d >= 25000)
        return 1500;
    if (d >= 12500)
        return 1000;
    if (d >= 5000)
        return 500;
    if (d >= 2500)
        return 200;
    if (d >= 1000)
        return 100;
    if (d >= 250)
        return 50;
    if (d >= 100)
        r = 10;
    return r;
}

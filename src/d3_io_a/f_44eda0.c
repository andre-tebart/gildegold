/* 0x44eda0, reads the points, one line each */

void agi_Points(void *f, AGICTX *c)
{
    char *line;
    int i;

    for (i = 0; i < c->npts; i++) {
        line = util_ReadLine(f);
        c->pts[c->ptidx].x = atof(strtok(line, " "));
        c->pts[c->ptidx].y = atof(strtok(0, " "));
        c->pts[c->ptidx].z = atof(strtok(0, " "));
        c->ptidx++;
    }
    util_ReadLine(f);
}

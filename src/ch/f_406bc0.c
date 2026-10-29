/* 0x406bc0, every step-th point of path (always the first and the last) into out; returns the count */

int ch_ThinPath(CHARACTER *ch, CHPATH *path, CHPATHPT *out, CHWALKDEF *w)
{
    int n = 0;
    int i = 1;
    int step;
    int x, z;

    if (ch && (ch->kind & 0x10))
        step = 2;
    else {
        step = (int)(12.0f / w->speed + 0.5f);
        if (step < 1)
            step = 1;
    }
    for (; i < path->n; i += step) {
        x = path->pts[i].x;
        z = path->pts[i].z;
        out[n].x = x;
        out[n].z = z;
        n++;
    }
    if (i != path->n - 1) {
        out[n].x = path->pts[path->n - 1].x;
        out[n].z = path->pts[path->n - 1].z;
        n++;
    }
    return n;
}

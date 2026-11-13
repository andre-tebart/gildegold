/* probe for the c2 call-skip map: scheduling (int / fpu interleave, add esp after a call), cross-jumping,
   epilogue copies, a loop */
typedef struct { int pad[13]; void *snd; int pad2[7]; } S;
extern S g[2];
extern int a, b, c, sc, scene, dirty, tab[8];
extern float Zero, Range[];
extern unsigned int Vol[];
typedef struct { int type, vol, pan, x, y, z; } HIT;
void stop(void *, int); void restart(void *, int);
int body(int); void use(HIT *, int); int get(void); void en(int, int); void pr(int);

void f_off(void)
{
    int x[40];
    if (!a || !b || !c) goto off;
    if (!scene) scene = sc; else if (scene != sc) { scene = sc; goto off; }
    x[0] = 0; x[1] = 0; x[2] = 0; x[3] = 0; x[5] = 0;
    body((int)x); body(0); body(0);
    return;
off:
    if (g[0].snd) { stop(g[0].snd, 0); restart(g[0].snd, 0); g[0].snd = 0; }
    if (g[1].snd) { stop(g[1].snd, 0); restart(g[1].snd, 0); g[1].snd = 0; }
}
void f_head(void)
{
    HIT found[2];
    int ground = get();
    int type = get();
    if (ground > 2000) ground = 2000;
    found[0].type = type;
    found[0].vol = (int)((Vol[type] - Vol[type] * Zero / (Range[type] * Range[type])) * (2000 - ground) * 0.0005f);
    found[0].pan = 63;
    use(found, ground);
}
void f_xjump(void)
{
    int x[40]; x[0] = 0; x[1] = 0; x[2] = 0; x[3] = 0; x[5] = 0; body((int)x);
    if (a) { stop(0, 0); restart(0, 0); } else if (g[0].snd) { stop(g[0].snd, 0); restart(g[0].snd, 0); g[0].snd = 0; }
    if (g[1].snd) { stop(g[1].snd, 0); restart(g[1].snd, 0); g[1].snd = 0; }
}
void f_ret(int *s)
{
    int z[4], i, n = 0;
    z[0] = 0; z[1] = 0; z[2] = 0; z[3] = 0; pr((int)z);
    for (i = 0; i < 3; i++) {
        if (tab[i] == i) {
            dirty = 1;
            if (get()) n++;
            if (n != 0) { dirty = 0; tab[i] = -1; en(s[29], 1); return; }
        }
    }
    en(s[29], 0);
    dirty = 0;
}
int f_loop(int *p, int n)
{
    int i, t = 0;
    for (i = 0; i < n; i++) { if (p[i] > 3) t += p[i] * 5; else t -= body(p[i]); }
    return t;
}

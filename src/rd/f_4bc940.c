/* 0x4bc940, loads plant q's model ("%svegetation/*pfl_<type>_0<n>[_FALL].ogr"), names it "_ID<id>" and places it
   on the height map
   @deep behaviour: far draft, 230 code hunks - the height-map block has another structure: the original keeps the three running sums in stack slots, accumulates x*m[0x28..0x2a]+m[0x24..0x26] then z*m[0x2c..0x2e], computes the cell pointer after that and adds the *hm*m[0x30..0x32] term last (rewriting our expression into that order compiles to the same code, so VC6 reassociates); frame/register allocation and the sprintf/strcat argument setup differ too; Q2/Q4 fixed (G15OBJ.mask, D3OBJECT.unk224 -> mask), readable */

void veg_LoadModel(G15PLANT *q, int fall)
{
    G15TREE *t;
    int n;
    char *fmt;
    char *o;
    int x, z;
    float *m;
    unsigned char *hm;
    float pos[3];
    char path[0x100];
    char buf[0x100];

    t = veg_FindType(q->type);
    if (fall == 0) {
        n = q->state / t->states;
        if (n < 1)
            n = 1;
        fmt = "%svegetation/*pfl_%s_0%i.ogr";
    } else {
        n = q->state / t->states;
        if (n < 1)
            n = 1;
        fmt = "%svegetation/*pfl_%s_0%i_FALL.ogr";
    }
    sprintf(path, fmt, ai_DataPath, t->name, n);
    o = d3_LoadGroup(path, 0, 0);
    if (o == 0) {
        sprintf(buf, "init_SetPflanzenMap(): Could not load 3D-Objekt-roup '%s'...", path);
        err_Message(buf);
        return;
    }
    q->obj = o;
    sprintf(buf, "_ID%i", q->id);
    strcat(o, buf);
    ((G15OBJ *)o)->mask = 1;
    if (d3_Terrain != 0) {
        m = (float *)d3_Terrain;
        x = q->x;
        z = q->z;
        hm = (unsigned char *)(((int *)d3_Terrain)[0] * z + ((int *)d3_Terrain)[4] + x);
        pos[0] = x * m[0x28] + m[0x24] + z * m[0x2c] + *hm * m[0x30];
        pos[1] = x * m[0x29] + m[0x25] + z * m[0x2d] + *hm * m[0x31];
        pos[2] = x * m[0x2a] + m[0x26] + z * m[0x2e] + *hm * m[0x32];
        d3_SetObjectPos(o, pos);
    }
    d3_RelightObjects(o);
}

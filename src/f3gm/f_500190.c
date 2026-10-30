/* 0x500190, creates an object instance of the prototype proto with the amount n in the list of the owner id (a building, an object or a character, found by 0x4fea50): appends it to the owner's list (the owner list head: +0x5d building, +0x14 object, +0x178 character), takes a free record (0x4fffc0), fills it (+0 prototype, +2 a new id, +6 the owner id, +0xa the owner's number, +0xe the amount, +0x12 0x64 = 100, +0x3f next 0) and counts it (0x6f1bdc); a few prototypes get their own start values (0xb8, 0x12c, 0x12d, 0x15e / 0x160, 0x174 / 0x176, 0x24b / 0x256 / 0x266) and the three transport carts 0x134 / 0x135 / 0x136 get a model (cart or horse, 0x403390 at the dummy "dummy_TRANSPORTER") with a load object (ob_KARREN_*). Returns the instance, 0 on failure. ROUGH */

extern int gm_NOffers;
int gm_ListStock();                     /* 0x50ab50 */
int gm_ListAmount();                     /* 0x50aaf0 */
float gm_GoodsPrice(unsigned id, unsigned char p);                   /* 0x5089c0 */
unsigned short * gm_Unk50a050(unsigned short *p);                     /* 0x50a050 */
void init_HookObject(char *o, unsigned short *p, int kind);                     /* 0x50d380 */
struct CHTRANSPORT * ch_AttachTransport();                     /* 0x4034e0 */
char d3if_SelectTextureSet(struct D3OBJECT *obj, struct D3SURF *surf, unsigned char set, char reload);                     /* 0x446f30 */
struct CHARACTER * ch_Create();                     /* 0x403390 */
void ch_PreloadAnims(struct CHARACTER *ch, int n, ...);                     /* 0x404200 */
void ch_SetVisible();                     /* 0x402480 */
int gm_IsHomeType();                     /* 0x502520 */
int init_BuildingSceneUniverse();                     /* 0x511e60 */

unsigned short *gm_CreateObject(int id, unsigned proto, int n)
{
    int b, o, c;
    int it, it2;
    int *list;
    int last;
    unsigned short *r;
    unsigned short *q;
    char *pr;
    char buf[1024];
    int pos[3], zero3[3];
    int saved, sc, m, ld;
    char *name;
    short s;
    int *hd;

    b = 0;
    o = 0;
    c = 0;
    last = 0;
    r = 0;
    pr = ai_Goods;
    if (ai_Goods == 0)
        return 0;
    if (id == -1)
        return 0;
    if (gm_FindById(&b, &o, &c, id) == 0)
        return 0;
    if (b == 0) {
        if (o == 0) {
            if (c == 0)
                return 0;
            list = (int *)(c + 0x178);
        } else
            list = (int *)(o + 0x14);
    } else
        list = (int *)(b + 0x5d);
    if (list != 0) {
        it = *list;
        if (*list == 0) {
            r = (unsigned short *)gm_FindFreeObject();
            if (r == 0)
                return 0;
            *list = (int)r;
            pr = ai_Goods;
        } else {
            do {
                last = it;
                it = *(int *)(last + 0x3f);
            } while (*(int *)(last + 0x3f) != 0);
            r = (unsigned short *)gm_FindFreeObject();
            if (r == 0)
                return 0;
            *(unsigned short **)(last + 0x3f) = r;
            pr = ai_Goods;
        }
    }
    pr = pr + proto * 0x41;
    *r = (unsigned short)proto;
    sprintf(buf, "----> ObjID: %i ObjProt: %i", ai_NextObjectId, proto & 0xffff);
    *(int *)(r + 1) = ai_NextObjectId;
    ai_NextObjectId++;
    if (b == 0) {
        if (o == 0) {
            if (c != 0)
                *(int *)(r + 5) = *(int *)(c + 4);
        } else
            *(int *)(r + 5) = *(int *)(o + 10);
    } else
        *(int *)(r + 5) = *(int *)(b + 1);
    *(int *)(r + 3) = id;
    *(int *)(r + 7) = n;
    *(char *)(r + 9) = 'd';
    *((char *)r + 0x13) = 0;
    r[10] = 0;
    r[0xb] = 0;
    *(int *)((char *)r + 0x3f) = 0;
    f3gm_ObjectCount++;
    memset(r + 0xe, 0, 0x1d);
    if ((int)proto > 0x160) {
        switch (proto) {
        case 0x174:
        case 0x176:
            r[0x10] = 6;
            r[0x11] = 0;
            break;
        case 0x24b:
        case 0x256:
        case 0x266:
            r[0xe] = 0;
            r[0xf] = 0;
            r[0x10] = 0;
            r[0x11] = 0;
        }
    } else if (proto == 0x160 || proto == 0x15e) {
        r[0x10] = 0xf;
        r[0x11] = 0;
    } else {
        switch (proto) {
        case 0xb8:
            *(char *)(r + 0xe) = 6;
            break;
        case 300:
            r[0xe] = 0xffff;
            r[0xf] = 0xffff;
            r[0x10] = 0xffff;
            r[0x11] = 0xffff;
            r[0x12] = 0xffff;
            r[0x13] = 0xffff;
            *(int *)(r + 0x14) = *(int *)ai_Time - 1;
            break;
        case 0x12d:
            r[0xe] = 0xffff;
            r[0xf] = 0xffff;
            r[0x10] = 0xffff;
            r[0x11] = 0xffff;
            r[0x12] = 0xffff;
            r[0x13] = 0xffff;
            r[0x14] = 0xffff;
            r[0x15] = 0xffff;
            break;
        case 0x134:
        case 0x135:
        case 0x136:
            if (b == 0) {
                if (o == 0) {
                    if (c != 0)
                        *(int *)(r + 5) = *(int *)(c + 4);
                } else {
                    it = gm_Unk50a050(o);
                    *(int *)(r + 5) = *(int *)(it + 4);
                }
            } else
                *(int *)(r + 5) = *(int *)(ai_Chars[*(unsigned short *)(b + 0x27)] + 4);
            m = gm_FindById(&b, &o, &c, id);
            it = id;
            while (m != 0 && b == 0) {
                if (o != 0)
                    it = *(int *)(o + 6);
                m = gm_FindById(&b, &o, &c, it);
            }
            *(char *)(r + 0x14) = 0;
            *((char *)r + 0x31) = 2;
            if (b == 0) {
                r[0xe] = 0xffff;
                r[0xf] = 0xffff;
            } else
                *(int *)(r + 0xe) = *(int *)(b + 1);
            it = (int)gm_CreateObject(*(int *)(r + 1), 0x1dd, 1);
            *(unsigned *)(it + 0xe) = *r - 0x133;
            *(char *)(it + 0x1c) = (char)*r - 0x33;
            it = (int)gm_BuildQuery(1, 1, id);
            ld = it;
            sc = 0;
            if (it != 0 && gm_IsHomeType(it) != 0)
                sc = init_BuildingSceneUniverse(it);
            saved = d3_CurrentScene;
            d3if_SelectScene(sc, 1);
            if (gm_IsHomeType(it) == 0)
                sc = *(int *)(it + 0x61);
            else
                sc = 0;
            m = d3_FindObject(sc, 0x100, 0, "dummy_TRANSPORTER");
            if (m == 0) {
                if (*(int *)(it + 0x61) == 0) {
                    pos[0] = pos[1] = pos[2] = 0;
                } else {
                    pos[0] = *(int *)(*(int *)(it + 0x61) + 0x4c);
                    pos[1] = *(int *)(*(int *)(it + 0x61) + 0x50);
                    pos[2] = *(int *)(*(int *)(it + 0x61) + 0x54);
                }
            } else
                d3_PointToWorld(m, m + 0x4c, pos);
            if (proto == 0x136)
                name = "pferd_PFERD";
            else {
                s = util_RandomU(3);
                if (s == 0)
                    name = "arbeiter_KARREN";
                else if (s == 1)
                    name = "holzfaeller_KARREN";
                else
                    name = "handwerker_KARREN";
            }
            it2 = ch_Create(name, pos);
            *(int *)((char *)r + 0x3b) = it2;
            ch_PreloadAnims(it2, 1, "bewegung/karren_ziehen");
            init_HookObject(*(int *)(it2 + 0x34), r, 1);
            sprintf((char *)*(int *)(it2 + 0x34), "sp_%i", *(int *)(r + 1));
            if (proto == 0x134)
                name = "ob_KARREN_klein";
            else if (proto == 0x135)
                name = "ob_KARREN_mittel";
            else
                name = "ob_KARREN_GROSS";
            m = d3_CreateObject(0, pos, zero3, name);
            if (m != 0) {
                *(unsigned char *)(m + 0x212) = *(unsigned char *)(m + 0x212) & 0xf7 | 4;
                *(unsigned char *)(m + 0x211) = *(unsigned char *)(m + 0x211) & 0xfd;
                ch_AttachTransport(it2, m);
                *(int *)(m + 0x224) = 1;
                init_HookObject(m, r, 1);
            }
            if (ld == 0) {
                *(int *)(it2 + 0x30) = -1;
                sc = -1;
            } else if (gm_IsHomeType(ld) == 0) {
                ch_SetVisible(it2, 0);
                *(int *)(it2 + 0x30) = -1;
                sc = -1;
            } else {
                sc = *(int *)(ld + 1);
                *(int *)(it2 + 0x30) = -1;
            }
            *(int *)(it2 + 0x2c) = sc;
            r[0x12] = 0;
            r[0x13] = 0;
            d3if_SelectScene(saved, 1);
            break;
        }
    }
    if (*pr == 0x17 || *pr == ' ' || *pr == '%') {
        r[0xe] = 0;
        r[0xf] = 0;
    }
    return r;
}

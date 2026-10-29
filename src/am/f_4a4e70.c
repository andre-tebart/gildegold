/* 0x4a4e70, picks one of the scene objects listed under name: the nearest (with noise) to the character's
   building or the current object, else a random one */

int ai_Unk4a4e70(AIPER *p, int name)
{
    int scene = d3_CurrentScene;
    char buf[64];
    int list[256];
    float o[6];
    float q[6];
    float dx, dy, dz, d, best;
    int n, i, r, obj;
    unsigned int k;

    d3if_SelectScene(0, 1);
    if (ai_BuildingName(name, buf) != 0) {
        memset(list, 0, sizeof(list));
        n = ai_FindBauplaetze(buf, list);
        if (n != 0) {
            if (p->bld2 != 0 && *(int *)((char *)p->bld2 + 0x61) != 0 && util_Random(3) != 0) {
                obj = *(int *)((char *)p->bld2 + 0x61);
                d3_PointToWorld((void *)obj, (char *)obj + 0x4c, o);
                r = 0;
                best = 1e21f;
                for (i = 0; i < n; i++) {
                    if (list[i] != 0) {
                        d3_PointToWorld((void *)list[i], (char *)list[i] + 0x4c, q);
                        dx = q[0] - o[0];
                        dy = q[1] - o[1];
                        dz = q[2] - o[2];
                        d = (float)sqrt(dx * dx + dy * dy + dz * dz) * (util_RandomF() * 0.5f + 0.75f);
                        if (d < best) {
                            best = d;
                            r = list[i];
                        }
                    }
                }
                d3if_SelectScene(scene, 1);
                return r;
            }
            if (ai_MarketBuilding != 0 && (obj = *(int *)((char *)ai_MarketBuilding + 0x61)) != 0) {
                d3_PointToWorld((void *)obj, (char *)obj + 0x4c, o);
                r = 0;
                best = 1e21f;
                for (i = 0; i < n; i++) {
                    if (list[i] != 0) {
                        d3_PointToWorld((void *)list[i], (char *)list[i] + 0x4c, q);
                        dx = q[0] - o[0];
                        dy = q[1] - o[1];
                        dz = q[2] - o[2];
                        d = (float)sqrt(dx * dx + dy * dy + dz * dz) * (util_RandomF() * 0.5f + 0.75f);
                        if (d < best) {
                            best = d;
                            r = list[i];
                        }
                    }
                }
                d3if_SelectScene(scene, 1);
                return r;
            }
            k = util_Random(n);
            for (i = n; i != 0; i--) {
                if (list[k] != 0) {
                    d3if_SelectScene(scene, 1);
                    return list[k];
                }
                k = (int)(k + 1) % n;
            }
        }
    }
    d3if_SelectScene(scene, 1);
    return 0;
}

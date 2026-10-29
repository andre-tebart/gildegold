/* 0x449b20, sets a light to its day-time keys: key (0..6) blended towards the next key by t (0..1),
   shifted by the light's phase; the intensity is only blended between two lit keys (type 8: always) */

char d3_SetLightKey(D3OBJECT *obj, unsigned int key, float t)
{
    float pos[4], rot[4];
    D3LIGHTKEYS *l;
    D3LIGHTKEY *c, *nk;
    unsigned int i, n, p;
    int pon, con, non;

    if (obj && key < 7 && t >= 0.0f && t <= 1.0f && D3_X(obj)->type >= 5 && (l = D3O_LIGHT(obj))) {
        t += l->phase;
        if (t > 1.0f) {
            t -= 1.0f;
            key = (key + 1) % 7;
        } else if (t < -1.0f) {
            t += 1.0f;
            key = (key - 1) % 7;
        }
        n = (key + 1) % 7;
        p = (key - 1) % 7;
        pon = l->k[p].intensity != 0.0f || D3_X(obj)->type == 8;
        c = &l->k[key];
        con = c->intensity != 0.0f || D3_X(obj)->type == 8;
        nk = &l->k[n];
        non = nk->intensity != 0.0f || D3_X(obj)->type == 8;
        if (pon && con && non)
            D3IF_LIGHT(obj)->power = (nk->intensity - c->intensity) * t + c->intensity;
        else if (con && t < 0.5f)
            D3IF_LIGHT(obj)->power = c->intensity;
        else if (non && t >= 0.5f)
            D3IF_LIGHT(obj)->power = nk->intensity;
        else
            D3IF_LIGHT(obj)->power = 0.0f;
        D3IF_LIGHT(obj)->range = (nk->range - c->range) * t + c->range;
        D3IF_LIGHT(obj)->atten = (nk->atten - c->atten) * t + c->atten;
        for (i = 0; i < 3; i++) {
            pos[i] = (((float *)&l->k[n].pos)[i] - ((float *)&l->k[key].pos)[i]) * t + ((float *)&l->k[key].pos)[i];
            ((float *)&D3IF_LIGHT(obj)->color)[i] = (((float *)&l->k[n].color)[i] - ((float *)&l->k[key].color)[i]) * t + ((float *)&l->k[key].color)[i];
            rot[i] = (((float *)&l->k[n].rot)[i] - ((float *)&l->k[key].rot)[i]) * t + ((float *)&l->k[key].rot)[i];
        }
        d3_SetObjectPos(obj, (D3VECTOR *)pos);
        d3_SetObjectRot(obj, (D3VECTOR *)rot);
        return 1;
    }
    return 0;
}

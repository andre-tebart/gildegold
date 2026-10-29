/* 0x4475f0, initializes an empty scene: its first object is a camera ("MegaCam") @deep Q2 unk3cc / unk3d0: the D3IFSCENE mirrors of 0x5fb5c0 / 0x5fb5c4, same write-only pair as in d3if_Reset (no reader in the exe, initials 4 / 0xf, no evidenced meaning); all other identifiers of the function named this session (hmap, mapSize, sceneFlags, hardware, loaded, dirty) */

char d3if_InitScene(D3IFSCENE *s)
{
    D3OBJECT *cam;
    int i;

    if (s && !s->first && !s->par1) {
        cam = d3_SpawnObject(3, "MegaCam");
        s->campos.x = 0;
        s->campos.y = 0;
        s->campos.z = -256.0f;
        d3_SetObjectPos(cam, &s->campos);
        s->camrot.y = 0;
        s->camrot.x = 0;
        s->camrot.z = 0;
        d3_SetObjectRot(cam, &s->camrot);
        cam->scene = (D3SCENE *)s;
        cam->next = &d3_ObjEnd;
        cam->prev = &d3_TopHead;
        cam->state |= 3;
        s->first = s->last = s->cam = cam;
        s->par1 = &d3_ParEnd;
        s->par2 = d3if_ParList;
        s->hardware = d3_Hardware;
        s->sceneFlags = 0;
        s->amb.x = 200.0f;
        s->amb.y = 200.0f;
        s->amb.z = 200.0f;
        s->map = 0;
        s->sky = 0;
        s->mapSize = 0;
        s->hmap = 0;
        s->dir.x = 0;
        s->dir.y = 0;
        s->dir.z = 0;
        s->fog.a = 0;
        s->fog.c = 0;
        s->fog.b = 0;
        for (i = 0; i < 6; i++) {
            s->fogs[i].a = s->fog.a;
            s->fogs[i].b = s->fog.b;
            s->fogs[i].c = s->fog.c;
        }
        for (i = 0; i < 7; i++) {
            s->levels[i].amb.x = s->amb.x;
            s->levels[i].amb.y = s->amb.y;
            s->levels[i].amb.z = s->amb.z;
            s->levels[i].dir.x = s->dir.x;
            s->levels[i].dir.y = s->dir.y;
            s->levels[i].dir.z = s->dir.z;
            D3_COPY3(s->levels[i].fog, s->fogs, sizeof(s->levels[i].fog));
        }
        s->unk3cc = 4;
        s->unk3d0 = 0xf;
        return 1;
    }
    return 0;
}

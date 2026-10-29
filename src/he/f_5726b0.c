/* 0x5726b0, the run of the event 0x91 (the meeting scene object): the partner event 0x53e400 (1, 1, +0xac) must be active and the building 0x501190 (1, 1, +0x10) must have a scene object +0x61; the old object +0xb0 (0x4284a0, tag 0x66675073 and +4 == +0x10) stays; else the text 0x628fac / 0x628f74 / 0x628f3c by +0xb4 (2 / 1 / other), a new object 0x4287b0 (tag 0x66675073, +4 = +0x10), 0x4478e0, 0x458a20 (the position, +0x5ed098 on the z), 0x429ab0 (3, the position), 0x4478e0, +0x104 = 0, +0xb0 = the handle +0x100. Without the partner or the building: the old object 0x429e90 and the event freed. ROUGH */


void he_Unk5726b0(int ev)
{
    char *bld, *other, *obj;
    float p[3];

    other = he_FindEvent(1, 1, HM_I(ev, 0xac));
    bld = (char *)gm_BuildQuery(1, 1, HM_I(ev, 0x10));
    if (bld != 0 && HM_I(bld, 0x61) != 0 && other != 0 &&
        HM_I(other, 0x70) != -2 && HM_I(other, 0x70) != -1) {
        if (HM_I(ev, 0xb0) != -1) {
            obj = (char *)evt_FindById(HM_I(ev, 0xb0));
            if (obj != 0 && *(int *)obj == 0x66675073 && HM_I(obj, 4) == HM_I(ev, 0x10))
                return;
            HM_I(ev, 0xb0) = -1;
        }
        if (HM_I(ev, 0xb4) == 2)
            obj = (char *)evt_Load((char *)(unsigned)&full_data_628fac);
        else if (HM_I(ev, 0xb4) == 1)
            obj = (char *)evt_Load((char *)(unsigned)&full_data_628f74);
        else
            obj = (char *)evt_Load((char *)(unsigned)&full_data_628f3c);
        if (obj == 0)
            return;
        *(int *)obj = 0x66675073;
        HM_I(obj, 4) = HM_I(ev, 0x10);
        d3if_SelectScene(0, 1);
        d3_PointToWorld(HM_I(bld, 0x61), HM_I(bld, 0x61) + 0x4c, p);
        p[2] += 500.0f;
        evt_RunScriptArgs((EVTSCRIPT *)obj, 3, (int)p[0], (int)p[1], (int)p[2]);
        d3if_SelectScene(*(int *)(unsigned)&d3_CurrentScene, 1);
        HM_I(obj, 0x104) = 0;
        HM_I(ev, 0xb0) = HM_I(obj, 0x100);
        return;
    }
    obj = (char *)evt_FindById(HM_I(ev, 0xb0));
    if (obj != 0 && *(int *)obj == 0x66675073 && HM_I(obj, 4) == HM_I(ev, 0x10)) {
        d3if_SelectScene(0, 1);
        evt_Stop((EVTSCRIPT *)obj);
        d3if_SelectScene(*(int *)(unsigned)&d3_CurrentScene, 1);
    }
    he_Unk53e1f0((int *)ev);
}

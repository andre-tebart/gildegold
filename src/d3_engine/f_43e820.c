/* 0x43e820, the same for a scene */

char d3e_RegisterSceneEvent(D3SCENE *scene, int id, D3EVHANDLER fn, char *name)
{
    int i;

    if (!scene)
        return 0;
    if (id) {
        if (fn) {
            if (!scene->events)
                scene->events = mem_Alloc(D3E_NEVENTS * sizeof(D3EVENT), "d3e:RegisterSceneEvent(eventtable)");
            ((D3EVENT *)scene->events)[id].name[0x7f] = 0;
            strncpy(((D3EVENT *)scene->events)[id].name, name, 0x7f);
            ((D3EVENT *)scene->events)[id].fn = fn;
            return 1;
        }
        if (scene->events && ((D3EVENT *)scene->events)[id].fn) {
            ((D3EVENT *)scene->events)[id].fn = 0;
            for (i = 0; i < D3E_NEVENTS; i++)
                if (((D3EVENT *)scene->events)[i].fn)
                    break;
            if (i == D3E_NEVENTS) {
                mem_Free(scene->events);
                scene->events = 0;
            }
        }
    }
    return 1;
}

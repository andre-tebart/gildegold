/* 0x510770 */


/* 0x510770, merges the scene objects of one building site group ("bk_BRUNNEN" .. "bk_STATUEN", the names of the
   caller 0x510ad0): the objects whose name contains the site name are collected (init_CollectSiteObject); a
   site name of its own ("bk_" / "!bk_" prefix) is recorded in the building sites ai_Bauplaetze with the
   bounding box of the 8 corners of its first object (min at +0x40, max at +0x50) unless it is known already;
   then every collected object is replaced by ONE object spawned from the loaded site scene (d3_SpawnObject
   type 2, named like the site) that takes over its parent, position, rotation and children, and the old
   objects are disposed. The site name is the collector key. */
void init_LoadSiteScene(char *name)
{
    INIT_SITELIST list;
    D3MATRIX cam;
    D3OBJECT *o, *p, *next, *id;
    D3BOXVERT *box;
    float *v;
    int i;

    memset(&list, 0, sizeof list);
    strcpy(list.key, name);
    d3_ForEachObject(d3_DefaultScene, 0, 0x1c0, init_CollectSiteObject, &list);
    if (list.n != 0) {
        o = list.obj[0];
        if (_strnicmp(name, "bk_", 3) == 0 || _strnicmp(name, "!bk_", 4) == 0) {
            i = 0;
            if (0 < ai_NBauplaetze) {
                do {
                    if (_strcmpi(name, ai_Bauplaetze[i].name) == 0) {
                        if (i < ai_NBauplaetze)
                            goto place;
                        break;
                    }
                    i++;
                } while (i < ai_NBauplaetze);
            }
            d3_MatIdentity(&cam);
            box = ((D3MESHOPS *)o->info->mesh)->GetBox(o, o->info, &cam);
            ai_Bauplaetze = (G15BK *)realloc(ai_Bauplaetze, (ai_NBauplaetze * 3 + 3) * 0x20);
            strcpy(ai_Bauplaetze[ai_NBauplaetze].name, name);
            ai_Bauplaetze[ai_NBauplaetze].x0 = box[0].x;
            ai_Bauplaetze[ai_NBauplaetze].y0 = box[0].y;
            ai_Bauplaetze[ai_NBauplaetze].z0 = box[0].z;
            ai_Bauplaetze[ai_NBauplaetze].x1 = box[0].x;
            ai_Bauplaetze[ai_NBauplaetze].y1 = box[0].y;
            ai_Bauplaetze[ai_NBauplaetze].z1 = box[0].z;
            v = (float *)box + 0x16;            /* the 7 remaining corners: 20 floats (0x50) apart */
            i = 7;
            do {
                ai_Bauplaetze[ai_NBauplaetze].x0 = min(v[-2], ai_Bauplaetze[ai_NBauplaetze].x0);
                ai_Bauplaetze[ai_NBauplaetze].y0 = min(v[-1], ai_Bauplaetze[ai_NBauplaetze].y0);
                ai_Bauplaetze[ai_NBauplaetze].z0 = min(v[0], ai_Bauplaetze[ai_NBauplaetze].z0);
                ai_Bauplaetze[ai_NBauplaetze].x1 = max(v[-2], ai_Bauplaetze[ai_NBauplaetze].x1);
                ai_Bauplaetze[ai_NBauplaetze].y1 = max(v[-1], ai_Bauplaetze[ai_NBauplaetze].y1);
                ai_Bauplaetze[ai_NBauplaetze].z1 = max(v[0], ai_Bauplaetze[ai_NBauplaetze].z1);
                v += 0x14;
                i--;
            } while (i != 0);
            ai_NBauplaetze++;
        }
place:
        for (i = 0; i < list.n; i++) {
            o = list.obj[i];
            id = d3_SpawnObject(2, name);
            d3_LinkTop(id);
            d3_SetParent(id, o->parent);
            d3_SetObjectPos(id, &o->pos);
            d3_SetObjectRot(id, &o->rotation);
            p = o->child;
            while (p != 0) {
                next = p->next;
                d3_SetParent(p, id);
                p = next;
            }
            d3if_Delete(o);
        }
    }
}

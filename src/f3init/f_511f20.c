/* 0x511f20 @deep behaviour: 777 code hunks / 881 of 1136 bytes - the far draft of a 1450-byte scene callback (0x511f20..0x5124c9, a function start Ghidra missed): the record walks of the three arms are summarised (the gm_Query/gm_QueryNext walk over the neighbouring prototypes appears three times in the draft with the max/min compare arms merged), so the body has to be rewritten against the disassembly - that is match-deep's byte job, not a missing statement base can add. Readability advanced this session: Q7 cleared (local prototypes for d3t_GetBoundingSphere, gm_IsBuildingKind, gm_FindObjectProto, unk_Unk503530) and Q3 cleared (parent + 0x48 & INIT_PROTO_MASK); three callees named from their own units: d3_InflateObject -> d3_InflateObject (d3.h), gm_IsBuildingKind -> gm_IsBuildingKind (f3gm.c), d3t_GetBoundingSphere -> d3t_GetBoundingSphere (d3_tools.c 0x411eb0). Q2 left with the evidence path: d3if_SelectTextureSet = d3if_SelectTextureSet (d3_interface.c / ecmd.c) but cm.h / f3gm.c / cm.c / sv.c / ls.c declare the same address with struct D3SURF * where ecmd.c uses void *list - unifying needs the shared-header decision; ai_Unkfc2dec = d3_Camera (d3.h, D3OBJECT *) but this unit also uses the address as an int at 0x513xxx (ai_Unkfc2dec + 0x84 / + 0x88 writes in unk_Unk513590's neighbour), so the rename is the d3.h type decision (127 refs in 6 files); init_TextureSet (0x6f1c24), ai_SceneObject (0x76fa84, the current building pointer) and ai_SceneBuilding (0x76fdc0, its object list at +0x5d) have no writer and no string anywhere - only their roles here. Q4: 41 raw accesses, the same shared-header blocker as init_HookObject: d3.h has no field at D3OBJECT +0x200 (owner record), +0x218, +0x224, +0x1cc / +0x1ec, and the record views (rec + 0x18, rec[0x13]) are AIOBJ fields that ai.h cannot be included for in this unit (gv.h's K&R d3sm_GetVertex conflicts). Q1: no evidenced name beyond 'the scene object callback that keeps o's scene record in sync'. */


/* 0x511f20 (a function start Ghidra missed, 1450 bytes up to 0x5124c9; registered with 0x438e60 (0x1e0) by 0x513050), a scene callback of the object o: its prototype (kind byte o + 0x4b = 1: the object prototypes 0x91aaf0, else a building) is looked up in the scene record (0x4ffe90 / 0x4ffcc0 iterate the records), the owner record o + 0x200 is set / cleared (0x447300 = show / hide), the matching records of the neighbouring prototypes are walked (0x4fe940), and an object that is flagged 0xa0 (+0x13 of the record) is given its effect (0x411eb0 position, 0x4287b0 / 0x429ab0 the effect of "dummy_"-less names); the objects named "no_*" (looked up by the name after the prefix, 0x4fe900) are shown / hidden by their record, "pfl_*" / "vg_*" get their placement (0x446f30). ROUGH: the record walks are summarised. */
extern int ai_FallbackBuilding;                /* 0x6f230c */
char d3t_GetBoundingSphere(struct D3OBJECT *obj, struct D3OBJECT *camera, struct D3VECTOR *center, float *radius);  /* 0x411eb0, d3_tools.c: the center and radius of the mesh's 8-corner box */
int gm_IsBuildingKind(int n);           /* 0x4fe940, f3gm.c: 1 when the object prototype n is a building part */
int gm_FindObjectProto(char *name);     /* 0x4fe900, f3gm.c: the object prototype of the name */
int unk_Unk503530(int p, void *b, void *o, int c);  /* 0x503530, f3gm.c: the permission bits of the character p in the building b (or the object o) */
extern unsigned char init_TextureSet;      /* 0x6f1c24 */
extern int ai_Unkfc2dec;                /* 0xfc2dec */
void init_SyncSceneObjects(char *o)
{
    char *base, *proto, *s, *rec, *e, *parent;
    unsigned id;
    int kind, hit;
    float pos[3];
    int fx, i;

    rec = 0;
    id = *(unsigned *)(o + 0x48) & INIT_PROTO_MASK;
    kind = o[0x4b];
    base = ai_SceneObject ? *(char **)((char *)ai_SceneObject + 0x14) : *(char **)((char *)ai_SceneBuilding + 0x5d);
    if (kind == 1) {
        proto = ai_Goods + id * 0x41;
        if (proto[0] == 2 || proto[0] == 0x21 || proto[0] == 6) {
            if (proto[0] != 0x1d) {
                rec = (char *)gm_Query(*(char **)((char *)ai_SceneBuilding + 0x5d), 2, 6, 0, id);
                if (rec != 0) {
                    d3if_ShowTree(o, 1);
                    *(char **)(o + 0x200) = rec;
                    if (proto[0] == 6 || proto[0] == 2) {
                        if (ai_FallbackBuilding != 0) {
                            unsigned r = unk_Unk503530(ai_PlayerId, *(char **)((char *)ai_SceneBuilding + 0x5d), rec, 0);

                            if (r & 0x4000) {
                                *(int *)(o + 0x224) = 1;
                                *(char **)(rec + 0x18) = o;
                                goto fx;
                            }
                        } else {
                            *(int *)(o + 0x224) = 0;
                        }
                    } else {
                        *(int *)(o + 0x224) = 0;
                    }
                    *(char **)(rec + 0x18) = o;
                }
            }
        } else if (proto[0] == 0x1d) {
            if (ai_Goods[id * 0x41] != 0x1d && ai_Goods[id * 0x41] != 0x21) {
                *(int *)(o + 0x200) = 0;
                if (*(int *)(o + 0x218) != 1)
                    d3if_ShowTree(o, 0);
            }
        } else {
            rec = (char *)gm_Query(base, 1, 0, id);
            if (rec != 0) {
                *(int *)(rec + 0x18) = 0;
                parent = *(char **)(o + 0x1f8);
                if (parent != 0) {
                    unsigned pid = *(unsigned *)(parent + 0x48) & INIT_PROTO_MASK;

                    if (parent[0x4b] == 1) {
                        s = (char *)gm_Query(base, 1, 0, pid);
                        if (s != 0) {
                            d3if_ShowTree(o, 1);
                            *(char **)(o + 0x200) = rec;
                            *(int *)(o + 0x224) = 0;
                            *(char **)(rec + 0x18) = o;
                            if (gm_IsBuildingKind(*(unsigned short *)rec) != 0) {
                                e = (char *)gm_Query(base, 4, 1, (unsigned char)proto[0]);
                                while (e != 0) {
                                    if (*(unsigned short *)e != *(unsigned short *)rec)
                                        d3if_ShowTree(o, 0);
                                    e = (char *)gm_QueryNext();
                                }
                            }
                        } else {
                            d3if_ShowTree(o, 0);
                            *(int *)(o + 0x200) = 0;
                        }
                    } else {
                        d3if_ShowTree(o, 1);
                        *(char **)(o + 0x200) = rec;
                        *(int *)(o + 0x224) = 0;
                        *(char **)(rec + 0x18) = o;
                        if (gm_IsBuildingKind(*(unsigned short *)rec) != 0) {
                            e = (char *)gm_Query(base, 4, 1, (unsigned char)proto[0]);
                            while (e != 0) {
                                if (*(unsigned short *)e > *(unsigned short *)rec)
                                    d3if_ShowTree(o, 0);
                                e = (char *)gm_QueryNext();
                            }
                        }
                    }
                } else {
                    d3if_ShowTree(o, 1);
                    *(char **)(o + 0x200) = rec;
                    if (proto[0] != 0x1d)
                        *(int *)(o + 0x224) = 0;
                    *(char **)(rec + 0x18) = o;
                    if (gm_IsBuildingKind(*(unsigned short *)rec) != 0) {
                        e = (char *)gm_Query(base, 4, 1, (unsigned char)proto[0]);
                        while (e != 0) {
                            if (*(unsigned short *)e > *(unsigned short *)rec)
                                d3if_ShowTree(o, 0);
                            e = (char *)gm_QueryNext();
                        }
                    }
                }
            }
        }
fx:
        if (rec != 0 && (rec[0x13] & 0xa0) != 0) {
            if (*(int *)(o + 0x1cc) == 0)
                d3_InflateObject(o);
            if (*(int *)(o + 0x1ec) != 0 && d3t_GetBoundingSphere(o, ai_Unkfc2dec, (struct D3VECTOR *)(pos + 2), pos) != 0) {
                fx = (int)(pos[0] * 1.5);
                s = evt_Load("Effekte/upgr");
                if (s != 0)
                    evt_RunScriptArgs((EVTSCRIPT *)s, 6, (int)(pos[0] * 1.0), (int)pos[1], (int)pos[2], fx, o);
            }
            rec[0x13] &= 0x5f;
        }
    }
    if (strncmp(o, "no_", 3) == 0) {
        i = gm_FindObjectProto(o + 6);
        if (i != -1) {
            if (ai_Goods[i * 0x41] == 2)
                s = (char *)gm_Query(*(char **)((char *)ai_SceneBuilding + 0x5d), 1, 0, i);
            else
                s = (char *)gm_Query(base, 1, 0, i);
            if (s != 0)
                d3if_ShowTree(o, 0);
            else
                d3if_ShowTree(o, 1);
        }
        return;
    }
    if (strncmp(o, "pfl_", 4) == 0 || strncmp(o, "vg_", 3) == 0) {
        if (*(int *)(o + 0x1cc) != 0)
            d3if_SelectTextureSet(o, *(int *)(o + 0x1cc), init_TextureSet, 1);
        else
            d3if_SelectTextureSet(o, *(int *)(o + 0x1ec) + 0xf4, init_TextureSet, 1);
    }
}

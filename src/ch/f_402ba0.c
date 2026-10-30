/* 0x402ba0, ch_Updatelowpoly: shows the body or the low-poly body and keeps the latter in sync @deep behaviour: 84 code hunks left, all read against asmdiff/Ghidra. The camera-distance if/else block order (original: far block inline, near out-of-line at 402db8 with the far block's jmp into the shared hide tail; ours: near inline whatever the spelling) and jp at 402ca6 vs jnp at 402ca2; last-bytes pass (spell menu): dup-tail 'statements in both arms' applied (180 -> 84 differing asm lines, the gain kept), 'pull them after the branch' and 'swap the branch polarity' (positive &&, !(&&) forms, also after the tail fix) give identical code; reload 402f9a: the original re-tests s (402f72/402f93) and reloads anim->slots[0].data into edx (402f9b), ours CSEs both - the local s stays in ecx in the original too, so the ch->slot / D3OBJ_ANIM(ch->lowPolyBody) re-read spellings cannot produce the reloads. Remaining hunks are that block order/branch, two register swaps (95/153) and the tail re-tests (287/298). Q4 left: D3OBJECT 0x48 / 0x200 have no evidenced name: 0x48 = the item type | 0x3000000 in the item-sprite spawners (f3gm.c 0x4fbb80, ls.c 0x51e900) and 0x3000000 in ch_CreateMesh, copied to the low-poly body; 0x200 = the per-object int d3_WriteObject (0x44a2c0) writes and rd_object reads, and the item pointer in the sprite spawners. Q1/Q2/Q3/Q5/Q6/Q7 pass (names ch_Updatelowpoly string, d3if_Show, d3_LoadAnimStream, ch_CurrentGroup/ch_CurrentScriptGroup, lowPolyBody, lowPolyAnimFailed). */

void ch_UpdateLowPoly(CHARACTER *ch)
{
    float dx, dy, dz;
    D3ANIMCTL *anim;
    D3ANIMSLOT *s;
    CHANIMFLAGS f;
    char name[0x100];
    char path[0x100];
    char msg[0x200];

    if (!ch->lowPolyBody)
        return;
    if (ch_LowPoly) {
        D3OBJ_UNK200(ch->lowPolyBody) = D3OBJ_UNK200(ch->obj);
        D3OBJ_UNK048(ch->lowPolyBody) = D3OBJ_UNK048(ch->obj);
        if (ch->flags.lowPoly) {
            if (ch->obj->type != 1)
                d3if_Show(ch->obj, 0);
            if (ch->lowPolyBody->type == 1) {
                d3if_Show(ch->lowPolyBody, 1);
                d3_RelightObject(ch->lowPolyBody);
            }
            return;
        }
        if (ch->scene == &d3_Scenes[0]) {
            if (ch_LowPoly != 1 || !d3_Camera
                || (dx = d3_Camera->pos.x - ch->obj->pos.x,
                    dy = d3_Camera->pos.y - ch->obj->pos.y,
                    dz = d3_Camera->pos.z - ch->obj->pos.z,
                    !((float)sqrt(dx * dx + dy * dy + dz * dz) < 1500.0f))) {
                if (ch->obj->type != 1)
                    d3if_Show(ch->obj, 0);
                if ((ch->group == ch_CurrentGroup && ch->scriptGroup == ch_CurrentScriptGroup) || !ch->scriptGroup) {
                    if (ch->lowPolyBody->type == 1 && !ch->flags.hidden) {
                        d3if_Show(ch->lowPolyBody, 1);
                        d3_RelightObject(ch->lowPolyBody);
                    }
                } else if (ch->lowPolyBody->type != 1)
                    d3if_Show(ch->lowPolyBody, 0);
                if (ch->lowPolyBody->type != 1 && ch->lowPolyBody->info
                    && !d3_VecEqual(&ch->lowPolyBody->pos, &ch->obj->pos, 2.0f))
                    d3_SetObjectPos(ch->lowPolyBody, &ch->obj->pos);
                if (ch->obj->info && !d3_VecEqual(&ch->lowPolyBody->rotation, &ch->obj->rotation, 0.05f))
                    d3_SetObjectRot(ch->lowPolyBody, &ch->obj->rotation);
            } else {
                if (ch->obj->type == 1 && !ch->flags.hidden) {
                    d3if_Show(ch->obj, 1);
                    d3_RelightObject(ch->obj);
                }
            }
            if (ch->lowPolyBody->type != 1)
                d3if_Show(ch->lowPolyBody, 0);
            anim = D3OBJ_ANIM(ch->lowPolyBody);
            if (!anim || ch->flags.lowPolyAnimFailed)
                return;
            s = ch->slot;
            if (s && !anim->slots[0].data) {
                if (!strstr(s->data->name, "gehen") && !strstr(ch->slot->data->name, "karren_ziehen"))
                    return;
                sprintf(path, "lowpolycharacter/%s/%s_%s_LOW.baf", ch->lowname, "gehen", ch->lowname);
                sprintf(name, "%s_%s_LOW.baf", "gehen", ch->lowname);
                vfs_ToSlashes(name);
                memset(&f, 0, sizeof(f));
                f.u.all = 0;
                f.b8 = 1;
                f.u.c.b1 = 8;
                f.u.c.b0 = 0;
                f.b = 0;
                f.a = 0;
                if (!d3_FindAnim(name))
                    d3_LoadAnimStream(path, name, 1);
                s = (D3ANIMSLOT *)d3anim_StartAnimCh(ch->lowPolyBody->material->animCtl, name, f);
                if (!s) {
                    sprintf(msg, "ch_Updatelowpoly(): Could not attach ani: %s", path);
                    ch->flags.lowPolyAnimFailed = 1;
                    return;
                }
                D3OBJ_ANIM(ch->lowPolyBody)->slots[0].frame = ch->slot->frame;
                s->timeout = -1;
                return;
            }
            if (!s && anim->slots[0].data) {
                d3t_ReleaseObjectTextures(ch->lowPolyBody);
                return;
            }
            if (s && anim->slots[0].data)
                anim->slots[0].speed = s->speed;
            return;
        }
        if ((ch->group == ch_CurrentGroup && ch->scriptGroup == ch_CurrentScriptGroup) || !ch->scriptGroup) {
            if (ch->obj->type == 1 && !ch->flags.hidden) {
                d3if_Show(ch->obj, 1);
                d3_RelightObject(ch->obj);
            }
        } else if (ch->obj->type != 1)
            d3if_Show(ch->obj, 0);
    } else if (ch->obj->type == 1 && !ch->flags.hidden) {
        d3if_Show(ch->obj, 1);
        d3_RelightObject(ch->obj);
    }
    if (ch->lowPolyBody->type != 1)
        d3if_Show(ch->lowPolyBody, 0);
}

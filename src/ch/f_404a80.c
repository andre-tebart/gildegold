/* 0x404a80, puts the character on the ground (force: also when the ground type is 0 or 0xd); returns the ground
   type (0: none)
   @deep behaviour: close (5 differing instructions), one code hunk. The original materialises the h==0 return value with a separate xor eax,eax at 404a98 (scheduled before test esi,esi, je to 404b7d past the second xor) and the d3_GetHeightAt==0 return with a second xor eax,eax at 404b7b *before* pop edi/pop esi; ours merges both failure paths into one shared xor placed after pop edi/pop esi, so 404a98 is missing and 404b7b moves. Same statements and returns (checked against 404a80..404b83). Tried this session: baseline nested if (h) {...} + final return 0 (kept), inner return 0 + final return 0, if (!h) return 0 guard (25 differing lines, layout flips cold), if (!d3_GetHeightAt) return 0 guard, else return 0 forms, add-lea/clamp untouched; spell 404b79 push-pop prescriptions both tried (early guard vs one final return; the callee-saved crossing is not expressible in C). Earlier session's list (early return before/after obj=ch->obj, else return 0, else type=0; return type;, int ret=0 local) is in notes/units/ch.md. Needs the tail-merge/vargen toolbox. */

int ch_PlaceOnGround(CHARACTER *ch, int force)
{
    D3SM *h;
    D3OBJECT *obj;
    D3VECTOR4 pos;
    int x, z;
    float y;
    int type;

    h = (D3SM *)ch_GetHMap(ch);
    obj = ch->obj;
    if (h) {
        pos.x = obj->pos.x;
        pos.y = obj->pos.y;
        pos.z = obj->pos.z;
        if (d3_GetHeightAt(h, (D3VECTOR *)&pos, &x, &z, &y)) {
            type = h->entries[h->size * z + x].type;
            if (force || type != 0 && type != 0xd) {
                if (ch->scene->map)
                    map_Pick(ch->scene->map, (D3VECTOR *)&pos, 1, &x, &z, &y);
                if (!d3t_SceneIndex(ch->scene))
                    pos.y = y + 3.0f;
                else
                    pos.y = y;
                d3_SetObjectPos(obj, (D3VECTOR *)&pos);
            }
            return type;
        }
    }
    return 0;
}

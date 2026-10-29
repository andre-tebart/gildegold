/* 0x405e70 */

int ch_TakeObject(CHARACTER *ch, char *name, D3OBJECT *obj, int frame)
{
    CHACTION *a = ch_Unk40b270(ch);

    if (a) {
        a->ch = ch;
        a->args[0] = frame;
        a->count = 0;
        a->func = (CHACTFUNC)ch_ActTakingObject;
        a->type = 0x31;
        a->next = 0;
        a->unk10 = 0;
        a->args[1] = 0;
        a->args[2] = 2;
        strcpy(a->u.name, name);
        if (obj && obj->material && D3MATERIAL_NAME104(obj->material)) {
            strcpy(CH_ACTOBJ(a)->name2, D3MATERIAL_NAME104(obj->material));
            return 1;
        }
        ch_RemoveAction(a);
    }
    return 0;
}

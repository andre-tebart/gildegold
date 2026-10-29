/* 0x5ba6b0, whether the character's ch inventory holds an object of the type id: 0x4ffe90 (the inventory
   +0x178, 1, 0, id) must find one, its type must have a table record (0x5ba620, the 0x62c420 table) and the
   object's +0x21 must be set. The callers read it as "the character has the skill" (cm.c: "the skill 0x16f" /
   0x2b5) or "has the good" (the item 0x290 gate of the ai message callbacks, the office bonus of f3gm.c) */

int ai_InventoryHas(void *ch, int id)
{
    G8INVOBJ *o;

    o = (G8INVOBJ *)gm_Query((void *)((G8INVCHAR *)ch)->inventory, 1, 0, id);
    if (o != 0 && ai_Unk5ba620(o->id) != 0 && o->unk21 != 0)
        return 1;
    return 0;
}

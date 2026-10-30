/* 0x47ed60, after a master's production choice: sends message 0x14 (cm_SendRemoveGoods) for every workshop
   entry that is not being produced (flags bit 8) and of which the workshop's store holds none (amount 0),
   so the clients drop the empty good from the stall's store (0x4c7d80); stall is the workshop's stall object */

void ai_SendRemoveEmptyGoods(AIOBJ *stall)
{
    int i;

    for (i = 0; i < ai_NEntries; i++) {
        if (ai_Entries[i].amount == 0 && !(ai_Entries[i].flags & 8))
            cm_SendRemoveGoodsI(stall->objId, ai_Entries[i].item);
    }
}

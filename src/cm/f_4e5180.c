/* 0x4e5180, clears the party record of the id (id -1 afterwards) */

void ai_Unk4e5180(int id)
{
    G28REC *r;

    r = (G28REC *)cm_PartyById(id);
    if (r != 0) {
        memset(r, 0, 0x114);
        r->id = -1;
    }
}

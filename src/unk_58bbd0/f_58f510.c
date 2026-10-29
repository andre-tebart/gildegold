/* 0x58f510, the shop-counter contact of a building (264 B): for a building of another player (the owner
   +0x27 differs from ai_PlayerId), the first existing counter dummy found by d3_FindObject (mask 0x60) among
   contact_EINKAUF, _METALL, _SKRIPTE, _HOLZ, _STEIN gets its name plate registered by sv_RegisterNamePlate
   (the counter's name, its plate text, 0x10); when that plate is the clicked object, the transport window
   ai_OpenTransport opens on the building record */


struct D3OBJECT;
struct D3OBJECT * d3_FindObject(int a, int flags, int c, char *name); /* 0x449690 (d3_interface.c, d3.h) */
int sv_RegisterNamePlate(char *name, char *text, int b);         /* 0x52e1c0 (sv.c) */
int ai_OpenTransport(int rec);                                   /* 0x5af1a0, below */

void ai_MarketContact(AIBLD *b)
{
    int plate;

    plate = 0;
    if (ai_PlayerId != b->owner) {
        if (d3_FindObject(0, 0x60, 0, "contact_EINKAUF"))
            plate = sv_RegisterNamePlate("contact_EINKAUF", ai_TextEinkauf, 0x10);
        else if (d3_FindObject(0, 0x60, 0, "contact_EINKAUF_METALL"))
            plate = sv_RegisterNamePlate("contact_EINKAUF_METALL", ai_TextEinkaufMetall, 0x10);
        else if (d3_FindObject(0, 0x60, 0, "contact_EINKAUF_SKRIPTE"))
            plate = sv_RegisterNamePlate("contact_EINKAUF_SKRIPTE", ai_TextEinkaufSkripte, 0x10);
        else if (d3_FindObject(0, 0x60, 0, "contact_EINKAUF_HOLZ"))
            plate = sv_RegisterNamePlate("contact_EINKAUF_HOLZ", ai_TextEinkaufHolz, 0x10);
        else if (d3_FindObject(0, 0x60, 0, "contact_EINKAUF_STEIN"))
            plate = sv_RegisterNamePlate("contact_EINKAUF_STEIN", ai_TextEinkaufStein, 0x10);
    }
    if (ai_PlayerId != b->owner && ai_ClickedObject != 0 && ai_ClickedObject == plate)
        ai_OpenTransport((int)b);
}

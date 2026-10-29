/* 0x43cab0, unlinks obj from its sibling list and clears its state bit 2 (linked); for the camera
   (type 3) d3_Camera is cleared too. */

void d3_UnlinkTop(D3OBJECT *obj)
{
    switch (obj->type) {
    case 3:
        if (d3_Camera == obj)
            d3_Camera = 0;
    case 1:
    case 2:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        obj->next->prev = obj->prev;
        obj->prev->next = obj->next;
    }
    obj->state &= 0xfd;
}

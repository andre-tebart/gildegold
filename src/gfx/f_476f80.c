/* 0x476f80, sets (pic != 0) or removes the sprite of the pointer */

void d2_SetPointerSprite(int a, int pic)
{
    if (pic != 0) {
        if (d2_PointerSprite != -1) {
            d2_FreeAnim(d2_PointerSprite);
            d2_PointerSprite = -1;
        }
        d2_PointerSprite = d2_StartAnim(d2_MouseX, d2_MouseY, d2_GetPic(pic), 0);
        return;
    }
    if (d2_PointerSprite != -1) {
        d2_FreeAnim(d2_PointerSprite);
        d2_PointerSprite = -1;
    }
    d2_PointerLook = 0;
}

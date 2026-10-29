/* 0x53d930, creates the panel "Panel\Event_Leiste" (bottom right: screen size - 0x7d / - 0x21) and frees all slots */

void he_Unk53d930(void)
{
    int *p;

    U_I((unsigned)&full_data_6faa60) = d2_OpenForm(0, 0, "Panel\\Event_Leiste");
    d2_SetForm(U_I((unsigned)&full_data_6faa60), 0);
    d2_MoveObj((((U_I((unsigned)&d2_LastWindow) * 0xf1) & 0xffff0000) | (unsigned short)(*(short *)(unsigned)&d2_ScreenW - 0x7d)), *(short *)(unsigned)&d2_ScreenH - 0x21,
                 U_I(((unsigned)&ai_Unkf8f13c + 0x250) + U_I((unsigned)&d2_LastWindow) * 0x3c4));
    p = (int *)(unsigned)&full_data_6faa84;
    do {
        *p = -1;
        p += 3;
    } while ((int)p < (int)((unsigned)&full_data_6faa84 + 0xc0));
    d2_SetViewEnabled(U_I((unsigned)&full_data_6faa60), 0);
}

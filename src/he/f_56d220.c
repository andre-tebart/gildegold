/* 0x56d220, registers the research event types 0x95 .. 0x98 (called by 0x53d4b0) and the texts of 0x95 / 0x96; 0 = ok, 1 = a registration failed */

void he_Unk56d2d0();
void he_Unk56d3d0();
void he_Unk56d610();
void he_Unk56d730();
void he_Unk56d990();
void he_Unk56daa0();
void he_Unk56dd20();
void he_Unk56de10();

int ai_Unk56d220(void)
{
    if (he_Unk53d600(0x95, (void *)he_Unk56d2d0, (void *)he_Unk56d3d0) == 0) {
        if (he_Unk53d600(0x96, (void *)he_Unk56d610, (void *)he_Unk56d730) == 0) {
            if (he_Unk53d600(0x97, (void *)he_Unk56d990, (void *)he_Unk56daa0) == 0) {
                if (he_Unk53d600(0x98, (void *)he_Unk56dd20, (void *)he_Unk56de10) == 0) {
                    sv_Unk52b350(0x95, "erforschen", 1, 0x500, -1);
                    sv_Unk52b350(0x96, "erforschen", 1, 0x500, -1);
                    return 0;
                }
            }
        }
    }
    return 1;
}

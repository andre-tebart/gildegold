/* 0x56e400, registers the event types 0x99 .. 0x9b (called by 0x53d4b0): the pairs (0x56e460, 0x56e5b0), (0x56efc0, 0x56f040) and (0x56f050, 0x56f0b0); 0 = ok, 1 = a registration failed */

void he_Unk56e460();
void he_Unk56e5b0();
void he_Unk56efc0();
void he_Unk56f0b0();
void he_Unk56f050();

int ai_Unk56e400(void)
{
    if (he_Unk53d600(0x99, (void *)he_Unk56e460, (void *)he_Unk56e5b0) == 0) {
        if (he_Unk53d600(0x9a, (void *)he_Unk56efc0, (void *)ai_Unk56f040) == 0) {
            if (he_Unk53d600(0x9b, (void *)he_Unk56f050, (void *)he_Unk56f0b0) == 0)
                return 0;
        }
    }
    return 1;
}

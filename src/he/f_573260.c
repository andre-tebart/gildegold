/* 0x573260, registers the event type 0xa2 (called by 0x53d4b0): the pair (0x573280, 0x5732d0); 0 = ok, 1 = a registration failed */

void he_Unk573280(int ev);          /* 0x573280 */
void he_Unk5732d0(int ev);          /* 0x5732d0 */

int ai_Unk573260(void)
{
    return he_Unk53d600(0xa2, (void *)he_Unk573280, (void *)he_Unk5732d0) != 0;
}

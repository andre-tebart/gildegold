/* 0x5733c0, registers the event types 0x9d .. 0xa1, 0xa3 and 0xa4 (called by 0x53d4b0): the pairs (0x5734b0, 0x573580), (0x573f20, 0x573fb0), (0x574060, 0x5740d0), (0x574120, 0x5743c0), (0x574550, 0x5748d0), (0x575150, 0x5751e0) and (0x575320, 0x5753a0), then the type text 0x50f of 0xa1 (0x52b350); 0 = ok, 1 = a registration failed */

void he_Unk5734b0(int ev);          /* 0x5734b0 */
void he_Unk573580(int ev);          /* 0x573580 */
void he_Unk573f20(int ev);          /* 0x573f20 */
void he_Unk573fb0(int ev);          /* 0x573fb0 */
void he_Unk574060(int ev);          /* 0x574060 */
void he_Unk5740d0(int ev);          /* 0x5740d0 */
void he_Unk574120(int ev);          /* 0x574120 */
void he_Unk5743c0(int ev);          /* 0x5743c0 */
void he_Unk574550(int ev);          /* 0x574550 */
void he_Unk5748d0(int ev);          /* 0x5748d0 */
void he_Unk575150(int ev);          /* 0x575150 */
void he_Unk5751e0(int ev);          /* 0x5751e0 */
void he_Unk575320(int ev);          /* 0x575320 */
void he_Unk5753a0(int ev);          /* 0x5753a0 */

int ai_Unk5733c0(void)
{
    if (he_Unk53d600(0x9d, (void *)he_Unk5734b0, (void *)he_Unk573580) == 0) {
        if (he_Unk53d600(0x9e, (void *)he_Unk573f20, (void *)he_Unk573fb0) == 0) {
            if (he_Unk53d600(0x9f, (void *)he_Unk574060, (void *)he_Unk5740d0) == 0) {
                if (he_Unk53d600(0xa0, (void *)he_Unk574120, (void *)he_Unk5743c0) == 0) {
                    if (he_Unk53d600(0xa1, (void *)he_Unk574550, (void *)he_Unk5748d0) == 0) {
                        if (he_Unk53d600(0xa3, (void *)he_Unk575150, (void *)he_Unk5751e0) == 0) {
                            if (he_Unk53d600(0xa4, (void *)he_Unk575320, (void *)he_Unk5753a0) == 0) {
                                sv_Unk52b350(0xa1, 0, 0, 0x50f, -1);
                                return 0;
                            }
                        }
                    }
                }
            }
        }
    }
    return 1;
}

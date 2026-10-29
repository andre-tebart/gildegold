/* 0x56f980, registers the event types 0x88 .. 0x94 (called by 0x53d4b0): (0x88, 0x56fc50, 0x56fda0), (0x89, 0x570130, 0x5702e0), (0x8c, 0x570ef0, 0x571000), (0x8d, 0x571550, 0x571640), (0x8a, 0x5706e0, 0x5707a0), (0x8b, 0x572860, 0x570cc0), (0x8d again), (0x8e, 0x571990, 0x5719e0), (0x8f, 0x571ac0, 0x571c30), (0x90, 0x571f60, 0x5725b0), (0x91, 0x5726a0, 0x5726b0), (0x93, 0x572b90, 0x572bf0), (0x94, 0x55e3a0, 0x572fd0), (0x92, 0x572860, 0x572a50), then the texts of 0x52b350; 0 = ok, 1 = a registration failed */

void he_Unk56fc50(int ev);          /* 0x56fc50 */
void he_Unk56fda0(int ev);          /* 0x56fda0 */
void he_Unk570130(int ev);          /* 0x570130 */
void he_Unk5702e0(int ev);          /* 0x5702e0 */
void he_Unk570ef0(int ev);          /* 0x570ef0 */
void he_Unk571000(int ev);          /* 0x571000 */
void he_Unk571550(int ev);          /* 0x571550 */
void he_Unk571640(int ev);          /* 0x571640 */
void he_Unk5706e0(int ev);          /* 0x5706e0 */
void he_Unk5707a0(int ev);          /* 0x5707a0 */
void he_Unk572860(int ev);          /* 0x572860 */
void he_Unk570cc0(int ev);          /* 0x570cc0 */
void he_Unk571990(int ev);          /* 0x571990 */
void he_Unk5719e0(int ev);          /* 0x5719e0 */
void he_Unk571ac0(int ev);          /* 0x571ac0 */
void he_Unk571c30(int ev);          /* 0x571c30 */
void he_Unk571f60(int ev);          /* 0x571f60 */
void he_Unk5725b0(int ev);          /* 0x5725b0 */
void he_Unk5726a0(int ev);          /* 0x5726a0 */
void he_Unk5726b0(int ev);          /* 0x5726b0 */
void he_Unk572b90(int ev);          /* 0x572b90 */
void he_Unk572bf0(int ev);          /* 0x572bf0 */
void he_Unk572fd0(int ev);          /* 0x572fd0 */
void he_Unk572a50(int ev);          /* 0x572a50 */

int he_Unk56f980(void)
{
    if (he_Unk53d600(0x88, (void *)he_Unk56fc50, (void *)he_Unk56fda0) == 0) {
        if (he_Unk53d600(0x89, (void *)he_Unk570130, (void *)he_Unk5702e0) == 0) {
            if (he_Unk53d600(0x8c, (void *)he_Unk570ef0, (void *)he_Unk571000) == 0) {
                if (he_Unk53d600(0x8d, (void *)he_Unk571550, (void *)he_Unk571640) == 0) {
                    if (he_Unk53d600(0x8a, (void *)he_Unk5706e0, (void *)he_Unk5707a0) == 0) {
                        if (he_Unk53d600(0x8b, (void *)he_Unk572860, (void *)he_Unk570cc0) == 0) {
                            if (he_Unk53d600(0x8d, (void *)he_Unk571550, (void *)he_Unk571640) == 0) {
                                if (he_Unk53d600(0x8e, (void *)he_Unk571990, (void *)he_Unk5719e0) == 0) {
                                    if (he_Unk53d600(0x8f, (void *)he_Unk571ac0, (void *)he_Unk571c30) == 0) {
                                        if (he_Unk53d600(0x90, (void *)he_Unk571f60, (void *)he_Unk5725b0) == 0) {
                                            if (he_Unk53d600(0x91, (void *)he_Unk5726a0, (void *)he_Unk5726b0) == 0) {
                                                if (he_Unk53d600(0x93, (void *)he_Unk572b90, (void *)he_Unk572bf0) == 0) {
                                                    if (he_Unk53d600(0x94, (void *)he_Unk55e3a0, (void *)he_Unk572fd0) == 0) {
                                                        if (he_Unk53d600(0x92, (void *)he_Unk572860, (void *)he_Unk572a50) == 0) {
                                                            sv_Unk52b350(0x8d, (char *)(unsigned)&full_data_628e8c, 1, 0x513, -1);
                                                            sv_Unk52b350(0x8c, (char *)(unsigned)&full_data_628e7c, 1, 0x500, 0xac);
                                                            sv_Unk52b350(0x8f, (char *)(unsigned)&full_data_628e70, 0, 0x512, -1);
                                                            sv_Unk52b350(0x8a, 0, 0, 0x514, -1);
                                                            sv_Unk52b350(0x88, 0, 0, 0x511, -1);
                                                            return 0;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 1;
}

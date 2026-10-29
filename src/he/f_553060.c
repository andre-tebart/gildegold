/* 0x553060, registers the alloc / run handler pairs of the event types 0x22 .. 0xa5 (he_Unk53d600; the pair of a type is (alloc, run); 1 = a registration failed; 0x52b350 (type, 0, 0, 0x507 / 0x50e, -1) registers the message type of a few of them). The handlers above 0x553060 are the ones of this file; 0x56f040 is a forwarder to the free (0x53e1f0); the rest lives in the stretch 0x553880 .. 0x575700 (drafts to come) */

void ai_Unk56f040();
int sv_Unk52b350();

int he_Unk553060(void)
{
    if (he_Unk53d600(0x22, (void *)d3io_NoopCallback, (void *)d3io_NoopCallback) != 0)
        return 1;
    if (he_Unk53d600(0x29, (void *)he_Unk541a50, (void *)ai_Unk56f040) != 0)
        return 1;
    if (he_Unk53d600(0x2a, (void *)he_Unk541a80, (void *)he_Unk542100) != 0)
        return 1;
    if (he_Unk53d600(0x2b, (void *)he_Unk542490, (void *)he_Unk542590) != 0)
        return 1;
    if (he_Unk53d600(0x2c, (void *)he_Unk542960, (void *)he_Unk542b60) != 0)
        return 1;
    if (he_Unk53d600(0x2d, (void *)ai_Unk56f040, (void *)ai_Unk56f040) != 0)
        return 1;
    if (he_Unk53d600(0x2e, (void *)he_Unk543020, (void *)he_Unk543180) != 0)
        return 1;
    if (he_Unk53d600(0x2f, (void *)he_Unk541a50, (void *)he_Unk5432f0) != 0)
        return 1;
    if (he_Unk53d600(0x30, (void *)ai_Unk56f040, (void *)ai_Unk56f040) != 0)
        return 1;
    if (he_Unk53d600(0x37, (void *)he_Unk5436a0, (void *)he_Unk5437c0) != 0)
        return 1;
    if (he_Unk53d600(0x36, (void *)he_Unk552040, (void *)he_Unk543ac0) != 0)
        return 1;
    if (he_Unk53d600(0x35, (void *)he_Unk544070, (void *)he_Unk544330) != 0)
        return 1;
    if (he_Unk53d600(0x41, (void *)he_Unk544630, (void *)he_Unk544840) != 0)
        return 1;
    if (he_Unk53d600(0x42, (void *)he_Unk5455e0, (void *)he_Unk545640) != 0)
        return 1;
    if (he_Unk53d600(0x43, (void *)he_Unk545670, (void *)he_Unk545840) != 0)
        return 1;
    sv_Unk52b350(0x43, 0x0, 0x0, 0x507, -1);
    if (he_Unk53d600(0x44, (void *)he_Unk546100, (void *)he_Unk5461c0) != 0)
        return 1;
    if (he_Unk53d600(0x45, (void *)he_Unk546c40, (void *)he_Unk546ef0) != 0)
        return 1;
    if (he_Unk53d600(0x46, (void *)he_Unk547170, (void *)he_Unk547430) != 0)
        return 1;
    if (he_Unk53d600(0x47, (void *)he_Unk547750, (void *)he_Unk547ba0) != 0)
        return 1;
    if (he_Unk53d600(0x4a, (void *)he_Unk547ee0, (void *)he_Unk547f90) != 0)
        return 1;
    if (he_Unk53d600(0x4b, (void *)he_Unk548110, (void *)he_Unk548230) != 0)
        return 1;
    if (he_Unk53d600(0x51, (void *)he_Unk5482c0, (void *)he_Unk548300) != 0)
        return 1;
    if (he_Unk53d600(0x52, (void *)he_Unk548330, (void *)he_Unk548380) != 0)
        return 1;
    if (he_Unk53d600(0x53, (void *)he_Unk5483a0, (void *)he_Unk5483f0) != 0)
        return 1;
    if (he_Unk53d600(0x54, (void *)he_Unk55e3a0, (void *)he_Unk5485b0) != 0)
        return 1;
    if (he_Unk53d600(0x55, (void *)he_Unk548750, (void *)he_Unk548790) != 0)
        return 1;
    if (he_Unk53d600(0x56, (void *)he_Unk5482c0, (void *)he_Unk5489b0) != 0)
        return 1;
    if (he_Unk53d600(0x57, (void *)he_Unk548b70, (void *)he_Unk548bb0) != 0)
        return 1;
    if (he_Unk53d600(0x58, (void *)he_Unk548b70, (void *)he_Unk548de0) != 0)
        return 1;
    if (he_Unk53d600(0x5a, (void *)he_Unk548ef0, (void *)he_Unk548f30) != 0)
        return 1;
    if (he_Unk53d600(0x5b, (void *)he_Unk549040, (void *)he_Unk548fa0) != 0)
        return 1;
    if (he_Unk53d600(0x5c, (void *)he_Unk549040, (void *)he_Unk549080) != 0)
        return 1;
    if (he_Unk53d600(0x5d, (void *)he_Unk549110, (void *)he_Unk549120) != 0)
        return 1;
    if (he_Unk53d600(0x5e, (void *)he_Unk5493a0, (void *)he_Unk549430) != 0)
        return 1;
    if (he_Unk53d600(0x5f, (void *)he_Unk5496f0, (void *)he_Unk549770) != 0)
        return 1;
    if (he_Unk53d600(0x60, (void *)he_Unk549970, (void *)he_Unk549aa0) != 0)
        return 1;
    if (he_Unk53d600(0x59, (void *)he_Unk549f70, (void *)he_Unk54a020) != 0)
        return 1;
    if (he_Unk53d600(0x61, (void *)he_Unk54a6f0, (void *)he_Unk54a7b0) != 0)
        return 1;
    sv_Unk52b350(0x61, 0x0, 0x0, 0x507, -1);
    if (he_Unk53d600(0x63, (void *)he_Unk54ae20, (void *)he_Unk54ae60) != 0)
        return 1;
    if (he_Unk53d600(0x62, (void *)he_Unk54b170, (void *)he_Unk54b1e0) != 0)
        return 1;
    sv_Unk52b350(0x62, 0x0, 0x0, 0x50e, -1);
    if (he_Unk53d600(0x64, (void *)he_Unk54b9b0, (void *)he_Unk54ba20) != 0)
        return 1;
    sv_Unk52b350(0x64, 0x0, 0x0, 0x507, -1);
    if (he_Unk53d600(0x65, (void *)he_Unk54be20, (void *)he_Unk54bec0) != 0)
        return 1;
    sv_Unk52b350(0x65, 0x0, 0x0, 0x507, -1);
    if (he_Unk53d600(0x66, (void *)he_Unk54c4c0, (void *)he_Unk54c570) != 0)
        return 1;
    if (he_Unk53d600(0x67, (void *)he_Unk54c870, (void *)he_Unk54c8e0) != 0)
        return 1;
    if (he_Unk53d600(0x68, (void *)he_Unk54d190, (void *)he_Unk54d260) != 0)
        return 1;
    if (he_Unk53d600(0x69, (void *)he_Unk54cc20, (void *)he_Unk54cc50) != 0)
        return 1;
    if (he_Unk53d600(0x6a, (void *)he_Unk54d790, (void *)he_Unk54d7d0) != 0)
        return 1;
    if (he_Unk53d600(0x6b, (void *)he_Unk54da20, (void *)he_Unk54da80) != 0)
        return 1;
    if (he_Unk53d600(0x6d, (void *)he_Unk54de60, (void *)he_Unk54dec0) != 0)
        return 1;
    if (he_Unk53d600(0x6f, (void *)he_Unk54e770, (void *)he_Unk54e890) != 0)
        return 1;
    if (he_Unk53d600(0x70, (void *)he_Unk54c4c0, (void *)he_Unk54ebd0) != 0)
        return 1;
    if (he_Unk53d600(0x74, (void *)he_Unk54ef70, (void *)he_Unk54f160) != 0)
        return 1;
    if (he_Unk53d600(0x76, (void *)he_Unk54fb50, (void *)he_Unk54fce0) != 0)
        return 1;
    if (he_Unk53d600(0x77, (void *)he_Unk54fe80, (void *)he_Unk54ff20) != 0)
        return 1;
    if (he_Unk53d600(0x78, (void *)he_Unk5509f0, (void *)he_Unk550b60) != 0)
        return 1;
    if (he_Unk53d600(0x79, (void *)he_Unk550ea0, (void *)he_Unk550ee0) != 0)
        return 1;
    if (he_Unk53d600(0x7a, (void *)he_Unk5512c0, (void *)he_Unk551b70) != 0)
        return 1;
    if (he_Unk53d600(0x7b, (void *)ai_Unk56f040, (void *)ai_Unk56f040) != 0)
        return 1;
    if (he_Unk53d600(0x7d, (void *)he_Unk551d00, (void *)he_Unk551d70) != 0)
        return 1;
    if (he_Unk53d600(0x7e, (void *)he_Unk552040, (void *)he_Unk552090) != 0)
        return 1;
    if (he_Unk53d600(0x7f, (void *)he_Unk552200, (void *)he_Unk552270) != 0)
        return 1;
    if (he_Unk53d600(0x80, (void *)he_Unk5524e0, (void *)he_Unk552520) != 0)
        return 1;
    if (he_Unk53d600(0x81, (void *)he_Unk5524e0, (void *)he_Unk5525a0) != 0)
        return 1;
    if (he_Unk53d600(0x82, (void *)he_Unk5527c0, (void *)he_Unk552820) != 0)
        return 1;
    if (he_Unk53d600(0x86, (void *)he_Unk552880, (void *)he_Unk552920) != 0)
        return 1;
    if (he_Unk53d600(0x87, (void *)he_Unk552b90, (void *)he_Unk552bc0) != 0)
        return 1;
    return he_Unk53d600(0xa5, (void *)he_Unk552c30, (void *)he_Unk552ca0) != 0;
}

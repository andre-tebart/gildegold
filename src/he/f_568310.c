/* 0x568310, registers the event types 0x02 .. 0xa6 (second part, after 0x553060): he_Unk53d600 (type, alloc, run) for each (a non-zero result = out of table space: return 1), the type texts 0x52b350 (type, name, 0, menu id, icon) for the types with an entry in the event list, and the tip table 0x505ca0 (0x1a, 0x6fa1e0); returns 0 */
int he_Unk568310(void)
{
    if (he_Unk53d600(2, (void *)he_Unk553e10, (void *)he_Unk553e90) != 0)
        return 1;
    if (he_Unk53d600(6, (void *)he_Unk554820, (void *)he_Unk554320) != 0)
        return 1;
    if (he_Unk53d600(7, (void *)he_Unk554820, (void *)he_Unk554860) != 0)
        return 1;
    if (he_Unk53d600(15, (void *)he_Unk554cb0, (void *)he_Unk556600) != 0)
        return 1;
    if (he_Unk53d600(16, (void *)he_Unk556dc0, (void *)he_Unk556ed0) != 0)
        return 1;
    if (he_Unk53d600(17, (void *)he_Unk557160, (void *)he_Unk557190) != 0)
        return 1;
    if (he_Unk53d600(18, (void *)he_Unk557b30, (void *)he_Unk5578a0) != 0)
        return 1;
    if (he_Unk53d600(19, (void *)he_Unk557b30, (void *)he_Unk557b70) != 0)
        return 1;
    if (he_Unk53d600(20, (void *)he_Unk557d70, (void *)he_Unk557e20) != 0)
        return 1;
    sv_Unk52b350(20, 0, 1, 1280, 174);
    if (he_Unk53d600(21, (void *)he_Unk5584d0, (void *)he_Unk558550) != 0)
        return 1;
    sv_Unk52b350(21, "Getier", 1, 1280, 174);
    if (he_Unk53d600(22, (void *)he_Unk5588c0, (void *)he_Unk558a40) != 0)
        return 1;
    sv_Unk52b350(22, "Trainieren", 0, 1289, -1);
    if (he_Unk53d600(23, (void *)he_Unk558de0, (void *)he_Unk558e10) != 0)
        return 1;
    if (he_Unk53d600(25, (void *)he_Unk558e70, (void *)he_Unk558f10) != 0)
        return 1;
    if (he_Unk53d600(24, (void *)he_Unk559d20, (void *)he_Unk55a030) != 0)
        return 1;
    if (he_Unk53d600(26, (void *)he_Unk55a5f0, (void *)he_Unk55a730) != 0)
        return 1;
    if (he_Unk53d600(27, (void *)he_Unk55af70, (void *)he_Unk55b140) != 0)
        return 1;
    if (he_Unk53d600(28, (void *)he_Unk55b3e0, (void *)he_Unk55b530) != 0)
        return 1;
    if (he_Unk53d600(30, (void *)he_Unk55b720, (void *)he_Unk55b830) != 0)
        return 1;
    if (he_Unk53d600(32, (void *)he_Unk55bd20, (void *)he_Unk55bd80) != 0)
        return 1;
    if (he_Unk53d600(33, (void *)he_Unk55bf50, (void *)he_Unk55bfd0) != 0)
        return 1;
    if (he_Unk53d600(31, (void *)he_Unk55ba00, (void *)he_Unk55bad0) != 0)
        return 1;
    if (he_Unk53d600(35, (void *)he_Unk55c540, (void *)he_Unk55c700) != 0)
        return 1;
    if (he_Unk53d600(36, (void *)he_Unk55e3a0, (void *)he_Unk55cb00) != 0)
        return 1;
    if (he_Unk53d600(37, (void *)he_Unk541a50, (void *)he_Unk55cc10) != 0)
        return 1;
    if (he_Unk53d600(38, (void *)he_Unk55ce00, (void *)he_Unk55ce60) != 0)
        return 1;
    if (he_Unk53d600(39, (void *)he_Unk55d310, (void *)he_Unk55d390) != 0)
        return 1;
    if (he_Unk53d600(40, (void *)he_Unk55d490, (void *)he_Unk55d5c0) != 0)
        return 1;
    sv_Unk52b350(40, 0, 1, 1279, 188);
    if (he_Unk53d600(49, (void *)he_Unk563980, (void *)he_Unk55dbe0) != 0)
        return 1;
    if (he_Unk53d600(50, (void *)he_Unk55dd70, (void *)he_Unk55de30) != 0)
        return 1;
    if (he_Unk53d600(51, (void *)he_Unk55e180, (void *)he_Unk55e1d0) != 0)
        return 1;
    if (he_Unk53d600(52, (void *)he_Unk55e3a0, (void *)he_Unk55e3d0) != 0)
        return 1;
    if (he_Unk53d600(56, (void *)he_Unk55e5c0, (void *)he_Unk55e600) != 0)
        return 1;
    if (he_Unk53d600(57, (void *)he_Unk561470, (void *)he_Unk55e730) != 0)
        return 1;
    if (he_Unk53d600(58, (void *)he_Unk55e9d0, (void *)he_Unk55efc0) != 0)
        return 1;
    if (he_Unk53d600(59, (void *)he_Unk55fa20, (void *)he_Unk55fa90) != 0)
        return 1;
    if (he_Unk53d600(60, (void *)he_Unk560820, (void *)he_Unk560900) != 0)
        return 1;
    sv_Unk52b350(60, 0, 0, 1284, -1);
    if (he_Unk53d600(61, (void *)he_Unk561130, (void *)he_Unk561190) != 0)
        return 1;
    if (he_Unk53d600(62, (void *)he_Unk561470, (void *)he_Unk5614c0) != 0)
        return 1;
    if (he_Unk53d600(63, (void *)he_Unk5619a0, (void *)he_Unk561a20) != 0)
        return 1;
    sv_Unk52b350(63, 0, 0, 1290, -1);
    if (he_Unk53d600(64, (void *)he_Unk563980, (void *)he_Unk5628e0) != 0)
        return 1;
    sv_Unk52b350(64, 0, 0, 1283, -1);
    if (he_Unk53d600(72, (void *)he_Unk563070, (void *)he_Unk5630f0) != 0)
        return 1;
    sv_Unk52b350(72, 0, 0, 1282, -1);
    if (he_Unk53d600(73, (void *)he_Unk563980, (void *)he_Unk5639c0) != 0)
        return 1;
    sv_Unk52b350(73, 0, 0, 1290, -1);
    if (he_Unk53d600(78, (void *)he_Unk5642e0, (void *)he_Unk5644e0) != 0)
        return 1;
    if (he_Unk53d600(79, (void *)he_Unk564860, (void *)he_Unk5649b0) != 0)
        return 1;
    if (he_Unk53d600(80, (void *)he_Unk5652b0, (void *)he_Unk565410) != 0)
        return 1;
    if (he_Unk53d600(108, (void *)he_Unk565500, (void *)he_Unk565620) != 0)
        return 1;
    sv_Unk52b350(108, "Heilen", 0, 1285, -1);
    if (he_Unk53d600(110, (void *)he_Unk565850, (void *)he_Unk565940) != 0)
        return 1;
    if (he_Unk53d600(113, (void *)he_Unk565b00, (void *)he_Unk565b60) != 0)
        return 1;
    if (he_Unk53d600(114, (void *)he_Unk565d10, (void *)he_Unk565db0) != 0)
        return 1;
    if (he_Unk53d600(115, (void *)he_Unk5662d0, (void *)he_Unk566370) != 0)
        return 1;
    if (he_Unk53d600(117, (void *)he_Unk563980, (void *)he_Unk566830) != 0)
        return 1;
    sv_Unk52b350(117, 0, 0, 1282, -1);
    if (he_Unk53d600(124, (void *)he_Unk567620, (void *)he_Unk567660) != 0)
        return 1;
    if (he_Unk53d600(131, (void *)he_Unk5679b0, (void *)he_Unk567a80) != 0)
        return 1;
    if (he_Unk53d600(132, (void *)he_Unk567b80, (void *)he_Unk567c00) != 0)
        return 1;
    if (he_Unk53d600(133, (void *)he_Unk567d20, (void *)he_Unk567d60) != 0)
        return 1;
    if (he_Unk53d600(166, (void *)he_Unk567f00, (void *)he_Unk568220) != 0)
        return 1;
    gm_Shuffled(0x1a, (unsigned)&full_data_6fa1e0);
    return 0;
}

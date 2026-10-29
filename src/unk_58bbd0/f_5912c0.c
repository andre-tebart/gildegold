/* 0x5912c0, the meister / building checks of the trade window (445 B): when the selected goods record
   0x76045c is a book (kind 6 / 2 at 0x91aaf0 + goods * 0x41) and 0x503530 says the player owns it, the pending
   contact 0x6f230c is finished (0x4ffe90 (0x5d, 2, 6, 0, 0xfd), 0x59f410, 0x513050, 0x513400, 0x6f22a4 =
   0x6f229c); the pending request 0x75e8c0 / 0x75e8c8 / 0x75e8c4 resolves the same way; 0x6f22fc does so too;
   for the building kinds 0x1e / 0x1f / 0x20 (or the records' 0x1d / 0x1f) 0x58f510 runs */

void ai_TradeContactChecks(void)
{
    char c;
    int r;
    int p;

    if (U_I((unsigned)&full_data_76045c) != 0 &&
        ((c = *(char *)(U_I((unsigned)&ai_Goods) + *(unsigned short *)U_I((unsigned)&full_data_76045c) * 0x41), c == 6 || c == 2))) {
        if (unk_Unk503530(U_W((unsigned)&ai_PlayerId), U_I((unsigned)&ai_FallbackBuilding), U_I((unsigned)&full_data_76045c), 0) & 0x4000) {
            r = gm_Query(*(int *)(U_I((unsigned)&ai_FallbackBuilding) + 0x5d), 2, 6, 0, 0xfd);
            ai_Unk59f410(U_I((unsigned)&full_data_76045c));
            init_ShowBuildingScene(U_I((unsigned)&ai_FallbackBuilding), 0xfd);
            init_FinishBuildingScene(U_I((unsigned)&ai_FallbackBuilding), r);
            U_I((unsigned)&sv_FrameBase) = U_I((unsigned)&sv_FrameDepth);
        }
    }
    if (U_I((unsigned)&sv_PanelFlag) != 0 && U_I((unsigned)&sv_PanelRecord) != 0 && U_I((unsigned)&sv_PanelObject) == U_I((unsigned)&ai_FallbackBuilding)) {
        r = gm_Query(*(int *)(U_I((unsigned)&ai_FallbackBuilding) + 0x5d), 2, 6, 0, 0xfd);
        if (U_I((unsigned)&sv_PanelRecord) == r) {
            U_I((unsigned)&sv_PanelFlag) = 0;
            U_I((unsigned)&sv_PanelRecord) = 0;
            U_I((unsigned)&sv_PanelObject) = 0;
        } else {
            ai_Unk59f410(U_I((unsigned)&sv_PanelRecord));
            init_ShowBuildingScene(U_I((unsigned)&ai_FallbackBuilding), 0xfd);
            init_FinishBuildingScene(U_I((unsigned)&ai_FallbackBuilding), r);
        }
    }
    if (U_I((unsigned)&full_data_6f22fc) != 0 && U_I((unsigned)&ai_Unk6f2334) == 0) {
        r = gm_Query(*(int *)(U_I((unsigned)&ai_FallbackBuilding) + 0x5d), 2, 6, 0, 0xfd);
        ai_Unk59f410(U_I((unsigned)&full_data_6f22fc));
        init_ShowBuildingScene(U_I((unsigned)&ai_FallbackBuilding), 0xfd);
        init_FinishBuildingScene(U_I((unsigned)&ai_FallbackBuilding), r);
    }
    p = *(unsigned char *)U_I((unsigned)&ai_FallbackBuilding);
    if (p == 0x20 || p == 0x1f || p == 0x1e ||
        (c = *(char *)(U_I((unsigned)&ai_BTypes) + p * 0x24d), c == 0x1f || c == 0x1d))
        ai_MarketContact((AIBLD *)U_I((unsigned)&ai_FallbackBuilding));
}

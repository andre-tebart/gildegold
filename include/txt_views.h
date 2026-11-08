/* The text pointer table and its views (the source of truth; edit by hand, `dcp rename` renames).

   The original's 0x143a1c0 is char *txt_Ptrs[0x4800], the 0x12000-byte block that holds every
   text pointer the game reads.  Every named text pointer / pointer table in
   [0x143a1c0, 0x144c1c0) is a view of a slot of it; dcp checks that each view's address is
   txt_Ptrs's plus the offset (L15).  As separate definitions the full rebuild let
   txt_LoadTextFile write one block and the game read another.  (First written by rea's
   build/mk_txt_views.py, rea 8cef7d5b.) */
#ifndef TXT_VIEWS_H
#define TXT_VIEWS_H

extern char *txt_Ptrs[0x4800];    /* 0x143a1c0, each text in its file's buffer */
#define d2_Strings txt_Ptrs       /* 0x143a1c0: the first 122 slots are d2's strings */

#define txt_NetworkSyncWait                            (*(int *)((char *)txt_Ptrs + 0x001e8))  /* 0x143a3a8 */
#define full_data_143a3c4                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x00204))  /* 0x143a3c4 */
#define txt_GateItemSuffix                       (*(char * *)((char *)txt_Ptrs + 0x0020c))  /* 0x143a3cc, text 0x83: what sv_DrawGateName appends to a gate's item name (a guess from the use) */
#define txt_GatePlaceSuffix                      (*(char * *)((char *)txt_Ptrs + 0x00210))  /* 0x143a3d0, text 0x84: what sv_DrawGateName appends to a room / building kind name (a guess from the use) */
#define txt_SpeedHostOnly                        (*(char * *)((char *)txt_Ptrs + 0x00218))  /* 0x143a3d8, the status text ai_SpeedKeys shows to a network client that presses a speed key (only the host sets the speed); its wording is unknown */
#define full_data_143a3e4                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x00224))  /* 0x143a3e4 */
#define ai_ContactTextMercenaryBook                        (*(char * *)((char *)txt_Ptrs + 0x00228))  /* 0x143a3e8, the hover text of the mercenary book ob_SOELDNERBUCH (ai_MercenaryBookContact); the wording is unknown */
#define txt_Tribune                              (*(char * *)((char *)txt_Ptrs + 0x0022c))  /* 0x143a3ec, text 0x8b: the name plate text of the market stand ob_TRIBUENE (ai_MarketStandListener) */
#define full_data_143a3f0                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x00230))  /* 0x143a3f0 */
#define ai_ContactTextParchment                 (*(char * *)((char *)txt_Ptrs + 0x00234))  /* 0x143a3f4, the text the ub_INFORMATIONSPERGAMENT plate of the room 0x14c listener shows (ai_GuildHallListener); the wording is unknown */
#define txt_InfoPanelButtons                            (*(int (*)[7])((char *)txt_Ptrs + 0x003c8))  /* 0x143a588 */
#define txt_HonourTitles                            (*(char * (*)[2])((char *)txt_Ptrs + 0x00450))  /* 0x143a610 */
#define txt_PseudoCharName                            (*(char * *)((char *)txt_Ptrs + 0x00458))  /* 0x143a618 */
#define txt_FemaleHonourTitle                       (*(char * *)((char *)txt_Ptrs + 0x0047c))  /* 0x143a63c, the honour title a female player's name line shows (the male one is txt_HonourTitles[1], 0x143a614) */
#define txt_JobNames                            (*(char * (*)[221])((char *)txt_Ptrs + 0x004c0))  /* 0x143a680 */
#define txt_FemaleJobNames                      (txt_JobNames + 0x5f)  /* 0x143a7fc = txt_JobNames + 0x17c, the female professions (the same job index as txt_JobNames) */
#define txt_TitleNames                            (*(char * (*)[70])((char *)txt_Ptrs + 0x00834))  /* 0x143a9f4 */
#define txt_OfficeNames                            (*(char * (*)[84])((char *)txt_Ptrs + 0x0094c))  /* 0x143ab0c */
#define txt_MaleFirstNames                            (*(char * (*)[197])((char *)txt_Ptrs + 0x00a9c))  /* 0x143ac5c */
#define txt_FemaleFirstNames                            (*(char * (*)[130])((char *)txt_Ptrs + 0x00db0))  /* 0x143af70 */
#define txt_FamilyNames                            (*(char * (*)[152])((char *)txt_Ptrs + 0x00fb8))  /* 0x143b178 */
#define txt_FaithNames                            (*(char * (*)[3])((char *)txt_Ptrs + 0x01218))  /* 0x143b3d8 */
#define txt_SexNames                            (*(char * (*)[4])((char *)txt_Ptrs + 0x01224))  /* 0x143b3e4 */
#define txt_NoRelative                            (*(char * *)((char *)txt_Ptrs + 0x01234))  /* 0x143b3f4 */
#define full_data_143b3f8                        (*(unsigned char (*)[12])((char *)txt_Ptrs + 0x01238))  /* 0x143b3f8 */
#define ai_BuildingNames                         (*(int (*)[1304])((char *)txt_Ptrs + 0x01240))  /* 0x143b400 */
#define txt_ObjectNames                            (*(int (*)[50][2])((char *)txt_Ptrs + 0x026a0))  /* 0x143c860 */
#define ai_ContactTextPersonnelBook              (*(char * *)((char *)txt_Ptrs + 0x02830))  /* 0x143c9f0 */
#define ai_ContactTextInformation                        (*(char * *)((char *)txt_Ptrs + 0x02978))  /* 0x143cb38, the hover text of the contact_INFORMATION plate and of ob_INFORMATIONSPERGAMENT (both open ai_SpyReportsWindow) */
#define ai_ContactTextMasterLetter               (*(char * *)((char *)txt_Ptrs + 0x02a08))  /* 0x143cbc8 */
#define full_data_143ccf0                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x02b30))  /* 0x143ccf0 */
#define full_data_143ccf8                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x02b38))  /* 0x143ccf8 */
#define full_data_143cd00                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x02b40))  /* 0x143cd00 */
#define full_data_143cd08                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x02b48))  /* 0x143cd08 */
#define full_data_143cd10                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x02b50))  /* 0x143cd10 */
#define full_data_143cd18                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x02b58))  /* 0x143cd18 */
#define full_data_143cd20                        (*(unsigned char (*)[560])((char *)txt_Ptrs + 0x02b60))  /* 0x143cd20 */
#define ai_ContactTextPond                       (*(char * *)((char *)txt_Ptrs + 0x02d90))  /* 0x143cf50 */
#define ai_ContactTextFlyGlass                   (*(char * *)((char *)txt_Ptrs + 0x02d98))  /* 0x143cf58 */
#define ai_ContactTextTerrarium                  (*(char * *)((char *)txt_Ptrs + 0x02db8))  /* 0x143cf78 */
#define full_data_143df38                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x03d78))  /* 0x143df38 */
#define full_data_143df40                        (*(unsigned char (*)[1848])((char *)txt_Ptrs + 0x03d80))  /* 0x143df40 */
#define txt_FightPaused                            (*(char * *)((char *)txt_Ptrs + 0x044b4))  /* 0x143e674 */
#define txt_WeaponEmpty15e                            (*(int *)((char *)txt_Ptrs + 0x046e4))  /* 0x143e8a4 */
#define txt_WeaponEmpty174                            (*(int *)((char *)txt_Ptrs + 0x046e8))  /* 0x143e8a8 */
#define txt_WeaponEmpty176                            (*(int *)((char *)txt_Ptrs + 0x046ec))  /* 0x143e8ac */
#define txt_ClickNoGround                            (*(int *)((char *)txt_Ptrs + 0x046f0))  /* 0x143e8b0 */
#define cm_FightButtonText                            (*(int *)((char *)txt_Ptrs + 0x046f4))  /* 0x143e8b4 */
#define txt_AuctionNoteLocal                            (*(char * *)((char *)txt_Ptrs + 0x046fc))  /* 0x143e8bc */
#define txt_AuctionNoteNet                            (*(char * *)((char *)txt_Ptrs + 0x04700))  /* 0x143e8c0 */
#define txt_BirthNoteTail                            (*(char * *)((char *)txt_Ptrs + 0x04708))  /* 0x143e8c8 */
#define txt_ExecNoteTail                            (*(char * *)((char *)txt_Ptrs + 0x04724))  /* 0x143e8e4 */
#define txt_DuelNoteTail                            (*(char * *)((char *)txt_Ptrs + 0x04734))  /* 0x143e8f4 */
#define txt_DuelDeathNews                            (*(char * *)((char *)txt_Ptrs + 0x04738))  /* 0x143e8f8 */
#define txt_DuelHeldNews                            (*(char * *)((char *)txt_Ptrs + 0x0473c))  /* 0x143e8fc */
#define ai_Unk143e948                            (*(char * *)((char *)txt_Ptrs + 0x04788))  /* 0x143e948 */
#define ai_Unk143e954                            (*(char * *)((char *)txt_Ptrs + 0x04794))  /* 0x143e954 */
#define ai_Unk143e960                            (*(char * *)((char *)txt_Ptrs + 0x047a0))  /* 0x143e960 */
#define full_data_143ec88                        (*(unsigned char (*)[936])((char *)txt_Ptrs + 0x04ac8))  /* 0x143ec88 */
#define full_data_143f030                        (*(unsigned char (*)[348])((char *)txt_Ptrs + 0x04e70))  /* 0x143f030 */
#define full_data_143f18c                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x04fcc))  /* 0x143f18c */
#define full_data_143f190                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x04fd0))  /* 0x143f190 */
#define full_data_143f194                        (*(unsigned char (*)[2352])((char *)txt_Ptrs + 0x04fd4))  /* 0x143f194 */
#define full_data_143fac4                        (*(unsigned char (*)[16])((char *)txt_Ptrs + 0x05904))  /* 0x143fac4 */
#define ai_TextGuildPriceRaise                   (*(char * *)((char *)txt_Ptrs + 0x05914))  /* 0x143fad4, text 0x1645: the raise button of ai_GuildLevel1Window */
#define ai_TextGuildPriceLower                   (*(char * *)((char *)txt_Ptrs + 0x05918))  /* 0x143fad8, text 0x1646: the lower button of ai_GuildLevel1Window */
#define ai_TextGuildPriceClose                   (*(char * *)((char *)txt_Ptrs + 0x0591c))  /* 0x143fadc, text 0x1647: the close button of ai_GuildLevel1Window */
#define full_data_143fb34                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x05974))  /* 0x143fb34 */
#define ai_TextGuildPowerPick                    (*(char * *)((char *)txt_Ptrs + 0x0597c))  /* 0x143fb3c, text 0x165f: the map-pick prompt of ai_GuildGiftEntry and the head of its callback's window */
#define ai_TextGuildLevel2Pick                   (*(char * *)((char *)txt_Ptrs + 0x059b0))  /* 0x143fb70, text 0x166c: the column menu prompt of ai_GuildLevel3Entry for the second offices 0x1f / 0x21 */
#define ai_TextGuildLevel3PowerPick              (*(char * *)((char *)txt_Ptrs + 0x059d8))  /* 0x143fb98, text 0x1676: the column menu prompt of ai_GuildLevel3Entry for the second office 0x20 */
#define ai_TextGuildLevel3Pick                   (*(char * *)((char *)txt_Ptrs + 0x059f8))  /* 0x143fbb8, text 0x167e: the character pick prompt of ai_GuildLevel3Entry for the second office 0x1e */
#define ai_TextGuildPowerUsed                    (*(char * *)((char *)txt_Ptrs + 0x05a18))  /* 0x143fbd8, text 0x1686: the town hall's guild power is used already (amt_CanUseMayorPower2 returns -1) */
#define full_data_143fc78                        (*(unsigned char (*)[296])((char *)txt_Ptrs + 0x05ab8))  /* 0x143fc78 */
#define full_data_143fda0                        (*(unsigned char (*)[24])((char *)txt_Ptrs + 0x05be0))  /* 0x143fda0 */
#define full_data_143fdb8                        (*(unsigned char (*)[32])((char *)txt_Ptrs + 0x05bf8))  /* 0x143fdb8 */
#define full_data_143fdd8                        (*(unsigned char (*)[224])((char *)txt_Ptrs + 0x05c18))  /* 0x143fdd8 */
#define full_data_143feb8                        (*(unsigned char (*)[52])((char *)txt_Ptrs + 0x05cf8))  /* 0x143feb8 */
#define full_data_143feec                        (*(unsigned char (*)[48])((char *)txt_Ptrs + 0x05d2c))  /* 0x143feec */
#define full_data_143ff1c                        (*(unsigned char (*)[28])((char *)txt_Ptrs + 0x05d5c))  /* 0x143ff1c */
#define full_data_143ff38                        (*(unsigned char (*)[32])((char *)txt_Ptrs + 0x05d78))  /* 0x143ff38 */
#define full_data_143ff58                        (*(unsigned char (*)[608])((char *)txt_Ptrs + 0x05d98))  /* 0x143ff58 */
#define ai_ContactTextRawMaterialSearch          (*(char * *)((char *)txt_Ptrs + 0x05ff4))  /* 0x14401b4 */
#define full_data_14401c4                        (*(unsigned char (*)[456])((char *)txt_Ptrs + 0x06004))  /* 0x14401c4 */
#define ai_ContactTextDiceGame                   (*(char * *)((char *)txt_Ptrs + 0x061cc))  /* 0x144038c, the hover text of the contact_WUERFELSPIEL plate (the tavern listener 0x599af0) */
#define ai_ContactTextStammtisch                 (*(char * *)((char *)txt_Ptrs + 0x0622c))  /* 0x14403ec, the hover text of the contact_STAMMTISCH plate (0x599af0) */
#define ai_ContactTextDarkCorner                 (*(char * *)((char *)txt_Ptrs + 0x062ac))  /* 0x144046c, the hover text of the ob_DUNKLE_ECKE plate (0x599af0) */
#define full_data_14404e0                        (*(unsigned char (*)[24])((char *)txt_Ptrs + 0x06320))  /* 0x14404e0 */
#define full_data_14404f8                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x06338))  /* 0x14404f8 */
#define full_data_14404fc                        (*(unsigned char (*)[20])((char *)txt_Ptrs + 0x0633c))  /* 0x14404fc */
#define full_data_1440510                        (*(unsigned char (*)[36])((char *)txt_Ptrs + 0x06350))  /* 0x1440510 */
#define full_data_1440534                        (*(unsigned char (*)[92])((char *)txt_Ptrs + 0x06374))  /* 0x1440534 */
#define full_data_1440590                        (*(unsigned char (*)[12])((char *)txt_Ptrs + 0x063d0))  /* 0x1440590 */
#define full_data_144059c                        (*(unsigned char (*)[20])((char *)txt_Ptrs + 0x063dc))  /* 0x144059c */
#define full_data_14405b0                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x063f0))  /* 0x14405b0 */
#define full_data_14405b8                        (*(unsigned char (*)[32])((char *)txt_Ptrs + 0x063f8))  /* 0x14405b8 */
#define full_data_14405d8                        (*(unsigned char (*)[28])((char *)txt_Ptrs + 0x06418))  /* 0x14405d8 */
#define full_data_14405f0                        (*(unsigned char (*)[332])((char *)txt_Ptrs + 0x06430))  /* 0x14405f0 */
#define ai_ContactTextIndulgence                 (*(char * *)((char *)txt_Ptrs + 0x0657c))  /* 0x144073c */
#define ai_ContactTextTop5                       (*(char * *)((char *)txt_Ptrs + 0x065a8))  /* 0x1440768 */
#define full_data_1440794                        (*(unsigned char (*)[36])((char *)txt_Ptrs + 0x065d4))  /* 0x1440794 */
#define full_data_14407b8                        (*(unsigned char (*)[24])((char *)txt_Ptrs + 0x065f8))  /* 0x14407b8 */
#define full_data_14407d0                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x06610))  /* 0x14407d0 */
#define full_data_14407d4                        (*(unsigned char (*)[36])((char *)txt_Ptrs + 0x06614))  /* 0x14407d4 */
#define full_data_14407f4                        (*(unsigned char (*)[124])((char *)txt_Ptrs + 0x06634))  /* 0x14407f4 */
#define ai_ContactTextDungeonPlan                (*(char * *)((char *)txt_Ptrs + 0x066b0))  /* 0x1440870 */
#define ai_ContactTextBribe                      (*(char * *)((char *)txt_Ptrs + 0x066c0))  /* 0x1440880, text 0x19b0: the plate text of the bribe contact contact_BESTECHEN (ai_DungeonBribeListener) */
#define full_data_1440894                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x066d4))  /* 0x1440894 */
#define ai_ContactTextKidnap                     (*(char * *)((char *)txt_Ptrs + 0x066dc))  /* 0x144089c, the hover text of the contact_ENTFUEHREN plate (ai_KidnapContact -> ai_KidnapStart) */
#define ai_TextKidnapMessage                     (*(char * *)((char *)txt_Ptrs + 0x066ec))  /* 0x14408ac, text 0x19bb: the message ai_KidnapStart shows when the building already has a kidnap event */
#define ai_ContactTextRansom                     (*(char * *)((char *)txt_Ptrs + 0x0670c))  /* 0x14408cc, the hover text of the contact_LOESEGELD plate (ai_KidnapContact -> ai_RansomWindow) */
#define full_data_14408e0                        (*(unsigned char (*)[12])((char *)txt_Ptrs + 0x06720))  /* 0x14408e0 */
#define full_data_14408e8                        (*(unsigned char (*)[92])((char *)txt_Ptrs + 0x06728))  /* 0x14408e8 */
#define ai_ContactTextBurglary                   (*(char * *)((char *)txt_Ptrs + 0x06784))  /* 0x1440944, the hover text of the contact_EINBRUCH plate of the thieves' guild hall (ai_ThievesGuildListener) */
#define full_data_144094c                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x0678c))  /* 0x144094c */
#define full_data_1440954                        (*(unsigned char (*)[32])((char *)txt_Ptrs + 0x06794))  /* 0x1440954 */
#define ai_MapPickPrompt                        (*(char * *)((char *)txt_Ptrs + 0x067b4))  /* 0x1440974, the prompt text the burglar-guild map picks (ai_OpenPickpocket, ai_AttackStart) pass to ai_MapPick */
#define ai_ContactTextSpyBuilding                (*(char * *)((char *)txt_Ptrs + 0x067d4))  /* 0x1440994, the hover text of the contact_GEBAEUDE_AUSSPIONIEREN plate (ai_ThievesGuildListener -> ai_EspionageStart) */
#define ai_TextOfficeNoneOpen                    (*(char * *)((char *)txt_Ptrs + 0x06808))  /* 0x14409c8, text 0x1a02: ai_OfficeApplicationWindow's message when no office is open to the player */
#define ai_TextOfficeNoRank                      (*(char * *)((char *)txt_Ptrs + 0x0680c))  /* 0x14409cc, text 0x1a03: ai_OfficeApplicationWindow's message when the player's AICHAR.b00d is below 2 (the slots 0x1a04 / 0x1a05 follow: 0x1a04 formats an office button) */
#define ai_TextOfficeAllFull                     (*(char * *)((char *)txt_Ptrs + 0x06818))  /* 0x14409d8, text 0x1a06: ai_OfficeApplicationWindow's message when the offices open to the player all hold 4 applicants */
#define ai_TextOfficeApply                       (*(char * *)((char *)txt_Ptrs + 0x06820))  /* 0x14409e0, text 0x1a08: the apply button of ai_OfficeApplicantsWindow */
#define ai_TextOfficeColumn                      (*(char * *)((char *)txt_Ptrs + 0x06824))  /* 0x14409e4, text 0x1a09: the button of ai_OfficeApplicantsWindow that opens the office's column in the special-amount list */
#define ai_TextOfficeInfo                        (*(char * *)((char *)txt_Ptrs + 0x06828))  /* 0x14409e8, text 0x1a0a: the button of ai_OfficeApplicantsWindow that opens the office's info (ai_AmountInfoWindow) */
#define full_data_14409ec                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x0682c))  /* 0x14409ec */
#define ai_TextOfficeApplied                     (*(char * *)((char *)txt_Ptrs + 0x06834))  /* 0x14409f4, text 0x1a0d: ai_OfficeApplicationWindow's message after a successful application */
#define full_data_14409f8                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x06838))  /* 0x14409f8 */
#define ai_ContactTextMapTable                   (*(char * *)((char *)txt_Ptrs + 0x06840))  /* 0x1440a00, text 0x1a10: the plate text of the map table ob_KARTENTISCH (ai_MapTableContact) */
#define full_data_1440a24                        (*(unsigned char (*)[44])((char *)txt_Ptrs + 0x06864))  /* 0x1440a24 */
#define full_data_1440a50                        (*(unsigned char (*)[68])((char *)txt_Ptrs + 0x06890))  /* 0x1440a50 */
#define full_data_1440a94                        (*(unsigned char (*)[32])((char *)txt_Ptrs + 0x068d4))  /* 0x1440a94 */
#define full_data_1440ab4                        (*(unsigned char (*)[20])((char *)txt_Ptrs + 0x068f4))  /* 0x1440ab4 */
#define txt_NoCharacter                            (*(char * *)((char *)txt_Ptrs + 0x06908))  /* 0x1440ac8 */
#define ai_ContactTextTarget                        (*(char * *)((char *)txt_Ptrs + 0x0690c))  /* 0x1440acc, the hover text of the ob_ZIELSCHEIBE plate (its click opens ai_TrainingWindow) */
#define ai_ContactTextTraining                        (*(char * *)((char *)txt_Ptrs + 0x06910))  /* 0x1440ad0, the hover text of the ob_STROHPUPPE and contact_NACHT plates (both open ai_TrainingWindow) */
#define full_data_1440ae4                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x06924))  /* 0x1440ae4 */
#define ai_ContactTextWineCellar                 (*(char * *)((char *)txt_Ptrs + 0x0692c))  /* 0x1440aec */
#define ai_ContactTextMistress                   (*(char * *)((char *)txt_Ptrs + 0x06938))  /* 0x1440af8 */
#define full_data_1440b30                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x06970))  /* 0x1440b30 */
#define full_data_1440b34                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x06974))  /* 0x1440b34 */
#define full_data_1440b38                        (*(unsigned char (*)[36])((char *)txt_Ptrs + 0x06978))  /* 0x1440b38 */
#define ai_ContactTextPatrol                     (*(char * *)((char *)txt_Ptrs + 0x0699c))  /* 0x1440b5c, the hover text of the contact_PATROL plate (ai_CityWatchListener); the wording is unknown */
#define ai_PatrolPickPrompt                     (*(char * *)((char *)txt_Ptrs + 0x069a0))  /* 0x1440b60, the prompt text of the city watch patrol map pick (ai_PatrolStart) */
#define ai_RaidPickPrompt                       (*(char * *)((char *)txt_Ptrs + 0x069b0))  /* 0x1440b70, the prompt text of the city watch raid map pick (ai_RaidStart) */
#define ai_ContactTextWatchPlan                  (*(char * *)((char *)txt_Ptrs + 0x069b8))  /* 0x1440b78, the hover text of the ob_WACHPLAN plate (ai_CityWatchListener); the wording is unknown */
#define ai_WatchPlanPickPrompt                  (*(char * *)((char *)txt_Ptrs + 0x069bc))  /* 0x1440b7c, the prompt text of the city watch plan map pick (ai_WatchPlanStart) */
#define ai_ContactTextCustomsBox                 (*(char * *)((char *)txt_Ptrs + 0x069c4))  /* 0x1440b84, the hover text of the ob_ZOLLKASSE and ob_ZOLLKASSE_MIT_GEHEIMFACH plates (ai_CityWatchListener); the wording is unknown */
#define ai_ArrestPickPrompt                     (*(char * *)((char *)txt_Ptrs + 0x069c8))  /* 0x1440b88, the prompt text of the city watch arrest map pick (ai_ArrestStart -> ai_ArrestWindow); the wording is unknown */
#define full_data_1440b90                        (*(unsigned char (*)[20])((char *)txt_Ptrs + 0x069d0))  /* 0x1440b90 */
#define full_data_1440ba4                        (*(unsigned char (*)[36])((char *)txt_Ptrs + 0x069e4))  /* 0x1440ba4 */
#define ai_ContactTextRaid                        (*(char * *)((char *)txt_Ptrs + 0x06a08))  /* 0x1440bc8, the hover text of the contact_RAUBUEBERFALL plate (its click opens ai_OpenRaid) */
#define ai_BribePickText                        (*(char * *)((char *)txt_Ptrs + 0x06a28))  /* 0x1440be8, the pick prompt ai_OpenBribe hands to ai_MapPick for the bribe */
#define ai_BreakInPickText                        (*(char * *)((char *)txt_Ptrs + 0x06a50))  /* 0x1440c10, the pick prompt ai_BreakInWindow hands to ai_MapPick for the burglar-guild break-in */
#define txt_DeathTreeCaption                            (*(int *)((char *)txt_Ptrs + 0x06ac8))  /* 0x1440c88 */
#define txt_Male                        (*(char **)((char *)txt_Ptrs + 0x06b54))  /* 0x1440d14 */
#define txt_Female                        (*(char **)((char *)txt_Ptrs + 0x06b58))  /* 0x1440d18 */
#define txt_Faith0                        (*(char **)((char *)txt_Ptrs + 0x06b60))  /* 0x1440d20 */
#define txt_Faith1                        (*(char **)((char *)txt_Ptrs + 0x06b64))  /* 0x1440d24 */
#define main_FamilyModelNames                    (*(char * (*)[9])((char *)txt_Ptrs + 0x06bc0))  /* 0x1440d80, the nine template model names main_ChooseFamily places (the family figure k) */
#define full_data_1440da4                        (*(unsigned char (*)[1420])((char *)txt_Ptrs + 0x06be4))  /* 0x1440da4 */
#define ai_TextLoadGame                          (*(char * *)((char *)txt_Ptrs + 0x07170))  /* 0x1441330 */
#define ai_TextSaveGame                          (*(char * *)((char *)txt_Ptrs + 0x07174))  /* 0x1441334 */
#define ai_TextGameOptions                       (*(char * *)((char *)txt_Ptrs + 0x07178))  /* 0x1441338 */
#define ai_TextGfxOptions                        (*(char * *)((char *)txt_Ptrs + 0x0717c))  /* 0x144133c */
#define ai_TextSfxOptions                        (*(char * *)((char *)txt_Ptrs + 0x07180))  /* 0x1441340 */
#define ai_TextLeaveGame                         (*(char * *)((char *)txt_Ptrs + 0x07184))  /* 0x1441344 */
#define ai_TextBack                              (*(char * *)((char *)txt_Ptrs + 0x07188))  /* 0x1441348 */
#define full_data_144134c                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x0718c))  /* 0x144134c */
#define full_data_1441350                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x07190))  /* 0x1441350 */
#define full_data_1441354                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x07194))  /* 0x1441354 */
#define full_data_1441358                        (*(unsigned char (*)[112])((char *)txt_Ptrs + 0x07198))  /* 0x1441358 */
#define txt_SpeedNames                            (*(char * (*)[10])((char *)txt_Ptrs + 0x07208))  /* 0x14413c8 */
#define ai_DifficultyTexts                       (*(char * (*)[38])((char *)txt_Ptrs + 0x07230))  /* 0x14413f0 */
#define txt_ScenePartySync                            (*(int *)((char *)txt_Ptrs + 0x072c8))  /* 0x1441488 */
#define txt_ScenePartyWait                       (*(char **)((char *)txt_Ptrs + 0x072cc))  /* 0x144148c, the status text while the scene party's members answer (cm_RunSceneParty; a guess from the use) */
#define init_StatusSendGame                      (*(int *)((char *)txt_Ptrs + 0x072d0))  /* 0x1441490 */
#define init_StatusReceiveGame                   (*(int *)((char *)txt_Ptrs + 0x072d4))  /* 0x1441494 */
#define init_StatusSetupGame                     (*(int *)((char *)txt_Ptrs + 0x072d8))  /* 0x1441498 */
#define init_StatusLocalData                     (*(int *)((char *)txt_Ptrs + 0x072dc))  /* 0x144149c */
#define init_StatusFormat                        (*(int *)((char *)txt_Ptrs + 0x072e0))  /* 0x14414a0 */
#define ai_QuickSaveNoPlayerText                 (*(char * *)((char *)txt_Ptrs + 0x072e4))  /* 0x14414a4 */
#define ai_QuickSaveDisabledText                 (*(char * *)((char *)txt_Ptrs + 0x072e8))  /* 0x14414a8 */
#define ai_SaveWaitText                          (*(char * *)((char *)txt_Ptrs + 0x072ec))  /* 0x14414ac */
#define full_data_14414b4                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x072f4))  /* 0x14414b4 */
#define full_data_14414b8                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x072f8))  /* 0x14414b8 */
#define full_data_14414bc                        (*(unsigned char (*)[12])((char *)txt_Ptrs + 0x072fc))  /* 0x14414bc */
#define txt_ChatConsoleSlider                    (*(int *)((char *)txt_Ptrs + 0x07308))  /* 0x14414c8 */
#define full_data_14414d0                        (*(unsigned char (*)[16])((char *)txt_Ptrs + 0x07310))  /* 0x14414d0 */
#define ai_EmptySlotNameFormat                   (*(char * *)((char *)txt_Ptrs + 0x07320))  /* 0x14414e0 */
#define ai_LoadGameConfirmText                   (*(char * *)((char *)txt_Ptrs + 0x07324))  /* 0x14414e4 */
#define ai_SaveProgressText                      (*(char * *)((char *)txt_Ptrs + 0x07328))  /* 0x14414e8 */
#define ai_SaveOverwriteText                     (*(char * *)((char *)txt_Ptrs + 0x0732c))  /* 0x14414ec */
#define txt_DefaultFileName                        (*(char **)((char *)txt_Ptrs + 0x07334))  /* 0x14414f4 */
#define ai_MessageFilterTexts                    (*(char * (*)[666])((char *)txt_Ptrs + 0x07340))  /* 0x1441500 */
#define txt_PanelBtnBuild                            (*(int *)((char *)txt_Ptrs + 0x07da8))  /* 0x1441f68 */
#define txt_PanelBtnOwn                            (*(int *)((char *)txt_Ptrs + 0x07dac))  /* 0x1441f6c */
#define txt_PanelBtnKind1e                            (*(int *)((char *)txt_Ptrs + 0x07db0))  /* 0x1441f70 */
#define txt_PanelBtnForeign                            (*(int *)((char *)txt_Ptrs + 0x07db4))  /* 0x1441f74 */
#define txt_PanelBtnItemOwn                            (*(int *)((char *)txt_Ptrs + 0x07db8))  /* 0x1441f78 */
#define txt_PanelBtnItem                            (*(int *)((char *)txt_Ptrs + 0x07dbc))  /* 0x1441f7c */
#define ai_TextEinkauf                           (*(int *)((char *)txt_Ptrs + 0x07dc0))  /* 0x1441f80 */
#define ai_ContactTextGelage                        (*(char * *)((char *)txt_Ptrs + 0x07dc8))  /* 0x1441f88, the hover text of the contact_GELAGE plate (ai_GelageWindow) */
#define ai_ContactTextStorage                    (*(char * *)((char *)txt_Ptrs + 0x07dd0))  /* 0x1441f90 */
#define ai_ContactTextProduction                        (*(char * *)((char *)txt_Ptrs + 0x07dd8))  /* 0x1441f98, the hover text of the production plates (contact_PRODUKTION / _METALL; ai_ProductionGoodsWindow) */
#define ai_ContactTextTransport                  (*(char * *)((char *)txt_Ptrs + 0x07de0))  /* 0x1441fa0 */
#define ai_ContactTextAttack                        (*(char * *)((char *)txt_Ptrs + 0x07de8))  /* 0x1441fa8, the hover text of the contact_ANGRIFF plate (its click opens ai_OpenPickpocket) */
#define ai_ContactTextAmbush                        (*(char * *)((char *)txt_Ptrs + 0x07df0))  /* 0x1441fb0, the hover text of the contact_AUF_LAUER_LEGEN plate (its click opens ai_BreakInWindow) */
#define ai_ContactTextEquip                        (*(char * *)((char *)txt_Ptrs + 0x07df8))  /* 0x1441fb8, the hover text of the AUSRUESTEN plate and of the weapon stand ub_STAENDER_WAFFEN (both open ai_EquipWindow); the wording is unknown */
#define ai_ContactTextSpy                        (*(char * *)((char *)txt_Ptrs + 0x07e20))  /* 0x1441fe0, the hover text of the contact_GEBAEUDE_AUSSPIONIEREN plate (its click opens ai_OpenSpying) */
#define ai_ContactTextRazzia                     (*(char * *)((char *)txt_Ptrs + 0x07e40))  /* 0x1442000, the hover text of the contact_RAZZIA plate of the room 0x14c listener (ai_GuildHallListener); the wording is unknown */
#define ai_ContactTextRegeneration                        (*(char * *)((char *)txt_Ptrs + 0x07e48))  /* 0x1442008, the hover text of the contact_REGENERATION plate (its click opens ai_TrainingWindow) */
#define ai_ContactTextExtort                        (*(char * *)((char *)txt_Ptrs + 0x07e50))  /* 0x1442010, the hover text of the contact_SCHUTZGELD_ERPRESSEN plate (its click opens ai_OpenBribe) */
#define ai_ContactTextPickpocket                 (*(char * *)((char *)txt_Ptrs + 0x07e58))  /* 0x1442018, the hover text of the contact_TASCHENDIEBSTAHL plate (ai_ThievesGuildListener -> ai_PickpocketGangWindow) */
#define ai_ContactTextFarBuy                     (*(char * *)((char *)txt_Ptrs + 0x07e68))  /* 0x1442028, the hover text of the FERNEINKAUF plate (ai_FarTradeContact) */
#define ai_ContactTextSell                       (*(char * *)((char *)txt_Ptrs + 0x07e88))  /* 0x1442048, the hover text of the VERKAUF plate (ai_FarTradeContact) */
#define full_data_1442050                        (*(unsigned char (*)[24])((char *)txt_Ptrs + 0x07e90))  /* 0x1442050 */
#define ai_TextEinkaufSkripte                    (*(int *)((char *)txt_Ptrs + 0x07ea8))  /* 0x1442068 */
#define full_data_1442078                        (*(unsigned char (*)[24])((char *)txt_Ptrs + 0x07eb8))  /* 0x1442078 */
#define ai_ContactTextProductionMixing           (*(char * *)((char *)txt_Ptrs + 0x07ed0))  /* 0x1442090, the hover text of the contact_PRODUKTION_MISCHEN plate (ai_MixingContact) */
#define ai_ContactTextProductionPerfumery        (*(char * *)((char *)txt_Ptrs + 0x07ed8))  /* 0x1442098 */
#define ai_ContactTextExtract                        (*(char * *)((char *)txt_Ptrs + 0x07ee8))  /* 0x14420a8, the hover text of the extraction plate (contact_ABBAUEN / contact_ABBAU, ai_MiningContact); the wording is unknown */
#define ai_TextEinkaufMetall                     (*(int *)((char *)txt_Ptrs + 0x07ef0))  /* 0x14420b0 */
#define ai_TextEinkaufStein                      (*(int *)((char *)txt_Ptrs + 0x07ef8))  /* 0x14420b8 */
#define ai_ContactTextSearchOre                        (*(char * *)((char *)txt_Ptrs + 0x07f10))  /* 0x14420d0, the hover text of the ore-search plate (contact_SUCHEN_ERZ, ai_MiningContact); the wording is unknown */
#define ai_ContactTextSearchStone                        (*(char * *)((char *)txt_Ptrs + 0x07f18))  /* 0x14420d8, the hover text of the stone-search plate (contact_SUCHEN_STEIN, ai_StoneKontorContact) */
#define full_data_14420e0                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x07f20))  /* 0x14420e0 */
#define full_data_14420e8                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x07f28))  /* 0x14420e8 */
#define full_data_14420f0                        (*(unsigned char (*)[16])((char *)txt_Ptrs + 0x07f30))  /* 0x14420f0 */
#define full_data_1442100                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x07f40))  /* 0x1442100 */
#define ai_ContactTextProductionSmithy           (*(char * *)((char *)txt_Ptrs + 0x07f48))  /* 0x1442108, the hover text of the contact_PRODUKTION_SCHMIEDEN plate (ai_SmithyContact) */
#define ai_ContactTextProductionStonecutter     (*(char * *)((char *)txt_Ptrs + 0x07f50))  /* 0x1442110, the hover text of the contact_PRODUKTION_STEINMETZ plate (ai_StonecutterContact) */
#define ai_ContactTextProductionCarpenter       (*(char * *)((char *)txt_Ptrs + 0x07f58))  /* 0x1442118, the hover text of the contact_PRODUKTION_TISCHLER plate (ai_CarpenterContact) */
#define ai_TextEinkaufHolz                       (*(int *)((char *)txt_Ptrs + 0x07f60))  /* 0x1442120 */
#define ai_ContactTextProductionWood                        (*(char * *)((char *)txt_Ptrs + 0x07f68))  /* 0x1442128, the hover text of the contact_PRODUKTION_HOLZ plate */
#define ai_ContactTextProductionBrewing          (*(char * *)((char *)txt_Ptrs + 0x07f70))  /* 0x1442130, the hover text of the contact_PRODUKTION_BRAUEN plate (ai_BreweryContact) */
#define full_data_1442160                        (*(unsigned char (*)[32])((char *)txt_Ptrs + 0x07fa0))  /* 0x1442160 */
#define full_data_1442180                        (*(unsigned char (*)[16])((char *)txt_Ptrs + 0x07fc0))  /* 0x1442180 */
#define full_data_1442190                        (*(unsigned char (*)[40])((char *)txt_Ptrs + 0x07fd0))  /* 0x1442190 */
#define full_data_14421b8                        (*(unsigned char (*)[348])((char *)txt_Ptrs + 0x07ff8))  /* 0x14421b8 */
#define ai_TextBuildingClosed                    (*(char * *)((char *)txt_Ptrs + 0x08154))  /* 0x1442314, text 0x2055: the title of the message ai_MayEnterBuilding shows when a building is closed (its texts 0x2056 .. 0x2058 follow) */
#define full_data_1442318                        (*(unsigned char (*)[32])((char *)txt_Ptrs + 0x08158))  /* 0x1442318 */
#define full_data_1442334                        (*(unsigned char (*)[256])((char *)txt_Ptrs + 0x08174))  /* 0x1442334 */
#define txt_TipTexts                             (*(char *(*)[52])((char *)txt_Ptrs + 0x08270))  /* 0x1442430, the 27 tip texts the tips event 0x84 shows (he.c 0x567c00; filled by txt_LoadTextFile) */
#define txt_PilloryPunishment                    (*(char * *)((char *)txt_Ptrs + 0x08340))  /* 0x1442500, text 0x20d0 ("_MESSAGES_PRANGER+0", "Eure Prangerstrafe..."): the AP event name the pillory-punishment event 0x5c registers */
#define full_data_1442504                        (*(unsigned char (*)[24248])((char *)txt_Ptrs + 0x08344))  /* 0x1442504 */
#define txt_DlgTitle                             (*(char * *)((char *)txt_Ptrs + 0x0e1fc))  /* 0x14483bc */
#define txt_DlgLabel104                          (*(char * *)((char *)txt_Ptrs + 0x0e200))  /* 0x14483c0 */
#define txt_DlgLabel105                          (*(char * *)((char *)txt_Ptrs + 0x0e204))  /* 0x14483c4 */
#define full_data_1449e24                        (*(unsigned char (*)[36])((char *)txt_Ptrs + 0x0fc64))  /* 0x1449e24 */
#define full_data_1449e48                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x0fc88))  /* 0x1449e48 */
#define full_data_1449e4c                        (*(unsigned char (*)[716])((char *)txt_Ptrs + 0x0fc8c))  /* 0x1449e4c */
#define full_data_144a118                        (*(unsigned char (*)[24])((char *)txt_Ptrs + 0x0ff58))  /* 0x144a118 */
#define full_data_144a130                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x0ff70))  /* 0x144a130 */
#define full_data_144a134                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x0ff74))  /* 0x144a134 */
#define full_data_144a138                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x0ff78))  /* 0x144a138 */
#define full_data_144a13c                        (*(unsigned char (*)[36])((char *)txt_Ptrs + 0x0ff7c))  /* 0x144a13c */
#define txt_AmountUnit                            (*(char * *)((char *)txt_Ptrs + 0x0ffa0))  /* 0x144a160 */
#define ai_ContactTextBreakOut                   (*(char * *)((char *)txt_Ptrs + 0x0ffbc))  /* 0x144a17c, the hover text of the contact_AUSBRECHEN plate (ai_KidnapContact -> ai_HostageEscape) */
#define full_data_144a19c                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x0ffdc))  /* 0x144a19c */
#define full_data_144a1a4                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x0ffe4))  /* 0x144a1a4 */
#define full_data_144a1a8                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x0ffe8))  /* 0x144a1a8 */
#define full_data_144a1b0                        (*(unsigned char (*)[12])((char *)txt_Ptrs + 0x0fff0))  /* 0x144a1b0 */
#define full_data_144a1bc                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x0fffc))  /* 0x144a1bc */
#define full_data_144a1c0                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10000))  /* 0x144a1c0 */
#define full_data_144a1c4                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10004))  /* 0x144a1c4 */
#define full_data_144a1c8                        (*(unsigned char (*)[36])((char *)txt_Ptrs + 0x10008))  /* 0x144a1c8 */
#define full_data_144a1ec                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x1002c))  /* 0x144a1ec */
#define full_data_144a1f4                        (*(unsigned char (*)[12])((char *)txt_Ptrs + 0x10034))  /* 0x144a1f4 */
#define full_data_144a200                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10040))  /* 0x144a200 */
#define full_data_144a204                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10044))  /* 0x144a204 */
#define full_data_144a208                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10048))  /* 0x144a208 */
#define full_data_144a20c                        (*(unsigned char (*)[32])((char *)txt_Ptrs + 0x1004c))  /* 0x144a20c */
#define full_data_144a22c                        (*(unsigned char (*)[20])((char *)txt_Ptrs + 0x1006c))  /* 0x144a22c */
#define full_data_144a240                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10080))  /* 0x144a240 */
#define full_data_144a244                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10084))  /* 0x144a244 */
#define full_data_144a248                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10088))  /* 0x144a248 */
#define full_data_144a24c                        (*(unsigned char (*)[28])((char *)txt_Ptrs + 0x1008c))  /* 0x144a24c */
#define full_data_144a268                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x100a8))  /* 0x144a268 */
#define full_data_144a270                        (*(unsigned char (*)[44])((char *)txt_Ptrs + 0x100b0))  /* 0x144a270 */
#define full_data_144a29c                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x100dc))  /* 0x144a29c */
#define full_data_144a2a0                        (*(unsigned char (*)[64])((char *)txt_Ptrs + 0x100e0))  /* 0x144a2a0 */
#define full_data_144a2e0                        (*(unsigned char (*)[24])((char *)txt_Ptrs + 0x10120))  /* 0x144a2e0 */
#define full_data_144a2f8                        (*(unsigned char (*)[60])((char *)txt_Ptrs + 0x10138))  /* 0x144a2f8 */
#define full_data_144a334                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10174))  /* 0x144a334 */
#define full_data_144a338                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10178))  /* 0x144a338 */
#define full_data_144a33c                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x1017c))  /* 0x144a33c */
#define full_data_144a340                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10180))  /* 0x144a340 */
#define full_data_144a344                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10184))  /* 0x144a344 */
#define full_data_144a348                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10188))  /* 0x144a348 */
#define full_data_144a34c                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x1018c))  /* 0x144a34c */
#define full_data_144a350                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10190))  /* 0x144a350 */
#define full_data_144a354                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10194))  /* 0x144a354 */
#define full_data_144a358                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10198))  /* 0x144a358 */
#define full_data_144a35c                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x1019c))  /* 0x144a35c */
#define full_data_144a360                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x101a0))  /* 0x144a360 */
#define full_data_144a364                        (*(unsigned char (*)[168])((char *)txt_Ptrs + 0x101a4))  /* 0x144a364 */
#define full_data_144a40c                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x1024c))  /* 0x144a40c */
#define full_data_144a410                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10250))  /* 0x144a410 */
#define full_data_144a414                        (*(unsigned char (*)[20])((char *)txt_Ptrs + 0x10254))  /* 0x144a414 */
#define full_data_144a428                        (*(unsigned char (*)[24])((char *)txt_Ptrs + 0x10268))  /* 0x144a428 */
#define full_data_144a440                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10280))  /* 0x144a440 */
#define full_data_144a444                        (*(unsigned char (*)[76])((char *)txt_Ptrs + 0x10284))  /* 0x144a444 */
#define full_data_144a490                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x102d0))  /* 0x144a490 */
#define full_data_144a498                        (*(unsigned char (*)[36])((char *)txt_Ptrs + 0x102d8))  /* 0x144a498 */
#define full_data_144a4bc                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x102fc))  /* 0x144a4bc */
#define full_data_144a4c0                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10300))  /* 0x144a4c0 */
#define full_data_144a4c4                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10304))  /* 0x144a4c4 */
#define full_data_144a4c8                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10308))  /* 0x144a4c8 */
#define full_data_144a4cc                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x1030c))  /* 0x144a4cc */
#define full_data_144a4d0                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10310))  /* 0x144a4d0 */
#define full_data_144a4d4                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10314))  /* 0x144a4d4 */
#define full_data_144a4d8                        (*(unsigned char (*)[24])((char *)txt_Ptrs + 0x10318))  /* 0x144a4d8 */
#define full_data_144a4f0                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10330))  /* 0x144a4f0 */
#define full_data_144a4f4                        (*(unsigned char (*)[32])((char *)txt_Ptrs + 0x10334))  /* 0x144a4f4 */
#define full_data_144a514                        (*(unsigned char (*)[128])((char *)txt_Ptrs + 0x10354))  /* 0x144a514 */
#define full_data_144a590                        (*(unsigned char (*)[64])((char *)txt_Ptrs + 0x103d0))  /* 0x144a590 */
#define full_data_144a5d0                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x10410))  /* 0x144a5d0 */
#define full_data_144a5d4                        (*(unsigned char (*)[76])((char *)txt_Ptrs + 0x10414))  /* 0x144a5d4 */
#define full_data_144a620                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10460))  /* 0x144a620 */
#define full_data_144a624                        (*(unsigned char (*)[16])((char *)txt_Ptrs + 0x10464))  /* 0x144a624 */
#define full_data_144a634                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10474))  /* 0x144a634 */
#define full_data_144a638                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10478))  /* 0x144a638 */
#define full_data_144a63c                        (*(unsigned char (*)[32])((char *)txt_Ptrs + 0x1047c))  /* 0x144a63c */
#define full_data_144a65c                        (*(unsigned char (*)[36])((char *)txt_Ptrs + 0x1049c))  /* 0x144a65c */
#define full_data_144a680                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x104c0))  /* 0x144a680 */
#define full_data_144a684                        (*(unsigned char (*)[24])((char *)txt_Ptrs + 0x104c4))  /* 0x144a684 */
#define full_data_144a69c                        (*(unsigned char (*)[48])((char *)txt_Ptrs + 0x104dc))  /* 0x144a69c */
#define full_data_144a6c8                        (*(unsigned char (*)[80])((char *)txt_Ptrs + 0x10508))  /* 0x144a6c8 */
#define full_data_144a714                        (*(unsigned char (*)[80])((char *)txt_Ptrs + 0x10554))  /* 0x144a714 */
#define full_data_144a764                        (*(unsigned char (*)[68])((char *)txt_Ptrs + 0x105a4))  /* 0x144a764 */
#define ai_Unk144a7a8                            (*(char * (*)[25])((char *)txt_Ptrs + 0x105e8))  /* 0x144a7a8 */
#define ai_Unk144a80c                            (*(char * (*)[30])((char *)txt_Ptrs + 0x1064c))  /* 0x144a80c */
#define ai_ContactTextBloodRevenge               (*(char * *)((char *)txt_Ptrs + 0x106c4))  /* 0x144a884 */
#define full_data_144a8a8                        (*(unsigned char (*)[12])((char *)txt_Ptrs + 0x106e8))  /* 0x144a8a8 */
#define full_data_144a8b4                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x106f4))  /* 0x144a8b4 */
#define full_data_144a8b8                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x106f8))  /* 0x144a8b8 */
#define ai_ContactTextCurse                      (*(char * *)((char *)txt_Ptrs + 0x10700))  /* 0x144a8c0 */
#define ai_ContactTextTheatre                    (*(char * *)((char *)txt_Ptrs + 0x107cc))  /* 0x144a98c */
#define ai_ContactTextHomage                     (*(char * *)((char *)txt_Ptrs + 0x10818))  /* 0x144a9d8 */
#define ai_ContactTextChat                       (*(char * *)((char *)txt_Ptrs + 0x10898))  /* 0x144aa58 */
#define ai_TailorBlockedText                     (*(char * *)((char *)txt_Ptrs + 0x108a0))  /* 0x144aa60 */
#define ai_TailorAmountText                      (*(char * *)((char *)txt_Ptrs + 0x108a8))  /* 0x144aa68 */
#define ai_ContactTextProductionTailor           (*(char * *)((char *)txt_Ptrs + 0x108c4))  /* 0x144aa84 */
#define ai_ContactTextCardGame                   (*(char * *)((char *)txt_Ptrs + 0x108dc))  /* 0x144aa9c */
#define txt_NetPlayers                           (*(char **)((char *)txt_Ptrs + 0x10a14))  /* 0x144abd4 */
#define txt_JobLabel                             (*(char * *)((char *)txt_Ptrs + 0x10a20))  /* 0x144abe0, the label of the lobby info window's profession line */
#define txt_FaithLabel                           (*(char * *)((char *)txt_Ptrs + 0x10a24))  /* 0x144abe4, the label of the lobby info window's faith line */
#define full_data_144abe8                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10a28))  /* 0x144abe8 */
#define full_data_144abec                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10a2c))  /* 0x144abec */
#define full_data_144abf0                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10a30))  /* 0x144abf0 */
#define txt_ReadyMark                            (*(char * *)((char *)txt_Ptrs + 0x10a34))  /* 0x144abf4, the ready marker lobby_ShowPlayers appends to a ready player's line */
#define full_data_144abf8                        (*(unsigned char (*)[224])((char *)txt_Ptrs + 0x10a38))  /* 0x144abf8 */
#define full_data_144acd8                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10b18))  /* 0x144acd8 */
#define full_data_144acdc                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10b1c))  /* 0x144acdc */
#define full_data_144ace0                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10b20))  /* 0x144ace0 */
#define full_data_144ace4                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10b24))  /* 0x144ace4 */
#define full_data_144ace8                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10b28))  /* 0x144ace8 */
#define full_data_144acec                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10b2c))  /* 0x144acec */
#define full_data_144acf0                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10b30))  /* 0x144acf0 */
#define full_data_144acf4                        (*(unsigned char (*)[28])((char *)txt_Ptrs + 0x10b34))  /* 0x144acf4 */
#define full_data_144ad10                        (*(unsigned char (*)[24])((char *)txt_Ptrs + 0x10b50))  /* 0x144ad10 */
#define txt_PanelBtnCart                            (*(char * *)((char *)txt_Ptrs + 0x10b64))  /* 0x144ad24 */
#define ai_Unk144ad28                            (*(char * *)((char *)txt_Ptrs + 0x10b68))  /* 0x144ad28 */
#define ai_Unk144ad2c                            (*(char * *)((char *)txt_Ptrs + 0x10b6c))  /* 0x144ad2c */
#define ai_Unk144ad30                            (*(int *)((char *)txt_Ptrs + 0x10b70))  /* 0x144ad30 */
#define ai_Unk144ad34                            (*(int *)((char *)txt_Ptrs + 0x10b74))  /* 0x144ad34 */
#define ai_Unk144ad38                            (*(int *)((char *)txt_Ptrs + 0x10b78))  /* 0x144ad38 */
#define ai_Unk144ad3c                            (*(int *)((char *)txt_Ptrs + 0x10b7c))  /* 0x144ad3c */
#define ai_Unk144ad44                            (*(int *)((char *)txt_Ptrs + 0x10b84))  /* 0x144ad44 */
#define ai_Unk144ad4c                            (*(int *)((char *)txt_Ptrs + 0x10b8c))  /* 0x144ad4c */
#define ai_Unk144ad54                            (*(int *)((char *)txt_Ptrs + 0x10b94))  /* 0x144ad54 */
#define ai_Unk144ad58                            (*(int *)((char *)txt_Ptrs + 0x10b98))  /* 0x144ad58 */
#define ai_Unk144ad5c                            (*(int *)((char *)txt_Ptrs + 0x10b9c))  /* 0x144ad5c */
#define ai_Unk144ad60                            (*(int *)((char *)txt_Ptrs + 0x10ba0))  /* 0x144ad60 */
#define ai_Unk144ad64                            (*(int *)((char *)txt_Ptrs + 0x10ba4))  /* 0x144ad64 */
#define ai_Unk144ad78                            (*(int (*)[14])((char *)txt_Ptrs + 0x10bb8))  /* 0x144ad78 */
#define txt_PanelPersons                            (*(int *)((char *)txt_Ptrs + 0x10bf0))  /* 0x144adb0 */
#define full_data_144adb4                        (*(unsigned char (*)[5132])((char *)txt_Ptrs + 0x10bf4))  /* 0x144adb4 */

#endif

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

#define ai_Unk143a3a8                            (*(int *)((char *)txt_Ptrs + 0x001e8))  /* 0x143a3a8 */
#define full_data_143a3c4                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x00204))  /* 0x143a3c4 */
#define full_data_143a3cc                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x0020c))  /* 0x143a3cc */
#define full_data_143a3d0                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x00210))  /* 0x143a3d0 */
#define full_data_143a3d8                        (*(unsigned char (*)[12])((char *)txt_Ptrs + 0x00218))  /* 0x143a3d8 */
#define full_data_143a3e4                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x00224))  /* 0x143a3e4 */
#define full_data_143a3e8                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x00228))  /* 0x143a3e8 */
#define full_data_143a3ec                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x0022c))  /* 0x143a3ec */
#define full_data_143a3f0                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x00230))  /* 0x143a3f0 */
#define full_data_143a3f4                        (*(unsigned char (*)[408])((char *)txt_Ptrs + 0x00234))  /* 0x143a3f4 */
#define ai_Unk143a588                            (*(int (*)[7])((char *)txt_Ptrs + 0x003c8))  /* 0x143a588 */
#define ai_Unk143a610                            (*(char * (*)[2])((char *)txt_Ptrs + 0x00450))  /* 0x143a610 */
#define txt_PseudoCharName                            (*(char * *)((char *)txt_Ptrs + 0x00458))  /* 0x143a618 */
#define full_data_143a63c                        (*(unsigned char (*)[68])((char *)txt_Ptrs + 0x0047c))  /* 0x143a63c */
#define ai_Unk143a680                            (*(char * (*)[221])((char *)txt_Ptrs + 0x004c0))  /* 0x143a680 */
#define ai_Unk143a9f4                            (*(char * (*)[70])((char *)txt_Ptrs + 0x00834))  /* 0x143a9f4 */
#define ai_Unk143ab0c                            (*(char * (*)[84])((char *)txt_Ptrs + 0x0094c))  /* 0x143ab0c */
#define txt_MaleFirstNames                            (*(char * (*)[197])((char *)txt_Ptrs + 0x00a9c))  /* 0x143ac5c */
#define txt_FemaleFirstNames                            (*(char * (*)[130])((char *)txt_Ptrs + 0x00db0))  /* 0x143af70 */
#define ai_Unk143b178                            (*(char * (*)[152])((char *)txt_Ptrs + 0x00fb8))  /* 0x143b178 */
#define ai_Unk143b3d8                            (*(char * (*)[3])((char *)txt_Ptrs + 0x01218))  /* 0x143b3d8 */
#define ai_Unk143b3e4                            (*(char * (*)[4])((char *)txt_Ptrs + 0x01224))  /* 0x143b3e4 */
#define txt_NoRelative                            (*(char * *)((char *)txt_Ptrs + 0x01234))  /* 0x143b3f4 */
#define full_data_143b3f8                        (*(unsigned char (*)[12])((char *)txt_Ptrs + 0x01238))  /* 0x143b3f8 */
#define ai_BuildingNames                         (*(int (*)[1304])((char *)txt_Ptrs + 0x01240))  /* 0x143b400 */
#define ai_Unk143c860                            (*(int (*)[50][2])((char *)txt_Ptrs + 0x026a0))  /* 0x143c860 */
#define ai_ContactTextPersonnelBook              (*(char * *)((char *)txt_Ptrs + 0x02830))  /* 0x143c9f0 */
#define full_data_143cb38                        (*(unsigned char (*)[144])((char *)txt_Ptrs + 0x02978))  /* 0x143cb38 */
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
#define ai_Unk143e674                            (*(char * *)((char *)txt_Ptrs + 0x044b4))  /* 0x143e674 */
#define ai_Unk143e8a4                            (*(int *)((char *)txt_Ptrs + 0x046e4))  /* 0x143e8a4 */
#define ai_Unk143e8a8                            (*(int *)((char *)txt_Ptrs + 0x046e8))  /* 0x143e8a8 */
#define ai_Unk143e8ac                            (*(int *)((char *)txt_Ptrs + 0x046ec))  /* 0x143e8ac */
#define ai_Unk143e8b0                            (*(int *)((char *)txt_Ptrs + 0x046f0))  /* 0x143e8b0 */
#define ai_Unk143e8b4                            (*(int *)((char *)txt_Ptrs + 0x046f4))  /* 0x143e8b4 */
#define ai_Unk143e8bc                            (*(char * *)((char *)txt_Ptrs + 0x046fc))  /* 0x143e8bc */
#define ai_Unk143e8c0                            (*(char * *)((char *)txt_Ptrs + 0x04700))  /* 0x143e8c0 */
#define ai_Unk143e8c8                            (*(char * *)((char *)txt_Ptrs + 0x04708))  /* 0x143e8c8 */
#define ai_Unk143e8e4                            (*(char * *)((char *)txt_Ptrs + 0x04724))  /* 0x143e8e4 */
#define ai_Unk143e8f4                            (*(char * *)((char *)txt_Ptrs + 0x04734))  /* 0x143e8f4 */
#define ai_Unk143e8f8                            (*(char * *)((char *)txt_Ptrs + 0x04738))  /* 0x143e8f8 */
#define ai_Unk143e8fc                            (*(char * *)((char *)txt_Ptrs + 0x0473c))  /* 0x143e8fc */
#define ai_Unk143e948                            (*(char * *)((char *)txt_Ptrs + 0x04788))  /* 0x143e948 */
#define ai_Unk143e954                            (*(char * *)((char *)txt_Ptrs + 0x04794))  /* 0x143e954 */
#define ai_Unk143e960                            (*(char * *)((char *)txt_Ptrs + 0x047a0))  /* 0x143e960 */
#define full_data_143ec88                        (*(unsigned char (*)[936])((char *)txt_Ptrs + 0x04ac8))  /* 0x143ec88 */
#define full_data_143f030                        (*(unsigned char (*)[348])((char *)txt_Ptrs + 0x04e70))  /* 0x143f030 */
#define full_data_143f18c                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x04fcc))  /* 0x143f18c */
#define full_data_143f190                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x04fd0))  /* 0x143f190 */
#define full_data_143f194                        (*(unsigned char (*)[2352])((char *)txt_Ptrs + 0x04fd4))  /* 0x143f194 */
#define full_data_143fac4                        (*(unsigned char (*)[16])((char *)txt_Ptrs + 0x05904))  /* 0x143fac4 */
#define full_data_143fad4                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x05914))  /* 0x143fad4 */
#define full_data_143fad8                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x05918))  /* 0x143fad8 */
#define full_data_143fadc                        (*(unsigned char (*)[88])((char *)txt_Ptrs + 0x0591c))  /* 0x143fadc */
#define full_data_143fb34                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x05974))  /* 0x143fb34 */
#define full_data_143fb3c                        (*(unsigned char (*)[52])((char *)txt_Ptrs + 0x0597c))  /* 0x143fb3c */
#define full_data_143fb70                        (*(unsigned char (*)[40])((char *)txt_Ptrs + 0x059b0))  /* 0x143fb70 */
#define full_data_143fb98                        (*(unsigned char (*)[32])((char *)txt_Ptrs + 0x059d8))  /* 0x143fb98 */
#define full_data_143fbb8                        (*(unsigned char (*)[36])((char *)txt_Ptrs + 0x059f8))  /* 0x143fbb8 */
#define full_data_143fbd8                        (*(unsigned char (*)[160])((char *)txt_Ptrs + 0x05a18))  /* 0x143fbd8 */
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
#define full_data_144038c                        (*(unsigned char (*)[96])((char *)txt_Ptrs + 0x061cc))  /* 0x144038c */
#define full_data_14403ec                        (*(unsigned char (*)[128])((char *)txt_Ptrs + 0x0622c))  /* 0x14403ec */
#define full_data_144046c                        (*(unsigned char (*)[116])((char *)txt_Ptrs + 0x062ac))  /* 0x144046c */
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
#define full_data_1440880                        (*(unsigned char (*)[20])((char *)txt_Ptrs + 0x066c0))  /* 0x1440880 */
#define full_data_1440894                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x066d4))  /* 0x1440894 */
#define full_data_144089c                        (*(unsigned char (*)[16])((char *)txt_Ptrs + 0x066dc))  /* 0x144089c */
#define full_data_14408ac                        (*(unsigned char (*)[32])((char *)txt_Ptrs + 0x066ec))  /* 0x14408ac */
#define full_data_14408cc                        (*(unsigned char (*)[20])((char *)txt_Ptrs + 0x0670c))  /* 0x14408cc */
#define full_data_14408e0                        (*(unsigned char (*)[12])((char *)txt_Ptrs + 0x06720))  /* 0x14408e0 */
#define full_data_14408e8                        (*(unsigned char (*)[92])((char *)txt_Ptrs + 0x06728))  /* 0x14408e8 */
#define full_data_1440944                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x06784))  /* 0x1440944 */
#define full_data_144094c                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x0678c))  /* 0x144094c */
#define full_data_1440954                        (*(unsigned char (*)[32])((char *)txt_Ptrs + 0x06794))  /* 0x1440954 */
#define full_data_1440974                        (*(unsigned char (*)[32])((char *)txt_Ptrs + 0x067b4))  /* 0x1440974 */
#define full_data_1440994                        (*(unsigned char (*)[52])((char *)txt_Ptrs + 0x067d4))  /* 0x1440994 */
#define full_data_14409c8                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x06808))  /* 0x14409c8 */
#define full_data_14409cc                        (*(unsigned char (*)[12])((char *)txt_Ptrs + 0x0680c))  /* 0x14409cc */
#define full_data_14409d8                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x06818))  /* 0x14409d8 */
#define full_data_14409e0                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x06820))  /* 0x14409e0 */
#define full_data_14409e4                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x06824))  /* 0x14409e4 */
#define full_data_14409e8                        (*(unsigned char (*)[12])((char *)txt_Ptrs + 0x06828))  /* 0x14409e8 */
#define full_data_14409f4                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x06834))  /* 0x14409f4 */
#define full_data_14409f8                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x06838))  /* 0x14409f8 */
#define full_data_1440a00                        (*(unsigned char (*)[36])((char *)txt_Ptrs + 0x06840))  /* 0x1440a00 */
#define full_data_1440a24                        (*(unsigned char (*)[44])((char *)txt_Ptrs + 0x06864))  /* 0x1440a24 */
#define full_data_1440a50                        (*(unsigned char (*)[68])((char *)txt_Ptrs + 0x06890))  /* 0x1440a50 */
#define full_data_1440a94                        (*(unsigned char (*)[32])((char *)txt_Ptrs + 0x068d4))  /* 0x1440a94 */
#define full_data_1440ab4                        (*(unsigned char (*)[20])((char *)txt_Ptrs + 0x068f4))  /* 0x1440ab4 */
#define txt_NoCharacter                            (*(char * *)((char *)txt_Ptrs + 0x06908))  /* 0x1440ac8 */
#define full_data_1440acc                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x0690c))  /* 0x1440acc */
#define full_data_1440ad0                        (*(unsigned char (*)[20])((char *)txt_Ptrs + 0x06910))  /* 0x1440ad0 */
#define full_data_1440ae4                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x06924))  /* 0x1440ae4 */
#define ai_ContactTextWineCellar                 (*(char * *)((char *)txt_Ptrs + 0x0692c))  /* 0x1440aec */
#define ai_ContactTextMistress                   (*(char * *)((char *)txt_Ptrs + 0x06938))  /* 0x1440af8 */
#define full_data_1440b30                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x06970))  /* 0x1440b30 */
#define full_data_1440b34                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x06974))  /* 0x1440b34 */
#define full_data_1440b38                        (*(unsigned char (*)[36])((char *)txt_Ptrs + 0x06978))  /* 0x1440b38 */
#define full_data_1440b5c                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x0699c))  /* 0x1440b5c */
#define full_data_1440b60                        (*(unsigned char (*)[16])((char *)txt_Ptrs + 0x069a0))  /* 0x1440b60 */
#define full_data_1440b70                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x069b0))  /* 0x1440b70 */
#define full_data_1440b78                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x069b8))  /* 0x1440b78 */
#define full_data_1440b7c                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x069bc))  /* 0x1440b7c */
#define full_data_1440b84                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x069c4))  /* 0x1440b84 */
#define full_data_1440b88                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x069c8))  /* 0x1440b88 */
#define full_data_1440b90                        (*(unsigned char (*)[20])((char *)txt_Ptrs + 0x069d0))  /* 0x1440b90 */
#define full_data_1440ba4                        (*(unsigned char (*)[36])((char *)txt_Ptrs + 0x069e4))  /* 0x1440ba4 */
#define full_data_1440bc8                        (*(unsigned char (*)[32])((char *)txt_Ptrs + 0x06a08))  /* 0x1440bc8 */
#define full_data_1440be8                        (*(unsigned char (*)[40])((char *)txt_Ptrs + 0x06a28))  /* 0x1440be8 */
#define full_data_1440c10                        (*(unsigned char (*)[120])((char *)txt_Ptrs + 0x06a50))  /* 0x1440c10 */
#define ai_Unk1440c88                            (*(int *)((char *)txt_Ptrs + 0x06ac8))  /* 0x1440c88 */
#define txt_Male                        (*(char **)((char *)txt_Ptrs + 0x06b54))  /* 0x1440d14 */
#define txt_Female                        (*(char **)((char *)txt_Ptrs + 0x06b58))  /* 0x1440d18 */
#define txt_Faith0                        (*(char **)((char *)txt_Ptrs + 0x06b60))  /* 0x1440d20 */
#define txt_Faith1                        (*(char **)((char *)txt_Ptrs + 0x06b64))  /* 0x1440d24 */
#define full_data_1440d80                        (*(unsigned char (*)[36])((char *)txt_Ptrs + 0x06bc0))  /* 0x1440d80 */
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
#define ai_Unk14413c8                            (*(char * (*)[10])((char *)txt_Ptrs + 0x07208))  /* 0x14413c8 */
#define ai_DifficultyTexts                       (*(char * (*)[38])((char *)txt_Ptrs + 0x07230))  /* 0x14413f0 */
#define ai_Unk1441488                            (*(int *)((char *)txt_Ptrs + 0x072c8))  /* 0x1441488 */
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
#define full_data_14414c8                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x07308))  /* 0x14414c8 */
#define full_data_14414d0                        (*(unsigned char (*)[16])((char *)txt_Ptrs + 0x07310))  /* 0x14414d0 */
#define ai_EmptySlotNameFormat                   (*(char * *)((char *)txt_Ptrs + 0x07320))  /* 0x14414e0 */
#define ai_LoadGameConfirmText                   (*(char * *)((char *)txt_Ptrs + 0x07324))  /* 0x14414e4 */
#define ai_SaveProgressText                      (*(char * *)((char *)txt_Ptrs + 0x07328))  /* 0x14414e8 */
#define ai_SaveOverwriteText                     (*(char * *)((char *)txt_Ptrs + 0x0732c))  /* 0x14414ec */
#define txt_DefaultFileName                        (*(char **)((char *)txt_Ptrs + 0x07334))  /* 0x14414f4 */
#define ai_MessageFilterTexts                    (*(char * (*)[666])((char *)txt_Ptrs + 0x07340))  /* 0x1441500 */
#define ai_Unk1441f68                            (*(int *)((char *)txt_Ptrs + 0x07da8))  /* 0x1441f68 */
#define ai_Unk1441f6c                            (*(int *)((char *)txt_Ptrs + 0x07dac))  /* 0x1441f6c */
#define ai_Unk1441f70                            (*(int *)((char *)txt_Ptrs + 0x07db0))  /* 0x1441f70 */
#define ai_Unk1441f74                            (*(int *)((char *)txt_Ptrs + 0x07db4))  /* 0x1441f74 */
#define ai_Unk1441f78                            (*(int *)((char *)txt_Ptrs + 0x07db8))  /* 0x1441f78 */
#define ai_Unk1441f7c                            (*(int *)((char *)txt_Ptrs + 0x07dbc))  /* 0x1441f7c */
#define ai_TextEinkauf                           (*(int *)((char *)txt_Ptrs + 0x07dc0))  /* 0x1441f80 */
#define full_data_1441f88                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x07dc8))  /* 0x1441f88 */
#define ai_ContactTextStorage                    (*(char * *)((char *)txt_Ptrs + 0x07dd0))  /* 0x1441f90 */
#define full_data_1441f98                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x07dd8))  /* 0x1441f98 */
#define ai_ContactTextTransport                  (*(char * *)((char *)txt_Ptrs + 0x07de0))  /* 0x1441fa0 */
#define full_data_1441fa8                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x07de8))  /* 0x1441fa8 */
#define full_data_1441fb0                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x07df0))  /* 0x1441fb0 */
#define full_data_1441fb8                        (*(unsigned char (*)[40])((char *)txt_Ptrs + 0x07df8))  /* 0x1441fb8 */
#define full_data_1441fe0                        (*(unsigned char (*)[32])((char *)txt_Ptrs + 0x07e20))  /* 0x1441fe0 */
#define full_data_1442000                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x07e40))  /* 0x1442000 */
#define full_data_1442008                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x07e48))  /* 0x1442008 */
#define full_data_1442010                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x07e50))  /* 0x1442010 */
#define full_data_1442018                        (*(unsigned char (*)[16])((char *)txt_Ptrs + 0x07e58))  /* 0x1442018 */
#define full_data_1442028                        (*(unsigned char (*)[32])((char *)txt_Ptrs + 0x07e68))  /* 0x1442028 */
#define full_data_1442048                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x07e88))  /* 0x1442048 */
#define full_data_1442050                        (*(unsigned char (*)[24])((char *)txt_Ptrs + 0x07e90))  /* 0x1442050 */
#define ai_TextEinkaufSkripte                    (*(int *)((char *)txt_Ptrs + 0x07ea8))  /* 0x1442068 */
#define full_data_1442078                        (*(unsigned char (*)[24])((char *)txt_Ptrs + 0x07eb8))  /* 0x1442078 */
#define full_data_1442090                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x07ed0))  /* 0x1442090 */
#define ai_ContactTextProductionPerfumery        (*(char * *)((char *)txt_Ptrs + 0x07ed8))  /* 0x1442098 */
#define full_data_14420a8                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x07ee8))  /* 0x14420a8 */
#define ai_TextEinkaufMetall                     (*(int *)((char *)txt_Ptrs + 0x07ef0))  /* 0x14420b0 */
#define ai_TextEinkaufStein                      (*(int *)((char *)txt_Ptrs + 0x07ef8))  /* 0x14420b8 */
#define full_data_14420d0                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x07f10))  /* 0x14420d0 */
#define full_data_14420d8                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x07f18))  /* 0x14420d8 */
#define full_data_14420e0                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x07f20))  /* 0x14420e0 */
#define full_data_14420e8                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x07f28))  /* 0x14420e8 */
#define full_data_14420f0                        (*(unsigned char (*)[16])((char *)txt_Ptrs + 0x07f30))  /* 0x14420f0 */
#define full_data_1442100                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x07f40))  /* 0x1442100 */
#define full_data_1442108                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x07f48))  /* 0x1442108 */
#define full_data_1442110                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x07f50))  /* 0x1442110 */
#define full_data_1442118                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x07f58))  /* 0x1442118 */
#define ai_TextEinkaufHolz                       (*(int *)((char *)txt_Ptrs + 0x07f60))  /* 0x1442120 */
#define full_data_1442128                        (*(unsigned char (*)[8])((char *)txt_Ptrs + 0x07f68))  /* 0x1442128 */
#define full_data_1442130                        (*(unsigned char (*)[48])((char *)txt_Ptrs + 0x07f70))  /* 0x1442130 */
#define full_data_1442160                        (*(unsigned char (*)[32])((char *)txt_Ptrs + 0x07fa0))  /* 0x1442160 */
#define full_data_1442180                        (*(unsigned char (*)[16])((char *)txt_Ptrs + 0x07fc0))  /* 0x1442180 */
#define full_data_1442190                        (*(unsigned char (*)[40])((char *)txt_Ptrs + 0x07fd0))  /* 0x1442190 */
#define full_data_14421b8                        (*(unsigned char (*)[348])((char *)txt_Ptrs + 0x07ff8))  /* 0x14421b8 */
#define full_data_1442314                        (*(unsigned char (*)[36])((char *)txt_Ptrs + 0x08154))  /* 0x1442314 */
#define full_data_1442334                        (*(unsigned char (*)[256])((char *)txt_Ptrs + 0x08174))  /* 0x1442334 */
#define full_data_1442430                        (*(unsigned char (*)[208])((char *)txt_Ptrs + 0x08270))  /* 0x1442430 */
#define full_data_1442500                        (*(unsigned char (*)[24252])((char *)txt_Ptrs + 0x08340))  /* 0x1442500 */
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
#define ai_Unk144a160                            (*(char * *)((char *)txt_Ptrs + 0x0ffa0))  /* 0x144a160 */
#define full_data_144a17c                        (*(unsigned char (*)[32])((char *)txt_Ptrs + 0x0ffbc))  /* 0x144a17c */
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
#define full_data_144abe0                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10a20))  /* 0x144abe0 */
#define full_data_144abe4                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10a24))  /* 0x144abe4 */
#define full_data_144abe8                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10a28))  /* 0x144abe8 */
#define full_data_144abec                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10a2c))  /* 0x144abec */
#define full_data_144abf0                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10a30))  /* 0x144abf0 */
#define full_data_144abf4                        (*(unsigned char (*)[4])((char *)txt_Ptrs + 0x10a34))  /* 0x144abf4 */
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
#define ai_Unk144ad24                            (*(char * *)((char *)txt_Ptrs + 0x10b64))  /* 0x144ad24 */
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
#define ai_Unk144adb0                            (*(int *)((char *)txt_Ptrs + 0x10bf0))  /* 0x144adb0 */
#define full_data_144adb4                        (*(unsigned char (*)[5132])((char *)txt_Ptrs + 0x10bf4))  /* 0x144adb4 */

#endif

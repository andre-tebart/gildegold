
short d2_Unkf8a650;  /* 0xf8a650 */
short d2_Unkf8a652;  /* 0xf8a652 */
int d2_Unkf8a654;  /* 0xf8a654 */
int d2_FocusObjIndex;  /* 0xf8a658 */
int d2_Unkf8a65c;  /* 0xf8a65c */
int d2_InputDragOffsetY;  /* 0xf8a660 */
int d2_SavedMaxY;  /* 0xf8a664 */
int d2_SavedMinY;  /* 0xf8a66c */
D2OBJ * d2_Unkf8a670;  /* 0xf8a670 */
D2HIT d2_HitRects[0x40];  /* 0xf8a680 (d2_AddHitRect allows i = 0x40, one past: the original's latent overrun) */
int d2_Unkf8aa80;  /* 0xf8aa80 */
int d2_HitCount;  /* 0xf8aa84 */
D2SLOT d2_ItemListSurf256;  /* 0xf8aa90 */
int d2_Unkf8aa9c;  /* 0xf8aa9c */
D2SLOT d2_ItemListSurf128[3];  /* 0xf8aaa0 */
int d2_Unkf8aac4;  /* 0xf8aac4 */
int d2_Unkf8aac8;  /* 0xf8aac8 */
int d2_Unkf8aacc;  /* 0xf8aacc */
int d2_TextCursorX;  /* 0xf8aad4 */
int d2_TextCursorY;  /* 0xf8aad8 */
void * d2_ItemListSurf64;  /* 0xf8aadc */
int d2_InputDragValue;  /* 0xf8ab60 */
int d2_Unkf8ab64;  /* 0xf8ab64 */
int d2_InputDragReleased;  /* 0xf8ab68 */
int d2_PointerSprite = -1;  /* 0x5febc8, sprite of the pointer, -1: none */
int d2_TextCursorOffset = -7;  /* 0x5febcc, -7 or 0 */
int d2_HitClicked = -1;  /* 0x5febd0 */
int d2_HitUnderPointer = -1;  /* 0x5febd4 */
int d2_Unk5febdc = -1;  /* 0x5febdc */
int d2_Unk5febe0 = -1;  /* 0x5febe0 */
int d2_LimitsChanged = -1;  /* 0x5febe4 */
int d2_TextCursorSprite = -1;  /* 0x5febe8, sprite of the text cursor, -1: none */
int d2_Unk5febec = -1;  /* 0x5febec */
int d2_Unk6ed990 = 0;  /* 0x6ed990 */
int d2_DragStartValue = 0;  /* 0x6ed9c4 */
void * d2_PointerPic = 0;  /* 0x6ed9cc, "_MOUSE_CURSOR" data */
int d2_Unk6ed9e4 = 0;  /* 0x6ed9e4 */
int d2_DragStartX = 0;  /* 0x6ed9e8 */
int d2_DragStartY = 0;  /* 0x6ed9ec */
int d2_ClickUsed = 0;  /* 0x6ed9f0 */
int d2_DragActive = 0;  /* 0x6ed9f4 */

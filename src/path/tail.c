
unsigned char path_SetFlag;  /* 0x1460aa0 */
float * path_Costs;  /* 0x1460aa4, path_Terrain[type] of the running search */
unsigned char path_TestFlag;  /* 0x1460aa8 */
float path_Tab1Sel;  /* 0x1460aac, path_Tab1[type], written per search, never read */
unsigned short path_Count;  /* 0x1460ab0 */
short path_Stamp;  /* 0x1460ab2 */
unsigned short path_Goal;  /* 0x1460ab4 */
unsigned short path_Start;  /* 0x1460ab6 */
float path_Tab2Sel;  /* 0x1460ab8, path_Tab2[type], written per search, never read */
unsigned short path_Target;  /* 0x1460abc */
unsigned short path_Meet;  /* 0x1460abe */
float path_Weight;  /* 0x1460ac0 */
int path_Mask = 63;  /* 0x5f7f60, size - 1 */
int path_Shift = 6;  /* 0x5f7f64, log2(size) */
short path_DX[8] = { -1, 0, 1, 0, -1, 1, 1, -1 };  /* 0x5f7f68 */
short path_DY[8] = { 0, -64, 0, 64, -64, -64, 64, 64 };  /* 0x5f7f78, set per search from the grid size */
float path_Step[8] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.41420996f, 1.41420996f, 1.41420996f, 1.41420996f };  /* 0x5f7f88, 1 or sqrt(2) */
float path_Terrain[][15] = { { 999.0f, 20.0f, 30.0f, 30.0f, 30.0f, 30.0f, 1.0f, 30.0f, 40.0f, 30.0f, -1.0f, 10.0f, 0.5f, -1.0f, 99.0f }, { 99.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.25f, 0.75f, -1.0f, 99.0f }, { 99.0f, 50.0f, 50.0f, 50.0f, 50.0f, 50.0f, 1.0f, 50.0f, 60.0f, 50.0f, -1.0f, 10.0f, 0.5f, -1.0f, 99.0f }, { 99.0f, 50.0f, 50.0f, 50.0f, 50.0f, 50.0f, 1.0f, 50.0f, 50.0f, 50.0f, -1.0f, 1.0f, 1.0f, -1.0f, 99.0f }, { 99.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 99.0f, 1.0f, 1.0f, -1.0f, 1.0f }, { 999.0f, 99.0f, 90.0f, 90.0f, 90.0f, 90.0f, 90.0f, 90.0f, 90.0f, 90.0f, 99.0f, 10.0f, 0.5f, -1.0f, 99.0f } };  /* 0x5f7fa8, cost per type (6 rows); < 0 = impassable */
float path_Tab1[6] = { 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.0f };  /* 0x5f8110, per type: 0.25, 0.0 for one type */
float path_Tab2[6] = { 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.0f };  /* 0x5f8128, same shape as path_Tab1 */
int path_Mode = 0;  /* 0x633184, 1 = one-directional search */

/* 0x4fb890, chooses the head icon number of the character c (+0x18c = icon index + 0x639; id at +0, number at +4). Without a model (+0x184) or icon names: the head numbers of the name record (0x4fb390, filled as 0x4fb620 does, "zigeunermeister" without the suffix), the character takes number (its id % count of the valid ones); returns id / count. With a model: the first model part name (0x40 bytes each, the count at +0x1e0 of its class) containing "kopf" / "KOPF" / "HEAD" / "head" whose icon "%s_ICON.bmp" exists. Otherwise +0x18c = 0x639. ROUGH */

extern char * ai_PlayerBuilding;  /* 0x6f2308 */
extern char * ai_FallbackBuilding;  /* 0x6f230c */
struct CHARACTER * ch_Create();                     /* 0x403390 */
int gm_IsHomeType();                     /* 0x502520 */
int init_ObjectSceneUniverse();                     /* 0x511dc0 */
int init_BuildingSceneUniverse();                     /* 0x511e60 */
void ch_PreloadAnims(struct CHARACTER *ch, int n, ...);                     /* 0x404200 */
void ch_SetVisible();                     /* 0x402480 */
unsigned *unk_Unk4fb390(int id);

int gm_ChooseHeadIcon(unsigned short *c)
{
    char name[128], suffix[128], icon[128];
    char *u, *part;
    unsigned *rec;
    int *m;
    int *q;
    int i, j, h, n, found, cls, cnt, stride;
    unsigned k;

    found = 0;
    i = 0;
    if (c == 0)
        return 0;
    m = *(int **)(c + 0xc2);
    if (m == 0 || ai_Unk6f1b94 == 0) {
        rec = unk_Unk4fb390(*c);
        if (rec[0xd] == 0xff) {
            strcpy(name, (char *)(rec + 1));
            u = strrchr(name, '_');
            if (u != 0) {
                strcpy(suffix, u + 1);
                *u = 0;
            }
            q = (int *)(rec + 0xd);
            *q = 0;
            for (h = 0; h < 4; h++, q++) {
                sprintf(icon, "%s_HEAD%i_%s_ICON.bmp", name, h, suffix);
                if (strstr(name, "zigeunermeister") != 0)
                    sprintf(icon, "%s_HEAD%i_ICON.bmp", name, h);
                *q = 0xff;
                for (j = 0; j < ai_Unk6f1b90; j++) {
                    if (ai_Unk6f1b94[j * 0x30] == 0)
                        break;
                    if (_stricmp(icon, ai_Unk6f1b94 + j * 0x30) == 0) {
                        *q = j;
                        break;
                    }
                }
            }
        }
        for (k = 0; k < 4; k++) {
            if (rec[0xd + k] == 0xff)
                break;
        }
        if (k != 0) {
            *(int *)(c + 0xc6) = rec[0xd + *(unsigned *)(c + 2) % k] + 0x639;
            return *(unsigned *)(c + 2) / k;
        }
    } else {
        if ((char *)m[0x22] == (char *)(unsigned)&d3_Scenes)
            return (int)m;
        cls = *(int *)(*(int *)(m[0xd] + 0x1ec) + 0x104);
        stride = *(unsigned char *)(*(int *)(m[0xd] + 0x1ec) + 0x289);
        n = *(int *)(cls + 0x1e0);
        if (n > 0) {
            do {
                if (found != 0)
                    return found;
                part = (char *)((n * stride + i) * 0x40 + *(int *)(cls + 0x204));
                if (*part != 0 && (strstr(part, "kopf") != 0 || strstr(part, "KOPF") != 0 || strstr(part, "HEAD") != 0 || strstr(part, "head") != 0)) {
                    sprintf(icon, "%s_ICON.bmp", part);
                    for (j = 0; j < ai_Unk6f1b90; j++) {
                        if (ai_Unk6f1b94[j * 0x30] == 0)
                            break;
                        if (_stricmp(icon, ai_Unk6f1b94 + j * 0x30) == 0) {
                            found = 1;
                            *(int *)(c + 0xc6) = j + 0x639;
                            break;
                        }
                    }
                }
                n = *(int *)(cls + 0x1e0);
                i++;
            } while (i < n);
            if (found != 0)
                return found;
        }
    }
    *(int *)(c + 0xc6) = 0x639;
    return 0;
}

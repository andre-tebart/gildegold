/* 0x4fbb80, creates the model of the character c (+0x184) in the scene model: at the position p (0x403390 with the name of the head record 0x4fb390 (c), 0x20-byte zero vector when p is 0), named "sp_<number>", linked back to c (+0x200 of its object, +0x48 = id | 0x3000000), copies the name to c + 500 and chooses the head (0x4fb5a0 / 0x4fb890); the position comes from the dummy of the object obj when a dummy name is given (0x449690 / 0x458a20); scene 0x633444 is selected again at the end. Returns the model. ROUGH */

int unk_Unk4fbb80(unsigned short *c, int scene, int *p, int *ori, int obj, char *dummy)
{
    int saved;
    int zero[4];
    int pos[3];
    int it;
    char *name;
    char *s;
    int *m;
    int *ori0;

    saved = d3_CurrentScene;
    ori0 = ori;
    zero[0] = 0;
    zero[1] = 0;
    zero[2] = 0;
    zero[3] = 0;
    if (p == 0)
        p = zero;
    if (ori == 0)
        ori = zero;
    if (scene < 0)
        return 0;
    d3if_SelectScene(scene, 1);
    if (dummy != 0 && (it = d3_FindObject(obj, 0x100, 0, dummy)) != 0) {
        d3_PointToWorld(it, it + 0x4c, pos);
    } else {
        pos[0] = p[0];
        pos[1] = p[1];
        pos[2] = p[2];
    }
    {
        unsigned *rec = unk_Unk4fb390(*c);
        if (rec != 0) {
            name = (char *)(rec + 1);
            s = name;
            m = (int *)ch_Create(name, pos);
            *(int **)(c + 0xc2) = m;
            if (m != 0) {
                sprintf((char *)m[0xd], "sp_%i", *(int *)(c + 2));
                *(unsigned short **)(*(int *)(*(int *)(c + 0xc2) + 0x34) + 0x200) = c;
                *(unsigned *)(*(int *)(*(int *)(c + 0xc2) + 0x34) + 0x48) = *c | 0x3000000;
                if (ori0 != 0)
                    d3_SetObjectRot(*(int *)(*(int *)(c + 0xc2) + 0x34), ori0);
                do {
                    *((char *)c + 500 + (s - name)) = *s;
                } while (*s++ != 0);
                gm_SelectModelVariant((char *)c);
                gm_ChooseHeadIcon(c);
            }
        }
    }
    d3if_SelectScene(saved, 1);
    return *(int *)(c + 0xc2);
}

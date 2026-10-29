/* 0x5a7cd0, the map window's per-frame draw (1556 B, ROUGH SKELETON: the two control-rectangle blocks and the
   else branch of 0x5a8131 are not decoded; the camera projection is the 0x5a6b70 shape with the scene centre
   +0 / +8 and the scale +0x10 / +0x18): the scroll changed flag +0x279c (0x5a79e0 / 0x5a7bc0, the time 0x6333cc
   at +0x27a0), the hover 0x5fe710 over the entries (0x5890c0 on the character 0x770b80 + the index * 0x21c), then
   the object list 0x1468ae0 .. 0x14696df: an object of the class 0xfd7ba0 with the character +0x34, its boss
   building +0x200, the cart 0x50a050 and the character kinds 5 .. 7 (0x770b82) give the picture id (0x46cd80 at
   the camera projection, id - 0x53, the ids +0x18dc); 0x46e710 removes the controls; the arrows 0x472760 */

void ai_Unk5a7cd0(int *m_)
{
    char *p = (char *)m_;
    char *scene;
    char *rec;
    char *obj;
    char *chr;
    char *owner;
    int *e;
    int *list;
    int entry;
    int found;
    int x;
    int y;
    int i;
    int ctrl;
    int id;
    int pos[4];
    float fx;
    float fy;
    float cx;
    float cy;
    float half;
    unsigned short idx;

    scene = (char *)*(int *)((unsigned)&ai_Unkfd7c50);
    rec = (char *)((unsigned)&d2_Wins + m_[2] * 0x3c4);
    *(int *)((unsigned)&d2_LastWindow) = m_[2];
    *(int *)((unsigned)&d2_LastWindowPtr) = (int)rec;
    if (*(int *)((unsigned)&ai_Unk62c26c) != 0) {
        m_[0x9e7] = 0;
        m_[0x9e7] = ai_Unk5a79e0((int)rec);
        m_[0x9e7] |= ai_Unk5a7bc0((int)rec);
        if (m_[0x9e7] != 0)
            m_[0x9e8] = *(int *)((unsigned)&ai_Unk6333cc);
    }
    if (*(int *)((unsigned)&d2_Unkf8aac8) != -1 && m_[0x9e8] + 0x4b < *(int *)((unsigned)&ai_Unk6333cc) &&
        *(int *)(((unsigned)&dm_L + 0x28)) != 0 && *(int *)((unsigned)&ai_Unk62a8cc) == -1 && m_[0x9e9] > 0) {
        i = 0;
        e = (int *)(p + 0xe0);
        do {
            if (*(int *)((unsigned)&ai_Unk5fe710) != -1 && *(int *)((unsigned)&ai_Unk5fe710) == e[-1] &&
                *(unsigned short *)(*e + 0x27) != 0xffff &&
                *(unsigned char *)(((unsigned)&ai_Chars + 0x2) + *(unsigned short *)(*e + 0x27) * 0x21c) < 10)
                ai_Unk5890c0((unsigned)&ai_Chars + *(unsigned short *)(*e + 0x27) * 0x21c);
            i++;
            e += 6;
        } while (i < m_[0x9e9]);
    }
    *(int *)((unsigned)&d2_LastWindow) = m_[2];
    entry = (unsigned)&ai_Unk1468ae0;
    e = (int *)(p + 0x18dc);
    while (entry <= ((unsigned)&ai_Unk1468ae0 + 0xbff)) {
        obj = (char *)*(int *)entry;
        if (obj != 0) {
            if (*(int *)(obj + 0x88) == (unsigned)&d3_Scenes) {
                chr = (char *)*(int *)(obj + 0x34);
                d3_PointToWorld((int)chr, (int)(chr + 0x4c), pos);
                half = (float)*(int *)(scene + 0x20) * 0.5f;
                fx = ((*(float *)pos - *(float *)(scene + 0)) / *(float *)(scene + 0x10) - half) * 5.33f;
                fy = ((*(float *)((char *)pos + 8) - *(float *)(scene + 8)) /
                      *(float *)(scene + 0x18) - half) * 5.33f;
                cx = (fabs(fx) / ((float)*(int *)((unsigned)&ai_MapSize) * 0.5f)) * 51.0f;
                if (0.0f <= fx)
                    cx = fx + cx;
                else
                    cx = fx - cx;
                cy = (fabs(fy) / ((float)*(int *)((unsigned)&full_data_6f4ee4) * 0.5f)) * 51.0f;
                if (0.0f <= fy)
                    cy = fy + cy;
                else
                    cy = fy - cy;
                x = (int)((float)(*(int *)((unsigned)&ai_MapSize) / 2 + *(int *)(p + 0x24)) + cx);
                y = (int)((float)(*(int *)((unsigned)&full_data_6f4ee4) / 2 + *(int *)(p + 0x28)) + cy);
                found = 0;
                id = 0;
                if (e[0] == -1) {
                    if ((*(int *)(chr + 0x218) != 1 || *(int *)(obj + 0x124) != 0) &&
                        (owner = (char *)*(int *)(chr + 0x200)) != 0) {
                        if (*(int *)(obj + 0x124) == 0) {
                            if (*(int *)(owner + 0x16c) != 0) {
                                idx = *(unsigned short *)(*(int *)(owner + 0x16c) + 0x27);
                                if (idx != 0xffff) {
                                    chr = (char *)((unsigned)&ai_Chars + idx * 0x21c);
                                    if (chr[2] == 6 || chr[2] == 7 || chr[2] == 5) {
                                        id = *(int *)(chr + 0x54);
                                        found = 1;
                                    }
                                }
                            }
                        } else if (owner != 0 && (owner = (char *)gm_Unk50a050((int)owner)) != 0 &&
                                   (owner[2] == 6 || owner[2] == 7 || owner[2] == 5)) {
                            id = *(int *)(owner + 0x54);
                            found = 1;
                        }
                        if (found) {
                            ctrl = d2_CreatePicObj(x + *(short *)(rec + 4), y + *(short *)(rec + 6),
                                                id - 0x53);
                            e[0] = ctrl;
                            if (ctrl != -1) {
                                *(short *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x1e) = *(short *)(rec + 4);
                                *(short *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x20) =
                                    *(short *)(rec + 8) + *(short *)(rec + 4);
                                *(short *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x22) = *(short *)(rec + 6);
                                *(short *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x24) =
                                    *(short *)(rec + 0xa) + *(short *)(rec + 6);
                                *(int *)((unsigned)&d2_Objs + ctrl * 0x2ec + 0x40) = (unsigned)&d2_Views + *m_ * 0xab * 4;
                            }
                        }
                    }
                } else if (*(int *)(chr + 0x218) != 1 || *(int *)(obj + 0x124) != 0) {
                    /* ROUGH: the existing control is moved to the projection minus the scroll */
                    d2_MoveObj((short)(x + *(short *)(rec + 4) - *(short *)(rec + 0x258)),
                                 (short)(y + *(short *)(rec + 6) - *(short *)(rec + 0x248)), e[0]);
                    *(short *)((unsigned)&d2_Objs + e[0] * 0x2ec + 0x1e) = *(short *)(rec + 4);
                    *(short *)((unsigned)&d2_Objs + e[0] * 0x2ec + 0x20) = *(short *)(rec + 8) + *(short *)(rec + 4);
                    *(short *)((unsigned)&d2_Objs + e[0] * 0x2ec + 0x22) = *(short *)(rec + 6);
                    *(short *)((unsigned)&d2_Objs + e[0] * 0x2ec + 0x24) =
                        *(short *)(rec + 0xa) + *(short *)(rec + 6);
                }
            }
            if (e[0] != -1 &&
                ((*(int *)(*(int *)(obj + 0x34) + 0x218) == 1 || *(int *)(obj + 0x88) != (unsigned)&d3_Scenes) &&
                 *(int *)(obj + 0x124) == 0)) {
                d2_RemoveObj(e[0]);
                e[0] = -1;
            }
        }
        entry += 4;
        e++;
    }
    if (*(unsigned char *)(((unsigned)&kb_Down + 0xc8)) != 0) {
        d2_ScrollWindowBy(0, -4, *(int *)((unsigned)&d2_LastWindow));
        return;
    }
    if (*(unsigned char *)(((unsigned)&kb_Down + 0xd0)) != 0) {
        d2_ScrollWindowBy(0, 4, *(int *)((unsigned)&d2_LastWindow));
        return;
    }
    if (*(unsigned char *)(((unsigned)&kb_Down + 0xcb)) != 0) {
        d2_ScrollWindowBy(-4, 0, *(int *)((unsigned)&d2_LastWindow));
        return;
    }
    if (*(unsigned char *)(((unsigned)&kb_Down + 0xcd)) != 0)
        d2_ScrollWindowBy(4, 0, *(int *)((unsigned)&d2_LastWindow));
}

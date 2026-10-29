/* 0x50b9f0, (re)builds the widgets of the nodes laid out by 0x50b3a0 for the building a and the player record b: per node the state 0 (not available) .. 3 (in progress) is derived (class 6 items: researched /
   queued objects 0x53e400 (2, 0, 0xc, 3, owner), parents known through 0x4ffe90; other items: the same with the order list kind 2), and when it differs from the node's last state (+0x30) the old widgets (+0x1c ..
   +0x2c) are freed (0x472ce0) and new ones created (0x472aa0 / 0x4731d0 with the images 0x52b / 0x529 / 0x52a / 0x4fb / 0x4fc, the label 0x54 and the text 0x515960 (item * 2 + 0x9a8)); state 3 nodes get their
   progress (+0x1e4 of the widget = the elapsed time over the duration, 0x4fe280). ROUGH */

extern char *d2_Objs;  /* 0xf8f080, 0x2ec bytes each; @original: d2.h declares D2OBJ *; the units add byte offsets to it and cannot include d2.h (vfs_Root / vfs_Tfopen clash with vfs.h) */
extern int d2_LastWindow;  /* 0x6ed938 */
int d2_RemoveWindow();                     /* 0x472ce0 */
int d2_CreateChildWindow(short x, short y, short w, short h, unsigned int flags, int win);                     /* 0x472aa0 */
int d2_WinOpenPic(short x, short y, int pic, int win);                     /* 0x4731d0 */
#define WIDGET(i) (d2_Objs + (i) * 0x2ec)

int gm_Unk50b9f0(char *a, short *b)
{
    int i, k, cnt, state, par, w, cls, img, sub;
    char *o;
    int saved;
    int parent;
    RTNODE *nd;

    parent = *(int *)(ai_Unkfb1030 + ai_Unk770af0 * 0x2ac);
    for (i = 0; i < ai_Unk7d6a40; i++) {
        nd = &NODES[i];
        state = 0;
        cls = *(char *)(ai_Goods + nd->id * 0x41);
        o = 0;
        if (cls == 6) {
            if (gm_Query(*(int *)(a + 0x5d), 1, 0, nd->id) != 0) {
                state = 1;
                for (o = (char *)he_FindEvent(2, 0, 0xc, 3, *(int *)(a + 1)); o != 0; o = (char *)he_NextEvent())
                    if (*(int *)(o + 0xb4) == nd->id) {
                        state = 3;
                        break;
                    }
            } else {
                cnt = 0;
                for (k = 0; k < i; k++) {
                    RTNODE *q = &NODES[k];
                    if ((q->id == nd->p1 || q->id == nd->p2)
                        && (gm_Query(*(int *)(a + 0x5d), 1, 0, q->id) != 0 || gm_Query(*(int *)(b + 10), 1, 0, q->id) != 0)
                        && *(int *)(WIDGET(q->l[2]) + 0x50) == 0)
                        cnt++;
                }
                if (cnt == 0)
                    par = nd->p1;
                else if (cnt == 1)
                    par = nd->p2;
                else
                    par = cnt - 2;
                if (par == 0)
                    state = 2;
            }
        } else if (gm_Query(*(int *)(b + 10), 1, 0, nd->id) != 0)
            state = 1;
        else if (*b == 0xfd && gm_Query(*(int *)(a + 0x5d), 1, 0, nd->id) != 0)
            state = 1;
        else {
            cnt = 0;
            for (k = 0; k < i; k++) {
                RTNODE *q = &NODES[k];
                if ((q->id == nd->p1 || q->id == nd->p2)
                    && (gm_Query(*(int *)(a + 0x5d), 1, 0, q->id) != 0 || gm_Query(*(int *)(b + 10), 1, 0, q->id) != 0))
                    cnt++;
            }
            if (cnt == 0)
                par = nd->p1 == 0;
            else if (cnt == 1)
                par = nd->p2 == 0;
            else
                par = cnt == 2;
            if (par) {
                state = 2;
                for (o = (char *)he_FindEvent(2, 0, 2, 3, *(int *)(a + 1)); o != 0; o = (char *)he_NextEvent())
                    if (*(int *)(o + 0xb0) == *(int *)(b + 2) && *(unsigned short *)(o + 0xac) == nd->id) {
                        state = 3;
                        break;
                    }
            }
        }
        if (state != nd->flag) {
            saved = d2_LastWindow;
            if (nd->l[0] != -1) {
                d2_RemoveWindow(nd->l[0]);
                nd->l[0] = nd->l[1] = nd->l[2] = nd->l[3] = -1;
            }
            if (nd->l[4] != -1) {
                d2_RemoveWindow(nd->l[4]);
                nd->l[4] = -1;
            }
            w = d2_CreateChildWindow(nd->x - 4, nd->y - 4, 0x36, 0x36, 0x400, parent);
            nd->l[0] = w;
            if (state == 0) {
                nd->l[1] = d2_WinOpenPic(0, 0, 0x52b, w);
                nd->l[2] = d2_WinOpenPic(3, 3, 0x4fb, nd->l[0]);
            } else {
                img = state == 2 ? 0x52a : 0x529;
                nd->l[1] = d2_WinOpenPic(0, 0, img, w);
                sub = nd->id < 0x2dc ? nd->id + 0xdf : 0xdf;
                nd->l[2] = d2_WinOpenPic(3, 3, sub, nd->l[0]);
                if (state == 1)
                    *(int *)(WIDGET(nd->l[2]) + 0x54) = 1;
                *(int *)(WIDGET(nd->l[2]) + 0x4c) = 1;
                d2_LastWindow = saved;
                if (state == 3)
                    nd->l[3] = d2_WinOpenPic(3, 3, 0x4fc, nd->l[0]);
                nd->l[4] = d2_CreateChildWindow(nd->x - 0x18, nd->y + 0x32, 0x60, 0x11, 0x4510, parent);
                *(int *)(ai_Unkf8f128 + 0x274 + nd->l[4] * 0x3c4) = 0x54;
                *(short *)(ai_Unkf8f128 + 0xc + nd->l[4] * 0x3c4) = 1;
                *(short *)(ai_Unkf8f128 + 0xe + nd->l[4] * 0x3c4) = 1;
                if (nd->id < 0x2dc)
                    io_PrintText(nd->id * 2 + 0x9a8);
                if (state == 3)
                    *(int *)(WIDGET(nd->l[2]) + 0x54) = 1;
            }
            d2_LastWindow = saved;
            nd->flag = (unsigned char)state;
        }
        if (nd->flag == 3) {
            if (cls == 6) {
                if (gm_Query(*(int *)(a + 0x5d), 1, 0, nd->id) != 0)
                    for (o = (char *)he_FindEvent(2, 0, 0xc, 3, *(int *)(a + 1)); o != 0; o = (char *)he_NextEvent())
                        if (*(int *)(o + 0xb4) == nd->id)
                            goto progress;
            } else {
                for (o = (char *)he_FindEvent(2, 0, 2, 3, *(int *)(a + 1)); o != 0; o = (char *)he_NextEvent())
                    if (*(int *)(o + 0xb0) == *(int *)(b + 2) && *(unsigned short *)(o + 0xac) == nd->id)
                        goto progress;
            }
            *(float *)(WIDGET(nd->l[2]) + 0x1e4) = 0;
            continue;
        progress:
            k = gm_MinutesBetween(o + 0x44, ai_Time);
            *(float *)(WIDGET(nd->l[2]) + 0x1e4) = (float)gm_MinutesBetween(o + 0x44, o + 0x52) ? (float)k / (float)gm_MinutesBetween(o + 0x44, o + 0x52) : 0.0f;
        }
    }
    return 0;
}

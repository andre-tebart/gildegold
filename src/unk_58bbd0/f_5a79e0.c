/* 0x5a79e0, the map drag scroll (473 B): while the mouse drag 0x14673a8 is held (0x6f26bc), the scroll of the
   screen record (+0x258 / +0x248) follows the drag position 0x14683c0 / 0x14683c2 from the anchors 0x6f26a4 /
   0x6f26a8; the previous anchors 0x6f268c .. and the bounds 0x6f2698 / 0x5f3238 / 0x6f26b4 / 0x6310b0 etc. are
   saved on entry and restored (with 0x4113f0) on release; returns whether the scroll changed */

int ai_Unk5a79e0(int rec)
{
    int changed = 0;
    int x;
    int y;

    if (*(int *)(((unsigned)&dm_L + 0x28)) == 0) {
        if (*(int *)((unsigned)&full_data_6f26bc) != 0) {
            *(int *)((unsigned)&di_MaxY) = *(int *)((unsigned)&full_data_6f268c);
            *(int *)((unsigned)&di_MinX) = *(int *)((unsigned)&full_data_6f269c);
            *(int *)((unsigned)&di_MaxX) = *(int *)((unsigned)&full_data_6f2698);
            *(int *)((unsigned)&full_data_6f26b4) = 0;
            *(int *)((unsigned)&full_data_6f26b8) = 0;
            *(int *)((unsigned)&di_MinY) = *(int *)((unsigned)&full_data_6f2690);
            *(int *)((unsigned)&di_Absolute) = 1;
            di_SetMousePos((short)*(int *)((unsigned)&d2_MouseX), (short)*(int *)((unsigned)&d2_MouseY));
            *(int *)((unsigned)&full_data_6f26bc) = 0;
            return 0;
        }
    } else {
        if (*(int *)((unsigned)&full_data_6f26bc) == 0) {
            *(int *)((unsigned)&full_data_6f26b4) = (short)*(int *)((unsigned)&dm_L);
            *(int *)((unsigned)&di_Absolute) = 0;
            *(int *)((unsigned)&full_data_6f26b8) = (short)*(int *)(((unsigned)&dm_L + 0x2));
            *(int *)((unsigned)&full_data_6f26bc) = 1;
            *(int *)((unsigned)&full_data_6f26a4) = *(int *)(rec + 0x258);
            *(int *)((unsigned)&full_data_6f26a8) = *(int *)(rec + 0x248);
            *(int *)((unsigned)&full_data_6f2698) = *(int *)((unsigned)&di_MaxX);
            *(int *)((unsigned)&full_data_6f268c) = *(int *)((unsigned)&di_MaxY);
            *(int *)((unsigned)&full_data_6f269c) = *(int *)((unsigned)&di_MinX);
            *(int *)((unsigned)&full_data_6f2690) = *(int *)((unsigned)&di_MinY);
            *(int *)((unsigned)&di_MinX) = *(int *)((unsigned)&full_data_6f26b4) - *(int *)((unsigned)&full_data_6f26a4);
            *(int *)((unsigned)&di_MaxX) = *(int *)((unsigned)&ai_MapSize) - *(int *)((unsigned)&full_data_6f26a4) - 0x200 + *(int *)((unsigned)&full_data_6f26b4);
            *(int *)((unsigned)&di_MinY) = *(int *)((unsigned)&full_data_6f26b8) - *(int *)((unsigned)&full_data_6f26a8);
            *(int *)((unsigned)&di_MaxY) = *(int *)((unsigned)&full_data_6f4ee4) - *(int *)((unsigned)&full_data_6f26a8) - 0x168 + *(int *)((unsigned)&full_data_6f26b8);
        }
        x = (int)((short)*(int *)((unsigned)&dm_P) - *(int *)((unsigned)&full_data_6f26b4));
        y = (int)((short)*(int *)(((unsigned)&dm_P + 0x2)) - *(int *)((unsigned)&full_data_6f26b8));
        if (*(int *)(rec + 0x258) != x + *(int *)((unsigned)&full_data_6f26a4) ||
            *(int *)(rec + 0x248) != *(int *)((unsigned)&full_data_6f26a8) + y)
            changed = 1;
        *(int *)(rec + 0x258) = x + *(int *)((unsigned)&full_data_6f26a4);
        *(int *)(rec + 0x248) = *(int *)((unsigned)&full_data_6f26a8) + y;
    }
    return changed;
}

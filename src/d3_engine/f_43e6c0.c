/* 0x43e6c0, the id of an event name ("SCENE_ENTER", case-insensitive), 0 when the name is unknown */

D3SCENE *d3_DefaultScene = d3_Scenes;  /* 0x5fb5b0, the current scene (d3.h; f3gm.c and cm.c use the
                                       bytes with their own ai_Unk5fb5b0 char * view) */
/* the initial D3D mode: bpp / mode / device flags, then d3if_Unk5fb5c0 (the last dword is that global,
   which the interface's scene select saves and restores; d3d_SetConfig reads only the first three) */
char d3_DefaultD3DConfig[] = { 94, 1, 0, 0, 3, 0, 0, 0, 2, 0, 0, 0, 4, 0, 0, 0 };  /* 0x5fb5b4 */
int d3if_Unk5fb5c4 = 15;  /* 0x5fb5c4, the scene pair's second dword (d3_interface.c's write-only state) */
/* 0x5fb800 */
D3EVNAME d3e_Events[] = {
    { "NONE", 0 },
    { "TEST", 1 },
    { "ZOOM_IN_OBJECT", 2 },
    { "ZOOM_OUT_OBJECT", 3 },
    { "TEST", 1 },
    { "SCENE_ENTER", 4 },
    { "SCENE_EXIT", 5 },
    { "SCENE_SUPERVISOR", 6 },
};

int d3e_EventId(char *name)
{
    unsigned int i;

    for (i = 0; i < sizeof(d3e_Events) / sizeof(d3e_Events[0]); i++)
        if (!_strcmpi(name, d3e_Events[i].name))
            return d3e_Events[i].id;
    return 0;
}

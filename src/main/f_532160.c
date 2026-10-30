/* 0x532160, main_OpenEngine: loads the text definitions ("<GamePath>die_gilde_add_on_text.def": txt_LoadIncludes, or txt_OpenText when main_TextReopen is set), shows the loading screen form
   "Misc\Loading_main" (window 1 above the bottom edge, window 0 at the top of the screen) while the history lists, the net states, the data tables, the effect slots, the icon names and the item
   prices are initialised, starts the sound (sound bank, 3D sound, "system.sbf", the "include_sfx.ini" of the program directory) and the music, then the game modules (sv's tick callback,
   scene universes, characters, snow mipmaps, the AI, the hmt tables), waits for the black fade ("BLACK") by running the frame loop 0x5309a0, clears the player profile and registers the
   event commands; returns 1. Reasoned: the item prices are called only for their (discarded) results' side effects, the order follows the original @deep body reconstructed and readable (typed d2 view / window views, sb_Init(10000000 ..), the d3anim_CacheMax = 0x400000 store the draft missed, text-definition branch in the original's polarity, gm_ItemPrice float prototype, all callees named from their comments). Left (asmdiff, 94 ins in 13 hunks, all one cause): the original is an ebp-frame function (push ebp; mov ebp,esp; sub esp,0x20c; tmp at [ebp-0x108], tmp2 at [ebp-0x20c]) because its memset(init_PlayerProfile, 0, 0x8c) is expanded by the generic C2 form (mov [ebp-4],0x8c; mov edi,addr; mov ebx,[ebp-4]; xor eax,eax; lea ecx,[edi+edi*2]; and ecx,3; sub ebx,ecx; jle; rep stosb; ...; rep stosd; add ecx,ebx; rep stosb), which also pins xor ebx,ebx as the zero register (push ebx instead of push 0, mov [x],ebx); ours compiles the same call to a plain rep stosd with an esp frame (235 vs 251 ins). Tried: memset(&p,0,0x8c), memset(p,0,0x8c) with extern unsigned char init_PlayerProfile[] and a local int n = 0x8c as count - all give the plain form. Needs the variant that makes C2 emit the alignment-fixup expansion (deep) */

#define SB_MEMORY 10000000     /* sb_Init's buffer size in bytes (a constant of the original) */
#define ANIM_CACHE_BYTES 0x400000   /* 4 MB: d3anim_CacheMax is set again here */

int sv_PumpMessages();
int sv_RunFrame(int cb, unsigned mask);

int main_OpenEngine(void)
{
    char tmp[260], tmp2[260];
    int f;
    struct D2FADE *fade;

    d2_CutsceneActive = 1;
    if (main_TextReopen != 0) {
        sprintf(tmp, "%s%s", main_GamePath, "die_gilde_add_on_text.def");
        txt_OpenText(tmp, 0);
    } else {
        sprintf(tmp, "%s%s", main_GamePath, "die_gilde_add_on_text.def");
        if (txt_LoadIncludes(tmp) == 0)
            err_Message("main_OpenEngine(): could not open txt-engine!");
    }
    hst_ResetLists();
    d2_ClearScreen();
    f = d2_OpenForm(0, 0, "Misc\Loading_main");
    d2_PlaceWindow(d2_Views[f].wins[0], 3);
    d2_MoveObj(0, d2_ScreenH - d2_Wins[d2_Views[f].wins[1]].h - 0x30, d2_Wins[d2_Views[f].wins[1]].parent);
    d2_PlaceWindow(d2_Views[f].wins[1], 1);
    io_PrintText("$ADie Gilde - Gold Edition - Version 2.05$X");
    net_Connect(0, 0);
    ai_InitStates();
    ai_ClearTurnTable();
    sprintf(tmp, "%sdata\\", main_GamePath);
    gm_LoadTables(tmp);
    unk_ResetEffectSlots();
    unk_LoadIconNames();
    ai_FreeSettings();
    gm_ItemPrice(0x1d5, 100);
    gm_ItemPrice(0x1d7, 100);
    gm_ItemPrice(0x1d4, 100);
    gm_ItemPrice(0x1da, 100);
    gm_ItemPrice(0x1d9, 100);
    gm_ItemPrice(0x1d6, 100);
    gm_ItemPrice(0x1d8, 100);
    if (snd_SfxEnabled != 0) {
        sfx_Startup();
        if (sb_Init(SB_MEMORY, 0x30, 2, 0x10, 0xac44) != 0) {
            snd_SfxEnabled = 0;
            snd_MusicEnabled = 0;
        }
        d3snd_Init(0x40);
        d3sndw_Init(0x30);
        sprintf(tmp, "%ssfx\\", main_GamePath);
        sb_SetPath(tmp);
        snd_SystemBank = sb_LoadSampleBank("system.sbf", 0);
        sprintf(tmp2, "%s\include_sfx.ini", main_AppDir);
        main_PreloadSound(tmp2);
    }
    if (snd_MusicEnabled != 0) {
        msx_Init(2, 0x10, 0xac44);
        sprintf(tmp, "%smsx\\", main_GamePath);
        msx_SetPath(tmp);
    }
    ai_ApplyVolumes();
    sv_InitTick();
    init_ResetSceneUniverses();
    ch_Open();
    d3t_CreateSnowMipmaps();
    ai_Init();
    ahm_LoadPrototypes(main_HmtMode);
    if (main_HmtBuild != 0)
        hmt_CreateBinaryFile();
    main_HmtBuild = 0;
    hmt_Init();
    ai_BuildFloatTable();
    d2_CloseView(f);
    fade = d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x5a, 1);
    while ((fade->flags & 4) == 0)
        sv_RunFrame(0, 0x24087);
    d2_ReleaseFade(fade);
    memset(init_PlayerProfile, 0, 0x8c);
    main_ScreenOpen = 1;
    evt_Init();
    evtc_Register();
    ocmd_Register();
    chc_Register();
    sndc_Register();
    d3e_Resolve = evt_GetStartScript;
    d3anim_CacheMax = ANIM_CACHE_BYTES;
    ai_ApplyOptions();
    sv_PumpMessages();
    return 1;
}

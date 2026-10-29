/* 0x512e40, the scene object callback: unless the object is of the kind 2 / 4 it switches the textures of the
   object's mesh (SCENEOBJ.mesh, the set list at +0xf4) to the current set (init_TextureSet, 0x446f30)
   @deep meaning fixed: the 2nd argument is the mesh + 0xf4 (ours dereferenced it), the typed views SCENEOBJ.kind/SCENEOBJ.mesh replace the raw casts, the return is char (the original ends mov al,1), d3if_SelectTextureSet -> d3if_SelectTextureSet (ecmd.c's void *list prototype, which also clears the cross-TU conflict) and ai_Unk6f1c24 -> init_TextureSet (written from gm_ClockSeason in cm.c, the set d3if_SelectTextureSet switches to): 9 -> 6 differing instructions. Remaining: the original's dead cmp ecx,0x3 between add edx,0xf4 and mov cl,[0x6f1c24] (its flags are overwritten by the next mov; tried s != 3 as a third && term (extra je), a switch with case 3/default merged (sub eax,2 chain), the tautology s != 3 || s == 3), plus ours loading the byte before push 0x1/add edx,0xf4 */


char init_TextureSetHook(int o)
{
    int s;

    s = ((SCENEOBJ *)o)->kind;
    if (s != 2 && s != 4)
        d3if_SelectTextureSet((D3OBJECT *)o, (char *)((SCENEOBJ *)o)->mesh + 0xf4, init_TextureSet, 1);
    return 1;
}

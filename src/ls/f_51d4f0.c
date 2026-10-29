/* 0x51d4f0, scene callback: runs init_HookObject (o, building, 2) for every used record of the 0x100 of ai_Blds
   (0xa9 bytes) whose id (+1) is the owner (+0x200) of the scene object o. Returns 1
   @deep Q4 blocked: the two remaining raw accesses (*(int *)(o + 0x200) the owner id of the scene object, *(int *)(p + 1) the building id) need typed fields - AIBLD from ai.h for the building record and a named D3OBJECT.+0x200 field from d3.h; ls.c cannot include ai.h (30 compile errors: gv.h declares ai_Blds/ai_Chars/gm_BuildQuery/ai_Unk6ffe90/... with different types than ai.h, first C2040 at include/gv.h(12)) nor get AIBLD any other way, and d3.h's D3OBJECT has only char pad200[0x8] there, so the field name is a shared-header decision. Everything else done: byte match kept, Q1/Q2/Q3/Q5/Q6/Q7 pass after init_HookObject and ai_Blds */


char ls_HookBuildingCB(char *o)
{
    int i;
    char *p;

    for (i = 0; i < 0xa900; i += 0xa9) {
        p = ai_Blds + i;
        if (*p != 0 && *(int *)(o + 0x200) == *(int *)(p + 1))
            init_HookObject(o, (unsigned short *)p, 2);
    }
    return 1;
}

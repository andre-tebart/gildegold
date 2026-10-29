/* 0x521960, selects the object o (of the player or a pause object) for the info panel: stores it in sv_PanelObject, its
   scene record of the id b in sv_PanelRecord (gm_Query of the object id +0x5d; the kind 0x47 uses the record 0xfd), copies
   the text c into sv_PanelText and raises sv_PanelFlag. When the record is the one shown (sv_PanelShown) the redraw counter
   sv_PopRequest is set to the frame difference (sv_FrameDepth - sv_FrameBase), else the record is taken over and shown
   (init_LoadObjectScene for a scene universe that is still dirty); finally sv_PopTo(0x591520)
   @deep behaviour draft after the base rewrites (256 -> 202 code hunks), readable done (Q1..Q7 pass). The statement set is complete; the reading matches the original instruction for instruction. Gains kept: (1) sv_PanelRecord is not copied into a local - the original reloads the global at every use (this alone 256 -> 202); (2) 'cur = 0; if (b) cur = b; else if (f) cur = f;' reproduces the original's shared conditional assign (cmp eax,ebp / jne / mov eax,[ai_FallbackBuilding] / cmp / je / mov edi,eax); (3) *sv_PanelRecord read unsigned (the original's xor edx,edx / mov dx into init_LoadObjectScene); (4) the record views SVOBJREC/SVCHARREC cost no hunk (202 with and without). What is left is one allocation cascade over four saved registers: the original pushes ebx/ebp/esi/edi with o=esi, cur=edi, the constant 0=ebp and the constant 1=ebx (mov ebx,0x1 before the first gm_Query, kept across both calls and both sv_PanelFlag = 1 stores); ours pushes ebp/esi/edi only, folds cur into ebp (so the one xor serves both cur = 0 and the zero constant), tests instead of cmp-with-zero and uses immediates for 1. Tried, same 202: cur as a declaration initializer, the nested if/else polarity swap, the inline ((SVOBJREC *)o)-> cast instead of the bld local. spell.py: 521965 reload (the original reloads the o parameter from its stack slot at one use; the bld local is not the cause - dropping it changes nothing), 5219db / 521b5a dup-tail (the cur tail and the sv_PopTo tail: both spellings give 202), 5219a1 code (the ai_BTypes address: the original lea's it into eax, ours folds it into the mov's SIB - sib/add-lea, no source form found). Remaining leads for deep: the constant-register CSE (0 in ebp, 1 in ebx) and cur in edi vs ebp, the ai_BTypes temp ebx vs edi, the lea vs SIB load - allocation, i.e. vargen territory. */

void sv_SelectPanelObject(unsigned char *o, int b, char *c)
{
    char k;
    unsigned char *cur = 0;
    int w;
    unsigned short *old;
    int oldv;
    SVOBJREC *bld;

    bld = (SVOBJREC *)o;
    if (sv_PanelFlag != 0)
        return;
    if (!(ai_PlayerId == 0xffff || ai_Chars[ai_PlayerId][0x1b1] == 0 || (k = ai_BTypes[bld->kind * 0x24d]) == 6 || (k == 4 && bld->ownerId == ((SVCHARREC *)ai_Chars[ai_PlayerId])->netId)))
        return;
    if (ai_PlayerBuilding != 0)
        cur = (unsigned char *)ai_PlayerBuilding;
    else if (ai_FallbackBuilding != 0)
        cur = (unsigned char *)ai_FallbackBuilding;
    sv_PanelObject = (char *)o;
    sv_PanelRecord = (unsigned short *)gm_Query(bld->objList, 1, 1, b);
    if (bld->kind == 0x47)
        sv_PanelRecord = (unsigned short *)gm_Query(bld->objList, 1, 0, 0xfd);
    strcpy(sv_PanelText, c);
    sv_PanelFlag = 1;
    if (sv_PanelRecord == 0 || *sv_PanelRecord == 0) {
        if (cur == o && ai_FallbackBuilding == 0) {
            sv_PanelFlag = 0;
            return;
        }
    } else {
        if (sv_PanelRecord == sv_PanelShown) {
            if (sv_FrameDepth <= sv_FrameBase) {
                sv_PanelFlag = 0;
                return;
            }
            sv_PopRequest = sv_FrameDepth - sv_FrameBase;
            return;
        }
        if (o == cur && gm_IsHomeType(cur) != 0) {
            bld->sceneRec = *sv_PanelRecord;
            if (sv_PanelShown != 0) {
                sv_PopRequest = (sv_FrameDepth - sv_FrameBase) + 1;
                return;
            }
            sv_PopRequest = sv_FrameDepth - sv_FrameBase;
            return;
        }
        old = sv_PanelShown;
        oldv = ai_FrameWait;
        bld->sceneRec = *sv_PanelRecord;
        if (gm_IsHomeType(o) == 0 && gm_IsType10(o) == 0) {
            sv_PanelShown = sv_PanelRecord;
            sv_PanelFlag = 0;
            ai_FrameWait = 0;
            w = init_ObjectSceneUniverse(sv_PanelRecord);
            if (init_SceneDirty[w * 0x3e0] != 0)
                init_LoadObjectScene(*sv_PanelRecord);
            sv_PanelFlag = 1;
            ai_FrameWait = oldv;
            sv_PanelShown = old;
        }
    }
    sv_PopTo(ai_GameFrame);
}

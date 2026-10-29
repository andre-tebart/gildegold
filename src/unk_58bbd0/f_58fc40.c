/* 0x58fc40, the construction site of a building record (247 B): the record's object (+0x61) must exist;
   d3if_SelectScene (0, 1) brackets the work; the record's kind byte names the building (ai_BuildingName) into
   out->name, the owner position goes to out->pos and is formatted (sprintf,
   "lc_GetGebaeudeBauplatz(): FindPos->Pos:%f  %f  %f") into the unused 0x100-byte local; the "bk_" finder
   ai_FindBauplatz (d3_ForEachObject, mask 0x180) over d3_DefaultScene fills out->object; 1 when a site was
   found, 0 else. The first parameter is unused (cm.c calls it with two arguments)
   @deep B 30 code hunks + 10 slots over 256 B: one register-role difference, not a meaning error. Read the whole original 0x58fc40..0x58fd36 and the asmdiff. The original keeps rec in ebx across the ai_BuildingName call, rematerialises rec->object in eax after it (3 callee-saved saves + a late push edi for the out+0x40 scratch, no ebp-frame); ours holds the same owner in ebx with rec in edi (4 saves, scene in ebp in both). The meaning fix is done: the sprintf destination is an unused char[0x100] local, not out (that alone removed the 0x100 frame error and 73 hunks), the calls go through d3if_SelectScene/sprintf/d3_ForEachObject/d3_PointToWorld/ai_FindBauplatz and the out record is the AIBUILDSITE view. Tried: owner read twice (short live ranges), int rec/int *out with casts, AIBLD* parameter, local declaration order, scene assignment before/after the first d3if_SelectScene call, raw-int spellings - every owner-live-across form gives 30 code + 10 slots, every rematerialised form 86 code + 4 regs; the register choice does not follow the source. c2prio / variant round. */

char d3if_SelectScene(int index, char keep);  /* 0x4478e0 (d3_interface.c) */

int lc_GetGebaeudeBauplatz(int unused, AIBLD *b, AIBUILDSITE *out)
{
    char text[0x100];
    int scene;
    int owner;

    owner = b->object;
    if (owner == 0)
        return 0;
    scene = d3_CurrentScene;
    d3if_SelectScene(0, 1);
    out->nameHead = 0;
    if (ai_BuildingName(b->kind, out->name) == 0) {
        d3if_SelectScene(scene, 1);
        return 0;
    }
    d3_PointToWorld((struct D3OBJECT *)owner, (struct D3VECTOR *)(owner + 0x4c),
                    (struct D3VECTOR *)out->pos);
    sprintf(text, "lc_GetGebaeudeBauplatz(): FindPos->Pos:%f  %f  %f",
            (double)out->pos[0], (double)out->pos[1], (double)out->pos[2]);
    out->object = 0;
    d3_ForEachObject(d3_DefaultScene, 0, 0x180,
                     (char (*)(struct D3OBJECT *, void *))ai_FindBauplatz, out);
    if (out->object == 0) {
        d3if_SelectScene(scene, 1);
        return 0;
    }
    d3if_SelectScene(scene, 1);
    return 1;
}

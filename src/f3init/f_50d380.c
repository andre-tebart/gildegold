/* 0x50d380, names the scene object o ("ob_NAME" for kind 1 = an object prototype p, "gb_NAME" for kind 2 = a building p) and hooks it up with p (+0x200 / the building's +0x61) @deep byte; only Q4 is left (8 raw *(T*) accesses). o is a D3OBJECT: +0x218 type / +0x220 kind / +0x224 mask exist in d3.h, but +0x200 (the owner record, here written with the prototype p) is untyped (d3.h: char pad200[0x8]) and the +0x1ec material ops[0] destroy-callback slot has no named field either - both are shared-header decisions (ls.c's 0x51d4f0 escalated the same +0x200 field). p is an AIBLD building record: +0x4 the prototype index, +0x5a the flags, +0x61 the scene object need ai.h, which neither f3init.c nor ls.c can include (gv.h declares ai_Blds / ai_Chars / gm_BuildQuery with types different from ai.h, 30 compile errors). No local view clears the AIBLD accesses. All other rules pass: Q1 init_HookObject (the name cm.h and ls.c already use for 0x50d380), Q2/Q7 cleared by folding the d3if_ShowTree aliases, Q6 described. Tried: the switch form (111 -> 8 code hunks) and the if(*b != 10) polarity (byte). */


void init_HookObject(char *o, unsigned short *p, int kind)
{
    char *b;

    switch (kind) {
    case 2:
        b = ai_BTypes + (unsigned)(unsigned char)*p * 0x24d;
        sprintf(o, "gb_%s", b + 1);
        *(char **)((char *)p + 0x61) = o;
        if (*b != 10) {
            *(int *)(o + 0x220) = 3;
            if (*(int *)(o + 0x218) == 1 && (p[0x2d] & 1) == 0)
                d3if_ShowTree(o, 1);
        } else {
            *(int *)(o + 0x220) = 0;
            *(int *)(o + 0x224) = 0;
        }
        break;
    case 1:
        sprintf(o, "ob_%s", ai_Goods + *p * 0x41 + 1);
        *(int *)(o + 0x220) = 4;
        break;
    }
    init_SceneNameToPrototype(o);
    *(unsigned short **)(o + 0x200) = p;
    if (*(int **)(o + 0x1ec) != 0)
        **(void (***)())(o + 0x1ec) = (void (*)())init_SceneObjectDestroyCB;
}

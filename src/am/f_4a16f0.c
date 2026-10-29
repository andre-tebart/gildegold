/* 0x4a16f0, registers the actions 0x01..0x3b (eval / decide / execute callbacks of this run and of am.c) */

int ai_Unk4a16f0(int a)
{
    AIACT act;

    AI_ZERO(&act, sizeof(act));
    act.id = 0x1;
    strcpy(act.name, "DummyUnversehrtheit");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk496030;
    act.exec = ai_AlwaysTrue;
    act.flags = 0x1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x2;
    strcpy(act.name, "DummyBeruf");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk496050;
    act.exec = ai_Unk496070;
    act.flags = 0x1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x3;
    strcpy(act.name, "DummyVergnuegen");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk496080;
    act.exec = ai_Unk4960a0;
    act.flags = 0x1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x4;
    strcpy(act.name, "Bildung");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk4960b0;
    act.exec = ai_Unk4965b0;
    act.flags = 0x1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x5;
    strcpy(act.name, "Bildung (Beruf)");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk496610;
    act.exec = ai_Unk496ab0;
    act.flags = 0x1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x6;
    strcpy(act.name, "DummyRechtschaffenheit");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk495fa0;
    act.exec = ai_Unk495fc0;
    act.flags = 0x1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x7;
    strcpy(act.name, "DummySicherheit");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk495fd0;
    act.exec = ai_Unk495ff0;
    act.flags = 0x1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x8;
    strcpy(act.name, "DummyFortpflanzung");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk496000;
    act.exec = ai_Unk496020;
    act.flags = 0x1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x9;
    strcpy(act.name, "Bestechung");
    act.eval = ai_Unk496b10;
    act.decide = ai_Unk496e40;
    act.exec = ai_Unk4972d0;
    act.flags = 0x19;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0xa;
    strcpy(act.name, "Amtsbewerbung");
    act.eval = ai_Unk4973f0;
    act.decide = ai_Unk497490;
    act.exec = ai_Unk497630;
    act.flags = 0x1b;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0xb;
    strcpy(act.name, "Objekt kaufen");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk497660;
    act.exec = ai_Unk4979d0;
    act.flags = 0x1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0xd;
    strcpy(act.name, "Objekt kaufen (AP)");
    act.eval = ai_Unk4a49a0;
    act.decide = sv_ReturnZero;
    act.exec = sv_ReturnZero;
    act.flags = 0x1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0xe;
    strcpy(act.name, "Raum kaufen");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk497af0;
    act.exec = ai_Unk497c80;
    act.flags = 0xff;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0xf;
    strcpy(act.name, "Repair Building");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk497cf0;
    act.exec = ai_Unk497ee0;
    act.flags = 0xff;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x10;
    strcpy(act.name, "Wohnsitz kaufen");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk497fa0;
    act.exec = ai_Unk498470;
    act.flags = 0x3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x11;
    strcpy(act.name, "Wohnsitz bauen");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk4984d0;
    act.exec = ai_Unk498a80;
    act.flags = 0x3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x12;
    strcpy(act.name, "Wohnsitz upgraden (Credits)");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk498b80;
    act.exec = sv_ReturnZero;
    act.flags = 0x3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x13;
    strcpy(act.name, "Betrieb bauen");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk498c70;
    act.exec = ai_Unk499290;
    act.flags = 0x3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x14;
    strcpy(act.name, "Betrieb upgraden (AP)");
    act.eval = ai_Unk4a49a0;
    act.decide = sv_ReturnZero;
    act.exec = sv_ReturnZero;
    act.flags = 0x3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x15;
    strcpy(act.name, "Betrieb upgraden (Credits)");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk499400;
    act.exec = sv_ReturnZero;
    act.flags = 0x3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x16;
    strcpy(act.name, "Medicus ausfsuchen");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk499520;
    act.exec = ai_Unk499580;
    act.flags = 0x3b;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x17;
    strcpy(act.name, "Sabotieren");
    act.eval = ai_Unk4995d0;
    act.decide = ai_Unk499660;
    act.exec = ai_Unk499a90;
    act.flags = 0x1c3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x18;
    strcpy(act.name, "Verpruegeln");
    act.eval = ai_Unk499b80;
    act.decide = ai_Unk499c10;
    act.exec = ai_Unk499f20;
    act.flags = 0x1c9;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x19;
    strcpy(act.name, "Buergerrecht");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49a020;
    act.exec = ai_Unk49a0a0;
    act.flags = 0x1b;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x1a;
    strcpy(act.name, "ApsSparen");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49a290;
    act.exec = ai_Unk49a380;
    act.flags = 0x1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x1b;
    strcpy(act.name, "UseObject (Military)");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49a390;
    act.exec = ai_Unk49a5b0;
    act.flags = 0x3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x1c;
    strcpy(act.name, "UseObject (Ansehen)");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49a690;
    act.exec = ai_Unk49aa80;
    act.flags = 0x19;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x1d;
    strcpy(act.name, "UseObject (APS)");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49ab60;
    act.exec = ai_Unk49ad80;
    act.flags = 0x39;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x1e;
    strcpy(act.name, "UseObject (Bildung)");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49ae50;
    act.exec = ai_Unk49b050;
    act.flags = 0x3b;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x1f;
    strcpy(act.name, "UseObject (Unversehrtheit)");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49b120;
    act.exec = ai_Unk49b2b0;
    act.flags = 0x3f;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x20;
    strcpy(act.name, "UseObject (Dynastie)");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49b380;
    act.exec = ai_Unk49b6f0;
    act.flags = 0x17;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x21;
    strcpy(act.name, "UseObject (Gemeinheit)");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49b7c0;
    act.exec = ai_Unk49bc50;
    act.flags = 0xc9;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x22;
    strcpy(act.name, "UseObject (Sicherheit)");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49bd90;
    act.exec = ai_Unk49c000;
    act.flags = 0x39;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x23;
    strcpy(act.name, "Use Parfuem");
    act.eval = ai_Unk49c0d0;
    act.decide = ai_Unk49c170;
    act.exec = ai_Unk49c190;
    act.flags = 0xff;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x24;
    strcpy(act.name, "Use Trank");
    act.eval = ai_Unk49c320;
    act.decide = ai_Unk49c3c0;
    act.exec = ai_Unk49c3e0;
    act.flags = 0xff;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x25;
    strcpy(act.name, "Spionage");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49c570;
    act.exec = ai_Unk49c700;
    act.flags = 0x69;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0xc;
    strcpy(act.name, "Objekt Kaufen (Markt)");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49c790;
    act.exec = ai_Unk49c880;
    act.flags = 0xff;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x26;
    strcpy(act.name, "Ablass Kaufen");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49c8e0;
    act.exec = ai_Unk49cb60;
    act.flags = 0x39;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x27;
    strcpy(act.name, "UseAmt");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49cd30;
    act.exec = ai_Unk49cdb0;
    act.flags = 0xff;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x28;
    strcpy(act.name, "Drohen");
    act.eval = ai_Unk49cdd0;
    act.decide = ai_Unk49ce50;
    act.exec = ai_Unk49d030;
    act.flags = 0x5d;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x29;
    strcpy(act.name, "Pamphlete");
    act.eval = ai_Unk49d050;
    act.decide = ai_Unk49d0f0;
    act.exec = ai_Unk49d280;
    act.flags = 0x5d;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x2a;
    strcpy(act.name, "Anschwaerzen");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49d2d0;
    act.exec = ai_Unk49d460;
    act.flags = 0x39;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x2b;
    strcpy(act.name, "Beleidigen");
    act.eval = ai_Unk49d4b0;
    act.decide = ai_Unk49d550;
    act.exec = ai_Unk49d5e0;
    act.flags = 0xc9;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x2c;
    strcpy(act.name, "Erpressen");
    act.eval = ai_Unk49d600;
    act.decide = ai_Unk49d6a0;
    act.exec = ai_Unk49d730;
    act.flags = 0xc9;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x2d;
    strcpy(act.name, "UsePrivs");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49d750;
    act.exec = ai_Unk49d7d0;
    act.flags = 0xff;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x2e;
    strcpy(act.name, "Buergermeister");
    act.eval = ai_Unk49d7f0;
    act.decide = ai_Unk49d8d0;
    act.exec = ai_Unk49dc10;
    act.flags = 0x39;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x2f;
    strcpy(act.name, "Baumeister");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49e7a0;
    act.exec = ai_Unk49ea90;
    act.flags = 0x3b;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x30;
    strcpy(act.name, "Buergermeister upgrade");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49dd90;
    act.exec = sv_ReturnZero;
    act.flags = 0x39;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x31;
    strcpy(act.name, "Buergermeister Rathaus");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49def0;
    act.exec = ai_Unk49e230;
    act.flags = 0x39;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x3c;
    strcpy(act.name, "Buergermeister Kerker");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49e350;
    act.exec = ai_Unk49e680;
    act.flags = 0x39;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x32;
    strcpy(act.name, "Dunkle Ecke");
    act.eval = ai_Unk49ec10;
    act.decide = ai_Unk49ed10;
    act.exec = ai_Unk49ee20;
    act.flags = 0x69;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x33;
    strcpy(act.name, "Kredit Nehmen");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49eec0;
    act.exec = ai_Unk49f370;
    act.flags = 0x7;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x34;
    strcpy(act.name, "Change Profession");
    act.eval = ai_Unk49f4f0;
    act.decide = ai_Unk49f5c0;
    act.exec = ai_Unk49f7f0;
    act.flags = 0x7;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x35;
    strcpy(act.name, "Stammtisch");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49f850;
    act.exec = ai_Unk49fb80;
    act.flags = 0x19;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x36;
    strcpy(act.name, "Zunft Preismanipulation");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk49fc90;
    act.exec = ai_Unk4a0040;
    act.flags = 0x3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x37;
    strcpy(act.name, "Zunft Aufruhr");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk4a0090;
    act.exec = ai_Unk4a01f0;
    act.flags = 0x43;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x38;
    strcpy(act.name, "Zunft Level 3");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk4a0890;
    act.exec = ai_Unk4a0940;
    act.flags = 0x4b;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x39;
    strcpy(act.name, "Upgrade Zunft");
    act.eval = ai_Unk4a0990;
    act.decide = ai_Unk4a0a70;
    act.exec = ai_Unk4a0e60;
    act.flags = 0x1b;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x3a;
    strcpy(act.name, "Berufsausbildung");
    act.eval = ai_Unk4a1000;
    act.decide = ai_Unk4a1170;
    act.exec = ai_Unk4a1360;
    act.flags = 0x1f;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x3b;
    strcpy(act.name, "Studium");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk4a13e0;
    act.exec = ai_Unk4a1640;
    act.flags = 0x13;
    return ai_Unk495520(&act, a) != 0;
}

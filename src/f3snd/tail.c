
int * ai_Unk760618;  /* 0x760618, the rain level */
/* ai_Selected: view of ai_Chars (tools/data_views.py) */
char snd_TimeCopy[14];  /* 0xf81240, a copy of ai_Time */
int snd_RainDrops;  /* 0xf81254, the "Tropfen_s" loop */
int snd_RainLightNear;  /* 0xf8125c, "LeichtNah_l_01" */
int snd_RainMediumNear;  /* 0xf81264, "MittelNah_l_01" */
int snd_RainHeavyNear;  /* 0xf8126c, "StarkNah_l_01" */
int snd_RainLightFar;  /* 0xf81274, "LeichtFern_l_01" */
int snd_RainMediumFar;  /* 0xf8127c, "MittelFern_l_01" */
int snd_RainHeavyFar;  /* 0xf81284, "StarkFern_l_01" */
float snd_VoiceVolume = 1.0f;  /* 0x602b08, voice volume (1.0, scaled by 127) */
char snd_GustNames[3][0x32] = { "Boehe_Leicht_s", "Boehe_Mittel_s", "Boehe_Stark_s" };  /* 0x602b10, "Boehe_Leicht_s", "Boehe_Mittel_s", "Boehe_Stark_s" */
G16SEASON snd_Gusts[] = { { 0.699999988f, 0.5f, 16000 }, { 0.300000012f, 0.200000003f, 27000 }, { 0.899999976f, 0.600000024f, 12000 }, { 0.5f, 0.400000006f, 20000 } };  /* 0x602ba8, by season */
G16AMB ai_Unk602bd8[][5] = { { { 0.899999976f, 0.699999988f, 2, 4000, 0 }, { 0.449999988f, 0.300000012f, 2, 7000, 0 }, { 0.400000006f, 0.5f, 2, 30000, 0 }, { 0.5f, 0.600000024f, 0, 27000, 0 }, { 0.600000024f, 0.600000024f, 0, 20000, 0 } }, { { 0.699999988f, 0.699999988f, 2, 5000, 0 }, { 0.899999976f, 0.699999988f, 4, 5000, 0 }, { 0.699999988f, 0.5f, 2, 20000, 0 }, { 0.600000024f, 0.600000024f, 0, 25000, 0 }, { 0.899999976f, 0.600000024f, 0, 17000, 0 } }, { { 0.300000012f, 0.25f, 1, 17000, 0 }, { 0.200000003f, 0.200000003f, 1, 18000, 0 }, { 0.800000012f, 0.5f, 2, 12000, 0 }, { 0.300000012f, 0.600000024f, 0, 20000, 0 }, { 0.600000024f, 0.600000024f, 0, 20000, 0 } }, { { 0.150000006f, 0.200000003f, 1, 30000, 0 }, { 0.0f, 0.0f, 1, 500000, 0 }, { 0.200000003f, 0.400000006f, 2, 23000, 0 }, { 0.200000003f, 0.5f, 0, 22000, 0 }, { 0.400000006f, 0.5f, 0, 20000, 0 } } };  /* 0x602bd8, per season: birds, crickets, chickens, horses, dogs */
float snd_MusicVolume = 0.5f;  /* 0x602d68, music volume 0..1: 0 = no music ( */
G16TRACKS ai_Unk602d70[] = { { 7, 0, -1, { "cd1\\LichtUndGnade.mp3", "cd1\\Moenchsgesang.mp3", "cd1\\Sakral.mp3", "cd2\\AmKuehlenGrabe.mp3", "", "" } }, { 2, 23, -1, { "cd1\\AufDemBall.mp3", "", "", "", "", "" } }, { 2, 20, -1, { "cd1\\FinsterePlaene.mp3", "cd1\\RuestetEuch.mp3", "cd1\\FinstererPriester.mp3", "", "", "" } }, { 2, 19, -1, { "cd1\\Rittersleut.mp3", "cd1\\Burgfraeulein.mp3", "", "", "", "" } }, { 2, 21, -1, { "cd2\\DerSeuchenzug.mp3", "cd1\\ImDunklenTann.mp3", "", "", "", "" } }, { 2, 24, -1, { "cd1\\AufDemBall.mp3", "", "", "", "", "" } }, { 2, 22, -1, { "cd1\\finsterePlaene.mp3", "", "", "", "", "" } }, { 22, 0, -1, { "cd1\\AufspielZumTanz.mp3", "cd1\\ZurSommerzeit.mp3", "cd2\\KraeuterUndPhiolen.mp3", "", "", "" } }, { 19, 0, -1, { "cd1\\RuestetEuch.mp3", "cd1\\ImDunklenTann.mp3", "cd1\\Flucht.mp3", "cd1\\Rittersleut.mp3", "", "" } }, { 4, 0, -1, { "cd1\\FinsterePlaene.mp3", "cd1\\Flucht.mp3", "cd2\\ImKerker.mp3", "cd1\\Hungerlohn.mp3", "", "" } }, { 16, 0, -1, { "cd1\\FinsterePlaene.mp3", "cd1\\flucht.mp3", "cd1\\Hungerlohn.mp3", "cd1\\ImDunklenTann.mp3", "", "" } }, { 11, 0, -1, { "cd1\\BeiDerQuelle.mp3", "cd1\\ImDunklenTann.mp3", "", "", "", "" } }, { 13, 0, 2, { "cd1\\Immerkalt.mp3", "", "", "", "", "" } }, { 13, 0, 3, { "cd1\\Immerkalt.mp3", "", "", "", "", "" } }, { 13, 0, -1, { "cd1\\Hungerlohn.mp3", "", "", "", "", "" } }, { 12, 0, 2, { "cd1\\Immerkalt.mp3", "", "", "", "", "" } }, { 12, 0, 3, { "cd1\\Immerkalt.mp3", "", "", "", "", "" } }, { 12, 0, -1, { "cd1\\Hungerlohn.mp3", "", "", "", "", "" } }, { 28, 0, -1, { "", "", "", "", "", "" } }, { 14, 0, -1, { "cd2\\DasParfuem.mp3", "cd2\\DerFernhaendler.mp3", "cd2\\KraeuterUndPhiolen.mp3", "", "", "" } }, { 8, 0, -1, { "cd2\\KraeuterUndPhiolen.mp3", "cd2\\DasParfuem.mp3", "cd2\\DerFernhaendler.mp3", "", "", "" } }, { 6, 0, -1, { "cd2\\ArmerDelinquent.mp3", "cd2\\ImKerker.mp3", "", "", "", "" } }, { 18, 0, -1, { "cd1\\DieTaeglichMueh.mp3", "cd1\\Hungerlohn.mp3", "cd2\\AufDemMarkt.mp3", "", "", "" } }, { 18, 0, 2, { "cd1\\ImmerKalt.mp3", "cd1\\DieTaeglichMueh.mp3", "cd1\\Hungerlohn.mp3", "cd2\\AufDemMarkt.mp3", "", "" } }, { 18, 0, 3, { "cd1\\ImmerKalt.mp3", "cd1\\DieTaeglichMueh.mp3", "cd1\\Hungerlohn.mp3", "cd2\\AufDemMarkt.mp3", "", "" } }, { 20, 0, -1, { "cd1\\DieTaeglichMueh.mp3", "cd1\\Hungerlohn.mp3", "cd2\\AufDemMarkt.mp3", "", "", "" } }, { 20, 0, 2, { "cd1\\ImmerKalt.mp3", "cd1\\DieTaeglichMueh.mp3", "cd1\\Hungerlohn.mp3", "cd2\\AufDemMarkt.mp3", "", "" } }, { 20, 0, 3, { "cd1\\ImmerKalt.mp3", "cd1\\DieTaeglichMueh.mp3", "cd1\\Hungerlohn.mp3", "cd2\\AufDemMarkt.mp3", "", "" } }, { 21, 0, -1, { "cd1\\DieTaeglichMueh.mp3", "cd1\\Hungerlohn.mp3", "cd2\\AufDemMarkt.mp3", "", "", "" } }, { 21, 0, 2, { "cd1\\ImmerKalt.mp3", "cd1\\DieTaeglichMueh.mp3", "cd1\\Hungerlohn.mp3", "cd2\\AufDemMarkt.mp3", "", "" } }, { 21, 0, 3, { "cd1\\ImmerKalt.mp3", "cd1\\DieTaeglichMueh.mp3", "cd1\\Hungerlohn.mp3", "cd2\\AufDemMarkt.mp3", "", "" } }, { 5, 0, -1, { "cd2\\DerFernhaendler.mp3", "cd2\\ZumGutenEnd.mp3", "cd1\\mauerundtor.mp3", "", "", "" } }, { 9, 0, -1, { "cd2\\DerFernhaendler.mp3", "cd2\\ZumGutenEnd.mp3", "cd1\\mauerundtor.mp3", "", "", "" } }, { 15, 0, -1, { "cd1\\AufDemBall.mp3", "cd2\\Hochzeit.mp3", "cd1\\mauerundtor.mp3", "", "", "" } }, { 23, 0, -1, { "cd1\\AufDemBall.mp3", "cd2\\Hochzeit.mp3", "", "", "", "" } }, { 24, 0, -1, { "cd1\\AufDemBall.mp3", "cd2\\Hochzeit.mp3", "", "", "", "" } }, { 25, 0, -1, { "cd1\\AufDemBall.mp3", "cd2\\Hochzeit.mp3", "", "", "", "" } }, { 26, 0, -1, { "cd1\\AufDemBall.mp3", "cd2\\Hochzeit.mp3", "", "", "", "" } }, { 29, 0, -1, { "CD3\\DuestresGemaeuer.mp3", "CD3\\DunkleMondesnacht.mp3", "CD2\\AmKuehlenGrabe.mp3", "CD1\\FinstrerPriester.mp3", "CD3\\Grabesklang.mp3", "CD2\\DerSeuchenzug.mp3" } }, { 31, 0, -1, { "CD3\\DerStolzenTraum.mp3", "CD3\\Vagabundentanz.mp3", "CD3\\SchmerzlichErinnerung.mp3", "CD1\\ImDunklenTann.mp3", "CD1\\BeiDerQuelle.mp3", "CD1\\AufspielZumTanz.mp3" } }, { 32, 0, -1, { "CD3\\MitFlinkemStich.mp3", "CD3\\Medley.mp3", "CD2\\DasParfum.mp3", "CD2\\DerFernhaendler.mp3", "CD2\\Hochzeit.mp3", "CD1\\AufDemBall.mp3" } }, { 30, 587, -1, { "CD2\\ZumGutenEnd.mp3", "cd2\\DasParfuem.mp3", "cd2\\KraeuterUndPhiolen.mp3", "CD2\\DerFernhaendler.mp3", "", "" } }, { 30, 598, -1, { "CD3\\BurgUndFeste.mp3", "CD3\\Heldenglanz.mp3", "CD2\\Krieg.mp3", "CD2\\Satisfaktion.mp3", "CD1\\AufDenStrassen.mp3", "CD1\\FinsterePlaene.mp3" } }, { 30, 614, -1, { "CD2\\ArmerDelinquent.mp3", "CD2\\DerSeuchenzug.mp3", "CD1\\FinstererPriester.mp3", "CD1\\FinsterePlaene.mp3", "CD1\\AufDemBall.mp3", "CD3\\Heldenglanz.mp3" } }, { 0, 0, -1, { "", "", "", "", "", "" } } };  /* 0x602d70 */
G16LOC snd_LocationTracks[] = { { { 7, 0, 0, 0 }, "", -20, 0, 0 }, { { 2, 0, 0, 0 }, "", -20, 0, 0 }, { { 22, 0, 0, 0 }, "", -20, 0, 0 }, { { 19, 0, 0, 0 }, "", -20, 0, 0 }, { { 4, 0, 0, 0 }, "", -20, 0, 0 }, { { 16, 0, 0, 0 }, "", -20, 0, 0 }, { { 11, 0, 0, 0 }, "", -20, 0, 0 }, { { 13, 0, 0, 0 }, "", -20, 0, 0 }, { { 12, 0, 0, 0 }, "", -20, 0, 0 }, { { 28, 0, 0, 0 }, "", -20, 0, 0 }, { { 14, 8, 0, 0 }, "", -20, 0, 0 }, { { 6, 0, 0, 0 }, "", -20, 0, 0 }, { { 18, 20, 21, 0 }, "", -20, 0, 0 }, { { 5, 0, 0, 0 }, "", -20, 0, 0 }, { { 9, 0, 0, 0 }, "", -20, 0, 0 }, { { 15, 0, 0, 0 }, "", -20, 0, 0 }, { { 23, 0, 0, 0 }, "", -20, 0, 0 }, { { 24, 0, 0, 0 }, "", -20, 0, 0 }, { { 25, 0, 0, 0 }, "", -20, 0, 0 }, { { 26, 0, 0, 0 }, "", -20, 0, 0 }, { { 29, 0, 0, 0 }, "", -20, 0, 0 }, { { 31, 0, 0, 0 }, "", -20, 0, 0 }, { { 32, 0, 0, 0 }, "", -20, 0, 0 }, { { 30, 0, 0, 0 }, "", -20, 0, 0 }, { { 9876, 0, 0, 0 }, "", -20, 0, 0 }, { { 0, 0, 0, 0 }, "", 0, 0, 0 } };  /* 0x6094d0 */
unsigned int ai_Unk60b0d8 = 1;  /* 0x60b0d8, start of the music pause */
char snd_TrackText[256] = {0};  /* 0x6f0760, a debug text */
char ai_Unk6f0860[256] = {0};  /* 0x6f0860, a debug text */
char ai_Unk6f0960[256] = {0};  /* 0x6f0960, a debug text */
char ai_Unk6f0a60[256] = {0};  /* 0x6f0a60, a debug text */
G16AMBIENTON snd_AmbientOn = {0};  /* 0x6f0b60, ambient sounds on */
unsigned int snd_AmbientOnSaved = 0;  /* 0x6f0b64, the saved snd_AmbientOn.on */
int snd_QueuePaused = 0;  /* 0x6f0b68, nonzero: the queue does not advance */
G16TRACKHANDLES snd_TrackHandles = {0};  /* 0x6f0b6c, the music track and its copy */
char * snd_SpecialTrack = 0;  /* 0x6f0b74, the special track of */
int snd_AmbientSaved = 0;  /* 0x6f0b78, 1 while the ambient flag is off and saved ( */
int snd_FanfareFade = 0;  /* 0x6f0b7c, 1 while a "Fanfare" has faded the music out */
int snd_AmbientTrack = 0;  /* 0x6f0b80, the d3snd handle of the ambient track (bird song, market ambience) */
G16SND * snd_Queue = 0;  /* 0x6f0b84, the queue (the first queued sound) */
int snd_QueueTime = 0;  /* 0x6f0b88, time of the last queue change */
int snd_Season = 0;  /* 0x6f0b8c, the season */
unsigned int snd_LastGust = 0;  /* 0x6f0b90, time of the last gust */
int ai_Unk6f0b94 = 0;  /* 0x6f0b94, hpLocustsPerm */
int ai_Unk6f0b98 = 0;  /* 0x6f0b98, hpLocusts */
int ai_Unk6f0b9c = 0;  /* 0x6f0b9c, the season */
void * ai_Unk6f0ba0 = 0;  /* 0x6f0ba0, the atmosphere mp3 (MSXTRACK *) */
char snd_LastTrack[256] = {0};  /* 0x6f0ba8, the outdoor track played last */
unsigned char * ai_Unk6f0ca8 = 0;  /* 0x6f0ca8, the building of the track */
int ai_Unk6f0cac = 0;  /* 0x6f0cac, the room of the track */
unsigned int ai_Unk6f0cb0 = 0;  /* 0x6f0cb0, the pause length */
int ai_Unk6f0cb4 = 0;  /* 0x6f0cb4, the season of the music */
unsigned int ai_Unk6f0cb8 = 0;  /* 0x6f0cb8, time of the last track decision */
int snd_VoiceBankOrders = 0;  /* 0x6f0cbc, voice bank: orders (snd_LoadBuildingVoices) */
int snd_VoiceBankClick = 0;  /* 0x6f0cc0, voice bank: click comments */
int snd_VoiceBankNoise = 0;  /* 0x6f0cc4, voice bank: click noises */
int snd_VoiceBankGreeting = 0;  /* 0x6f0ccc, voice bank: greeting */
int snd_Bank = 0;  /* 0x6f2404, the sample bank */
int snd_MusicEnabled = 0;  /* 0x6f2578, the "msx" setting of game.INI [Sound]: music enabled */
int snd_SfxEnabled = 0;  /* 0x6f257c, the "sfx" setting of game.INI [Sound]: effects and voices on */

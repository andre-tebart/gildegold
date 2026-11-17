/* aip.h: the public prototypes and data of unit aip, the AI's social action callbacks (0x4a8760..0x4ac140); poker.h
   includes it where these prototypes stood before the split. */
#ifndef AIP_H
#define AIP_H

int ai_DecideOfficePartner(int a, struct AICHAR *p, int *ctx, int d, struct AIARG *m);   /* 0x4a8760 */
int ai_DecideTableValue(int a, AIPER *p, int *ctx, int d, struct AIARG *m);           /* 0x4a8a50 */
int ai_DecidePartnerOtherSex(int a, struct AICHAR *p, int c, int d, struct AIARG *m);              /* 0x4a8ad0 */
int ai_EvalOneInFour(void);                                                          /* 0x4a8bf0 */
int ai_AlwaysTrue(void);                                                          /* 0x4a8c10 */
int ai_EvalOwnTimeSlot(int a, AIPER *p);                                               /* 0x4a8c20 */
int ai_DecideTwoNearChars(int a, AIPER *p, int c, int d, struct AIARG *m);              /* 0x4a8c80 */
int ai_DecideNearChar(int a, AIPER *p, int c, int d, struct AIARG *m);              /* 0x4a8d20 */
int ai_DecideBuildingResident(int a, AIPER *p, int c, int d, struct AIARG *m);              /* 0x4a8db0 */
int ai_EvalTitleGroup7(int a, AIPER *p);                                               /* 0x4a8ee0 */
int ai_DecideUnfriendlyPair(int a, AIPER *p, int c, int d, struct AIARG *m);              /* 0x4a8f20 */
int ai_EvalGraveCount(int a, AIPER *p, int c, AIARG *m);                            /* 0x4a9030 */
int ai_DecideAcquaintance(int a, AICHAR *p, int c, int d, AIARG *m);                     /* 0x4a9080 */
int ai_EvalOwnYearTurn(int a, AICHAR *p);                                                /* 0x4a9150 */
int ai_DecideSuitor(int a, AICHAR *p, int c, int d, AIARG *m);                 /* 0x4a91a0 */
int ai_DecideCloseAcquaintance(int a, AICHAR *p, int c, int d, AIARG *m);  /* 0x4a9280 */
int ai_DecideOfficeAcquaintance(int a, AICHAR *p, int c, int d, AIARG *m);  /* 0x4a9350 */
int ai_DecideAcceptedChar(int a, AIPER *p, int c, int d, struct AIARG *m);              /* 0x4a9420 */
int ai_DecideUnflaggedChar(int a, AIPER *p, int c, int d, struct AIARG *m);              /* 0x4a94e0 */
int ai_DecideFarChar(int a, AIPER *p, int c, int d, struct AIARG *m);              /* 0x4a95d0 */
int ai_DecideCloseChar(int a, AIPER *p, int c, int d, struct AIARG *m);              /* 0x4a9660 */
int ai_DecideColumn4Officer(int a, AICHAR *p, int *ctx, int d, AIARG *m);  /* 0x4a9700 */
int ai_DecideColumn5Officer(int a, AICHAR *p, int *ctx, int d, AIARG *m);  /* 0x4a9880 */
extern int ai_LeisureActions[19];                                                     /* 0x600e10, the leisure actions */
int ai_DecideLeisureAction(int a, struct AIPER *p, int c, struct AIARG *in, struct AIARG *m);  /* 0x4a9a00 */
int ai_CheckLeisureAction(struct AIPER *p, struct AIARG *in, struct AIARG *out);              /* 0x4a9fa0 */
extern int ai_VisitActions[3];                                                      /* 0x600e5c */
int ai_DecideVisitAction(int a, struct AIPER *p, int c, struct AIARG *in, struct AIARG *m);  /* 0x4aa120 */
int ai_CheckVisitAction(struct AICHAR *p, struct AIARG *in, struct AIARG *out);              /* 0x4aa430 */
int ai_PickPamphletTarget(struct AIPER *p, int c, struct AIARG *m);                             /* 0x4aa580 */
int ai_DecideLittleKnownChar(struct AICHAR *p, struct AIARG *in, struct AIARG *m);                 /* 0x4aa6a0 */
int ai_DecideCharPair(struct AIPER *p, int c, struct AIARG *m);                             /* 0x4aa800 */
int aip_ExecPamphlet(struct AIPER *p, int c, struct AIARG *m);                             /* 0x4aa940 */
int aip_ExecDrohen(struct AIPER *p, int c, struct AIARG *m);                             /* 0x4aaa10 */
int aip_ExecAnschwaerzen(struct AIPER *p, int c, struct AIARG *m);                             /* 0x4aaae0 */
int ai_TrustedOfTwo(AIPER *p, AIPER *b, AIPER *c, int *k);                          /* 0x4aab90 */
int ai_TrustedOfFour(unsigned char k, AIPER *p, int *ids, int *out);                           /* 0x4aad30 */
int ai_FindDarkCornerTarget(struct AIPER *p, struct AIARG *a, struct AIARG *b);                  /* 0x4aaef0 */
int ai_ExecMessage74(struct AIPER *p, struct AIARG *a, struct AIARG *b);                          /* 0x4aafb0 */
int ai_DecideBuyDunkleEcke(struct AIPER *p, struct AIARG *a, struct AIARG *b);                               /* 0x4ab080 */
int ai_ExecBuyDunkleEcke(struct AIPER *p, struct AIARG *a, struct AIARG *b);                               /* 0x4ab2c0 */
int ai_DecideBlutrache(int a, AIPER *p, int c, struct AIARG *m, struct AIARG *m2);  /* 0x4ab3e0 */
int ai_ExecBlutrache(AIPER *p, struct AIARG *m, struct AIARG *m2);                 /* 0x4ab6b0 */
int ai_DecideVerfluchen(int a, AIPER *p, int c, struct AIARG *m);                     /* 0x4ab7f0 */
int ai_ExecVerfluchen(AIPER *p, struct AIARG *m);                                    /* 0x4ab9f0 */
int ai_DecideTheater(int a, AIPER *p, int c, struct AIARG *m);                     /* 0x4abac0 */
int ai_ExecTheater(struct AIPER *p, struct AIARG *m);                                    /* 0x4abc60 */
int ai_DecideEhrerbietung(int a, struct AIPER *p, int c, struct AIARG *m);                     /* 0x4abd00 */
int ai_ExecEhrerbietung(struct AIPER *p, struct AIARG *m);                                    /* 0x4abe60 */
int ai_RegisterSocialActions(int a);                                                         /* 0x4abed0 */
#endif

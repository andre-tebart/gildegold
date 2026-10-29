/* unk_4b39f0.c -- missions ("_AUFTRAEGE_VERGABE_HS_%.2d", "special\mission"): the running missions
   ai_Missions, the mission types ai_MissionTypes (.data right after hst.c's literals), an own random generator.
   Unit start and name are guesses (open boundary). Names are ours.
   unit of rea 8cef7d5b (src/game/unk_4b39f0.c); boundaries in decl/text.dcl, notes in src/unk_4b39f0/open.md */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unk_4b39f0.h"
#include "di_input.h"


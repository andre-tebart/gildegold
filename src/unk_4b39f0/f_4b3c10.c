/* 0x4b3c10, gives owner a random mission of kind */

G13MIS *ai_NewRandomMission(int owner, int kind, int x)
{
    return ai_NewMission(owner, ai_RandomMissionType(kind), x);
}

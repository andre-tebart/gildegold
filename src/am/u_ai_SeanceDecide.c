/* unlinked: the "Use Seance" decide callback returns 0; LINK folds it (/OPT:ICF) into sv_ReturnZero, 0x52a040,
   which is where the original's reference in ai_Unk490c10 points */
int ai_SeanceDecide(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    return 0;
}

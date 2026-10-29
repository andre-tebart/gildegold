/* unlinked: the "Use Seance" execute callback returns 0; LINK folds it (/OPT:ICF) into sv_ReturnZero, 0x52a040,
   which is where the original's reference in ai_Unk490c10 points */
int ai_SeanceExec(G5CH *p, AIARG *s1, AIARG *s2)
{
    return 0;
}

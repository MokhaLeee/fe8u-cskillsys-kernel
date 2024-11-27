/**
 * Quintessance stealing effect functions
 * Disabled some externs because of compile warnings about "useless storage class specifier in empty declaration"
 */
extern const struct ProcCmd ProcScr_QuintessenceFx[];
//extern struct QuintessenceFxProc;
extern void fe7j_sub_80781C8(u16 * buf, s16 phase, s16 amplitude, s16 frequency, s16 param_5, s16 param_6, s16 param_7);
extern u16 * fe7j_sub_8077CEC(int buf_id, int scanline);
extern void QuintessenceFx_OnHBlank(void);
//extern struct Proc08C01654;
//extern void fe7j_sub_80120D8(struct Proc08C01654 * proc);
//extern void fe7j_sub_80120E0(struct Proc08C01654 * proc);
extern const struct ProcCmd ProcScr_FE7J_08C01654;
extern void fe7j_sub_8012150(int a, int b, int c, int d, int e, ProcPtr parent);
extern void FE7J_EventE5_Sim_Type_A(ProcPtr proc);
extern void FE7J_EventE5_Sim_Type_B(ProcPtr proc);
extern void fe7j_sub_80121FC(void);
//extern void QuintessenceFx_ParallelWorker(struct QuintessenceFxProc * proc);
//extern void QuintFxBg2_Init(struct QuintessenceFxProc * proc);
//extern void QuintFxBg2_Loop(struct QuintessenceFxProc * proc);
extern const struct ProcCmd ProcScr_QuintessenceFxBg2Scroll;
//extern void QuintessenceFx_Init_Main(struct QuintessenceFxProc * proc);
//extern void QuintessenceFx_Loop_A(struct QuintessenceFxProc * proc);
//extern void QuintessenceFx_ResetBlend(struct QuintessenceFxProc * proc);
//extern void QuintessenceFx_Loop_B(struct QuintessenceFxProc * proc);
//extern void QuintessenceFx_Loop_C(struct QuintessenceFxProc * proc);
extern void QuintessenceFx_OnEnd(void);
extern void StartQuintessenceStealEffect(struct Proc * parent);
extern void QuintessenceFx_Goto_B(void);
extern void QuintessenceFx_Goto_C(void);
extern void EndQuintessenceStealEffect(void);

extern void GiveScroll(void);

extern void IsTraineeLevelCappedOrPromoted(void);
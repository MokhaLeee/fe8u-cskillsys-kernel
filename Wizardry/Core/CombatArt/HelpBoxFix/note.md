- RText on draw
StartHelpBoxExt
gProcScr_HelpBox
StartHelpBoxTextInit(proc->item, proc->mid)
gUnknown_08A01650
sub_8089F58

其利用 proc->item 获得 item-attribute, 并据此产生 HB_EXTINFO_NONE

struct ProcHelpBoxIntro：
    + 58: proc->item 序号
    + 5C: MSG ()
    + 64: 行数, 通过 sub_8089F58 配置

修改策略：
1. 在 info->populate 中配置 proc->item 序号于 msg
2. GetHelpBoxItemInfoKind 中为 proc->item 添加一个新的配置 ×
3. sub_8089F58 填入新行

需要用户 DIY 的策略
info->populate 配置 msg 与 sHelpBoxType


StartHelpBoxExt
HelpBox_OnLoop

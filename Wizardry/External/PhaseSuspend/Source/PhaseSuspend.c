#include <common-chax.h>

const char msg_uiconfig_name_suspend[] = "Suspend\x1F\0";
const char msg_uiconfig_desc_suspend[] = "Only save suspend at player phase\x1F\0";

LYN_REPLACE_CHECK(PlayerPhase_Suspend);
void PlayerPhase_Suspend(void) {}

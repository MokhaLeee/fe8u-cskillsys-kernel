.equiv oThreadInfo_sub_thread_running, 0x00
.equiv oThreadInfo_sub_thread_state, 0x01
.equiv oThreadInfo_sub_thread_break_reason, 0x02
.equiv oThreadInfo_main_thread_sp, 0x04
.equiv oThreadInfo_sub_thread_sp, 0x08
.equiv oThreadInfo_func, 0x0C

.equiv DEFAULT_SUBTHREAD_RUNNING_MODE, 0x00
.equiv INVALID_SUBTHREAD_RUNNING_MODE, 0xFF

.equiv SUBTHRED_BREAK_FROM_IRQ, 0
.equiv SUBTHRED_BREAK_FROM_NORMAL, 1

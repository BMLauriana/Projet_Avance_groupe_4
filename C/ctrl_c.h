#ifndef __CTRLC_H__
#define __CTRLC_H__

#include <signal.h>


extern volatile sig_atomic_t g_ctrl_c;

void install_ctrl_c_handler(void);

#endif


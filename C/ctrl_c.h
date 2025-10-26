#ifndef __CTRL_C_H__
#define __CTRL_C_H__

#include <signal.h>

extern volatile sig_atomic_t interrompre;

void install_ctrl_c_handler(void);

#endif /* __CTRL_C_H__ */

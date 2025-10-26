#include <signal.h>
#include "ctrl_c.h"


volatile sig_atomic_t interrompre = 0;

static void handler_signal(int sig) {
    (void)sig;
    interrompre = 1;
}

void install_ctrl_c_handler(void) {
    signal(SIGINT, handler_signal);
}

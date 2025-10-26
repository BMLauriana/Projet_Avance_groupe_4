#include <string.h>
#include <signal.h>
#include "ctrl_c.h"

volatile sig_atomic_t g_ctrl_c = 0;

static void on_sigint(int sig) {
    (void)sig;
    g_ctrl_c = 1;
}


void install_ctrl_c_handler(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = on_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
}

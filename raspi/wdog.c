#include "raspihw.h"
#include "bcm2835.h"

void wdog_start(unsigned int timeout)
{
    PM_WDOG = PM_PASSWORD | (timeout & PM_WDOG_TIME_MASK); // watchdog timer = timer clock / 16; need password (31:16) + value (11:0)
    PM_RSTC = PM_PASSWORD | (PM_RSTC & PM_RSTC_WRCFG_CLR) | PM_RSTC_WRCFG_FULL_RESET;
}

void wdog_stop()
{
    PM_RSTC = PM_PASSWORD | PM_RSTC_RESET;
}

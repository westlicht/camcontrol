#include "qpn_port.h"                                       /* QP-nano port */
#include "bsp.h"                             /* Board Support Package (BSP) */
#include "pelican.h"                               /* application interface */

/*..........................................................................*/
static QEvent  l_pelicanQueue[2];
static QEvent  l_pedQueue[1];

/* QF_active[] array defines all active object control blocks --------------*/
QActiveCB const Q_ROM Q_ROM_VAR QF_active[] = {
    { (QActive *)0,           (QEvent *)0,    0                     },
    { (QActive *)&AO_Pelican, l_pelicanQueue, Q_DIM(l_pelicanQueue) },
    { (QActive *)&AO_Ped,     l_pedQueue,     Q_DIM(l_pedQueue)     }
};

/* make sure that the QF_active[] array matches QF_MAX_ACTIVE in qpn_port.h */
Q_ASSERT_COMPILE(QF_MAX_ACTIVE == Q_DIM(QF_active) - 1);

/*..........................................................................*/
void main (void) {
    Pelican_ctor();                          /* instantiate the  Pelican AO */
    Ped_ctor();                              /* instantiate the  Ped     AO */

    BSP_init();                                     /* initialize the board */

    QF_run();                                /* transfer control to QF-nano */
}

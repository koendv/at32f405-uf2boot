#include "at32f402_405.h"
#include "at32f402_405_qspi.h"
#include "qspi1.h"

void wk_qspi1_init(void);

void qspi1_init(void) {
    /* enable qspi gpio pins */
    crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
    /* enable qspi1 periph clock */
    crm_periph_clock_enable(CRM_QSPI1_PERIPH_CLOCK, TRUE);
    /* enable qspi1 */
    wk_qspi1_init();
}


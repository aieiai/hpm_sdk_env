
#include "tx_api.h"
#include "usb_osal.h"

#include <stdio.h>
#include "board.h"
#include "hpm_gpio_drv.h"
#include "usb_config.h"


#define task1_PRIORITY          (TX_MAX_PRIORITIES - 5U)
#define TX_MEM_POOL_SIZE        (16U * 1024U)
#define LED_FLASH_PERIOD_IN_MS  (300)
#define HAL_HID_KBD_REPORT_MAX  (32U)

extern void hid_keyboard_init(uint8_t busid, uint32_t reg_base);
extern void hid_keyboard_test(uint8_t busid);

TX_BYTE_POOL tx_byte_pool;
static UCHAR tx_memory_area[TX_MEM_POOL_SIZE] __attribute__((aligned(8)));

static void task1(void *pvParameters)
{
    (void)pvParameters;
    printf("[cherryusb hid keyboard with freertos sample]: task started.\n");

    while (1) {
        hid_keyboard_test(0);
    }
}

/* Define what the initial system looks like.  */
void tx_application_define(void *first_unused_memory)
{
    (void)first_unused_memory;
    
    tx_byte_pool_create(&tx_byte_pool, "tx byte pool", tx_memory_area, TX_MEM_POOL_SIZE);

    if (usb_osal_thread_create("task1", 8192U, task1_PRIORITY, task1, NULL) == NULL) {
        perror("Task1 creation failed!.\n");
        for (;;) {
            ;
        }
    }
}
int main(void)
{
    board_init();
    board_init_led_pins();
    board_init_gpio_pins();
    gpio_set_pin_input(BOARD_APP_GPIO_CTRL, BOARD_APP_GPIO_INDEX, BOARD_APP_GPIO_PIN);
    board_timer_create(LED_FLASH_PERIOD_IN_MS, board_led_toggle);

    printf("cherry usb threadx hid_keyboard device sample.\n");

    hid_keyboard_init(0, CONFIG_HPM_USBD_BASE);

    /* Enter the ThreadX kernel.  */
    tx_kernel_enter();

    for (;;) {
        ;
    }
    return 0;
}


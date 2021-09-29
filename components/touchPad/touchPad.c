#include "touchPad.h"

#include <stdio.h>

#include "driver/touch_pad.h"

#define TAG "touch_driver"
#define TOUCHPAD_FILTER_TOUCH_PERIOD (10)
EventGroupHandle_t eventGroup = NULL;
EventBits_t touch_bit;

int64_t timestamp = 0;

void touchCallback(void *arg)
{
    uint32_t pad_intr = touch_pad_get_status();
    //clear interrupt
    touch_pad_clear_status();
    if (eventGroup != NULL)
    {
        bool touch = pad_intr & (1 << TOUCH_PAD_NUM6);

        BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
        // ESP_LOGI(TAG, "Touch pad status: %d", pad_intr);
        int64_t now = esp_timer_get_time();
        if (touch && (now - timestamp > 100000)) //.1 sec debounce
        {
            timestamp = now;
            xEventGroupSetBitsFromISR(eventGroup, touch_bit, &pxHigherPriorityTaskWoken);
        }
    }
}

esp_err_t touchpad_init(EventGroupHandle_t *eventgroup, EventBits_t eventBits)
{
    eventGroup = *eventgroup;
    touch_bit = eventBits;

    esp_err_t err;

    err = touch_pad_init();

    if (err != ESP_OK)
        return err;

    touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER);
    // Set reference voltage for charging/discharging
    // For most usage scenarios, we recommend using the following combination:
    // the high reference valtage will be 2.7V - 1V = 1.7V, The low reference voltage will be 0.5V.
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
    // Init touch pad IO
    // tp_example_touch_pad_init();
    touch_pad_config(TOUCH_PAD_NUM6, 0);
    // Initialize and start a software filter to detect slight change of capacitance.
    touch_pad_filter_start(TOUCHPAD_FILTER_TOUCH_PERIOD);

    uint16_t touch_value, threshold;

    touch_pad_read_filtered(TOUCH_PAD_NUM6, &touch_value);
    threshold = (uint16_t)touch_value * 4. / 5.;
    ESP_LOGI(TAG, "test init: touch pad [%d] val is %d, %d", TOUCH_PAD_NUM6, touch_value, threshold);
    //set interrupt threshold.

    ESP_ERROR_CHECK(touch_pad_set_thresh(TOUCH_PAD_NUM6, threshold));

    touch_pad_isr_register(touchCallback, NULL);
    return touch_pad_intr_enable();
}
esp_err_t touch_deinit()
{
    touch_pad_intr_disable();
    touch_pad_isr_deregister(touchCallback, NULL);

    touch_pad_deinit();

    return ESP_OK;
}

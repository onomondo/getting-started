
#include "esp_err.h"
#include "esp_types.h"
#include "lwip/sockets.h"
#include "esp_event.h"
#include "esp_timer.h"

#ifdef GEN_1_DEVICE
#define RX_PIN 26
#define TX_PIN 27
#define CTS_PIN 2
#define RTS_PIN 1
#elif GEN_2_DEVICE
#define RX_PIN 16
#define TX_PIN 17
#define CTS_PIN 18
#define RTS_PIN 19
#else
#define RX_PIN 9
#define TX_PIN 10
#define CTS_PIN -1
#define RTS_PIN -1
#endif

ESP_EVENT_DECLARE_BASE(CELLULAR_EVENT);

enum CELLULAR_EVENTS
{
    CELLULAR_PPP_STARTED,
    CELLULAR_PPP_STOPPED,
    CELLULAR_ATTACHED,
    CELLULAR_STOPPED_SEARCHING,
    CELLULAR_SOCKET_READY,
    CELLULAR_NOT_AVAILABLE,
    CELLULAR_POWERED_DOWN,
    CELLULAR_SEARCHING

};

#define TIMER_PERIOD 1000000 // period of the timer event source in microseconds

esp_err_t initCellular();

esp_err_t openSocket(char *host, int port);

esp_err_t cellular_set_event_handler(esp_event_handler_t handler, int32_t event_id, void *handler_args);

esp_err_t clearFPLMMN();

esp_err_t sendData(char *data, int len, int timeout);

esp_err_t closeSocket(void);

esp_err_t killandclean();

int getSignalQuality();

esp_err_t forcePowerDown();

esp_err_t dnsLookup(const char *host);

esp_err_t requestPPP();

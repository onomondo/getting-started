
#include "esp_err.h"
#include "esp_types.h"
#include "lwip/sockets.h"

#ifdef GEN_1_DEVICE
#define RX_PIN 26
#define TX_PIN 27
#define CTS_PIN 2
#define RTS_PIN 1
#else
#define RX_PIN 16
#define TX_PIN 17
#define CTS_PIN 18
#define RTS_PIN 19
#endif

enum supportedModems {
    SIM800,
    SIM7xxx
};

//maybe...
struct cellular_config {
};

// Supported modems: SIM800, SIM7xxx(TODO). A full modem init should be issues at first boot (TODO).
esp_err_t initCellular(enum supportedModems modem, bool fullModemInit);

// handle internal
esp_err_t openSocket(char* host, int port);

esp_err_t sendData(char* data, int len, int timeout);

esp_err_t closeSocket(void);

esp_err_t detachAndPowerDown();

esp_err_t killandclean();

int getSignalQuality();

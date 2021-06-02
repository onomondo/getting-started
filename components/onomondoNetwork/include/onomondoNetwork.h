
#include "esp_err.h"
#include "esp_types.h"
#include "lwip/sockets.h"

enum supportedModems {
    SIM800,
    SIM7xxx
};

//maybe...
struct cellular_config {
};

esp_err_t initCellular(enum supportedModems modem);

// handle internal
esp_err_t openSocket(char* host, int port);

esp_err_t sendData(char* data, int len, int timeout);

esp_err_t closeSocket(void);

esp_err_t detachAndPowerDown();

esp_err_t killandclean();

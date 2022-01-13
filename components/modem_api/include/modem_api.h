// port of lib_at to ESP IDF

#ifndef _MODEM_API_H
#define _MODEM_API_H

// includes
#include "esp_err.h"

// defines
#define MAX_NETWORK_NAME_LENGTH 20

// event IDs
enum
{ // declaration of the specific events under the NETWORK_EVENT_BASE
    ATTACHED,
    REGISTRATION_DENIED,
    PPP_INITIALIZED,
    PPP_DROPPED,
    UNKNOWN_STATE
};

struct modem_status_t
{
    char[MAX_NETWORK_NAME_LENGTH];
    uint8_t ppp_mode_active;
    uint8_t attached;
}

void
modem_api_init(esp_event_base_t event_base);

void modem_api_get_status

#endif
#ifndef _LIB_AT_PARSER_H
#define _LIB_AT_PARSER_H

#define AT_HEAD "AT"
#define AT_HEAD_SIZE 2
#define AT_CMS_ERROR "+CMS ERROR:"
#define AT_CMS_ERROR_SIZE 11
#define AT_HEADER_SIZE 15
#define AT_STORAGE_SIZE 50

typedef int32_t lib_at_parser_resp;
enum
{
    AT_DONE,
    AT_URC,
    AT_UNKNOWN,
    AT_PARTIAL, //parser for command expects more data. I.e. AT+COPS=?
    AT_ERROR
};

typedef lib_at_parser_resp (*at_cmd_cb)(char *l, uint16_t *len);

typedef struct
{
    char *cmdString;
    uint32_t timeout;
    at_cmd_cb cb;

} lib_at_cmd;

void parse(char *resp, int len, lib_at_cmd *atcmd);

#endif
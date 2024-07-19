/*
 * server.c
 *
 *  Created on: 2024��6��10��
 *      Author: ssfxfss
 */

#include "server.h"

#include <stdlib.h>
#include <string.h>

#include "eth_driver.h"
#include "rpc_cjson.h"
#include "rpc_fun.h"
#include "motor.h"
#include "application_pca9685.h"

char TxBuffer1[BufSize];

rpc_handle_t rpc;

static char *uvm_strstr(const char *p1, int len, const char *p2)
{
    if (p1 == NULL || p2 == NULL)
        return NULL;
    if (*p2 == '\0' || *p1 == '\0')
        return (char *)p1;

    char *s1 = NULL;
    char *s2 = NULL;
    char *current = (char *)p1;

    for (; *current != '\0'; current++)
    {
        if (len == 0)
            return NULL;

        s1 = current;
        s2 = (char *)p2;

        while ((*s1 != '\0') && (*s2 != '\0') && (*s1 == *s2))
        {
            s1++;
            s2++;
        }

        if (*s2 == '\0')
            return current;
        if (*s1 == '\0')
            return NULL;

        len--;
    }
    return NULL;
}

#define HTTP_HEADER_BUF_SIZE 128

void server_routine(char *rx_buf, size_t len)
{
    char *body = uvm_strstr(rx_buf, len, "\r\n\r\n");
    if (body != NULL) {
        body += 4;    // "\r\n\r\n"
        len = len - (rx_buf - body);
    } else
    {
        if(rx_buf[0] == 0x33)
        {
            printf("recv config\r\n");
            config_handle(rx_buf, TxBuffer1);
            return;
        }
        printf("no a http request.\r\n");
        len = 0;
    }

    char *jresp = rpc_process(&rpc, body, len);
    if (jresp == NULL) {
        printf("fall to create resp!");
    }
    int jresp_len = strlen(jresp);
    memset(TxBuffer1, 0, sizeof(TxBuffer1));
    sprintf(TxBuffer1,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json; charset=UTF-8\r\n"
            "Accept: application/json\r\n"
            "Content-Length: %d\r\n\r\n%s ",
            jresp_len, jresp);
    free(jresp);
}

#undef HTTP_HEADER_BUF_SIZE

void server_init(void)
{
    uvm_motor_init(&default_params);
    rpc_add_all_handler(&rpc);
}

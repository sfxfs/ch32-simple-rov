/*
 * server.h
 *
 *  Created on: 2024��6��10��
 *      Author: ssfxfss
 */

#ifndef USER_SERVER_H_
#define USER_SERVER_H_

#include "debug.h"

#define BufSize    2048

extern char TxBuffer1[BufSize];

void server_routine(char *rx_buf, size_t len);
void server_init(void);

#endif /* USER_SERVER_H_ */

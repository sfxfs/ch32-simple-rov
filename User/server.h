/*
 * server.h
 *
 *  Created on: 2024Äê6ÔÂ10ÈÕ
 *      Author: ssfxfss
 */

#ifndef USER_SERVER_H_
#define USER_SERVER_H_

#include "debug.h"

#define BufSize    1024

extern char TxBuffer1[BufSize];
extern char RxBuffer1[BufSize];

void server_routine(size_t len);
void server_init(void);

#endif /* USER_SERVER_H_ */

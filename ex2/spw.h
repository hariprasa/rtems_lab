

#ifndef __SPW_H__
#define __SPW_H__

#include <stdint.h>     // uint8_t

#define NODE_ADR_RX 5   // an integer in the range 0 to 255
//#define NODE_ADR_TX 22

/******************************************************************************/

int spw_setup(char *dev_name);

void spw_close(int fd);

void rasta_spw_receive(uint8_t *videoFrame, int fd);

void rasta_spw_sendback(char *buffer, int fd);


void* spw_linux_init(void* hDevice, int trans_link);

void spw_linux_close(void* hDevice, int trans_link);

void linux_spw_receive(void* hDevice, int trans_link, uint8_t *videoFrame, 
						int width, int height);

void linux_spw_sendback(void* hDevice, int trans_link, char *buffer, int bpl, 
								int height);

/******************************************************************************/
#endif /* __SPW_H__ */



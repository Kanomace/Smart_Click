/**
 * @file	: xmodem.c
 * @author	: MIoT
 * @brief	:
 * @version : 0.1
 * @date	: 2019-09-10
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "miio_define.h" /* include macro USER_OTA_ENABLE */

#if (defined(USER_OTA_ENABLE) && USER_OTA_ENABLE)

#include "arch_dbg.h"
#include "crc16.h"
#include "miio_api.h"
#include "xmodem.h"
#include "arch_ota.h"

#undef	TAG
#define TAG "xmodem"
/* for ota debug */
#define DEBUG_OTA 0
/* Xmodem flag definition */
#define SOH		(0x01)
#define STX		(0x02)
#define EOT		(0x04)
#define ACK		(0x06)
#define NAK		(0x15)
#define CAN		(0x18)
#define CTRLZ	(0x1A)

/* Xmodem definition */
#define XMODEM_MTU				(128)
#define XMODEM_HEADER_POS		(0)
#define XMODEM_PACKNO_POS		(1)
#define XMODEM_DATA_POS			(3)
#define XMODEM_CRC_POS			(131)
/* Xmodem-1k definition */
#define XMODEM_1K_MTU			(1024)
#define XMODEM_1K_HEADER_POS	(0)
#define XMODEM_1K_PACKNO_POS	(1)
#define XMODEM_1K_DATA_POS		(3)
#define XMODEM_1K_CRC_POS		(1027)

#define XMODEM_RECV_TIMEOUT		(1 * 100)//100ms
#define XMODEM_INTERVAL_MS		(100)
#define TRANS_RETRY_MAX			(20)
#define PACK_RETRY_MAX			(10)
#define CRC_RETRY_MAX			(10)

/* =================================================================== */
/* ========================= miio xmodem start ======================= */
/* =================================================================== */


static miio_uart_t *miio_uart;

#define XMODEM_OK			MIIO_OK
#define XMODEM_ERR			MIIO_ERROR
#define XMODEM_PARAM_ERR	MIIO_ERROR_PARAM


#define xmodem_printf		printf

#define XMODEM_LOG_INFO_TAG(TAG, fmt, ...) \
	do { \
		LOG_INFO_TAG(TAG, fmt, ##__VA_ARGS__); \
	}while(false);
	
#define XMODEM_LOG_ERR_TAG(TAG, fmt, ...) \
		do { \
			LOG_ERROR_TAG(TAG, fmt, ##__VA_ARGS__); \
		}while(false);

static inline int xmodem_recv_byte(unsigned char *c, int timeout_ms)
{
	return miio_uart->recv_byte(miio_uart, c, timeout_ms);
}

static int xmodem_recv_str(unsigned char *str, int timeout_ms)
{
	return miio_uart->xmodem_recv_str(miio_uart, str, timeout_ms);
}

static inline int xmodem_send_byte(unsigned char c)
{
	return miio_uart->send_byte(miio_uart, c);
}

static inline int xmodem_send_str(const char *str)
{
	return miio_uart->send_str(miio_uart, str);
}


static unsigned short crc16_ccitt(const unsigned char* buf, int len)
{
	unsigned short crc = 0;
	while (len--) {
		int i;
		crc ^= *buf++ << 8;

		for (i = 0; i < 8; ++i) {
			if (crc & 0x8000)
				crc = (crc << 1) ^ 0x1021;
			else
				crc = crc << 1;
		}
	}
	return crc;
}

static void xmodem_print_data(miio_xmodem_t *x)
{
	unsigned char *p = x->xbuff;
	p += XMODEM_DATA_POS;
	xmodem_printf("xmodem data : ");
	for(int i = 0;i < XMODEM_MTU;i++){
		xmodem_printf("%x ", *p++);
	}
	xmodem_printf("\r\n");
}

static void xmodem_print_header_info(miio_xmodem_t *x)
{
	unsigned char *pbuf = x->xbuff;

	switch(x->type){

		case XMODEM : {
			LOG_INFO_TAG(TAG, "==================== xmodem info[hex] ====================");
			LOG_INFO_TAG(TAG, "\thead : %x", pbuf[XMODEM_HEADER_POS]);
			LOG_INFO_TAG(TAG, "\tnum(~num) : %x(%x)", pbuf[XMODEM_PACKNO_POS], pbuf[XMODEM_PACKNO_POS + 1]);
			LOG_INFO_TAG(TAG, "\tcrc : %x%x", pbuf[XMODEM_CRC_POS], pbuf[XMODEM_CRC_POS + 1]);
		}

		break;
		case XMODEM_1K : {
			LOG_INFO_TAG(TAG, "=================== xmodem-1k info[hex] ===================");
			LOG_INFO_TAG(TAG, "\thead : %x", pbuf[XMODEM_1K_HEADER_POS]);
			LOG_INFO_TAG(TAG, "\tnum(~num) : %x(%x)", pbuf[XMODEM_1K_PACKNO_POS], pbuf[XMODEM_1K_PACKNO_POS + 1]);
			LOG_INFO_TAG(TAG, "\tcrc : %x%x", pbuf[XMODEM_1K_CRC_POS], pbuf[XMODEM_1K_CRC_POS + 1]);
		}
		break;

		default :
		break;
	}
}

static inline bool xmodem_check_packno(miio_xmodem_t *x, uint8_t packno)
{
	if(NULL == x) {
		return false;
	}
	unsigned char *pbuf = x->xbuff;
	uint8_t no = 0, no_r = 0;

	switch(x->type) {
		case XMODEM :
		case XMODEM_1K : {
			no = pbuf[XMODEM_PACKNO_POS];
			no_r = pbuf[XMODEM_PACKNO_POS + 1];
		}
		break;

		default:
		break;
	}

	return (((packno == no) || (packno == no + 1)) && (((no + no_r) & 0xff) == 0xff));
}

static inline bool xmodem_check_crc(miio_xmodem_t *x, uint16_t crc)
{
	if(NULL == x){
		return false;
	}

	unsigned char *pbuf = x->xbuff;
	uint8_t crc_h = 0, crc_l = 0;

	switch(x->type){

		case XMODEM : {
			crc_h = pbuf[XMODEM_CRC_POS];
			crc_l = pbuf[XMODEM_CRC_POS + 1];
		}
		break;

		case XMODEM_1K : {
				crc_h = pbuf[XMODEM_1K_CRC_POS];
			crc_l = pbuf[XMODEM_1K_CRC_POS + 1];
		}
		break;

		default :
		break;
	}

	return (crc == ((((uint16_t)crc_h << 8) & 0xff00) | ((uint16_t)crc_l & 0x00ff)));
}

static void xmodem_print_packno_error(miio_xmodem_t *x, uint8_t packno)
{
	if(NULL == x)
		return;

	unsigned char *pbuf = x->xbuff;

	switch(x->type){

		case XMODEM :
		case XMODEM_1K : {
			LOG_INFO_TAG(TAG, "packno error : %u[cnt], %u[recv]", packno, pbuf[1]);
		}
		break;

		default : 
		break;
	}
}

static void xmodem_print_crc_error(miio_xmodem_t *x, uint16_t crc)
{
	if(NULL == x)
		return;

	unsigned char *pbuf = x->xbuff;

	switch(x->type){
		case XMODEM : {
			LOG_INFO_TAG(TAG, "crc error : %x[cnt], %x%x[recv]", crc, pbuf[XMODEM_CRC_POS], pbuf[XMODEM_CRC_POS + 1]);
		}
		break;
		case XMODEM_1K : {
			LOG_INFO_TAG(TAG, "crc error : %x[cnt], %x%x[recv]", crc, pbuf[XMODEM_1K_CRC_POS], pbuf[XMODEM_1K_CRC_POS + 1]);
		}
		break;
		default : 
		break;
	}
}

static unsigned int xmodem_cal_data_len(unsigned char *xbuff)
{
	unsigned char pos = XMODEM_CRC_POS - 1;

	// reverse find the byte before final 0xFF
	while(pos >= XMODEM_DATA_POS && xbuff[pos] == 0xFF) pos--;

	if(pos == XMODEM_DATA_POS - 1 || pos == XMODEM_CRC_POS - 1) return XMODEM_MTU;
	else if(xbuff[pos] == CTRLZ) return pos - XMODEM_DATA_POS;
	else return XMODEM_MTU;
}

int miio_xmodem_create_instance(miio_xmodem_t *xmodem, miio_uart_t *uart)
{
	int ret = XMODEM_OK;

	if(NULL == uart || NULL == xmodem){
		XMODEM_LOG_ERR_TAG(TAG, "uart or xmodem pointer is NULL");
		return XMODEM_PARAM_ERR;
	}

	miio_uart = uart;

	return ret;
}

void miio_xmodem_destroy(miio_xmodem_t *x)
{
	LOG_INFO_TAG(TAG, "destroy xmodem instance");
	return;
}
/* =================================================================== */
/* ========================= miio xmodem end ========================= */
/* =================================================================== */

int xmodem_recv_data(miio_xmodem_t *x)
{

#if DEBUG_OTA	/* for linux debug */
	int fd = open("./ota_data.txt", O_CREAT|O_RDWR, S_IRWXU | S_IXGRP | S_IROTH | S_IXOTH);
#endif

	int trans_retry = 0;
	int pack_retry = 0;
	int crc_retry = 0;
	uint8_t pack_no = 0;
	unsigned char *xbuff = NULL;
	size_t sum_data_bytes = 0;
	int data_len = 0;
	xbuff = (unsigned char *)arch_malloc(134);//x->xbuff;
	if (xbuff == NULL) {
		XMODEM_LOG_INFO_TAG(TAG, "ota failed malloc buffer error\n");
		return XMODEM_ERR;
	}
	// send "C" : support CRC
	// timeout after 3S
	xmodem_send_str("C");

	int n_read = 0;
	uint8_t fist_package = 1;
	int lwrite = 0;
	while (true) {
		arch_msleep(XMODEM_INTERVAL_MS);
		
		n_read = xmodem_recv_str(x->xbuff, XMODEM_RECV_TIMEOUT);
		pack_no++;
		
		if(++trans_retry > TRANS_RETRY_MAX && n_read <= 0) {
			XMODEM_LOG_ERR_TAG(TAG, "xmodem transmit failed, retry %d\n", trans_retry);
			xmodem_send_byte(CAN);
			xmodem_send_byte(CAN);
			xmodem_send_byte(CAN);

			return XMODEM_ERR;
		}
		trans_retry--;	/* important */

#if XMODEM_PRINT_LOG
		xmodem_print_header_info(x);
#endif

		unsigned char c = x->xbuff[0];
		switch (c) {

			case SOH : {
				x->type = XMODEM;
				trans_retry = 0;
				goto data_check;
			}
//			break;

			case STX : {
				x->type = XMODEM_1K;
				trans_retry = 0;
				goto data_check;
			}
//			break;

			case EOT : {
				/* reach normal end */
				trans_retry = 0;
				
				xmodem_send_byte(ACK);
				data_len = xmodem_cal_data_len(xbuff);
#if DEBUG_OTA	/* for linux debug */
				lwrite = write(fd, xbuff + XMODEM_DATA_POS, data_len);
				LOG_INFO_TAG(TAG, "ota bin write: %d", lwrite);
				close(fd);
#endif
				sum_data_bytes += data_len;
				XMODEM_LOG_INFO_TAG(TAG, "xmodem recv %u[cur] %u[sum]", data_len, sum_data_bytes);
				arch_ota_func(xbuff, data_len, sum_data_bytes);
			
				
				XMODEM_LOG_INFO_TAG(TAG, "ota success");
				return MIIO_OK;
			}
//			break;

			case CAN : {
				trans_retry = 0;
				xmodem_recv_byte(&c, 100);
				/* canceled by remote */
				if (c == CAN) {
					XMODEM_LOG_INFO_TAG(TAG, "ota canceled");
					xmodem_send_byte(ACK);

					return XMODEM_OK;
				}
			}
			break;

			default: {
				if(++trans_retry > TRANS_RETRY_MAX) {
					XMODEM_LOG_INFO_TAG(TAG, "xmodem transmit failed");
					XMODEM_LOG_ERR_TAG(TAG, "xmodem transmit failed, retry %d\n", trans_retry);
					xmodem_send_byte(CAN);
					xmodem_send_byte(CAN);
					xmodem_send_byte(CAN);

					return XMODEM_ERR;
				} else {
					//trans_retry++;
					XMODEM_LOG_ERR_TAG(TAG, "xmodem transmit error, retry %d\n", trans_retry);
					pack_no--;
					xmodem_send_byte(NAK);
					continue;
				}
			}
//			break;
		}

data_check:

		if (!xmodem_check_packno(x, pack_no)) {

			pack_retry++;

			if (pack_retry >= PACK_RETRY_MAX) {
				xmodem_send_byte(CAN);
				xmodem_send_byte(CAN);
				xmodem_send_byte(CAN);

				XMODEM_LOG_INFO_TAG(TAG, "pack error retry max");
				XMODEM_LOG_INFO_TAG(TAG, "ota failed");

				return XMODEM_ERR;

			} else {
				xmodem_print_packno_error(x, pack_no);
				pack_no--;
				xmodem_send_byte(NAK);

				continue;
			}
			
		}
		else {
			pack_retry = 0;
		}

		unsigned short ccrc = crc16_ccitt(&(x->xbuff[3]), 128);
		
		if (!xmodem_check_crc(x, ccrc)) {

			crc_retry++;

			if (crc_retry > 10) {
				xmodem_send_byte(CAN);
				xmodem_send_byte(CAN);
				xmodem_send_byte(CAN);

				XMODEM_LOG_INFO_TAG(TAG, "crc error retry max");
				XMODEM_LOG_INFO_TAG(TAG, "ota failed");

				return XMODEM_ERR;
			}
			else {
				xmodem_print_crc_error(x, ccrc);

				pack_no--;
				xmodem_send_byte(NAK);
				continue;
			}
		}
		else {
			crc_retry = 0;
		}

#if XMODEM_PRINT_LOG
		xmodem_print_data(x);
#endif
		if (fist_package == 1) {
			fist_package = 0;
		} else {
		// count recv data len
			data_len = XMODEM_MTU;

#if DEBUG_OTA	/* for linux debug */
		write(fd, xbuff + XMODEM_DATA_POS, data_len);
#endif
		
		sum_data_bytes += data_len;
		XMODEM_LOG_INFO_TAG(TAG, "xmodem recv %u[cur] %u[sum]", data_len, sum_data_bytes);

			arch_ota_func(xbuff, data_len, sum_data_bytes);
		}
		arch_memcpy(xbuff, x->xbuff, 133);
		/* received normal data */
		xmodem_send_byte(ACK);
	}
	
//	return XMODEM_ERR;
}

#endif /* (defined(USER_OTA_ENABLE) && USER_OTA_ENABLE) */

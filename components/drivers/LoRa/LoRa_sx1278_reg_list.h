/*
 * Copyright (c) 2006-2019,
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           	Author       		Notes
 * 2019-01-22     Mingfen XIAO       	the first version
 */
 #ifndef _LORA_SX1278_REG_LIST_H__
 #define _LORA_SX1278_REG_LIST_H__

/*common register in the FSK/OOK and LoRa Mode*/
#define SX1278_COM_REG_Fifo                 (0x00) /*reset value and default value is 0x00*/
#define SX1278_COM_REG_OpMode               (0x01) /*reset value and default value is 0x01*/
#define SX1278_COM_REG_FrfMsb               (0x06) /*reset value and default value is 0x6C*/
#define SX1278_COM_REG_FrfMid               (0x07) /*reset value and default value is 0x80*/
#define SX1278_COM_REG_FrfLsb               (0x08) /*reset value and default value is 0x00*/
#define SX1278_COM_REG_PaConfig             (0x09) /*reset value and default value is 0x4F*/
#define SX1278_COM_REG_PaRamp               (0x0A) /*reset value and default value is 0x09*/
#define SX1278_COM_REG_Ocp            		(0x0B) /*reset value and default value is 0x2B*/
#define SX1278_COM_REG_Lna		            (0x0C) /*reset value and default value is 0x20*/






/*register map in the LongRangeMode for FSK/OOK*/
#define SX1278_FSK_REG_BitrateMsb           (0x02) /*reset value and default value is 0x1A*/
#define SX1278_FSK_REG_BitrateLsb       	(0x03) /*reset value and default value is 0x0B*/
#define SX1278_FSK_REG_FdevMsb          	(0x04) /*reset value and default value is 0x00*/
#define SX1278_FSK_REG_FdevLsb          	(0x05) /*reset value and default value is 0x52*/
#define SX1278_FSK_REG_RxConfig          	(0x0D) /*reset value is 0x08, default value is 0x00*/
#define SX1278_FSK_REG_RssiConfig          	(0x0E) /*reset value is 0x02, default value is 0x80*/






/*register map in the LongRangeMode for LoRa*/
#define SX1278_LORA_REG_FifoAddrPtr         (0x0D) /*reset value is 0x08, default value is 0x00*/
#define SX1278_LORA_REG_RssiConfig          (0x0E) /*reset value is 0x02, default value is 0x80*/


 

#endif


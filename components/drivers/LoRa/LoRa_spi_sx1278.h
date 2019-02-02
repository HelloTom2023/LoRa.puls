/*
 * Copyright (c) 2006-2019,
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           	Author       		Notes
 * 2019-01-22     Mingfen XIAO       	the first version
 */
#ifndef __LORA_SPI_SX1278_H__
#define __LORA_SPI_SX1278_H__

#include <rtthread.h>
#include <rtdevice.h>
#include "LoRa_sx1278_cmd_list.h"
#include "LoRa_sx1278_reg_list.h"
#include "stm32f10x.h" /*there is not specifiation,in components can not include macl head files.*/

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifdef RT_USING_LORA_DEBUG
#define LoRa_printf(fmt, ...)   rt_kprintf(fmt, ##__VA_ARGS__)
#else
#define	LoRa_printf(fmt, ...)
#endif

typedef struct LoRa_Device_Port_Config_tag
{
    uint32_t RCC_Periph;
    GPIO_TypeDef * GPIOx;
    uint16_t GPIO_Pin;
}LoRa_Device_Port_Config;


typedef struct Lora_Device_tag
{
    struct rt_device                lora_device;
    struct rt_spi_device          	rt_spi_device;
    struct rt_mutex                 lock;
    LoRa_Device_Port_Config 		Reset_port;
    LoRa_Device_Port_Config 		Spi_Cs_port;
    void *                          user_data;
}Lora_Device;





#endif

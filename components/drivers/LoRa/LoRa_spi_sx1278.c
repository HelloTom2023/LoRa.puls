/*
 * Copyright (c) 2006-2019, 
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           	Author       		Notes
 * 2019-01-22     Mingfen XIAO       	the first version
 */



#ifndef __LORA_SPI_SX1278_C__
#define __LORA_SPI_SX1278_C__

#include <stdint.h>
#include "LoRa_spi_sx1278.h"
/*
 * Pin mapping table
 *
 * Modules types : Ai-Thinker Ra-01
 * Index	PinName		Maping2Mcu
 * 	1 		GND
 * 	2 		3v3
 * 	3		RST				PG6
 * 	4		NSS				PG7
 * 	5		SCLK			PB13/SPI2_SCK
 * 	6		MOSI			PB15/SPI2_MOSI
 * 	7		MISO			PB14/SPI2_MISO
 * 	8		DIO0			PG8
 *
 */
Lora_Device spi_lora_sx1278_device;
LoRa_Device_Port_Config sx1278_reset_port = {
    RCC_APB2Periph_GPIOG,
    GPIOG,
    GPIO_Pin_6
};

LoRa_Device_Port_Config sx1278_spi_cs_port = {
    RCC_APB2Periph_GPIOG,
    GPIOG,
    GPIO_Pin_7
};
/*
* RT-Thread device initiation function
* Initiation IO related sx1278
* 
*/
static rt_err_t LoRa_sx1278_init(rt_device_t dev)
{
    /*configuration reset pin*/
    {
        LoRa_Device_Port_Config sx1278_reset_port = spi_lora_sx1278_device.Reset_port;
        
        GPIO_InitTypeDef GPIO_InitStructure;

		/*Initiation NRESET pin RCC*/
		RCC_APB2PeriphClockCmd(sx1278_reset_port.RCC_Periph, ENABLE);

		/*Initiation NRESET port and mode*/
		GPIO_InitStructure.GPIO_Pin = sx1278_reset_port.GPIO_Pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
		GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
		GPIO_Init(sx1278_reset_port.GPIOx, &GPIO_InitStructure);
		GPIO_SetBits(sx1278_reset_port.GPIOx, sx1278_reset_port.GPIO_Pin); 	/*enable NERESET pin*/
    }

    return RT_EOK;
}

/*
* RT-Thread device open function
* 
*/
static rt_err_t  LoRa_sx1278_open(rt_device_t dev, rt_uint16_t oflag)
{
	return RT_EOK;
}

/*
* RT-Thread device close function
* 
*/
static  rt_err_t  LoRa_sx1278_close(rt_device_t dev)
{
	return RT_EOK;
}

/*
* RT-Thread device read function
* 
*/
static rt_size_t LoRa_sx1278_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
	rt_size_t ret = 0x00;
	return ret;
}

/*
* RT-Thread device write function
* 
*/
static rt_size_t LoRa_sx1278_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
	rt_size_t ret = 0x00;
	return ret;
}

/*
* RT-Thread device control function
* 
*/
static rt_err_t  LoRa_sx1278_control(rt_device_t dev, int cmd, void *args)
{
	return RT_EOK;
}

/*
* sx1278 reset function
* flag : ture is reset operation
*         flase is not reset operation
*
*/
static rt_err_t LoRa_device_sx1278_reset(rt_device_t dev)
{
	LoRa_Device_Port_Config sx1278_reset_port = spi_lora_sx1278_device.Reset_port; /*get the reset port parameters*/

	/*Reset SX1278 device*/
	GPIO_ResetBits(sx1278_reset_port.GPIOx, sx1278_reset_port.GPIO_Pin); 	/*NRESET pin setting to Low*/

	/*wait 1ms(>=100us)*/
	rt_thread_mdelay(1);/*delay 1ms*/

	GPIO_SetBits(sx1278_reset_port.GPIOx, sx1278_reset_port.GPIO_Pin);/*NRESET pin setting to High*/
	/*wait 6ms(>=5ms)*/
	rt_thread_mdelay(10);/*delay 10ms*/
}

/*
* Device initiation function
*  
*/
rt_err_t LoRa_device_sx1278_init(const char * LoRa_device_name, const char * spi_bus_name)
{
	rt_err_t ret = RT_EOK;
    struct rt_spi_device *rt_spi_device;

    /* initialize mutex */
    /*not understand this function */
    #if 0
    if (rt_mutex_init(&spi_lora_sx1278_device.lock, spi_bus_name, RT_IPC_FLAG_FIFO) != RT_EOK)
    {
        LoRa_printf("init lock mutex failed\r\n");
        return -RT_ENOSYS;
    }
    #endif

    /*find device in the device list*/
    rt_spi_device = (struct rt_spi_device *)rt_device_find(LoRa_device_name);
    if(rt_spi_device == RT_NULL)
    {
    	rt_device_t spi_bus;
		struct rt_spi_bus *bus;
        LoRa_printf("LoRa spi device %s not found!, shall be attach to system...\r\n", LoRa_device_name);

        /* get physical spi bus */
		spi_bus = rt_device_find(spi_bus_name);
		if (spi_bus != RT_NULL && spi_bus->type == RT_Device_Class_SPIBUS)
		{

			spi_lora_sx1278_device.rt_spi_device.bus = (struct rt_spi_bus *)spi_bus;
			spi_lora_sx1278_device.rt_spi_device.config.data_width = 8;
			spi_lora_sx1278_device.rt_spi_device.config.max_hz = 1;
			spi_lora_sx1278_device.rt_spi_device.config.mode = RT_SPI_MODE_0 | RT_SPI_MSB | RT_SPI_MASTER;

			ret = rt_spi_bus_attach_device(&spi_lora_sx1278_device.rt_spi_device, LoRa_device_name, spi_bus_name, (void*)&sx1278_spi_cs_port);
			if (ret != RT_EOK)
			{
				LoRa_printf("LoRa device[%s] register failed !\r\n",LoRa_device_name);
				return ret;
			}
			else
			{
				LoRa_printf("LoRa device[%s] register successful !\r\n",LoRa_device_name);
			}
		}
		else /*not found the spi bus*/
		{
			return -RT_ERROR;
		}
    }

    spi_lora_sx1278_device.Reset_port = sx1278_reset_port;

    /*configuration SPI bus*/
    {
		struct rt_spi_configuration spi_cfg;

		/*find device again in the device list*/
		rt_spi_device = (struct rt_spi_device *)rt_device_find(LoRa_device_name);
		if (!rt_spi_device)
		{
			LoRa_printf("Find again : LoRa device %s not found!\r\n", LoRa_device_name);
			return -RT_ERROR;
		}

		spi_cfg.data_width = 8;
		spi_cfg.max_hz = 1;
		spi_cfg.mode = RT_SPI_MODE_3 | RT_SPI_MSB | RT_SPI_MASTER;

		LoRa_printf("LoRa device type is 0x%x\r\n", rt_spi_device->bus->lock.parent.parent.type);

		rt_spi_configure(rt_spi_device,&spi_cfg);
    }

    /*Initiation sx1278 io and reset sx1278*/
    {
        /*Initiation sx1278 GPIO*/
        LoRa_sx1278_init(&spi_lora_sx1278_device.lora_device);  
    
        /*Reset SX1278 device*/
        LoRa_device_sx1278_reset(&spi_lora_sx1278_device.lora_device);
    }

    /*test spi communication is ok.*/
    {
    	unsigned char SendData[2] = {SX1278_COM_REG_FrfMsb,0x00};
		unsigned char RecData[2] = {0x00};
		rt_spi_transfer(rt_spi_device, &SendData, &RecData, 2);
		if(0x6C != RecData[1])
		{
			LoRa_printf("LoRa sx1278 initialization failed.\r\n ");
		}
		else
		{
			LoRa_printf("LoRa sx1278 initialization successful.\r\n ");
		}
    }
}

int rt_hw_sx1278_init(void)
{
	LoRa_device_sx1278_init("LoRa","spi2");
}


//INIT_BOARD_EXPORT(rt_hw_sx1278_init);
MSH_CMD_EXPORT(rt_hw_sx1278_init, rt_hw_sx1278_init);

#endif


/*
 * Copyright (c) 2006-2018,
 *
 * the sample base on STM32F103ZE
 *
 * Change Logs:
 * Date           Author      	 Notes
 * 2019-01-05     Mingfen.XIAO   first version
 *
 */
#include "stm32f10x_spi_bus.h"
#include <stm32f10x.h>
/*
 * spi1 hardware initiation base on stm32f10x family mcu
 * */
static void stm32f10x_spi1_bus_HwInit(Enum_SpiModeType mode)
{

}
/*
 * spi2 hardware initiation base on stm32f10x family mcu
 * */
static void stm32f10x_spi2_bus_HwInit(Enum_SpiModeType mode)
{
	/*Input parameters check*/
	if((SPI_MODE_MASTER != mode) && (SPI_MODE_SLAVE != mode))
	{
		/*Reserved, shall add assertion in future*/
	}
	else
	{
		/*Doing nothing*/
	}

	/*configure spi2 clock*/
	{
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE );
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE );
	}

	/*Configure spi2 port and pin*/
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		if(SPI_MODE_MASTER == mode)
		{
			/*
			 * In the master mode:
			 * NSS : Output,This pin has a device to configure.
			 * SCK : Output
			 * MISO : Input
			 * MOSI : Output
			 * */
			GPIO_InitStructure.GPIO_Pin = SPI2_GPIO_SCK | SPI2_GPIO_MOSI | SPI2_GPIO_MISO;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;
			GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
			GPIO_Init(SPI2_GPIO, &GPIO_InitStructure);
			GPIO_SetBits(SPI2_GPIO,SPI2_GPIO_SCK | SPI2_GPIO_MOSI | SPI2_GPIO_MISO);/*setting the pin to high*/

			/*GPIO_InitStructure.GPIO_Pin = SPI2_GPIO_MISO;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;
			GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
			GPIO_Init(SPI2_GPIO, &GPIO_InitStructure);*/

		}
		else if(SPI_MODE_SLAVE == mode)
		{
			/*
			 * In the Slave mode:
			 * NSS : Input
			 * SCK : Input
			 * MISO : Output
			 * MOSI : Input
			 * */
			GPIO_InitStructure.GPIO_Pin = SPI2_GPIO_MOSI;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;
			GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
			GPIO_Init(SPI2_GPIO, &GPIO_InitStructure);
			GPIO_SetBits(SPI2_GPIO,SPI2_GPIO_SCK | SPI2_GPIO_MOSI);/*setting the pin to high*/

			GPIO_InitStructure.GPIO_Pin = SPI2_GPIO_SCK | SPI2_GPIO_NSS | SPI2_GPIO_MISO;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;
			GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
			GPIO_Init(SPI2_GPIO, &GPIO_InitStructure);
		}
		else
		{
			/*Doing nothing*/
		}
	}

	/*configure spi */
	{
		SPI_InitTypeDef  SPI_InitStructure;
		if(SPI_MODE_MASTER == mode)
		{
			SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
			SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
			SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;/*the 8 bit is default setting,This parameter can be reconfigured in the device*/
			SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;/*the SPI_CPOL_High is default setting,This parameter can be reconfigured in the device*/
			SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
			SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
			SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;/*the SPI_BaudRatePrescaler_256 is default setting,This parameter can be reconfigured in the device*/
			SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;/*the SPI_FirstBit_MSB is default setting,This parameter can be reconfigured in the device*/
			SPI_InitStructure.SPI_CRCPolynomial = 7;

			SPI_Init(SPI2, &SPI_InitStructure);

			SPI_Cmd(SPI2, ENABLE);

			stm32f10x_spi_bus_WriteByte(SPI2,0xff);/*Reset spi*/
		}
		else if(SPI_MODE_SLAVE == mode)
		{
			SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
			SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
			SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
			SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
			SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
			SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
			SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
			SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
			SPI_InitStructure.SPI_CRCPolynomial = 7;

			SPI_Init(SPI2, &SPI_InitStructure);

			SPI_Cmd(SPI2, ENABLE);

			//stm32f10x_spi_bus_WriteByte(SPI2,0xff);/*Reset spi*/
		}
	}
}

/*
 * spi3 hardware initiation base on stm32f10x family mcu
 * */
static void stm32f10x_spi3_bus_HwInit(Enum_SpiModeType mode)
{

}

/*
 * read one byte data from hardware spi bus base on spi channel
 * */
static rt_err_t stm32f10x_spi_bus_WriteReadByte(SPI_TypeDef* SPIx, unsigned char WriteData, unsigned char *ReadData)
{
	int timeout_cnt = 0;
	/*
	 * Write data to Spi register
	 * */
	{
		/*wait the send flag is ready*/
		while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)
		{
			timeout_cnt++;
			/*Wait the send timeout check*/
			if(timeout_cnt>200)
			{
				return RT_ETIMEOUT;
			}
			else
			{
				/*doing noting*/
			}
		}

		SPI_I2S_SendData(SPIx, WriteData);/*read data from SPI bus*/
	}

	/*
	 * Read data from Spi register
	 * */
	{
		/*wait the receive flag*/
		while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)
		{
			timeout_cnt++;
			/*Wait the receive timeout check*/
			if(timeout_cnt > 200)
			{
				return RT_ETIMEOUT;
			}
			else
			{
				/*doing noting*/
			}
		}

		*ReadData = SPI_I2S_ReceiveData(SPIx);/*read data from SPI bus*/
		return RT_EOK;
	}
}

/*
 * read one byte data from hardware spi bus base on spi channel
 * */
static rt_err_t stm32f10x_spi_bus_ReadByte(SPI_TypeDef* SPIx, unsigned char *Data)
{
	int timeout_cnt = 0;
	/*wait the receive flag*/
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)
	{
		timeout_cnt++;
		/*Wait the receive timeout check*/
		if(timeout_cnt > 200)
		{
			return RT_ETIMEOUT;
		}
		else
		{
			/*doing noting*/
		}
	}

	*Data = SPI_I2S_ReceiveData(SPIx);/*read data from SPI bus*/
	return RT_EOK;
}

/*
 * write one byte data from hardware spi bus base on spi channel
 * */
static rt_err_t stm32f10x_spi_bus_WriteByte(SPI_TypeDef* SPIx, unsigned char Data)
{
	int timeout_cnt = 0;
	/*wait the send flag is ready*/
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)
	{
		timeout_cnt++;
		/*Wait the send timeout check*/
		if(timeout_cnt>200)
		{
			return RT_ETIMEOUT;
		}
		else
		{
			/*doing noting*/
		}
	}

	SPI_I2S_SendData(SPIx, Data);/*read data from SPI bus*/
	return RT_EOK;
}

/*
 * read some byte data from hardware spi bus base on spi channel
 * */
static rt_err_t stm32f10x_spi_bus_WriteReadData(SPI_TypeDef* SPIx, const unsigned char *WriteData, unsigned char *ReadData,int length)
{
	rt_err_t ret = RT_EOK;
	int i = 0x00;

	GPIO_WriteBit(SPI2_GPIO,SPI2_GPIO_NSS,	Bit_RESET);/*Enable CS Pin. Note : CS PIN is active low*/

	for(i = 0x00; i < length; i++)
	{
		ret = stm32f10x_spi_bus_WriteReadByte(SPIx, *(WriteData+i), (ReadData+i) );
	}

	GPIO_WriteBit(SPI2_GPIO,SPI2_GPIO_NSS,	Bit_SET);/*Disable CS Pin*/
    
    return ret;
}

/*
 * read size of byte data from hardware spi bus base on spi channel
 * pos : stm32f10x spi bus channel
 * */
static rt_size_t stm32f10x_spi_bus_ReadData(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
	rt_size_t i = 0x00;
	rt_err_t ret = RT_EOK;
    SPI_TypeDef *SPIx = SPI1;
    unsigned char buf[size];

	/*Check the device is valid*/

    /*pos parameters convert to SPI bus channel*/
    if(0x01 == pos)
    {
    	SPIx = stm32f10x_spi_bus_channel(1);
    }
    else if(0x02 == pos)
    {
    	SPIx = stm32f10x_spi_bus_channel(2);
    }
    else if(0x03 == pos)
    {
    	SPIx = stm32f10x_spi_bus_channel(3);
    }
    else
    {
    	ret = RT_ERROR;
    	return ret;
    }

	/*Read data from hardware spi bus*/
	for(i = 0x00; i < size; i++)
	{
		ret = stm32f10x_spi_bus_ReadByte(SPIx,&buf[i]);
		/*if read data happen error,exit this loop*/
		if(RT_EOK != ret)
		{
			break;
		}
	}

	/*Copy data to *buffer*/
	rt_memcpy(buffer, buf,size);

	return ret;
}

/*
 * write size of byte data from hardware spi bus base on spi channel
 * pos : stm32f10x spi bus channel
 * */
static rt_size_t stm32f10x_spi_bus_WriteData(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
	rt_size_t i = 0x00;
	rt_err_t ret = RT_EOK;
	SPI_TypeDef *SPIx = SPI1;
	unsigned char buf[size];

	/*Check the device is valid*/

	/*pos parameters convert to SPI bus channel*/
	if(0x01 == pos)
	{
		SPIx = stm32f10x_spi_bus_channel(1);
	}
	else if(0x02 == pos)
	{
		SPIx = stm32f10x_spi_bus_channel(2);
	}
	else if(0x03 == pos)
	{
		SPIx = stm32f10x_spi_bus_channel(3);
	}
	else
	{
		ret = RT_ERROR;
		return ret;
	}

	/*Copy data to *buf*/
	rt_memcpy(buf, buffer,size);

	/*Read data from hardware spi bus*/
	for(i = 0x00; i < size; i++)
	{
		ret = stm32f10x_spi_bus_WriteByte(SPIx,buf[i]);
		/*if read data happen error,exit this loop*/
		if(RT_EOK != ret)
		{
			break;
		}
	}

	return ret;
}

/*
 * args[0] : stm32f10x spi bus channel
 * */
static rt_err_t stm32f10x_spi_bus_Control(rt_device_t dev, int cmd, void *args)
{
	rt_err_t ret = RT_EOK;

	/*Check the device is valid*/


	return ret;
}

/*spi1 bus configuration function*/
static rt_err_t stm32f10x_spi1_bus_config(struct rt_spi_device *device, struct rt_spi_configuration *configuration)
{
    rt_err_t ret = RT_EOK;

	return ret;
}

/*spi1 bus transfer message function*/
static rt_uint32_t stm32f10x_spi1_bus_transfer_message(struct rt_spi_device *device, struct rt_spi_message *message)
{
    rt_uint32_t ret = RT_EOK;

    spi_bus_printf("Enter stm32f10x_spi1_bus_transfer_message function\r\n");

	ret = stm32f10x_spi_bus_WriteReadData(SPI1, message->send_buf, message->recv_buf, message->length);

	spi_bus_printf("Exit stm32f10x_spi1_bus_transfer_message function\r\n");

	return ret;
}

/*spi2 bus configuration function*/
static rt_err_t stm32f10x_spi2_bus_config(struct rt_spi_device *device, struct rt_spi_configuration *configuration)
{
	rt_err_t ret = RT_EOK;

	spi_bus_printf("enter stm32f10x_spi_bus_config function\r\n");

	/*Configure cs pin.*/
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  // cs
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_SetBits(GPIOB,GPIO_Pin_12);
	}

	/*Configure SPI baud rate prescaler */
	{
		SPI_InitTypeDef  SPI_InitStructure;

		/*The 3 parameters is not configure for device*/
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
		SPI_InitStructure.SPI_CRCPolynomial = 7;

		/*Configure SPI SPI_Mode*/
		if(RT_SPI_SLAVE == (configuration->mode & RT_SPI_SLAVE))
		{
			SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
			spi_bus_printf("Spi mode setting to SPI_Mode_Slave\n\r");
		}
		else
		{
			SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
			spi_bus_printf("Spi mode setting to SPI_Mode_Master\n\r");
		}

		/*Configure Data Size*/
		if(8 == configuration->data_width)
		{
			SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
			spi_bus_printf("Spi data size setting to SPI_DataSize_8b\n\r");
		}
		else if(16 == configuration->data_width)
		{
			SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
			spi_bus_printf("Spi data size setting to SPI_DataSize_16b\n\r");
		}
		else
		{
			spi_bus_printf("SPI Configuration : data_width parameters error\n\r");
			ret = RT_ERROR;
			return ret;
		}

		/*configure SPI_CPOL*/
		if(RT_SPI_CPOL == (configuration->mode & RT_SPI_CPOL))
		{
			SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
			spi_bus_printf("Spi CPOL setting to SPI_CPOL_High\n\r");
		}
		else
		{
			SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
			spi_bus_printf("Spi CPOL setting to SPI_CPOL_Low\n\r");
		}
		/*Configure SPI_CPHA*/
		if(RT_SPI_CPHA == (configuration->mode & RT_SPI_CPHA))
		{
			SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
			spi_bus_printf("Spi CPHA setting to SPI_CPHA_2Edge\n\r");
		}
		else
		{
			SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
			spi_bus_printf("Spi CPHA setting to SPI_CPHA_1Edge\n\r");
		}

		/*Configure SPI SPI_FirstBit*/
		if(RT_SPI_MSB == (configuration->mode & RT_SPI_MSB) )
		{
			SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
			spi_bus_printf("Spi first bit setting to SPI_FirstBit_MSB\n\r");
		}
		else
		{
			SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
			spi_bus_printf("Spi first bit setting to SPI_FirstBit_LSB\n\r");
		}

		/*configure spi baud rate prescaler*/
		if(configuration->max_hz != 0)
		{
			SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;/*fastest*/
			spi_bus_printf("Spi baud rate prescaler setting to SPI_BaudRatePrescaler_2\n\r");
		}
		else
		{
			SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;/*slowest*/
			spi_bus_printf("Spi baud rate prescaler setting to SPI_BaudRatePrescaler_256\n\r");
		}

		SPI_Init(SPI2, &SPI_InitStructure);

		SPI_Cmd(SPI2,ENABLE);

		stm32f10x_spi_bus_WriteByte(SPI2,0xff);/*Reset spi*/
	}

	spi_bus_printf("exit stm32f10x_spi_bus_config function\r\n");
	return ret;
}

/*spi2 bus transfer message function*/
static rt_uint32_t stm32f10x_spi2_bus_transfer_message(struct rt_spi_device *device, struct rt_spi_message *message)
{
	rt_uint32_t ret = 0x00;

	spi_bus_printf("Enter stm32f10x_spi2_bus_transfer_message function\r\n");

	ret = stm32f10x_spi_bus_WriteReadData(SPI2, message->send_buf, message->recv_buf, message->length);

	spi_bus_printf("Exit stm32f10x_spi2_bus_transfer_message function\r\n");
	return ret;
}

/*spi3 bus configuration function*/
static rt_err_t stm32f10x_spi3_bus_config(struct rt_spi_device *device, struct rt_spi_configuration *configuration)
{
    rt_err_t ret = 0x00;
    
    return ret;
}

/*spi3 bus transfer message function*/
static rt_uint32_t stm32f10x_spi3_bus_transfer_message(struct rt_spi_device *device, struct rt_spi_message *message)
{
    rt_uint32_t ret = 0x00;

	spi_bus_printf("Enter stm32f10x_spi3_bus_transfer_message function\r\n");

	ret = stm32f10x_spi_bus_WriteReadData(SPI3, message->send_buf, message->recv_buf, message->length);

	spi_bus_printf("Exit stm32f10x_spi3_bus_transfer_message function\r\n");
	return ret;
}

#ifdef RT_USING_SPI1
static struct rt_spi_bus spi1_bus;
static const struct rt_spi_ops spi1_ops =
{
		stm32f10x_spi1_bus_config,
		stm32f10x_spi1_bus_transfer_message
};
#endif
/*spi2 using struct*/
#ifdef RT_USING_SPI2
static struct rt_spi_bus spi2_bus;
static const struct rt_spi_ops spi2_ops =
{
		stm32f10x_spi2_bus_config,
		stm32f10x_spi2_bus_transfer_message
};
#endif
#ifdef RT_USING_SPI3
static struct rt_spi_bus spi3_bus;
static const struct rt_spi_ops spi3_ops =
{
		stm32f10x_spi3_bus_config,
		stm32f10x_spi3_bus_transfer_message
};
#endif
/*
 * Initiation spi bus,
 * include:
 * init hw;
 * register spi bus to list_device
 * */
static rt_err_t stm32f10x_spi_bus_init(void)
{
	rt_err_t ret = 0x00;

#ifdef RT_USING_SPI1
	stm32f10x_spi1_bus_HwInit(SPI_MODE_MASTER);	/*initialization the spi hardware*/
	spi1_bus.parent.parent.type = RT_Object_Class_Device;

	spi1_bus.parent.read = stm32f10x_spi_bus_ReadData;
	spi1_bus.parent.write = stm32f10x_spi_bus_WriteData;
	spi1_bus.parent.control = stm32f10x_spi_bus_Control;

	/*attach bus to device list*/
	ret = rt_spi_bus_register(&spi1_bus,"spi1",&spi1_ops);
	if(RT_EOK != ret)
	{
		spi_bus_printf("spi1 bus register failed !\r\n");
		return ret;
	}
	else
	{
		spi_bus_printf("spi1 bus register successful!\r\n");
	}
#endif

#ifdef RT_USING_SPI2
	stm32f10x_spi2_bus_HwInit(SPI_MODE_MASTER);	/*initialization the spi hardware*/
	spi2_bus.parent.parent.type = RT_Object_Class_Device;

	spi2_bus.parent.read = stm32f10x_spi_bus_ReadData;
	spi2_bus.parent.write = stm32f10x_spi_bus_WriteData;
	spi2_bus.parent.control = stm32f10x_spi_bus_Control;

	/*attach bus to device list*/
	ret = rt_spi_bus_register(&spi2_bus,"spi2",&spi2_ops);
	if(RT_EOK != ret)
	{
		spi_bus_printf("spi2 bus register failed !\r\n");
		return ret;
	}
	else
	{
		spi_bus_printf("spi2 bus register successful!\r\n");
	}
#endif

#ifdef RT_USING_SPI3
	stm32f10x_spi3_bus_HwInit(SPI_MODE_MASTER);	/*initialization the spi hardware*/
	spi3_bus.parent.parent.type = RT_Object_Class_Device;

	spi3_bus.parent.read = stm32f10x_spi_bus_ReadData;
	spi3_bus.parent.write = stm32f10x_spi_bus_WriteData;
	spi3_bus.parent.control = stm32f10x_spi_bus_Control;

	/*attach bus to device list*/
	ret = rt_spi_bus_register(&spi3_bus,"spi3",&spi3_ops);
	if(RT_EOK != ret)
	{
		spi_bus_printf("spi3 bus register failed !\r\n");
		return ret;
	}
	else
	{
		spi_bus_printf("spi3 bus register successful!\r\n");
	}
#endif
    
    return ret;
}

static struct rt_spi_device spi_device;
static stm32f10x_spi_cs_config spi_device_cs;
static rt_err_t stm32f10x_spi_device_init(void)
{
	rt_err_t ret = RT_EOK;
	char spi_device_name[] = "spi20";

	/*
	 * Setting SPI parameters.
	 * Inlcude :
	 * Data width:
	 * mode :
	 * CPOL :
	 * CPHA :
	 * Frist bit :
	 * baud rate :
	 *
	 * */
	spi_device.bus = &spi2_bus;
	spi_device.config.data_width = 8;
	spi_device.config.max_hz = 1;
	spi_device.config.mode = RT_SPI_MODE_0 | RT_SPI_MSB | RT_SPI_MASTER;

	ret = rt_spi_bus_attach_device(&spi_device, spi_device_name, "spi2", (void*)&spi_device_cs);
	if (ret != RT_EOK)
	{
		spi_bus_printf("spi device[%s] register failed !\r\n",spi_device_name);
		return ret;
	}
	else
	{
		spi_bus_printf("spi device[%s] register successful !\r\n",spi_device_name);
	}

	return ret;
}

int rt_hw_stm32f10x_spi_init(void)
{
	stm32f10x_spi_bus_init();
	stm32f10x_spi_device_init();
	return 0x00;
}

static void stm32f10x_spi_bus_test_sample(int argc, char *argv[])
{
	const char name[] = "spi20";
	struct rt_spi_configuration spi_cfg;
	struct rt_spi_device* spi_device;

	spi_cfg.data_width = 8;
	spi_cfg.max_hz = 1;
	spi_cfg.mode = RT_SPI_MODE_3 | RT_SPI_MSB | RT_SPI_MASTER;

	spi_device = (struct rt_spi_device *)rt_device_find(name);
	if (!spi_device)
	{
		spi_bus_printf("stm32f10x spi bus test sample run failed! can't find %s device!\n", name);
	}
	else
	{
		spi_bus_printf("stm32f10x spi bus test sample run successful! %s device exist is system!\n", name);
	}

	spi_bus_printf("the device type is 0x%x\n", spi_device->bus->lock.parent.parent.type);

	rt_spi_configure(spi_device,&spi_cfg);

	{
        /*Test code*/
		unsigned char SendData[] = {0x90,0x00,0x00,0x00,0xff,0xff};
		unsigned char RecData[] = {0x00,0x00,0x00,0x00,0x00,0x00};
		rt_spi_transfer(spi_device, SendData, RecData, 6);
		spi_bus_printf("use rt_spi_send_then_recv() write w25q id is %x, read w25q ID is:%x %x\n", SendData[0],RecData[4],RecData[5]);
	}


}

MSH_CMD_EXPORT(stm32f10x_spi_bus_test_sample, stm32f10x spi bus test sample);

INIT_BOARD_EXPORT(rt_hw_stm32f10x_spi_init);

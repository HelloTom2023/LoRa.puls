#ifndef __STM32F10X_SPI_BUS_H__
#define __STM32F10X_SPI_BUS_H__

#include <rtthread.h>
#include <rtdevice.h>
#include "stm32f10x.h"

#ifdef USING_SPI_BUS_DEBUG
#define spi_bus_printf(fmt, ...)   rt_kprintf(fmt, ##__VA_ARGS__)
#else
#define	spi_bus_printf(fmt, ...)
#endif

typedef struct stm32f10x_spi_cs_config_tag
{
	GPIO_TypeDef * GPIOx;
    uint16_t GPIO_Pin;
}stm32f10x_spi_cs_config;

/*Define SPIx pin and clock*/
#ifdef RT_USING_SPI1
/*Macro definition area*/
#define SPI1_GPIO 		GPIOA
#define SPI1_GPIO_NSS 	GPIO_Pin_4
#define SPI1_GPIO_SCK 	GPIO_Pin_5
#define SPI1_GPIO_MISO 	GPIO_Pin_6
#define SPI1_GPIO_MOSI 	GPIO_Pin_7
#endif

#ifdef RT_USING_SPI2
#define SPI2_GPIO 		GPIOB
#define SPI2_GPIO_NSS 	GPIO_Pin_12
#define SPI2_GPIO_SCK 	GPIO_Pin_13
#define SPI2_GPIO_MISO 	GPIO_Pin_14
#define SPI2_GPIO_MOSI 	GPIO_Pin_15

#endif

#ifdef RT_USING_SPI3
#define SPI3_GPIO 		GPIOB
//#define SPI3_GPIO_NSS 	GPIO_Pin_12 /*SPI3_NSS located GPIOA_Pin15*/
#define SPI3_GPIO_SCK 	GPIO_Pin_3
#define SPI3_GPIO_MISO 	GPIO_Pin_4
#define SPI3_GPIO_MOSI 	GPIO_Pin_5
#endif

/*Type definition area*/
typedef enum enum_SpiModeType_TAG
{
	SPI_MODE_NONE,
	SPI_MODE_MASTER,
	SPI_MODE_SLAVE,
	SPI_MODE_MAX
} Enum_SpiModeType;

#define stm32f10x_spi_bus_channel(channel)	SPI##channel

/*Function declaration area*/
static void stm32f10x_spi1_bus_HwInit(Enum_SpiModeType mode);
static void stm32f10x_spi2_bus_HwInit(Enum_SpiModeType mode);
static void stm32f10x_spi3_bus_HwInit(Enum_SpiModeType mode);
static rt_err_t stm32f10x_spi_bus_WriteReadByte(SPI_TypeDef* SPIx, unsigned char WriteData, unsigned char *ReadData);
static rt_err_t stm32f10x_spi_bus_ReadByte(SPI_TypeDef* SPIx, unsigned char *Data);
static rt_err_t stm32f10x_spi_bus_WriteByte(SPI_TypeDef* SPIx, unsigned char Data);
static rt_size_t stm32f10x_spi_bus_ReadData(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size);
static rt_size_t stm32f10x_spi_bus_WriteData(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size);
static rt_err_t stm32f10x_spi_bus_Control(rt_device_t dev, int cmd, void *args);
static rt_err_t stm32f10x_spi1_bus_config(struct rt_spi_device *device, struct rt_spi_configuration *configuration);
static rt_uint32_t stm32f10x_spi1_bus_transfer_message(struct rt_spi_device *device, struct rt_spi_message *message);
static rt_err_t stm32f10x_spi2_bus_config(struct rt_spi_device *device, struct rt_spi_configuration *configuration);
static rt_uint32_t stm32f10x_spi2_bus_transfer_message(struct rt_spi_device *device, struct rt_spi_message *message);
static rt_err_t stm32f10x_spi3_bus_config(struct rt_spi_device *device, struct rt_spi_configuration *configuration);
static rt_uint32_t stm32f10x_spi3_bus_transfer_message(struct rt_spi_device *device, struct rt_spi_message *message);
static rt_err_t stm32f10x_spi_bus_init(void);
static rt_err_t stm32f10x_spi_device_init(void);
int rt_hw_spi1_init(void);


#endif

/*
 * @Descripttion: �׻�������ͨѶ�豸��UART������Դ�ļ�
 */

#include "uartConfig.h"
#include <uartRevData.h>

rt_device_t SerialDev = RT_NULL;

uint8_t recv_flag = 0;
static rt_err_t DeviceRecvCallBack(rt_device_t dev, rt_size_t size)
{
    rt_uint8_t DevRxData = 0;
	
    if (1 == rt_device_read(SerialDev, -1, &DevRxData, 1))
    {
        if (1 == CheckRingBuffFull(&RecvBLEBuf))  
        {
            rt_kprintf("DeviceRecvCallBack: No space to store data!\n");
            return 0;
        }
        else
        {
			if(recv_flag == 0)
			{
				if(DevRxData == 0)
				{
					return 0;
				}
				recv_flag = 1;
			}
			
            rt_ringbuffer_putchar(&RecvBLEBuf, DevRxData);
            // rt_sem_release(&SemRecvDataFromBLE);  
            ReleaseSemRecvDataFromUart();
        }
         
    }
		
	return 0;
}

/**
 * @name: BLECommDevice 
 * @msg: �����׻�������ͨѶ�豸
 * @param [in]
 *        device_name ͨѶ�豸����
 * @param [out]
 *        �� 
 * @return {*}
 * @note: 1��ͨѶ�豸һ��ΪUart
 *        2���ú�������ڲ�����menuconfig������BLE_COMM_DEVICE_NAME����ֵ
 */
void uartCommDevice(const char *device_name)
{
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
    
    SerialDev = rt_device_find(device_name);
    if (!SerialDev)
    {
        rt_kprintf("BLECommDevice: can not find device: %s\n", device_name);
    }
    
    //TODO: �������Կ����Ż�Ϊ:DMA����+UART�����ж�
    if (RT_EOK == rt_device_open(SerialDev,RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX))
    {
		if(0 == strcmp("uart2", device_name)) 
        {
            config.baud_rate = UART2_BAUD_RATE;
        }

        if (RT_EOK != rt_device_control(SerialDev, RT_DEVICE_CTRL_CONFIG, &config))
        {
            rt_kprintf("change the baud rate of %s failed!\n", device_name);
        }
        
		if (strcmp(device_name, "uart2") == 0)
		{
			rt_kprintf("BLECommDevice: find device: %s\n", device_name);
			rt_device_set_rx_indicate(SerialDev, DeviceRecvCallBack);
		}
    } 

}

/**
 * @name: UARTDeviceWrite 
* @msg:  ��UART�������ݰ�
 * @param [in]
 *        DataBuffer: ���ݻ�����
 *        DataSize�����ݴ�С
 * @param [out]
 *        �� 
 * @return ��
 * @note:  ��
 */
void UARTDeviceWrite(const void *DataBuffer, rt_size_t DataSize)
{
    rt_device_write(SerialDev, 0, DataBuffer, DataSize);
}



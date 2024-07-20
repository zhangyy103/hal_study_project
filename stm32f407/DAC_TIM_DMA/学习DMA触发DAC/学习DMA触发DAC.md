![轮询方式设置DAC的DMA](.\轮询方式设置DAC的DMA.png)  
## DMA（Direct Memory Access），字面意思“直接访问内存”，无需 CPU 干预直接读写内存。DMA进行循环搬运


![定义正弦波数组](.\定义正弦波数组.png)  
## 设置有100个数据的正弦波数组


![开启DMA](.\开启DMA.png)  
##  开启DMA

### 函数讲解
```C
HAL_StatusTypeDef HAL_DAC_Start_DMA(DAC_HandleTypeDef *hdac, uint32_t Channel, const uint32_t *pData, uint32_t Length,uint32_t Alignment)
```

#### 返回值的类型HAL_StatusTypeDef
```C
typedef enum 
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;
```
HAL_StatusTypeDef是一个枚举类型，用于表示函数的返回状态。它通常用于与硬件抽象层（hal）库一起使用，该库用于驱动各种硬件设备。该枚举类型定义了以下可能的返回状态：

* _OK：操作成功完成
* HAL_ERROR：操作发生错误
* HAL_BUSY：操作正在进行中
* HAL_TIMEOUT：操作超时
* HAL_CRC_ERROR：CRC（循环冗余校验）错误
* HAL_DMA_ERROR：DMA（直接内存访问）错误
* HAL_INVALID_PARAM：无效的参数

#### DAC_HandleTypeDef *hdac  选择DAC f407zgt6只有一个DAC

#### uint32_t Channel 选择DAC的通道

#### const uint32_t *pData 数组，DMA将数组中的值传给DAC

#### uint32_t Length 数据的长度

#### uint32_t Alignment 对齐方式
```C
#define DAC_ALIGN_12B_R                    0x00000000U
#define DAC_ALIGN_12B_L                    0x00000004U
#define DAC_ALIGN_8B_R                     0x00000008U
```
STM32的DAC数据位数可配置为12位或者8位，对齐格式分为左对齐和右对齐。左对齐是指数据的高位对齐到DAC数据寄存器的高位，低位补0；右对齐是指数据的低位对齐到DAC数据寄存器的低位，高位补0。在单通道和双通道模式下的对齐格式如下图所示  
![](https://shequ.stmicroelectronics.cn/data/attachment/forum/202004/23/112020al2vulwlaxwf4uxu.png)

左对齐和右对齐的选择取决于具体的应用场景和要求。一般来说，如果需要更高的精度和更低的噪声，则应选择左对齐；如果需要更高的速度和更低的功耗，则应选择右对齐。

#### DAC产生的波的频率：定时器触发频率/strlen(Sin12bit)



---
---
# 计算事件触发的频率的公式
psc：预分频系数  
arr：重装载数  
Tclk：定时器的输入时钟源（单位MHZ）  
计数器的arr和时钟分频psc都要加1，因为这两个值是配置在寄存器中的，从0开始计数  
推荐配置时使用 XX - 1 这样的方式表达


# 定时器触发频率 = $\frac{定时器时钟频率}{(预分频系数 + 1) * (计数周期 + 1)}$
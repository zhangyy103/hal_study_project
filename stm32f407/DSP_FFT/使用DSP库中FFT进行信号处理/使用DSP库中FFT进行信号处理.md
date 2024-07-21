#### 快速傅里叶变换（FFT）是信号处理中最常用的算法  
#### 本文介绍了在hal库中使用FFT来进行信号处理(Basing stm32f4)

1. 通过[一个视频](https://www.bilibili.com/video/BV1pW411J7s8/?share_source=copy_web&vd_source=452925c151685f7ba8a44f434b04c8a9)快速了解快速傅里叶变化

2. 观看[大佬的教程](https://blog.csdn.net/qq_34022877/article/details/117855263)使用stm32cubemx + keil 配置环境  

3. 编写并理解业务代码

```C
#define FFT_LEN 1024
```
定义采样点数(建议使用 4^n)

```C
float data[FFT_LEN];
// input 
float data_in[FFT_LEN * 2];//复数，实部和虚部
// output
float data_out[FFT_LEN];
```

* data[FFT_LEN] : 原始数据，可以为AD采集的电压
* data_in[FFT_LEN * 2] ；FFT时的输入数据，由于涉及复数，故 *2
* data_out[FFT_LEN] ：输出数据

```C
for(i = 0; i < FFT_LEN; i++){
	data[i] = sin(2 * 3.14 / FFT_LEN * i);
}
```

模拟一个正弦波  
注意观察，该模拟波在1024个采样点内只有一个周期，我们假设采集该样本用时1s，故样本的采样率为$1024/1$ 样本的频率是1  
(NOTIC 采样频率要大于你要采集的信号频率的两倍)
```C
for(i = 0; i < FFT_LEN; i++){
    data_in[i * 2] = data[i];//实部
	data_in[i * 2 + 1] = 0;//虚部
}
```
初始化data_in[FFT_LEN * 2] 虚部补0
```C
arm_cfft_f32(&arm_cfft_sR_f32_len1024, data_in, 0, 1);
```
对输入数据进行傅里叶变换,它的输入参数含义如下:
* arm_cfft_sR_f32_len1024为傅里叶变换结构体，1024是要运算的点数  
我们在进行其他点数的计算时，比如说32个点的FFT，就可以用arm_cfft_sR_f32_len32
* fft_inputbuf为傅里叶变换需要处理的数据的首地址
* 第三个参数0是正变换，1是反变换
* 第四个参数一般是1

经过傅里叶变换后的结果仍然为复数，我们直接对复数取模  
​ 取模是实部和虚部的平方和取平均来算，不过我们没必要自己去这样写，因为DSP库为我们提供了取模函数：
```C
arm_cmplx_mag_f32(data_in, data_out, FFT_LEN);
```
* fft_inputbuf源数据，形式为复数
* fft_outputbuf取完模后的数据，形式为实数
* FFT_LENGTH是取模的点数
```C
data_out[0] /= FFT_LEN;
for(i = 0; i < FFT_LEN; i++){
	data_out[i] /= (FFT_LEN/2);
}
```
​运算完成的结果还需要进行上面的处理，背后的原理就跟FFT算法有关了，这里不做解释  
接着，我们将数据输出
```C
for (int i = 0; i < FFT_LEN; i++){
    printf("%d:\t%.2f\r\n", i, data_out[i]);
}
```
我们注意到，输出的数据中i = 1对应着1，i = 1023也对应着1  
其实FFT计算出来的数据是对称的，我们需观察前一半的数据  
​ FFT输出数组的下标表示的频率，计算关系为：  
#### $频率 = 数组下标*\frac{采样率}{fft计算的点数}$
代入数据得:
#### $ f = 1 * \frac{1024}{FFT\_LEN(1024)}$ = 1

---
栅栏效应：
由于数组的下标为整数， 会出现频率对应的值落在两个下标之间，这是我们需要频率点附近的能量聚集起来，将附近频率点的幅度平方求和，再取平均  
或者重新选取合适的样本计算

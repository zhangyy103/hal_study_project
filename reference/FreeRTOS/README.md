# FreeRTOS

## 创建线程
**1. 动态分配内存**
```c
	BaseType_t xTaskCreate(	TaskFunction_t pxTaskCode,
							const char * const pcName,		/*lint !e971 Unqualified char types are allowed for strings and single characters only. */
							const configSTACK_DEPTH_TYPE usStackDepth,
							void * const pvParameters,
							UBaseType_t uxPriority,
							TaskHandle_t * const pxCreatedTask )
```  
    arg1 : 任务函数的函数名 void类型 void * 参数
    arg2 : 任务的名字
    arg3 : 栈大小,单位为word,10表示40字节
    arg4 : 调用任务函数时传入的参数(用于不同线程之间传递参数，当有多个参数时使用结构体指针)
    arg4 : 优先级
    arg5 : 任务句柄, 可以使用它来操作这个任务 

    ret  : 用于指示任务创建是否成功。BaseType_t ret;
        pdPASS：任务创建成功。  
        errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY：任务创建失败，原因是无法分配所需的内存。  
        其他错误代码：根据具体的 FreeRTOS 配置和实现，可能会有其他错误代码。  


**2. 静态分配内存**
```
在全局变量中定义线程的栈  
在全局变量中定义静态分配的任务结构体的指针，用它来操作这个任务  
```
```c
static StackType_t g_pucStackOfYourTask [128]; // Stack: uint32_t
static StaticTask_t g_TCBOfYourTask;


	TaskHandle_t xTaskCreateStatic(	TaskFunction_t pxTaskCode,
									const char * const pcName,		/*lint !e971 Unqualified char types are allowed for strings and single characters only. */
									const uint32_t ulStackDepth,
									void * const pvParameters,
									UBaseType_t uxPriority,
									StackType_t * const puxStackBuffer,
									StaticTask_t * const pxTaskBuffer )
```
    arg1 : 任务函数的函数名 void类型 void * 参数
    arg2 : 任务的名字
    arg3 : 栈大小,单位为word,10表示40字节
    arg4 : 调用任务函数时传入的参数(用于不同线程之间传递参数，当有多个参数时使用结构体指针)
    arg5 : 优先级
    arg6 : 在全局变量中定义线程的栈
    arg7 : 静态分配的任务结构体的指针，用它来操作这个任务

    ret  : 返回任务的句柄。如果任务创建成功，返回任务的句柄；如果任务创建失败，返回 NULL

+ 在 FreeRTOS 中，"线程" 和 "任务" 通常是互换使用的术语
+ 一个线程有它独立的栈


## 删除任务
```c
vTaskDelete(NULL) // 在自己的线程中删除自己
vTaskDelete(pvTaskCode) // 在线程中删除其他线程 pvTaskCode是其他线程的句柄
```


## 任务函数
任务函数中是一个`while(1)`循环
```c
void StartTask(void *arg){
	my_func_init();

	for(;;){
		my_func();
	}
}
```

## 如何计算栈

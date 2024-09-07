# FreeRTOS

## 创建任务
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
    arg5 : 优先级
    arg6 : 任务句柄, 可以使用它来操作这个任务 

    ret  : 用于指示任务创建是否成功。BaseType_t ret;
        pdPASS：任务创建成功。  
        errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY：任务创建失败，原因是无法分配所需的内存。  
        其他错误代码：根据具体的 FreeRTOS 配置和实现，可能会有其他错误代码。  
+ 使用 xTaskCreate 动态分配内存时，主要分配了以下两部分内存：  
    + 任务控制块（TCB）：用于存储任务的元数据，如任务状态、优先级、任务句柄等。
    + 任务栈：用于存储任务执行时的局部变量、函数调用的返回地址、函数参数和寄存器的保存值等。栈的大小由 usStackDepth 参数指定。

+ FreeRTOS 使用以下内存分配函数来管理动态内存：
    + pvPortMalloc：用于分配内存。
    + vPortFree：用于释放内存。(详见heap_1234.c)

**2. 静态分配内存**
```
在全局变量中定义线程的内存池  
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
    arg6 : 在全局变量中定义线程的栈，其的大小应该与arg3相匹配
    arg7 : 静态分配的任务结构体的指针，用它来操作这个任务

    ret  : 返回任务的句柄。如果任务创建成功，返回任务的句柄；如果任务创建失败，返回 NULL

+ 在 FreeRTOS 中，"线程" 和 "任务" 通常是互换使用的术语
+ 每一个线程都有它独立的栈


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

## 线程的栈
**1. 如何计算栈**  

一个栈包括:
-  栈中函数的返回地址(函数的相互调用, 调用程度越深，所占空间越大)
-  栈中函数的参数
-  栈中的局部变量
-  寄存器的保存值(现场)

**2.判断栈的剩余大小**
```c
// 获取任务栈的高水位标记
UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark(xYourTaskHandle);
```
**3.启用栈溢出检测**
```c
#define configCHECK_FOR_STACK_OVERFLOW 1  // 或 2，根据需要选择检测级别

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    // 处理栈溢出，例如重启系统或记录错误信息
    printf("Stack overflow in task: %s\n", pcTaskName);
    for (;;);  // 停止系统
}
```

## 任务优先级和 Tick
**1. 任务优先级**  
当我们使用`xTaskCreate()` API函数创建一个任务的时候，会为任务赋予一个初始的优先级，当然这个优先级可以在调度器启动后，我们可以使用`vTaskPrioritySet()` API函数来进行优先级修改的。
```c
void vTaskPrioritySet( TaskHandle_t xTask, UBaseType_t uxNewPriority );
```
其中`xTask`参数是传递进某个任务的句柄，NULL则表示修改自己的优先级。  
`uxNewPriority`参数表示新设置的优先级，取值范围`0~(configMAX_PRIORITIES – 1)`。数值越大 任务的优先级越高，也就是任务更优先执行
```c
UBaseType_t uxTaskPriorityGet( const TaskHandle_t xTask );
```
`xTask`参数是某个任务句柄，NULL表示获取自己的优先级。返回值就是该任务的优先级。

## 两个Delay函数
```c
void vTaskDelay( const TickType_t xTicksToDelay )
```
xTicksToDelay：任务阻塞的时间，以系统节拍（tick）为单位。

在函数实现中，vTaskDelay中有挂机函数vTaskSuspendAll();  和恢复函数xTaskResumeAll();  
所以freertos中的延时函数只是把任务挂起了，等延时时间到，在把任务恢复。
保证了操作系统的实时性
```c
void vTaskDelayUntil(TickType_t *pxPreviousWakeTime, const TickType_t xTimeIncrement);
```
vTaskDelayUntil 函数使调用任务进入阻塞状态，直到指定的时间点。这个函数适用于需要精确定时的任务。

## 
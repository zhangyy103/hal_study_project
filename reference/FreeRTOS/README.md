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

**3. TCB控制块**
TaskHandle_t：TCB控制块

精简后的TCB任务控制块：
```c

typedef struct tskTaskControlBlock       /* The old naming convention is used to prevent breaking kernel aware debuggers. */
{
    volatile StackType_t * pxTopOfStack; /*< Points to the location of 
    ListItem_t xStateListItem;                  /*< The list that the state list item of a task is reference from denotes the state of that task (Ready, Blocked, Suspended ). */
    ListItem_t xEventListItem;                  /*< Used to reference a task from an event list. */
    UBaseType_t uxPriority;                     /*< The priority of the task.  0 is the lowest priority. */
    StackType_t * pxStack;                      /*< Points to the 
    char pcTaskName[ configMAX_TASK_NAME_LEN ];
} tskTCB;
```
`pxTopOfStack`：这个参数指向任务堆栈的最顶部，即最近放入任务堆栈的项目的位置。这必须是 TCB 结构的第一个成员。  
`ListItem_t xStateListItem`：这是一个用于任务状态管理的链表项。它用于将任务插入到就绪、阻塞或挂起状态链表中，以便操作系统可以有效地管理任务状态。  
`ListItem_t xEventListItem`：这是用于将任务插入到事件列表中的链表项。当任务等待某个事件发生时，它会被插入到事件列表中。这允许任务在事件发生时被及时唤醒。  
`UBaseType_t uxPriority`：这是任务的优先级。任务的优先级用于决定它在多任务系统中的调度顺序。较低的数值表示更高的优先级，0通常是最低优先级。  
`StackType_t * pxStack`：这个参数指向任务堆栈的起始位置。任务堆栈是用于保存任务上下文信息的内存区域，包括寄存器值、局部变量等。  
`char pcTaskName[configMAX_TASK_NAME_LEN]`：这个数组用于保存任务的名称，以便在调试和诊断中使用。configMAX_TASK_NAME_LEN 是一个配置参数，定义了任务名称的最大长度。  

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
**2. 任务状态**  
任务状态分为运行和非运行  
其中，非运行状态分为 阻塞状态(Blocked) 暂停状态(Suspended)  就绪状态(Ready)  

下图展示了任务状态之间的转化关系
![任务状态的转化关系](markdown_assets\任务状态转换.png "任务状态的转化关系")

**3. Tick**  
Tick: 用定时器产生固定间隔的中断  
两次中断之间的时间被称为时间片(time slice、tick period)  
时间片的长度由 configTICK_RATE_HZ 决定，假设 configTICK_RATE_HZ 为100，那么时间片长度就是 10ms
```c
vTaskDelay(2); // 等待2个Tick，假设configTICK_RATE_HZ=100, Tick周期时10ms, 等待20ms
// 还可以使用pdMS_TO_TICKS宏把ms转换为tick
vTaskDelay(pdMS_TO_TICKS(100)); // 等待100ms
```
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

## Hook Function
#### 1. 空闲任务钩子函数 (`vApplicationIdleHook`)

空闲任务钩子函数在系统空闲时调用。需要在 `FreeRTOSConfig.h` 中定义 `configUSE_IDLE_HOOK` 为 1。

```c
// FreeRTOSConfig.h
#define configUSE_IDLE_HOOK 1

// 实现钩子函数
void vApplicationIdleHook(void) {
    // 空闲时执行的代码
}
```

#### 2. 堆栈溢出钩子函数 (`vApplicationStackOverflowHook`)

堆栈溢出钩子函数在检测到任务堆栈溢出时调用。需要在 `FreeRTOSConfig.h` 中定义 `configCHECK_FOR_STACK_OVERFLOW`

```c
// FreeRTOSConfig.h
#define configCHECK_FOR_STACK_OVERFLOW 1  // 或 2

// 实现钩子函数
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    // 处理栈溢出，例如重启系统或记录错误信息
    printf("Stack overflow in task: %s\n", pcTaskName);
    for (;;);  // 停止系统
}
```

#### 3. 内存分配失败钩子函数 (`vApplicationMallocFailedHook`)

内存分配失败钩子函数在内存分配失败时调用。需要在 `FreeRTOSConfig.h` 中定义 `configUSE_MALLOC_FAILED_HOOK` 为 1。

```c
// FreeRTOSConfig.h
#define configUSE_MALLOC_FAILED_HOOK 1

// 实现钩子函数
void vApplicationMallocFailedHook(void) {
    // 处理内存分配失败，例如重启系统或记录错误信息
    printf("Memory allocation failed\n");
    for (;;);  // 停止系统
}
```

#### 4. Tick 钩子函数 (`vApplicationTickHook`)

Tick 钩子函数在每个系统 Tick 中断时调用。需要在 `FreeRTOSConfig.h` 中定义 `configUSE_TICK_HOOK` 为 1。

```c
// FreeRTOSConfig.h
#define configUSE_TICK_HOOK 1

// 实现钩子函数
void vApplicationTickHook(void) {
    // 每个系统 Tick 中断时执行的代码
}
```

## 任务调度
FreeRTOS 支持以下几种任务调度方法：

**抢占式调度（Preemptive Scheduling）**  
**时间片轮转调度（Time Slicing）**  
**协作式调度（Cooperative Scheduling）**  

**1. 抢占式调度（Preemptive Scheduling）**

在抢占式调度中，任务可以被更高优先级的任务抢占。当一个高优先级任务变得可用时，它可以打断当前正在执行的低优先级任务，从而使系统立即切换到高优先级任务执行。

在 FreeRTOS 中，任务调度是基于任务优先级的。当一个任务抢占另一个任务时，它会立即执行，无论被抢占的任务是否已经执行完其时间片。这种方式确保了高优先级任务能够及时响应，并在需要时立即执行，不受低优先级任务的阻碍。

在 FreeRTOS 中通过配置 `configUSE_PREEMPTION` 来决定是否启动抢占。

```c
// FreeRTOSConfig.h
#define configUSE_PREEMPTION 1
```

**2. 时间片轮转调度（Time Slicing）**

时间片轮转是指操作系统为每个任务分配一个时间片，即预定义的时间量。在时间片轮转调度方式下，每个任务可以执行一个时间片，然后系统将控制权移交给下一个就绪的任务。如果一个任务在其时间片结束前没有完成，系统会暂停该任务，将控制权交给下一个就绪的任务。

FreeRTOS 允许你在配置系统时启用或禁用时间片轮转。时间片的大小可以根据应用程序的需要进行调整。这种调度方式有助于确保任务之间的公平性，避免某些任务长时间占用处理器，同时允许多个任务分享处理时间。

在 FreeRTOS 中通过配置 `configUSE_TIME_SLICING` 来决定是否启动时间片轮转。

```c
// FreeRTOSConfig.h
#define configUSE_TIME_SLICING 1
```

**3. 协作式调度（Cooperative Scheduling）**

协作式调度是指任务主动放弃 CPU 控制权，系统不会强制切换任务。在这种调度方式下，任务需要主动调用 `taskYIELD()` 函数来放弃 CPU 控制权，以便其他任务能够运行。

虽然协作式调度在某些应用场景中可能不如优先级抢占调度高效，但它仍然是 FreeRTOS 的一个有效选项，适用于需要精确控制任务切换时机的场景。

在 FreeRTOS 中通过配置 `configUSE_PREEMPTION` 为 0 来启用协作式调度。

```c
// FreeRTOSConfig.h
#define configUSE_PREEMPTION 0
```

启用协作式调度后，任务需要主动调用 `taskYIELD()` 函数来放弃 CPU 控制权。

```c
void vTaskFunction(void *pvParameters) {
    for (;;) {
        // 任务代码
        // ...

        // 主动放弃 CPU 控制权
        taskYIELD();
    }
}
```
reference:
[FreeRTOS深入教程（任务创建的深入和任务调度机制分析）](https://bbs.huaweicloud.com/blogs/414127)

## 同步、互斥与通信
`同步:`同步用于协调任务之间的执行顺序或确保多个任务对共享资源的访问是有序的   
同步用于在多个任务之间控制其执行的顺序和时间  

`互斥:`互斥用于确保多个任务不会同时访问共享资源，避免数据竞争  
互斥用于保护共享资源（如全局变量、队列等），确保任务访问这些资源时的原子性，避免数据错误


---

**实现同步与互斥的方法**

FreeRTOS 提供了多种方法实现同步和互斥：

1. **任务通知（Task Notification）**  
   用于任务之间的轻量级同步，可以用作信号通知。

2. **二值信号量（Binary Semaphore）**  
   用于任务和中断之间的事件通知和同步，或者用于任务间的简单同步。

3. **计数信号量（Counting Semaphore）**  
   用于限制访问共享资源的并发任务数，或者用于任务间的复杂同步机制。

4. **互斥量（Mutex）**  
   用于保护共享资源，确保只有一个任务能够访问该资源，同时防止优先级反转。

5. **递归互斥量（Recursive Mutex）**  
   允许任务递归地获取互斥量，适用于递归函数场景。

6. **事件组（Event Groups）**  
   用于多个任务之间的同步，特别是当任务需要等待多个条件满足时。

7. **队列（Queue）**  
   尽管主要用于任务间通信，队列也可用于同步任务之间的操作，特别是与中断服务例程通信时。

---
各种方法的详细讲解

---
## 队列  
**1. 创建队列**
```c
// 动态分配内存
QueueHandle_t xQueueCreate( UBaseType_t uxQueueLength, UBaseType_t uxItemSize );
```
`uxQueueLength` 队列长度，最多能存放多少个数据(item)  
`uxItemSize`每个数据(item)的大小：以字节为单位  
`retuen` 非 0：成功，返回句柄，以后使用句柄来操作队列  NULL：失败，因为内存不足
```c
// 静态分配内存
QueueHandle_t xQueueCreateStatic(
    UBaseType_t uxQueueLength,
    UBaseType_t uxItemSize,
    uint8_t *pucQueueStorageBuffer,
    StaticQueue_t *pxQueueBuffer
 );
```
`uxQueueLength` 队列长度，最多能存放多少个数据(item)  
`uxItemSize` 每个数据(item)的大小：以字节为单位  
`pucQueueStorageBuffer` 如果 uxItemSize 非 0，pucQueueStorageBuffer 必须
指向一个 uint8_t 数组，
此数组大小至少为`uxQueueLength * uxItemSize`  
`pxQueueBuffer` 必须执行一个 StaticQueue_t 结构体，用来保存队列
的数据结构  
`return` 非 0：成功，返回句柄，以后使用句柄来操作队列
NULL：失败，因为 pxQueueBuffer 为 NULL  

**2. 复位**
```c
 BaseType_t xQueueReset( QueueHandle_t pxQueue);
```
返回值pdPASS(必定成功)

**3. 删除**
```c
void vQueueDelete( QueueHandle_t xQueue );
```
只能删除使用动态方法创建的队列，它会释放内存  

**4.增**  
可以把数据写到队列头部，也可以写到尾部，这些函数有两个版本：在任务中使用、在ISR 中使用
```c
/* 等同于xQueueSendToBack
* 往队列尾部写入数据，如果没有空间，阻塞时间为xTicksToWait
*/
BaseType_t xQueueSend(
    QueueHandle_t xQueue,
    const void *pvItemToQueue,
    TickType_t xTicksToWait
);
/* 
* 往队列尾部写入数据，如果没有空间，阻塞时间为xTicksToWait
*/
BaseType_t xQueueSendToBack(
    QueueHandle_t xQueue,
    const void *pvItemToQueue,
    TickType_t xTicksToWait
);
/* 
* 往队列尾部写入数据，此函数可以在中断函数中使用，不可阻塞
*/
BaseType_t xQueueSendToBackFromISR(
    QueueHandle_t xQueue,
    const void *pvItemToQueue,
    BaseType_t *pxHigherPriorityTaskWoken
);
/* 
* 往队列头部写入数据，如果没有空间，阻塞时间为xTicksToWait
*/
BaseType_t xQueueSendToFront(
    QueueHandle_t xQueue,
    const void *pvItemToQueue,
    TickType_t xTicksToWait
);
/* 
* 往队列头部写入数据，此函数可以在中断函数中使用，不可阻塞
*/
BaseType_t xQueueSendToFrontFromISR(
    QueueHandle_t xQueue,
    const void *pvItemToQueue,
    BaseType_t *pxHigherPriorityTaskWoken
);
```
这些函数用到的参数是类似的  
`xQueue` 队列句柄，要写哪个队列  
`pvItemToQueue`
数据指针，这个数据的值会被复制进队列，
复制多大的数据？在创建队列时已经指定了数据大小  
`xTicksToWait`如果队列满则无法写入新数据，可以让任务进入阻塞状
态  
`xTicksToWait` 表示阻塞的最大时间(Tick Count)。 
如果被设为 0，无法写入数据时函数会立刻返回；
如果被设为 portMAX_DELAY，则会一直阻塞直到有空间可写  
`return` pdPASS：数据成功写入了队列
errQUEUE_FULL：写入失败，因为队列满了。

**5. 读**  
使用 xQueueReceive()函数读队列，读到一个数据后，队列中该数据会被移除。这个函数有两个版本：在任务中使用、在 ISR 中使用。函数原型如下：
```c
BaseType_t xQueueReceive( QueueHandle_t xQueue,
    void * const pvBuffer,
    TickType_t xTicksToWait );
    BaseType_t xQueueReceiveFromISR(
    QueueHandle_t xQueue,
    void *pvBuffer,
    BaseType_t *pxTaskWoken
);
```
`xQueue` 队列句柄，要读哪个队列  
`pvBuffer bufer` 指针，队列的数据会被复制到这个 buffer 在创建队列时已经指定了数据大小  
`xTicksToWait` 果队列空则无法读出数据，可以让任务进入阻塞状态，  
`xTicksToWait` 表示阻塞的最大时间(Tick Count)
如果被设为 0，无法读出数据时函数会立刻返回；如果被设为 portMAX_DELAY，则会一直阻塞直到有数据可写  
`return` pdPASS：从队列读出数据入
errQUEUE_EMPTY：读取失败，因为队列空了。  

**6. 查**
```c
/*
* 返回队列中可用数据的个数
*/
UBaseType_t uxQueueMessagesWaiting( const QueueHandle_t xQueue );
/*
* 返回队列中可用空间的个数
*/
UBaseType_t uxQueueSpacesAvailable( const QueueHandle_t xQueue );

/* 偷看队列
* xQueue: 偷看哪个队列
* pvItemToQueue: 数据地址, 用来保存复制出来的数据
* xTicksToWait: 没有数据的话阻塞一会
* 返回值: pdTRUE表示成功, pdFALSE表示失败
*/
BaseType_t xQueuePeek(
    QueueHandle_t xQueue,
    void * const pvBuffer,
    TickType_t xTicksToWait
);
BaseType_t xQueuePeekFromISR(
    QueueHandle_t xQueue,
    void *pvBuffer,
);
```

**7. 改**  
当队列长度为 1 时，可以使用 xQueueOverwrite()或 xQueueOverwriteFromISR()
来覆盖数据。
注意，队列长度必须为 1。当队列满时，这些函数会覆盖里面的数据，这也以为着这
些函数不会被阻塞。
```c
/* 覆盖队列
* xQueue: 写哪个队列
* pvItemToQueue: 数据地址
* 返回值: pdTRUE表示成功, pdFALSE表示失败
*/
BaseType_t xQueueOverwrite(
    QueueHandle_t xQueue,
    const void * pvItemToQueue
);
BaseType_t xQueueOverwriteFromISR(
    QueueHandle_t xQueue,
    const void * pvItemToQueue,
    BaseType_t *pxHigherPriorityTaskWoken
);
```

## 软件定时器

## 调试与监控


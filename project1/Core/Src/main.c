/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "gpio.h"
#include "string.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_led.h"
#include "bsp_beep.h"
#include "device_manager.h"
#include "board.h"
#include "stdio.h"
#include "hal.h"
#include "board.h"
#include "hal_rtc.h"

#include "ff.h"
#include "diskio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t buffer[5];
uint8_t pbuffer[512]__attribute__((aligned(4)));
//uint8_t pbuuefr[3];
uint8_t pbuffer1[512]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */	
uint8_t shi;

uint8_t wen;
uint8_t aaa=0;
	
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
	



void my_key_handler(struct device *dev, void *event)
{
	key_event_t key_event=*(key_event_t*)event;		
	printf("lalala\r\n");

	printf("evt=%d SHORT_PRESS=%d dev=%s\r\n",key_event, SHORT_PRESS, dev->name);

	if(key_event==SHORT_PRESS)
	{
		//device_write("led1",0);
		device_write("led1",3);
		//printf("short press!\r\n");

	}
	else if(key_event==LONG_PRESS)
	{
		device_write("led1",3);
		//printf("short press!\r\n");

	}
	return;
	//printf("evt=%d SHORT_PRESS=%d dev=%s\r\n",key_event, SHORT_PRESS, dev->name);
}

int fputc(int ch,FILE* f)
{
	HAL_UART_Transmit(&huart1,(uint8_t*)&ch,1,10);
	return ch;
}
	
void dht11(void)
{
	device_read("dht11",buffer);
	hal_delay_ms(5000);
	shi=buffer[0];
	wen=buffer[2];
//	uint16_t a=buffer[0]+buffer[1]+buffer[2]+buffer[3];
//	if(a==buffer[4])
//	{
//		aaa=1;
//	}
//	else
//	{
//		aaa=0;
//	}
	printf("湿度：%d%%,温度%d\r\n",shi,wen);
}


static FATFS fs;        // 文件系统对象
FIL file;        // 文件对象
DIR dir;



void sd_app(void)
{

	uint8_t buf[]="你好FATS，我是顾航硕，你是谁呀\r\n";
	static uint8_t buf1[2580]=0;
	UINT bw;
	UINT br;
	if(disk_initialize(0)!=0) printf("SD初始化失败\r\n");
	if(f_mount(&fs,"0:",1)!=0) printf("SD卡挂载失败\r\n");
	if(f_opendir(&dir,"0:/test")!=0)  printf("SD卡文件夹打开失败\r\n");
	if(f_open(&file,"0:/test/666.txt",FA_WRITE|FA_OPEN_APPEND)!=0) printf("SD卡文件打开失败\r\n");

	if(f_write(&file,buf,strlen(buf),&bw)!=0) printf("SD卡文件写入失败\r\n"); 
	if(f_write(&file,"\r\n",2,&bw)!=0) printf("SD卡文件写入失败\r\n"); 	
	if(f_close(&file)!=0) printf("SD卡文件关闭失败\r\n"); 
	if(f_closedir(&dir)!=0) printf("SD卡文件夹关闭失败\r\n");
	
	
	if(f_opendir(&dir,"0:/test")!=0)  printf("SD卡文件夹打开失败\r\n");
	if(f_open(&file,"0:/test/666.txt",FA_READ)!=0) printf("SD卡文件打开失败\r\n"); 
	
	if(f_read(&file,buf1,sizeof(buf1)-1,&br)!=0) printf("SD卡文件读出失败\r\n"); 
	
	if(f_close(&file)!=0)  printf("SD卡文件读出失败\r\n");
	if(f_closedir(&dir)!=0) printf("SD卡文件夹关闭失败\r\n");
	
	printf("666.txt:\r\n");
	printf("%s\r\n",buf1);
	
	
}
int main(void)
{
	
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  //MX_GPIO_Init();
	MX_USART1_UART_Init();
	MX_DMA_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  //osKernelInitialize();  /* Call init function for freertos objects (in freertos.c) */
  //MX_FREERTOS_Init();

  /* Start scheduler */
  //osKernelStart();
	//gpio_init();
	board_led_init();
	device_open("led1");

	//board_beep_init();
	device_open("beep1");
	board_dht11_init();
	device_open("dht11");
	//printf("hello\r\n");
	board_key_init();
	device_open("key1");
	printf("hello\r\n");
  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	
	device_write("led1",1);
	//hal_rtc_init();
	

//	int aaaa=sd_init();
//	printf("aaaa=%02X\r\n",aaaa);
//		uint32_t *id=SD_HandleStruct.CID;
//	printf("id=%X ",id[0]);
//	printf("%X ",id[1]);
//	printf("%X ",id[2]);
//	printf("%X ",id[3]);
	
//	char name[6];
//	memcpy(name, &id[3], 4); // 假设是小端对齐，可能需要调整字节序
//	name[5] = '\0';
//	printf("Card Name: %s\n", name);
//	
//	//HAL_Delay(2000);
//	aaaa=sd_write(pbuffer1,3000,1);
	
	

	//printf("aaaa  =%d\r\n",aaaa);
	
//	aaaa=sd_read(pbuffer);
//	
//	printf("aaaa  =%d\r\n",aaaa);



//	    // 1. 初始化 SD 卡底层
//    if (disk_initialize(0) != HAL_OK) {
//        printf("SD card init failed!\r\n");
//        while(1);
//    }
//    printf("SD card initialized.\r\n");
//	int staus=disk_status(0);
//		printf("staus=%d\r\n",staus);
		
		
//		uint8_t test_buf[512] __attribute__((aligned(4)));
//		DRESULT res = disk_read(0, test_buf, 0, 1); // 尝试读第 0 块

		
		
//if (res == RES_OK) {
//    // 检查最后两个字节是否为 55 AA
//    if (test_buf[510] == 0x55 && test_buf[511] == 0xAA) {
//        printf("Success! Found 55AA signature.\n");
//    } else {
//        printf("Error: Sector 0 data is wrong! (End: %02X %02X)\n", test_buf[510], test_buf[511]);
//    }
//} else {
//    printf("Error: Disk read failed completely!\n");
//}



		
//    // 2. 挂载文件系统
//    fr = f_mount(&fs, "0:", 1);
//    if (fr != FR_OK) {
//        printf("Mount failed: %d\r\n", fr);
//        while(1);
//    }
//    printf("File system mounted.\r\n");

//    // 3. 创建并写入文件
//    fr = f_open(&file, "test.txt", FA_CREATE_ALWAYS | FA_WRITE);
//    if (fr != FR_OK) {
//        printf("File open failed: %d\r\n", fr);
//        while(1);
//    }

//    char buf[] = "你好呀 FATS,我是顾航硕，终于成功移植并使用你啦嘻嘻!\r\n";
//    UINT bw;
//    fr = f_write(&file, buf, strlen(buf), &bw);
//    if (fr != FR_OK || bw != strlen(buf)) {
//        printf("File write failed: %d\r\n", fr);
//        f_close(&file);
//        while(1);
//    }

//    f_close(&file);
//    printf("File written successfully.\r\n");

//    // 4. 读取文件
//    fr = f_open(&file, "test.txt", FA_READ);
//    if (fr != FR_OK) {
//        printf("File open for read failed: %d\r\n", fr);
//        while(1);
//    }

//    static char rbuf[2580];
//    UINT br;
//    fr = f_read(&file, rbuf, sizeof(rbuf)-1, &br);
//    if (fr != FR_OK) {
//        printf("File read failed: %d\r\n", fr);
//        f_close(&file);
//        while(1);
//    }
//    rbuf[br] = '\0';
//    printf("File read: %s\r\n", rbuf);

//    f_close(&file);
	
  while (1)
  {
		sd_app();
		HAL_Delay(10000);
//		for(uint8_t i=0;i<11;i++)
//		{
//			pbuffer1[i]+=1;
//		}
//		sd_write(pbuffer1,3000,1);
//		memset(pbuffer,0,sizeof(pbuffer));
//		aaaa=sd_read(pbuffer,3000,1);

//		while(aaaa)
//		{
//			aaaa=sd_read(pbuffer,3000,1);
//		}
//		for(int i=0;i<11;i++)
//			printf("%02X ", pbuffer[i]);
//		printf("\n");
////		
//		HAL_Delay(2000);
//			HAL_RTC_GET_Time();
//			hal_delay_ms(2000);
//		device_write("key1",0);
//		HAL_Delay(10);
		
			
			//dht11();
//		device_write("led1",1);
//		device_write("beep1",1);
//		HAL_Delay(100);
//		device_write("led1",0);
//		device_write("beep1",0);
//		HAL_Delay(22);
		
//		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET);
//HAL_Delay(2000);
//HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_RESET);
//HAL_Delay(2000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
//void sd_init(void)
//{
////	SD_HandleTypeDef SDhandleStruct={0};
////	SDhandleStruct.Instance=
////	//SDhandleStruct.Instance=;
////	HA-L_SD_Init(&SDhandleStruct);
//}
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

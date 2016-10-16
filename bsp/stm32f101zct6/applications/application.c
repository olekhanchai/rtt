/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 * 2013-07-12     aozima       update for auto initial.
 */

/**
 * @addtogroup STM32
 */
/*@{*/

#include <board.h>
#include <rtthread.h>
#include "spi_flash_w25qxx.h"
#include "LM096.h"
#include "pm25.h"
#ifdef  RT_USING_COMPONENTS_INIT
#include <components.h>
#endif  /* RT_USING_COMPONENTS_INIT */

#ifdef RT_USING_DFS
/* dfs filesystem:ELM filesystem init */
#include <dfs_elm.h>
/* dfs Filesystem APIs */
#include <dfs_fs.h>
#endif

ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t led_stack[ 512 ];
static struct rt_thread led_thread;
static void led_thread_entry(void* parameter)
{	
	unsigned int count=0;
	rt_hw_led_init();
	while (1)
	{
		/* led1 on */
#ifndef RT_USING_FINSH
		rt_kprintf("led on, count : %d\r\n",count);
#endif
		count++;
		rt_hw_led_on(0);
		rt_thread_delay( RT_TICK_PER_SECOND/2 ); /* sleep 0.5 second and switch to other thread */

		/* led1 off */
#ifndef RT_USING_FINSH
		rt_kprintf("led off\r\n");
#endif
		rt_hw_led_off(0);
		rt_thread_delay( RT_TICK_PER_SECOND/2 );

		if(count==10)
		{
			//ask_pm25();
			count=0;
		}
	}
}
void rt_init_thread_entry(void* parameter)
{
#ifdef RT_USING_COMPONENTS_INIT
	/* initialization RT-Thread Components */
	rt_components_init();
#endif

	/* Filesystem Initialization */
#if defined(RT_USING_DFS) && defined(RT_USING_DFS_ELMFAT)
	rt_hw_spi_init();
	w25qxx_init("sd0","spi10");
	/* mount sd card fat partition 1 as root directory */
	if (dfs_mount("sd0", "/", "elm", 0, 0) == 0)
	{
		rt_kprintf("File System initialized!\n");
	}
	else
		rt_kprintf("File System initialzation failed!\n");
#endif  /* RT_USING_DFS */
	char *test=rt_malloc(2048*200);
	if(test!=RT_NULL)
	{
		memset(test,0,2048*200);
		rt_kprintf("malloc 2048*200 ok\n");
		free(test);
	}
	else
		rt_kprintf("malloc 2048*200 failed\n");
	ssd1306_init();
	/*while(1)
	{
		rt_sprintf(str,"%03d",count);
		clear();
		draw(str,str);
		display();
		rt_thread_delay(1000);
	}*/
	pm25_init();
}

int rt_application_init(void)
{
	rt_thread_t init_thread;

	rt_err_t result;

	/* init led thread */
	result = rt_thread_init(&led_thread,
			"led",
			led_thread_entry,
			RT_NULL,
			(rt_uint8_t*)&led_stack[0],
			sizeof(led_stack),
			20,
			5);
	if (result == RT_EOK)
	{
		rt_thread_startup(&led_thread);
	}

#if (RT_THREAD_PRIORITY_MAX == 32)
	init_thread = rt_thread_create("init",
			rt_init_thread_entry, RT_NULL,
			2048, 8, 20);
#else
	init_thread = rt_thread_create("init",
			rt_init_thread_entry, RT_NULL,
			2048, 80, 20);
#endif

	if (init_thread != RT_NULL)
		rt_thread_startup(init_thread);

	return 0;
}

/*@}*/

/**
 * @file boot.c
 * @brief boot from rt-thread
 * @author RedlightASl (dddbbbdd@foxmail.com)
 * @version 1.0
 * @date 2022-01-23
 * 
 * @copyright Copyright (c) 2022  RedlightASl
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date    <th>Version  <th>Author  <th>Description
 * <tr><td>2022-01-23  <td>1.0      <td>wangh   <td>Content
 * </table>
 */

#include <rthw.h>
#include <rtthread.h>

void rt_application_init(void);
void rt_hw_board_init(void);
int rtthread_startup(void);

#if defined(__CC_ARM) || defined(__CLANG_ARM)
extern int $Super$$main(void);
/* re-define main function */
int $Sub$$main(void)
{
    rtthread_startup();
    return 0;
}
#elif defined(__ICCARM__)
extern int main(void);
/* __low_level_init will auto called by IAR cstartup */
extern void __iar_data_init3(void);
int __low_level_init(void)
{
    // call IAR table copy function.
    __iar_data_init3();
    rtthread_startup();
    return 0;
}
#elif defined(__GNUC__)
/* Add -eentry to arm-none-eabi-gcc argument */
int entry(void)
{
    rtthread_startup();
    return 0;
}
#endif

#ifndef RT_USING_HEAP
/* if there is not enable heap, we should use static thread and stack. */
ALIGN(8)
static rt_uint8_t main_stack[RT_MAIN_THREAD_STACK_SIZE];
struct rt_thread main_thread;
#endif

/* the system main thread */
void main_thread_entry(void *parameter)
{
    extern int main(void);
    extern int $Super$$main(void);

#ifdef RT_USING_COMPONENTS_INIT
    /* RT-Thread components initialization */
    rt_components_init();
#endif
#ifdef RT_USING_SMP
    rt_hw_secondary_cpu_up();
#endif
    /* invoke system main function */
#if defined(__CC_ARM) || defined(__CLANG_ARM)
    $Super$$main(); /* for ARMCC. */
#elif defined(__ICCARM__) || defined(__GNUC__)
    main();
#endif
}

void rt_application_init(void)
{
    rt_thread_t tid;

#ifdef RT_USING_HEAP
    tid = rt_thread_create("main", main_thread_entry, RT_NULL,
                           2048, 10, 20);
    RT_ASSERT(tid != RT_NULL);
#else
    rt_err_t result;

    tid = &main_thread;
    result = rt_thread_init(tid, "main", main_thread_entry, RT_NULL,
                            main_stack, sizeof(main_stack), RT_MAIN_THREAD_PRIORITY, 20);
    RT_ASSERT(result == RT_EOK);

    /* if not define RT_USING_HEAP, using to eliminate the warning */
    (void)result;
#endif

    rt_thread_startup(tid);
}

void art_pi_boot_show_logo(void)
{
    rt_kprintf("  _    _ ______ _____  ___    ____              _            \n");
    rt_kprintf(" | |  | |____  | ____|/ _ \\  |  _ \\            | |           \n");
    rt_kprintf(" | |__| |   / /| |__ | | | | | |_) | ___   ___ | |_ ___ _ __ \n");
    rt_kprintf(" |  __  |  / / |___ \\| | | | |  _ < / _ \\ / _ \\| __/ _ \\ '__|\n");
    rt_kprintf(" | |  | | / /   ___) | |_| | | |_) | (_) | (_) | ||  __/ |   \n");
    rt_kprintf(" |_|  |_|/_/   |____/ \\___/  |____/ \\___/ \\___/ \\__\\___|_|   \n");
    rt_kprintf("\n Powered by RT-Thread.\n");
    rt_kprintf("Reconfigured for H750.\n\n");
}

int rtthread_startup(void)
{
    rt_hw_interrupt_disable();

    /* board level initialization
     * NOTE: please initialize heap inside board initialization.
     */
    rt_hw_board_init();

    /* show bootloader bar */
    art_pi_boot_show_logo();

    /* timer system initialization */
    rt_system_timer_init();

    /* scheduler system initialization */
    rt_system_scheduler_init();

#ifdef RT_USING_SIGNALS
    /* signal system initialization */
    rt_system_signal_init();
#endif

    /* create init_thread */
    rt_application_init();

    /* timer thread initialization */
    rt_system_timer_thread_init();

    /* idle thread initialization */
    rt_thread_idle_init();

#ifdef RT_USING_SMP
    rt_hw_spin_lock(&_cpus_lock);
#endif /*RT_USING_SMP*/

    /* start scheduler */
    rt_system_scheduler_start();

    /* never reach here */
    return 0;
}

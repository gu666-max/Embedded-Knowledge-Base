/**
 * @file    uart_port.h
 * @brief   UART + DMA + IDLE 的移植接口（port 层）
 *
 * ─────────────────────────────────────────────────────────────
 * 分工：
 *   core 层（uart_core.c）   ：帧边界判定、环形缓冲、错误恢复状态机 —— 平台无关
 *   port 层（本文件 + uart_port_stm32f4.c）：寄存器、DMA 流、中断、时钟 —— 平台相关
 *
 * 规矩：
 *   1. core 层不允许出现寄存器名、GPIOx、HAL_、芯片型号。
 *   2. port 层只暴露"能力"，不暴露"寄存器"。
 *   3. 回调在中断上下文执行，core 里对应处理必须短且不得阻塞。
 * ─────────────────────────────────────────────────────────────
 */

#ifndef UART_PORT_H
#define UART_PORT_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    UART_PORT_OK      = 0,
    UART_PORT_TIMEOUT = 1,
    UART_PORT_BUSY    = 2,
    UART_PORT_ERROR   = 3,
} uart_port_status_t;

/* 硬件错误位，对应 core 的恢复策略 */
typedef enum {
    UART_PORT_ERR_OVERRUN = 1u << 0,   /* ORE：数据被覆盖（读太慢） */
    UART_PORT_ERR_FRAME   = 1u << 1,   /* FE ：停止位不对（波特率/干扰） */
    UART_PORT_ERR_NOISE   = 1u << 2,   /* NE ：采样到噪声 */
    UART_PORT_ERR_PARITY  = 1u << 3,   /* PE ：校验错 */
    UART_PORT_ERR_DMA     = 1u << 4,   /* DMA 传输错误 */
} uart_port_err_t;

typedef struct {
    uint32_t instance;      /* 0=USART1, 1=USART2 ... 由 port 解释 */
    uint32_t baudrate;
    uint32_t irq_priority;
    void    *user_ctx;      /* core 的状态指针，port 原样回传 */
} uart_port_cfg_t;

/* ── core 需要实现、由 port 在中断里调用 ──
 * 注意：这两个回调都在 ISR 上下文。core 里只允许置标志 / 记录偏移，
 *       不允许 memcpy 大块数据、不允许 printf、不允许等锁。 */
typedef void (*uart_port_idle_cb_t)(void *ctx, uint32_t received_len);
typedef void (*uart_port_err_cb_t)(void *ctx, uint32_t err_bits);

typedef struct {
    /* 配置引脚/时钟/DMA/中断；不启动接收。失败返回 UART_PORT_ERROR。 */
    uart_port_status_t (*init)(const uart_port_cfg_t *cfg,
                               uart_port_idle_cb_t on_idle,
                               uart_port_err_cb_t  on_err);

    void (*deinit)(void);

    /* 启动一次 DMA 接收（循环模式）。缓冲区由 core 提供，port 不得释放。 */
    uart_port_status_t (*start_rx)(uint8_t *buf, uint32_t len);

    /* 停止接收并关闭 DMA 请求。 */
    void (*stop_rx)(void);

    /* 当前还有多少字节没写进 buf（DMA 剩余计数）。
     * core 用它换算"已收长度"，这是 IDLE 判帧的关键原始数据。 */
    uint32_t (*get_remaining)(void);

    /* 软件复位外设（错误恢复用）：关闭 → 清标志 → 重新使能。不依赖中断。 */
    void (*reset)(void);

    void     (*delay_us)(uint32_t us);
    uint32_t (*tick_ms)(void);

    /* 临界区，必须可嵌套。 */
    uint32_t (*irq_save)(void);
    void     (*irq_restore)(uint32_t state);
} uart_port_t;

const uart_port_t *uart_port_get(void);

#ifdef __cplusplus
}
#endif

#endif /* UART_PORT_H */

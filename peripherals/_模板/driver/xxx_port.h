/**
 * @file    xxx_port.h
 * @brief   平台无关驱动的移植接口（port 层）
 *
 * ─────────────────────────────────────────────────────────────
 * 这个文件是「core / port 分离」的契约模板。
 *
 *    core 层（xxx_core.c）  ：协议、时序逻辑、状态机、错误恢复 —— 平台无关
 *    port 层（本文件 + xxx_port_<platform>.c）：寄存器、延时、中断、锁 —— 平台相关
 *
 * 规矩：
 *   1. core 层不允许出现寄存器名、GPIOx、HAL_ 前缀、芯片型号。
 *   2. port 层函数不得超过 10 个。超过说明抽象切错了。
 *   3. 每个函数的语义约束写在注释里，换平台时必须满足，否则 core 会静默出错。
 *   4. 用 DHT11_Demo 的 dht11_port.h 作对照参考。
 * ─────────────────────────────────────────────────────────────
 */

#ifndef XXX_PORT_H
#define XXX_PORT_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── 返回码：core 层依赖这几个值做状态机分支，不要用平台自带的错误码 ── */
typedef enum {
    XXX_PORT_OK      = 0,
    XXX_PORT_TIMEOUT = 1,   /* 等待条件超时 —— core 据此重试或上报 */
    XXX_PORT_BUSY    = 2,   /* 外设忙 —— core 据此排队而非报错 */
    XXX_PORT_ERROR   = 3,   /* 硬件错误（如总线错误、校验失败） */
} xxx_port_status_t;

/* ── 一次性传给 core 的硬件配置（由应用层填，core 不关心具体含义） ── */
typedef struct {
    uint32_t instance;      /* 外设实例编号，port 层自行解释 */
    uint32_t param_a;       /* 如：波特率 / 分频 / 通道号 */
    uint32_t param_b;       /* 如：中断优先级 / 采样时间 */
    void    *user_ctx;      /* port 层私有数据，core 不触碰 */
} xxx_port_cfg_t;

/* ── core 层需要 port 层提供的全部能力 ── */
typedef struct {
    /* 1. 初始化。只做配置，不启动传输。失败必须返回 XXX_PORT_ERROR。 */
    xxx_port_status_t (*init)(const xxx_port_cfg_t *cfg);

    /* 2. 反初始化。释放引脚/时钟/中断，保证可重复 init。 */
    void (*deinit)(void);

    /* 3. 硬件复位。用于错误恢复；不得依赖中断。 */
    void (*reset)(void);

    /* 4. 发起一次异步传输（非阻塞）。立即返回后由 core 轮询状态。 */
    xxx_port_status_t (*start)(const void *tx, void *rx, uint32_t len);

    /* 5. 查询本次传输状态。返回 BUSY 表示未完；OK 表示全部完成。 */
    xxx_port_status_t (*status)(uint32_t *transferred);

    /* 6. 微秒级延时。误差 ≤5%，且不得使用依赖中断的实现（SysTick 除外）。 */
    void (*delay_us)(uint32_t us);

    /* 7. 毫秒级时间戳。用于超时判定，单调递增即可，允许回绕。 */
    uint32_t (*tick_ms)(void);

    /* 8. 临界区保护。必须支持嵌套（保存并返回原 PRIMASK）。 */
    uint32_t (*irq_save)(void);
    void     (*irq_restore)(uint32_t state);
} xxx_port_t;

/**
 * @brief 由平台实现文件提供（xxx_port_stm32f4.c 等）
 *
 * core 层通过 xxx_port_get() 拿到这张表，不直接引用任何平台符号。
 */
const xxx_port_t *xxx_port_get(void);

#ifdef __cplusplus
}
#endif

#endif /* XXX_PORT_H */

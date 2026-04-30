// verif/tests/custom/hello_mock_uart/hello_mock_uart.c
//
// Bare-metal CVA6 Verilator test:
//   - _putchar() writes to the mock UART THR register
//   - _exit(code) writes the HTIF/tohost completion value
//
// Tested convention:
//   tohost = (exit_code << 1) | 1
// so exit_code 0 writes 1, which the Verilator harness treats as success.

#include <stdint.h>

#define UART_BASE 0x10000000UL
#define UART_THR  0x0UL

static inline void mmio_write8(uintptr_t addr, uint8_t value) {
    *(volatile uint8_t *)addr = value;
}

void _putchar(char c) {
    // The mock_uart prints the low byte written to THR.
    mmio_write8(UART_BASE + UART_THR, (uint8_t)c);
}

static void print(const char *s) {
    while (*s) {
        _putchar(*s++);
    }
}

// The Verilator testharness / SimDTM looks up the address of this symbol
// through +tohost_addr=<nm output>.
volatile uint64_t tohost __attribute__((section(".tohost"))) = 0;
volatile uint64_t fromhost __attribute__((section(".fromhost"))) = 0;

void _exit(int code) __attribute__((noreturn));
void _exit(int code) {
    tohost = (((uint64_t)(uint32_t)code) << 1) | 1ULL;

    // Wait for the testharness to observe tohost and terminate simulation.
    while (1) {
        __asm__ volatile ("wfi");
    }
}

// Small local stack, since we are not using the C runtime startup files.
uint8_t boot_stack[4096] __attribute__((aligned(16)));

int main(void) {
    print("INFO: Hello world!\n");
    return 0;
}

void _start(void) __attribute__((naked, noreturn, section(".text.init")));
void _start(void) {
    __asm__ volatile (
        ".option push\n"
        ".option norelax\n"
        "la sp, boot_stack\n"
        ".option pop\n"
        "li t0, 4096\n"
        "add sp, sp, t0\n"
        "call main\n"
        "tail _exit\n"
    );
}


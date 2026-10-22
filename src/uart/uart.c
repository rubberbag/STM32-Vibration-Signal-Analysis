#include "stm32f4xx.h"
#include "uart.h"

#define UART_BAUD 0x008B

static void USART1_Clock_Enable(void)
{
    /* GPIOA and USART1 CLOCK*/
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
}

static void USART1_GPIO_Init(void)
{
    /* Alternate Function */
    GPIOA->MODER &= ~(3U << (9 * 2));

    GPIOA->MODER |= (2U << (9 * 2)) ;

    /* High Speed */
    GPIOA->OSPEEDR |= (3U << (9 * 2));

    /* Alternater Function 7 (AF7 = USART1) */
    GPIOA->AFR[1] &= ~((0xF << 4) | (0xF << 8));

    GPIOA->AFR[1] |= (7 << 4) | (7 << 8);


    /* no pull on TX (PA9) */
    GPIOA->PUPDR &= ~(3U << (9 * 2));
}

static void USART1_Config(void)
{
    USART1->CR1 = 0;

    USART1->BRR = UART_BAUD; /* 16MHz / 0x008B */

    USART1->CR1 |= USART_CR1_TE;

    USART1->CR1 |= USART_CR1_UE;
}

void uart_init(void)
{
    USART1_Clock_Enable();

    USART1_GPIO_Init();

    USART1_Config();
}

void USART1_SendChar(char c)
{
    while(!(USART1->SR & USART_SR_TXE));
    USART1->DR = c;
}

void USART1_SendString(const char *str)
{
    while(*str)
    {
        USART1_SendChar(*str);
        str++;
    }
    while(!(USART1->SR & USART_SR_TC));
}

void uart_send_byte(uint8_t byte)
{
    // Wait until the Transmit Data Register is empty (TXE flag goes high)
    while (!(USART1->SR & USART_SR_TXE)) {
        // Safe hardware polling block
    }

    // Write to the actual DR register to trigger peripheral execution
    USART1->DR = (byte & 0xFFUL);
}

void uart_transmit_packet(const struct DiagnosticPacket *packet)
{
    if (packet == NULL) return;

    // Direct byte-by-byte memory tracking over wire
    const uint8_t *raw_packet_bytes = (const uint8_t *)packet;
    size_t packet_total_size = sizeof(struct DiagnosticPacket);

    for (size_t i = 0; i < packet_total_size; i++) {
        uart_send_byte(raw_packet_bytes[i]);
    }
}

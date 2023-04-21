#include "stm8s.h"
#include "delay.h"

/**
 * @brief  Configure Serial monitor.
 * @param  BaudRate : Speed of serial monitor.
 * @retval None
 */
void Serial_Begin(uint32_t BaudRate)
{
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART2, ENABLE);
    UART2_Init(BaudRate, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, UART2_PARITY_NO, UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
    UART2_Cmd(ENABLE);
    delay_ms_2(50); //! TADY MŮŽE NASTAT PROBBLÉM podle logické analýzátoru je delay(50) = 63,17 ms
    // TODO TADY JE TŘEBA VYŘEŠIT tento delay
}

/**
 * @brief  Print string to serial monitor.
 * @param  message : message to write.
 * @retval None
 */
void Serial_Print_String(char message[])
{
    char i = 0;

    while (message[i] != 0x00)
    {
        UART2_SendData8(message[i]);

        while (UART2_GetFlagStatus(UART2_FLAG_TXE) == RESET)
            ;
        i++;
    }
}

void Serial_Print_Int(int number)
{
    char count = 0;
    char digit[5] = "";
    while (number != 0)
    {
        digit[count] = number % 10;
        count++;
        number = number / 10;
    }
    {
        UART2_SendData8(digit[count - 1] + 0x30);
        while (UART2_GetFlagStatus(UART2_FLAG_TXE) == RESET)
            ;
        count--;
    }
}

char Serial_Read_Char()
{
    while (UART2_GetFlagStatus(UART2_FLAG_RXNE) == RESET)
        ;
    UART2_ClearFlag(UART2_FLAG_RXNE);

    return UART2_ReceiveData8();
}

/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
#include <stdio.h>



int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    // Initialize USB
    USBUART_Start(0u, USBUART_3V_OPERATION);

    int16 temperature;
    cystatus Status;
    uint8 count;
    
    LCD_Char_1_Start();
    
    LCD_Char_1_Position(1u, 0u);
    LCD_Char_1_PrintString("OKKK");
    
    CyGlobalIntEnable; 
    
    // Wait for USB
    while (!USBUART_GetConfiguration());
    USBUART_CDC_Init();

    char8 usbBuffer[64] = "";
    uint8 buffer[64] = "";
    
    for(;;)
    {
        /* Place your application code here. */
        
         CyDelay(50);
         LCD_Char_1_Position(0u, 0u);
         LCD_Char_1_PrintString("Aguardando");
       
        // Wait for temperature read command
        if(USBUART_DataIsReady() != 0u)               /* Check for input data from PC */
        {   
            count = USBUART_GetAll(buffer);           /* Read received data and re-enable OUT endpoint */
          
            // Read data from sensor
            
            Status = DieTemp_GetTemp(&temperature);

             /* Displaying the Die Temperature value on the LCD */
            LCD_Char_1_Position(0u, 0u);
            LCD_Char_1_PrintString("Temp = ");

            if (temperature >= 0 )
            {
                LCD_Char_1_PrintString("+");
            }
            else
            {
                /* If the temperature value is negative, display "-" sign and make value positive */
                LCD_Char_1_PrintString("-");
                temperature = (uint16)(~temperature + 1u);
            }
            
            LCD_Char_1_PrintNumber((uint16) (temperature));
            LCD_Char_1_PrintString(" ");
            
            LCD_Char_1_PutChar(LCD_Char_1_CUSTOM_7);
            LCD_Char_1_PrintString("C");

            LCD_Char_1_Position(1u, 0u);            
            LCD_Char_1_PrintNumber(buffer[0]);
            
            
            sprintf(usbBuffer,"%d\n", temperature);

            // Send the data read
            while(USBUART_CDCIsReady() == 0u);
            USBUART_PutString(usbBuffer);
        }
    }
}

/* [] END OF FILE */

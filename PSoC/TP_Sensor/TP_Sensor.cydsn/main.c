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
    
    
    CyGlobalIntEnable; 
    
    // Wait for USB
    while (!USBUART_GetConfiguration());
    USBUART_CDC_Init();

    char8 usbBuffer[64] = "";
    uint8 buffer[64] = "";
    
    for(;;)
    {
        /* Place your application code here. */
        
       
        // Wait for temperature read command
        if(USBUART_DataIsReady() != 0u)               /* Check for input data from PC */
        {   
            count = USBUART_GetAll(buffer);           /* Read received data and re-enable OUT endpoint */
          
            // Read data from sensor
            
            Status = DieTemp_GetTemp(&temperature);

    
            
            sprintf(usbBuffer,"%d\n", temperature);

            // Send the data read
            while(USBUART_CDCIsReady() == 0u);
            USBUART_PutString(usbBuffer);
        }
    }
}

/* [] END OF FILE */

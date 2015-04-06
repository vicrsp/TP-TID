# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Users\Victor Ruela\Desktop\UFMG\2015-1\Integracao de Dados\TP pratico\TP-TID\PSoC\TP_Sensor\TP_Sensor.cydsn\TP_Sensor.cyprj
# Date: Mon, 06 Apr 2015 19:47:22 GMT
#set_units -time ns
create_clock -name {CyIMO} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyPLL_OUT} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/pllout}]]
create_clock -name {CyILO} -period 10000 -waveform {0 5000} [list [get_pins {ClockBlock/ilo}] [get_pins {ClockBlock/clk_100k}] [get_pins {ClockBlock/clk_1k}] [get_pins {ClockBlock/clk_32k}]]
create_clock -name {CyMASTER_CLK} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/clk_sync}]]
create_generated_clock -name {CyBUS_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2 3} [list [get_pins {ClockBlock/clk_bus_glb}]]


# Component constraints for C:\Users\Victor Ruela\Desktop\UFMG\2015-1\Integracao de Dados\TP pratico\TP-TID\PSoC\TP_Sensor\TP_Sensor.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\Victor Ruela\Desktop\UFMG\2015-1\Integracao de Dados\TP pratico\TP-TID\PSoC\TP_Sensor\TP_Sensor.cydsn\TP_Sensor.cyprj
# Date: Mon, 06 Apr 2015 19:47:19 GMT
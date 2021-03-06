EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 4
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L sim7070-rescue:R-Device R?
U 1 1 5F91C2EA
P 1700 2100
AR Path="/5F76B017/5F91C2EA" Ref="R?"  Part="1" 
AR Path="/5F91C2EA" Ref="R?"  Part="1" 
AR Path="/5F8E40AD/5F91C2EA" Ref="R8"  Part="1" 
F 0 "R8" H 1770 2146 50  0000 L CNN
F 1 "1M" H 1770 2055 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 1630 2100 50  0001 C CNN
F 3 "~" H 1700 2100 50  0001 C CNN
F 4 "C26087" H 1700 2100 50  0001 C CNN "LCSC"
	1    1700 2100
	1    0    0    -1  
$EndComp
$Comp
L sim7070-rescue:R-Device R?
U 1 1 5F91C2F0
P 1700 2450
AR Path="/5F76B017/5F91C2F0" Ref="R?"  Part="1" 
AR Path="/5F91C2F0" Ref="R?"  Part="1" 
AR Path="/5F8E40AD/5F91C2F0" Ref="R12"  Part="1" 
F 0 "R12" H 1770 2496 50  0000 L CNN
F 1 "1M" H 1770 2405 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 1630 2450 50  0001 C CNN
F 3 "~" H 1700 2450 50  0001 C CNN
F 4 "C26087" H 1700 2450 50  0001 C CNN "LCSC"
	1    1700 2450
	1    0    0    -1  
$EndComp
$Comp
L sim7070-rescue:GND-power #PWR?
U 1 1 5F91C2F6
P 1700 2700
AR Path="/5F76B017/5F91C2F6" Ref="#PWR?"  Part="1" 
AR Path="/5F91C2F6" Ref="#PWR?"  Part="1" 
AR Path="/5F8E40AD/5F91C2F6" Ref="#PWR0134"  Part="1" 
F 0 "#PWR0134" H 1700 2450 50  0001 C CNN
F 1 "GND" H 1705 2527 50  0000 C CNN
F 2 "" H 1700 2700 50  0001 C CNN
F 3 "" H 1700 2700 50  0001 C CNN
	1    1700 2700
	1    0    0    -1  
$EndComp
$Comp
L sim7070-rescue:CP-Device C?
U 1 1 5F91C2FC
P 1450 2500
AR Path="/5F76B017/5F91C2FC" Ref="C?"  Part="1" 
AR Path="/5F91C2FC" Ref="C?"  Part="1" 
AR Path="/5F8E40AD/5F91C2FC" Ref="C12"  Part="1" 
F 0 "C12" H 1568 2546 50  0000 L CNN
F 1 "100n" H 1568 2455 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 1488 2350 50  0001 C CNN
F 3 "~" H 1450 2500 50  0001 C CNN
F 4 "C1525" H 1450 2500 50  0001 C CNN "LCSC"
	1    1450 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 2250 1450 2250
Wire Wire Line
	1450 2250 1450 2350
Wire Wire Line
	1700 2300 1700 2250
Connection ~ 1700 2250
Wire Wire Line
	1700 1950 1700 1850
Wire Wire Line
	1700 2700 1700 2650
Wire Wire Line
	1450 2650 1700 2650
Connection ~ 1700 2650
Wire Wire Line
	1700 2650 1700 2600
Text GLabel 1700 1850 2    50   Input ~ 0
VBAT_CONN
Text GLabel 1450 2250 0    50   Input ~ 0
VBAT_SENSE
$Comp
L sim7070-rescue:C_Small-Device C15
U 1 1 5F91C35D
P 9300 1450
F 0 "C15" H 9392 1496 50  0000 L CNN
F 1 "100n" H 9392 1405 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 9300 1450 50  0001 C CNN
F 3 "~" H 9300 1450 50  0001 C CNN
F 4 "C1525" H 9300 1450 50  0001 C CNN "LCSC"
	1    9300 1450
	1    0    0    -1  
$EndComp
$Comp
L sim7070-rescue:C_Small-Device C14
U 1 1 5F91C367
P 9100 1450
F 0 "C14" H 9192 1496 50  0000 L CNN
F 1 "100n" H 9192 1405 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 9100 1450 50  0001 C CNN
F 3 "~" H 9100 1450 50  0001 C CNN
F 4 "C1525" H 9100 1450 50  0001 C CNN "LCSC"
	1    9100 1450
	1    0    0    -1  
$EndComp
Connection ~ 9100 1350
Wire Wire Line
	9100 1350 8850 1350
$Comp
L sim7070-rescue:C_Small-Device C13
U 1 1 5F91C36F
P 8850 1450
F 0 "C13" H 8942 1496 50  0000 L CNN
F 1 "100n" H 8942 1405 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 8850 1450 50  0001 C CNN
F 3 "~" H 8850 1450 50  0001 C CNN
F 4 "C1525" H 8850 1450 50  0001 C CNN "LCSC"
	1    8850 1450
	1    0    0    -1  
$EndComp
Connection ~ 8850 1350
Wire Wire Line
	9300 1550 9100 1550
Connection ~ 9100 1550
Wire Wire Line
	9100 1550 8950 1550
$Comp
L sim7070-rescue:GND-power #PWR0138
U 1 1 5F91C379
P 8950 1600
F 0 "#PWR0138" H 8950 1350 50  0001 C CNN
F 1 "GND" H 8955 1427 50  0000 C CNN
F 2 "" H 8950 1600 50  0001 C CNN
F 3 "" H 8950 1600 50  0001 C CNN
	1    8950 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	8950 1600 8950 1550
Connection ~ 8950 1550
Wire Wire Line
	8950 1550 8850 1550
Wire Wire Line
	8600 1350 8850 1350
Wire Wire Line
	8850 1550 8600 1550
Connection ~ 8850 1550
Text GLabel 7700 2050 2    50   Input ~ 0
MCU_RX
Text GLabel 7700 2150 2    50   Input ~ 0
MCU_TX
Text GLabel 7700 1850 2    50   Input ~ 0
TS_SENSE
Text GLabel 9200 3600 0    50   Input ~ 0
TS_SENSE
Text GLabel 7700 3050 2    50   Input ~ 0
VBAT_SENSE
Text GLabel 7700 1450 2    50   Input ~ 0
LED_1
Text GLabel 7700 1550 2    50   Input ~ 0
LED_0
$Comp
L sim7070-rescue:C_Small-Device C11
U 1 1 5F91C3ED
P 8600 1450
F 0 "C11" H 8692 1496 50  0000 L CNN
F 1 "4u7" H 8692 1405 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 8600 1450 50  0001 C CNN
F 3 "~" H 8600 1450 50  0001 C CNN
	1    8600 1450
	1    0    0    -1  
$EndComp
Text GLabel 7700 2750 2    50   Input ~ 0
LED_LOGO
Wire Wire Line
	9100 1350 9300 1350
Text GLabel 7700 2450 2    50   Input ~ 0
SDA
Text GLabel 7700 2550 2    50   Input ~ 0
SCL
Text GLabel 3150 4900 2    50   Input ~ 0
MOTION_SENSOR_IT
Text GLabel 1700 5200 0    50   Input ~ 0
SDA
Text GLabel 1700 5100 0    50   Input ~ 0
SCL
Text GLabel 7700 2950 2    50   Input ~ 0
MCU_MODEM_PWRKEY
$Comp
L sim7070-rescue:CP-Device C?
U 1 1 5FC71181
P 950 5250
AR Path="/5F76B017/5FC71181" Ref="C?"  Part="1" 
AR Path="/5FC71181" Ref="C?"  Part="1" 
AR Path="/5F8E40AD/5FC71181" Ref="C23"  Part="1" 
F 0 "C23" H 1068 5296 50  0000 L CNN
F 1 "100n" H 1068 5205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 988 5100 50  0001 C CNN
F 3 "~" H 950 5250 50  0001 C CNN
F 4 "C1525" H 950 5250 50  0001 C CNN "LCSC"
	1    950  5250
	-1   0    0    1   
$EndComp
$Comp
L sim7070-rescue:CP-Device C?
U 1 1 5FC7220C
P 1200 5250
AR Path="/5F76B017/5FC7220C" Ref="C?"  Part="1" 
AR Path="/5FC7220C" Ref="C?"  Part="1" 
AR Path="/5F8E40AD/5FC7220C" Ref="C24"  Part="1" 
F 0 "C24" H 1318 5296 50  0000 L CNN
F 1 "100n" H 1318 5205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 1238 5100 50  0001 C CNN
F 3 "~" H 1200 5250 50  0001 C CNN
F 4 "C1525" H 1200 5250 50  0001 C CNN "LCSC"
	1    1200 5250
	-1   0    0    1   
$EndComp
$Comp
L sim7070-rescue:GND-power #PWR?
U 1 1 5FC72478
P 1100 5500
AR Path="/5F76B017/5FC72478" Ref="#PWR?"  Part="1" 
AR Path="/5FC72478" Ref="#PWR?"  Part="1" 
AR Path="/5F8E40AD/5FC72478" Ref="#PWR0155"  Part="1" 
F 0 "#PWR0155" H 1100 5250 50  0001 C CNN
F 1 "GND" H 1105 5327 50  0000 C CNN
F 2 "" H 1100 5500 50  0001 C CNN
F 3 "" H 1100 5500 50  0001 C CNN
	1    1100 5500
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1100 5500 1100 5400
Wire Wire Line
	1100 5400 1200 5400
Wire Wire Line
	1100 5400 950  5400
Connection ~ 1100 5400
Wire Wire Line
	950  5100 1050 5100
Wire Wire Line
	1050 5000 1050 5100
Connection ~ 1050 5100
Wire Wire Line
	1050 5100 1200 5100
Text GLabel 6100 1850 0    50   Input ~ 0
MOTION_SENSOR_IT
$Comp
L sim7070-rescue:GND-power #PWR0158
U 1 1 5FD6DE94
P 7700 3350
F 0 "#PWR0158" H 7700 3100 50  0001 C CNN
F 1 "GND" H 7705 3177 50  0000 C CNN
F 2 "" H 7700 3350 50  0001 C CNN
F 3 "" H 7700 3350 50  0001 C CNN
	1    7700 3350
	1    0    0    -1  
$EndComp
$Comp
L sim7070-rescue:Conn_02x02_Odd_Even-Connector_Generic J6
U 1 1 608DAD79
P 10200 2250
F 0 "J6" H 10250 2467 50  0000 C CNN
F 1 "Conn_02x02_Odd_Even" H 10250 2376 50  0000 C CNN
F 2 "GettingStarted:PinSocket_2x02_P2.54mm_Vertical_centered" H 10200 2250 50  0001 C CNN
F 3 "~" H 10200 2250 50  0001 C CNN
F 4 "C239342" H 10200 2250 50  0001 C CNN "LCSC"
	1    10200 2250
	1    0    0    -1  
$EndComp
NoConn ~ 10000 2350
NoConn ~ 10500 2250
NoConn ~ 10500 2350
$Comp
L sim7070-rescue:Conn_02x02_Odd_Even-Connector_Generic J8
U 1 1 608DE384
P 10200 1450
F 0 "J8" H 10250 1667 50  0000 C CNN
F 1 "Conn_02x02_Odd_Even" H 10250 1576 50  0000 C CNN
F 2 "GettingStarted:PinSocket_2x02_P2.54mm_Vertical_centered" H 10200 1450 50  0001 C CNN
F 3 "~" H 10200 1450 50  0001 C CNN
F 4 "C239342" H 10200 1450 50  0001 C CNN "LCSC"
	1    10200 1450
	1    0    0    -1  
$EndComp
NoConn ~ 10000 1450
NoConn ~ 10000 1550
NoConn ~ 10500 1450
NoConn ~ 10500 1550
Text Notes 10150 1150 0    50   ~ 0
for mounting\n
$Comp
L sim7070-rescue:Conn_02x02_Odd_Even-Connector_Generic J9
U 1 1 608E9FA8
P 10200 1850
F 0 "J9" H 10250 2067 50  0000 C CNN
F 1 "Conn_02x02_Odd_Even" H 10250 1976 50  0000 C CNN
F 2 "GettingStarted:PinSocket_2x02_P2.54mm_Vertical_centered" H 10200 1850 50  0001 C CNN
F 3 "~" H 10200 1850 50  0001 C CNN
F 4 "C239342" H 10200 1850 50  0001 C CNN "LCSC"
	1    10200 1850
	1    0    0    -1  
$EndComp
NoConn ~ 10000 2250
$Comp
L sim7070-rescue:Conn_02x01-Connector_Generic J10
U 1 1 60910568
P 9650 3600
F 0 "J10" H 9700 3817 50  0000 C CNN
F 1 "Conn_02x01" H 9700 3726 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 9650 3600 50  0001 C CNN
F 3 "~" H 9650 3600 50  0001 C CNN
F 4 "C124411" H 9650 3600 50  0001 C CNN "LCSC"
	1    9650 3600
	1    0    0    -1  
$EndComp
NoConn ~ 9950 3600
$Comp
L sim7070-rescue:KXTJ3-1057-KXTJ3-1057 U8
U 1 1 6077AA1B
P 2300 5100
F 0 "U8" H 2300 5867 50  0000 C CNN
F 1 "KXTJ3-1057" H 2300 5776 50  0000 C CNN
F 2 "GettingStarted:KIONIX_KXTJ3-1057" H 2300 5100 50  0001 L BNN
F 3 "" H 2300 5100 50  0001 L BNN
F 4 "4.0" H 2300 5100 50  0001 L BNN "PARTREV"
F 5 "Manufacturer Recommendations" H 2300 5100 50  0001 L BNN "STANDARD"
F 6 "KIONIX" H 2300 5100 50  0001 L BNN "MANUFACTURER"
F 7 "C442603" H 2300 5100 50  0001 C CNN "LCSC"
	1    2300 5100
	1    0    0    -1  
$EndComp
$Comp
L sim7070-rescue:GND-power #PWR?
U 1 1 6077ADF5
P 2900 5600
AR Path="/5F76B017/6077ADF5" Ref="#PWR?"  Part="1" 
AR Path="/6077ADF5" Ref="#PWR?"  Part="1" 
AR Path="/5F8E40AD/6077ADF5" Ref="#PWR0115"  Part="1" 
F 0 "#PWR0115" H 2900 5350 50  0001 C CNN
F 1 "GND" H 2905 5427 50  0000 C CNN
F 2 "" H 2900 5600 50  0001 C CNN
F 3 "" H 2900 5600 50  0001 C CNN
	1    2900 5600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 4600 2900 4700
Connection ~ 2900 4600
$Comp
L sim7070-rescue:GND-power #PWR?
U 1 1 6077CC71
P 1400 4900
AR Path="/5F76B017/6077CC71" Ref="#PWR?"  Part="1" 
AR Path="/6077CC71" Ref="#PWR?"  Part="1" 
AR Path="/5F8E40AD/6077CC71" Ref="#PWR0121"  Part="1" 
F 0 "#PWR0121" H 1400 4650 50  0001 C CNN
F 1 "GND" H 1405 4727 50  0000 C CNN
F 2 "" H 1400 4900 50  0001 C CNN
F 3 "" H 1400 4900 50  0001 C CNN
	1    1400 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	1400 4900 1700 4900
Wire Wire Line
	2900 5600 2900 5400
Connection ~ 2900 5600
$Comp
L sim7070-rescue:ESP32-WROOM-32-ESP32-WROOM-32 U6
U 1 1 60A346B3
P 6900 2150
F 0 "U6" H 6900 3517 50  0000 C CNN
F 1 "ESP32-WROOM-32" H 6900 3426 50  0000 C CNN
F 2 "SnapEDA Library:MODULE_ESP32-WROOM-32" H 6900 2150 50  0001 L BNN
F 3 "" H 6900 2150 50  0001 L BNN
F 4 "Espressif Systems" H 6900 2150 50  0001 L BNN "MANUFACTURER"
F 5 "2.9" H 6900 2150 50  0001 L BNN "PARTREV"
F 6 "Manufacturer Recommendations" H 6900 2150 50  0001 L BNN "STANDARD"
F 7 "3.2 mm" H 6900 2150 50  0001 L BNN "MAXIMUM_PACKAGE_HEIGHT"
F 8 "C82899" H 6900 2150 50  0001 C CNN "LCSC"
	1    6900 2150
	1    0    0    -1  
$EndComp
$Comp
L sim7070-rescue:+3V3-power #PWR0122
U 1 1 60A3BCB0
P 9100 1150
F 0 "#PWR0122" H 9100 1000 50  0001 C CNN
F 1 "+3V3" H 9115 1323 50  0000 C CNN
F 2 "" H 9100 1150 50  0001 C CNN
F 3 "" H 9100 1150 50  0001 C CNN
	1    9100 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9100 1150 9100 1350
$Comp
L sim7070-rescue:+3V3-power #PWR0127
U 1 1 60A3D771
P 7750 1000
F 0 "#PWR0127" H 7750 850 50  0001 C CNN
F 1 "+3V3" H 7765 1173 50  0000 C CNN
F 2 "" H 7750 1000 50  0001 C CNN
F 3 "" H 7750 1000 50  0001 C CNN
	1    7750 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	7750 1000 7700 1000
Wire Wire Line
	7700 1000 7700 1050
Wire Wire Line
	9200 3600 9450 3600
$Comp
L sim7070-rescue:CH340C-Interface_USB U5
U 1 1 60A3F473
P 3550 2350
F 0 "U5" H 3550 1661 50  0000 C CNN
F 1 "CH340C" H 3550 1570 50  0000 C CNN
F 2 "Package_SO:SOIC-16_3.9x9.9mm_P1.27mm" H 3600 1800 50  0001 L CNN
F 3 "https://datasheet.lcsc.com/szlcsc/Jiangsu-Qin-Heng-CH340C_C84681.pdf" H 3200 3150 50  0001 C CNN
F 4 "C84681" H 3550 2350 50  0001 C CNN "LCSC"
	1    3550 2350
	1    0    0    -1  
$EndComp
$Comp
L sim7070-rescue:CP-Device C?
U 1 1 60A3FD81
P 9400 2150
AR Path="/5F76B017/60A3FD81" Ref="C?"  Part="1" 
AR Path="/60A3FD81" Ref="C?"  Part="1" 
AR Path="/5F8E40AD/60A3FD81" Ref="C18"  Part="1" 
F 0 "C18" H 9518 2196 50  0000 L CNN
F 1 "100n" H 9518 2105 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 9438 2000 50  0001 C CNN
F 3 "~" H 9400 2150 50  0001 C CNN
F 4 "C1525" H 9400 2150 50  0001 C CNN "LCSC"
	1    9400 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 1400 3550 1750
$Comp
L sim7070-rescue:GND-power #PWR0131
U 1 1 60A41832
P 3550 3050
F 0 "#PWR0131" H 3550 2800 50  0001 C CNN
F 1 "GND" H 3555 2877 50  0000 C CNN
F 2 "" H 3550 3050 50  0001 C CNN
F 3 "" H 3550 3050 50  0001 C CNN
	1    3550 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 3050 3550 2950
Text HLabel 3150 2250 0    50   Input ~ 0
D+
Text HLabel 3150 2350 0    50   Input ~ 0
D-
Text HLabel 3550 1400 1    50   Input ~ 0
V_USB
$Comp
L sim7070-rescue:CP-Device C?
U 1 1 60A60B39
P 8950 2150
AR Path="/5F76B017/60A60B39" Ref="C?"  Part="1" 
AR Path="/60A60B39" Ref="C?"  Part="1" 
AR Path="/5F8E40AD/60A60B39" Ref="C16"  Part="1" 
F 0 "C16" H 9068 2196 50  0000 L CNN
F 1 "100n" H 9068 2105 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 8988 2000 50  0001 C CNN
F 3 "~" H 8950 2150 50  0001 C CNN
F 4 "C1525" H 8950 2150 50  0001 C CNN "LCSC"
	1    8950 2150
	1    0    0    -1  
$EndComp
$Comp
L sim7070-rescue:GND-power #PWR0135
U 1 1 60A6153B
P 8950 2300
F 0 "#PWR0135" H 8950 2050 50  0001 C CNN
F 1 "GND" H 8955 2127 50  0000 C CNN
F 2 "" H 8950 2300 50  0001 C CNN
F 3 "" H 8950 2300 50  0001 C CNN
	1    8950 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9400 2300 8950 2300
Connection ~ 8950 2300
Text HLabel 9400 2000 1    50   Input ~ 0
V_USB
Text GLabel 4300 1950 2    50   Input ~ 0
ESP_PROG_RX
Text GLabel 4300 2050 2    50   Input ~ 0
ESP_PROG_TX
Text Label 3450 1650 0    50   ~ 0
V3
Text Label 9050 1950 0    50   ~ 0
V3
Wire Wire Line
	9050 1950 8950 1950
Wire Wire Line
	8950 1950 8950 2000
Wire Wire Line
	3450 1650 3450 1750
$Comp
L sim7070-rescue:R_Small-Device R14
U 1 1 60A69B36
P 4150 1950
F 0 "R14" V 3954 1950 50  0000 C CNN
F 1 "4k7" V 4045 1950 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric" H 4150 1950 50  0001 C CNN
F 3 "~" H 4150 1950 50  0001 C CNN
F 4 "C25900" V 4150 1950 50  0001 C CNN "LCSC"
	1    4150 1950
	0    1    1    0   
$EndComp
$Comp
L sim7070-rescue:R_Small-Device R19
U 1 1 60A6AC4C
P 4150 2050
F 0 "R19" V 3954 2050 50  0000 C CNN
F 1 "4k7" V 4045 2050 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric" H 4150 2050 50  0001 C CNN
F 3 "~" H 4150 2050 50  0001 C CNN
F 4 "C25900" V 4150 2050 50  0001 C CNN "LCSC"
	1    4150 2050
	0    1    1    0   
$EndComp
Wire Wire Line
	3950 1950 4050 1950
Wire Wire Line
	4250 1950 4300 1950
Wire Wire Line
	4300 2050 4250 2050
Wire Wire Line
	4050 2050 3950 2050
$Comp
L sim7070-rescue:MMSS8050-H-TP-dk_Transistors-Bipolar-BJT-Single Q2
U 1 1 60A6F584
P 5700 4900
F 0 "Q2" H 5888 4953 60  0000 L CNN
F 1 "MMSS8050-H-TP" H 5888 4847 60  0000 L CNN
F 2 "digikey-footprints:SOT-23-3" H 5900 5100 60  0001 L CNN
F 3 "https://www.mccsemi.com/pdf/Products/MMSS8050(SOT-23).pdf" H 5900 5200 60  0001 L CNN
F 4 "MMSS8050-H-TPMSCT-ND" H 5900 5300 60  0001 L CNN "Digi-Key_PN"
F 5 "MMSS8050-H-TP" H 5900 5400 60  0001 L CNN "MPN"
F 6 "Discrete Semiconductor Products" H 5900 5500 60  0001 L CNN "Category"
F 7 "Transistors - Bipolar (BJT) - Single" H 5900 5600 60  0001 L CNN "Family"
F 8 "https://www.mccsemi.com/pdf/Products/MMSS8050(SOT-23).pdf" H 5900 5700 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/micro-commercial-co/MMSS8050-H-TP/MMSS8050-H-TPMSCT-ND/2825945" H 5900 5800 60  0001 L CNN "DK_Detail_Page"
F 10 "TRANS NPN 25V 1.5A SOT23" H 5900 5900 60  0001 L CNN "Description"
F 11 "Micro Commercial Co" H 5900 6000 60  0001 L CNN "Manufacturer"
F 12 "Active" H 5900 6100 60  0001 L CNN "Status"
F 13 "c2150" H 5700 4900 50  0001 C CNN "LCSC"
	1    5700 4900
	1    0    0    -1  
$EndComp
$Comp
L sim7070-rescue:MMSS8050-H-TP-dk_Transistors-Bipolar-BJT-Single Q8
U 1 1 60A7019E
P 5700 5500
F 0 "Q8" H 5888 5447 60  0000 L CNN
F 1 "MMSS8050-H-TP" H 5888 5553 60  0000 L CNN
F 2 "digikey-footprints:SOT-23-3" H 5900 5700 60  0001 L CNN
F 3 "https://www.mccsemi.com/pdf/Products/MMSS8050(SOT-23).pdf" H 5900 5800 60  0001 L CNN
F 4 "MMSS8050-H-TPMSCT-ND" H 5900 5900 60  0001 L CNN "Digi-Key_PN"
F 5 "MMSS8050-H-TP" H 5900 6000 60  0001 L CNN "MPN"
F 6 "Discrete Semiconductor Products" H 5900 6100 60  0001 L CNN "Category"
F 7 "Transistors - Bipolar (BJT) - Single" H 5900 6200 60  0001 L CNN "Family"
F 8 "https://www.mccsemi.com/pdf/Products/MMSS8050(SOT-23).pdf" H 5900 6300 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/micro-commercial-co/MMSS8050-H-TP/MMSS8050-H-TPMSCT-ND/2825945" H 5900 6400 60  0001 L CNN "DK_Detail_Page"
F 10 "TRANS NPN 25V 1.5A SOT23" H 5900 6500 60  0001 L CNN "Description"
F 11 "Micro Commercial Co" H 5900 6600 60  0001 L CNN "Manufacturer"
F 12 "Active" H 5900 6700 60  0001 L CNN "Status"
F 13 "c2150" H 5700 5500 50  0001 C CNN "LCSC"
	1    5700 5500
	1    0    0    1   
$EndComp
Text Label 5900 5750 0    50   ~ 0
ESP_IO0
Text Label 7850 1250 0    50   ~ 0
ESP_IO0
Wire Wire Line
	7850 1250 7700 1250
Wire Wire Line
	5800 5700 5800 5750
Wire Wire Line
	5800 5750 5900 5750
$Comp
L sim7070-rescue:R-Device R?
U 1 1 60A7296A
P 5250 5500
AR Path="/5F76B017/60A7296A" Ref="R?"  Part="1" 
AR Path="/60A7296A" Ref="R?"  Part="1" 
AR Path="/5F8E40AD/60A7296A" Ref="R21"  Part="1" 
F 0 "R21" H 5320 5546 50  0000 L CNN
F 1 "10k" H 5320 5455 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 5180 5500 50  0001 C CNN
F 3 "~" H 5250 5500 50  0001 C CNN
F 4 "C25531" H 5250 5500 50  0001 C CNN "LCSC"
	1    5250 5500
	0    1    1    0   
$EndComp
$Comp
L sim7070-rescue:R-Device R?
U 1 1 60A72C64
P 5250 4900
AR Path="/5F76B017/60A72C64" Ref="R?"  Part="1" 
AR Path="/60A72C64" Ref="R?"  Part="1" 
AR Path="/5F8E40AD/60A72C64" Ref="R20"  Part="1" 
F 0 "R20" H 5320 4946 50  0000 L CNN
F 1 "10k" H 5320 4855 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 5180 4900 50  0001 C CNN
F 3 "~" H 5250 4900 50  0001 C CNN
F 4 "C25531" H 5250 4900 50  0001 C CNN "LCSC"
	1    5250 4900
	0    1    1    0   
$EndComp
Wire Wire Line
	5400 4900 5500 4900
Wire Wire Line
	5400 5500 5500 5500
Text Label 4100 2650 0    50   ~ 0
~DTR~
Text Label 4100 2750 0    50   ~ 0
~RTS~
Wire Wire Line
	4100 2750 3950 2750
Wire Wire Line
	3950 2650 4100 2650
Text Label 5450 1250 2    50   ~ 0
ESP_EN
Wire Wire Line
	5450 1250 5650 1250
Text Label 5800 4600 0    50   ~ 0
ESP_EN
Wire Wire Line
	5800 4600 5800 4700
Text Label 5950 5150 0    50   ~ 0
~RTS~
Wire Wire Line
	5950 5150 5800 5150
Wire Wire Line
	5800 5150 5800 5100
Text Label 5000 4900 2    50   ~ 0
~DTR~
Wire Wire Line
	5000 4900 5100 4900
Text Label 5650 5300 2    50   ~ 0
~DTR~
Wire Wire Line
	5650 5300 5800 5300
Text Label 5000 5500 2    50   ~ 0
~RTS~
Wire Wire Line
	5000 5500 5100 5500
NoConn ~ 3950 2450
NoConn ~ 3950 2350
NoConn ~ 3950 2250
NoConn ~ 3950 2550
NoConn ~ 3150 2050
$Comp
L sim7070-rescue:CP-Device C?
U 1 1 60A80D54
P 5650 1450
AR Path="/5F76B017/60A80D54" Ref="C?"  Part="1" 
AR Path="/60A80D54" Ref="C?"  Part="1" 
AR Path="/5F8E40AD/60A80D54" Ref="C19"  Part="1" 
F 0 "C19" H 5768 1496 50  0000 L CNN
F 1 "100n" H 5768 1405 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 5688 1300 50  0001 C CNN
F 3 "~" H 5650 1450 50  0001 C CNN
F 4 "C1525" H 5650 1450 50  0001 C CNN "LCSC"
	1    5650 1450
	1    0    0    -1  
$EndComp
$Comp
L sim7070-rescue:GND-power #PWR0136
U 1 1 60A823C5
P 5650 1600
F 0 "#PWR0136" H 5650 1350 50  0001 C CNN
F 1 "GND" H 5655 1427 50  0000 C CNN
F 2 "" H 5650 1600 50  0001 C CNN
F 3 "" H 5650 1600 50  0001 C CNN
	1    5650 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 1300 5650 1250
Connection ~ 5650 1250
Wire Wire Line
	5650 1250 6100 1250
$Comp
L sim7070-rescue:R_Small-Device R23
U 1 1 60A83654
P 5650 1050
F 0 "R23" H 5591 1004 50  0000 R CNN
F 1 "10k" H 5591 1095 50  0000 R CNN
F 2 "Resistor_SMD:R_0402_1005Metric" H 5650 1050 50  0001 C CNN
F 3 "~" H 5650 1050 50  0001 C CNN
F 4 "C25531" H 5650 1050 50  0001 C CNN "LCSC"
	1    5650 1050
	-1   0    0    1   
$EndComp
Wire Wire Line
	5650 1150 5650 1250
$Comp
L sim7070-rescue:+3V3-power #PWR0137
U 1 1 60A84C82
P 5650 900
F 0 "#PWR0137" H 5650 750 50  0001 C CNN
F 1 "+3V3" H 5665 1073 50  0000 C CNN
F 2 "" H 5650 900 50  0001 C CNN
F 3 "" H 5650 900 50  0001 C CNN
	1    5650 900 
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 900  5650 950 
Text GLabel 6100 2250 0    50   Input ~ 0
ESP_PROG_RX
Text GLabel 6100 2150 0    50   Input ~ 0
ESP_PROG_TX
$Comp
L sim7070-rescue:+3V3-power #PWR0142
U 1 1 60B2A180
P 3100 4600
F 0 "#PWR0142" H 3100 4450 50  0001 C CNN
F 1 "+3V3" H 3115 4773 50  0000 C CNN
F 2 "" H 3100 4600 50  0001 C CNN
F 3 "" H 3100 4600 50  0001 C CNN
	1    3100 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 4600 2900 4600
$Comp
L sim7070-rescue:+3V3-power #PWR0143
U 1 1 60B2B9EF
P 900 5000
F 0 "#PWR0143" H 900 4850 50  0001 C CNN
F 1 "+3V3" H 915 5173 50  0000 C CNN
F 2 "" H 900 5000 50  0001 C CNN
F 3 "" H 900 5000 50  0001 C CNN
	1    900  5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	900  5000 1050 5000
Text GLabel 6000 1250 1    50   Input ~ 0
~RESET~
$Comp
L sim7070-rescue:R-Device R?
U 1 1 60A57415
P 3000 5050
AR Path="/5F76B017/60A57415" Ref="R?"  Part="1" 
AR Path="/60A57415" Ref="R?"  Part="1" 
AR Path="/5F8E40AD/60A57415" Ref="R5"  Part="1" 
AR Path="/5F893C1E/60A57415" Ref="R5"  Part="1" 
F 0 "R5" H 3070 5096 50  0000 L CNN
F 1 "1M" H 3070 5005 50  0000 L CNN
F 2 "Resistor_SMD:R_0402_1005Metric" V 2930 5050 50  0001 C CNN
F 3 "~" H 3000 5050 50  0001 C CNN
F 4 "C26087" H 3000 5050 50  0001 C CNN "LCSC"
	1    3000 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 4900 3000 4900
Connection ~ 3000 4900
Wire Wire Line
	3000 4900 3150 4900
$Comp
L sim7070-rescue:GND-power #PWR?
U 1 1 60A58D70
P 3000 5200
AR Path="/5F76B017/60A58D70" Ref="#PWR?"  Part="1" 
AR Path="/60A58D70" Ref="#PWR?"  Part="1" 
AR Path="/5F8E40AD/60A58D70" Ref="#PWR0133"  Part="1" 
F 0 "#PWR0133" H 3000 4950 50  0001 C CNN
F 1 "GND" H 3005 5027 50  0000 C CNN
F 2 "" H 3000 5200 50  0001 C CNN
F 3 "" H 3000 5200 50  0001 C CNN
	1    3000 5200
	1    0    0    -1  
$EndComp
Text Notes 2900 1000 0    118  ~ 0
USB <-> Serial\n
Text Notes 1000 1000 0    118  ~ 0
Battery sensor
Text Notes 1000 4200 0    118  ~ 0
Accelerometer
Text Notes 8800 3200 0    118  ~ 0
Touch input\n
Text Notes 4950 4350 0    118  ~ 0
Upload reset related stuff
$EndSCHEMATC

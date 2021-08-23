EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
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
L GettingStarted:ESP32TTGOTCALL U4
U 1 1 5EEB46A0
P 3700 4450
F 0 "U4" H 3700 5715 50  0000 C CNN
F 1 "ESP32TTGOTCALL" H 3700 5624 50  0000 C CNN
F 2 "GettingStarted:ESP32T-CAll" H 3650 5450 50  0001 C CNN
F 3 "" H 3650 5450 50  0001 C CNN
	1    3700 4450
	1    0    0    -1  
$EndComp
$Comp
L Battery_Management:MCP73831-2-OT U3
U 1 1 5EEB63CB
P 1450 6650
F 0 "U3" H 1450 7131 50  0000 C CNN
F 1 "MCP73831-2-OT" H 1450 7040 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5_HandSoldering" H 1500 6400 50  0001 L CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/20001984g.pdf" H 1300 6600 50  0001 C CNN
	1    1450 6650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR010
U 1 1 5EEBABFE
P 4650 3450
F 0 "#PWR010" H 4650 3200 50  0001 C CNN
F 1 "GND" H 4655 3277 50  0000 C CNN
F 2 "" H 4650 3450 50  0001 C CNN
F 3 "" H 4650 3450 50  0001 C CNN
	1    4650 3450
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR08
U 1 1 5EEBAFB8
P 3150 3400
F 0 "#PWR08" H 3150 3250 50  0001 C CNN
F 1 "+3.3V" H 3165 3573 50  0000 C CNN
F 2 "" H 3150 3400 50  0001 C CNN
F 3 "" H 3150 3400 50  0001 C CNN
	1    3150 3400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR06
U 1 1 5EEBD0A3
P 3100 4750
F 0 "#PWR06" H 3100 4500 50  0001 C CNN
F 1 "GND" H 3105 4577 50  0000 C CNN
F 2 "" H 3100 4750 50  0001 C CNN
F 3 "" H 3100 4750 50  0001 C CNN
	1    3100 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 4750 3300 4750
$Comp
L power:GND #PWR09
U 1 1 5EEBDB1A
P 4350 4050
F 0 "#PWR09" H 4350 3800 50  0001 C CNN
F 1 "GND" H 4355 3877 50  0000 C CNN
F 2 "" H 4350 4050 50  0001 C CNN
F 3 "" H 4350 4050 50  0001 C CNN
	1    4350 4050
	1    0    0    -1  
$EndComp
$Comp
L 0532610271:0532610271 P1
U 1 1 5EEC0B98
P 1650 3200
F 0 "P1" H 1780 3246 50  0000 L CNN
F 1 "0532610271" H 1780 3155 50  0000 L CNN
F 2 "molex:MOLEX_0532610271" H 1650 3200 50  0001 L BNN
F 3 "Molex" H 1650 3200 50  0001 L BNN
	1    1650 3200
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C1
U 1 1 5EEC3321
P 800 6550
F 0 "C1" H 918 6596 50  0000 L CNN
F 1 "4.7u" H 918 6505 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 838 6400 50  0001 C CNN
F 3 "~" H 800 6550 50  0001 C CNN
	1    800  6550
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C2
U 1 1 5EEC4294
P 2400 6700
F 0 "C2" H 2518 6746 50  0000 L CNN
F 1 "4.7u" H 2518 6655 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 2438 6550 50  0001 C CNN
F 3 "~" H 2400 6700 50  0001 C CNN
	1    2400 6700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5EEC4FD6
P 950 6900
F 0 "R1" H 1020 6946 50  0000 L CNN
F 1 "2,2k" H 1020 6855 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 880 6900 50  0001 C CNN
F 3 "~" H 950 6900 50  0001 C CNN
	1    950  6900
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D1
U 1 1 5EEC5B22
P 2000 6900
F 0 "D1" V 2039 6782 50  0000 R CNN
F 1 "GREEN" V 1948 6782 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric" H 2000 6900 50  0001 C CNN
F 3 "~" H 2000 6900 50  0001 C CNN
	1    2000 6900
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR03
U 1 1 5EEC6CD2
P 1450 7450
F 0 "#PWR03" H 1450 7200 50  0001 C CNN
F 1 "GND" H 1455 7277 50  0000 C CNN
F 2 "" H 1450 7450 50  0001 C CNN
F 3 "" H 1450 7450 50  0001 C CNN
	1    1450 7450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 7050 1450 6950
Wire Wire Line
	950  7050 1450 7050
Wire Wire Line
	950  6750 1050 6750
Wire Wire Line
	800  6400 800  6350
Wire Wire Line
	800  6700 800  7050
Wire Wire Line
	800  7050 950  7050
Connection ~ 950  7050
$Comp
L Device:R R2
U 1 1 5EEC8A7D
P 2000 7250
F 0 "R2" H 2070 7296 50  0000 L CNN
F 1 "200" H 2070 7205 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 1930 7250 50  0001 C CNN
F 3 "~" H 2000 7250 50  0001 C CNN
	1    2000 7250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 7450 2000 7400
Wire Wire Line
	2000 7100 2000 7050
Wire Wire Line
	2000 6750 1850 6750
Wire Wire Line
	2400 6850 2400 7450
Wire Wire Line
	2400 7450 2000 7450
Connection ~ 2000 7450
Wire Wire Line
	1450 7050 1450 7450
Connection ~ 1450 7050
Connection ~ 1450 7450
Wire Wire Line
	1450 7450 2000 7450
Text Notes 750  3950 0    50   ~ 0
MOLEX battery connector\n\n\n
Wire Wire Line
	4100 3450 4650 3450
Wire Wire Line
	4350 4050 4100 4050
Wire Wire Line
	1450 6350 800  6350
Connection ~ 1450 6350
Wire Wire Line
	3300 3400 3300 3450
$Comp
L Device:CP C4
U 1 1 5EF0A739
P 2850 3550
F 0 "C4" H 2968 3596 50  0000 L CNN
F 1 "100n" H 2968 3505 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 2888 3400 50  0001 C CNN
F 3 "~" H 2850 3550 50  0001 C CNN
	1    2850 3550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 5EF0B04B
P 2850 3700
F 0 "#PWR05" H 2850 3450 50  0001 C CNN
F 1 "GND" H 2855 3527 50  0000 C CNN
F 2 "" H 2850 3700 50  0001 C CNN
F 3 "" H 2850 3700 50  0001 C CNN
	1    2850 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 3400 3150 3400
Connection ~ 3150 3400
Wire Wire Line
	3150 3400 3300 3400
$Comp
L Device:CP C3
U 1 1 5EF0C3BE
P 3150 5400
F 0 "C3" H 3268 5446 50  0000 L CNN
F 1 "100n" H 3268 5355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 3188 5250 50  0001 C CNN
F 3 "~" H 3150 5400 50  0001 C CNN
	1    3150 5400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 5EF0CA9B
P 3150 5550
F 0 "#PWR04" H 3150 5300 50  0001 C CNN
F 1 "GND" H 3155 5377 50  0000 C CNN
F 2 "" H 3150 5550 50  0001 C CNN
F 3 "" H 3150 5550 50  0001 C CNN
	1    3150 5550
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D5
U 1 1 5EF304F9
P 8500 5100
F 0 "D5" V 8539 4982 50  0000 R CNN
F 1 "GREEN" V 8448 4982 50  0000 R CNN
F 2 "Diode_SMD:D_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 8500 5100 50  0001 C CNN
F 3 "~" H 8500 5100 50  0001 C CNN
	1    8500 5100
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D6
U 1 1 5EF30FF3
P 10750 5100
F 0 "D6" V 10789 4982 50  0000 R CNN
F 1 "GREEN" V 10698 4982 50  0000 R CNN
F 2 "Diode_SMD:D_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 10750 5100 50  0001 C CNN
F 3 "~" H 10750 5100 50  0001 C CNN
	1    10750 5100
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D7
U 1 1 5EF313C6
P 7700 5100
F 0 "D7" V 7739 4982 50  0000 R CNN
F 1 "GREEN" V 7648 4982 50  0000 R CNN
F 2 "Diode_SMD:D_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 7700 5100 50  0001 C CNN
F 3 "~" H 7700 5100 50  0001 C CNN
	1    7700 5100
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R10
U 1 1 5EF32918
P 8500 5400
F 0 "R10" H 8570 5446 50  0000 L CNN
F 1 "200" H 8570 5355 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 8430 5400 50  0001 C CNN
F 3 "~" H 8500 5400 50  0001 C CNN
	1    8500 5400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R13
U 1 1 5EF32DB9
P 9250 5400
F 0 "R13" H 9320 5446 50  0000 L CNN
F 1 "200" H 9320 5355 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 9180 5400 50  0001 C CNN
F 3 "~" H 9250 5400 50  0001 C CNN
	1    9250 5400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R16
U 1 1 5EF330D5
P 10000 5400
F 0 "R16" H 10070 5446 50  0000 L CNN
F 1 "200" H 10070 5355 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 9930 5400 50  0001 C CNN
F 3 "~" H 10000 5400 50  0001 C CNN
	1    10000 5400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR014
U 1 1 5EF31F65
P 8500 6250
F 0 "#PWR014" H 8500 6000 50  0001 C CNN
F 1 "GND" H 8505 6077 50  0000 C CNN
F 2 "" H 8500 6250 50  0001 C CNN
F 3 "" H 8500 6250 50  0001 C CNN
	1    8500 6250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR015
U 1 1 5EFC4436
P 9250 6200
F 0 "#PWR015" H 9250 5950 50  0001 C CNN
F 1 "GND" H 9255 6027 50  0000 C CNN
F 2 "" H 9250 6200 50  0001 C CNN
F 3 "" H 9250 6200 50  0001 C CNN
	1    9250 6200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR017
U 1 1 5EFC471E
P 10000 6250
F 0 "#PWR017" H 10000 6000 50  0001 C CNN
F 1 "GND" H 10005 6077 50  0000 C CNN
F 2 "" H 10000 6250 50  0001 C CNN
F 3 "" H 10000 6250 50  0001 C CNN
	1    10000 6250
	1    0    0    -1  
$EndComp
Wire Wire Line
	10000 5600 10000 5550
Wire Wire Line
	9250 5600 9250 5550
Wire Wire Line
	8500 5600 8500 5550
$Comp
L Device:R R9
U 1 1 5EFE26D7
P 8150 6050
F 0 "R9" H 8220 6096 50  0000 L CNN
F 1 "100k" H 8220 6005 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 8080 6050 50  0001 C CNN
F 3 "~" H 8150 6050 50  0001 C CNN
	1    8150 6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	8150 5900 8200 5900
$Comp
L Device:R R11
U 1 1 5EFE29F3
P 8950 6050
F 0 "R11" H 9020 6096 50  0000 L CNN
F 1 "100k" H 9020 6005 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 8880 6050 50  0001 C CNN
F 3 "~" H 8950 6050 50  0001 C CNN
	1    8950 6050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R15
U 1 1 5EFE2E42
P 9700 6050
F 0 "R15" H 9770 6096 50  0000 L CNN
F 1 "100k" H 9770 6005 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 9630 6050 50  0001 C CNN
F 3 "~" H 9700 6050 50  0001 C CNN
	1    9700 6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	8150 6200 8500 6200
Wire Wire Line
	8500 6000 8500 6200
Connection ~ 8500 6200
Wire Wire Line
	8500 6200 8500 6250
Wire Wire Line
	9250 6000 9250 6200
Wire Wire Line
	8950 6200 9250 6200
Connection ~ 9250 6200
Wire Wire Line
	10000 6000 10000 6200
Wire Wire Line
	9700 6200 10000 6200
Connection ~ 10000 6200
Wire Wire Line
	10000 6200 10000 6250
Wire Wire Line
	750  3300 950  3300
$Comp
L GettingStarted:ChargePad U1
U 1 1 5EFD521E
P 1050 2950
F 0 "U1" H 1368 2976 50  0000 L CNN
F 1 "ChargePad" H 1368 2885 50  0000 L CNN
F 2 "GettingStarted:chargePadFootprint" H 1050 3050 50  0001 C CNN
F 3 "" H 1050 3050 50  0001 C CNN
	1    1050 2950
	1    0    0    -1  
$EndComp
$Comp
L GettingStarted:ChargePad U2
U 1 1 5EFD591F
P 1050 3400
F 0 "U2" H 1368 3426 50  0000 L CNN
F 1 "ChargePad" H 1368 3335 50  0000 L CNN
F 2 "GettingStarted:chargePadFootprint" H 1050 3500 50  0001 C CNN
F 3 "" H 1050 3500 50  0001 C CNN
	1    1050 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1050 2950 1050 3100
Wire Wire Line
	1050 3100 1350 3100
Wire Wire Line
	1050 3400 1050 3300
Connection ~ 1050 3300
Wire Wire Line
	1050 3300 1350 3300
Wire Wire Line
	3150 4150 3300 4150
Text Label 3200 4050 0    50   ~ 0
SWITCH
$Comp
L dk_Rectangular-Connectors-Headers-Male-Pins:302-S101 J1
U 1 1 5F0345B9
P 10550 3450
F 0 "J1" H 10550 3875 50  0000 C CNN
F 1 "302-S101" H 10550 3784 50  0000 C CNN
F 2 "digikey-footprints:PinHeader_2x5_P2.54mm_Drill1.2mm" H 10750 3650 60  0001 L CNN
F 3 "http://www.on-shore.com/wp-content/uploads/2018/04/302-SXX1.pdf" V 10750 3750 60  0001 L CNN
F 4 "ED1543-ND" H 10750 3850 60  0001 L CNN "Digi-Key_PN"
F 5 "302-S101" H 10750 3950 60  0001 L CNN "MPN"
F 6 "Connectors, Interconnects" H 10750 4050 60  0001 L CNN "Category"
F 7 "Rectangular Connectors - Headers, Male Pins" H 10750 4150 60  0001 L CNN "Family"
F 8 "http://www.on-shore.com/wp-content/uploads/2018/04/302-SXX1.pdf" H 10750 4250 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/on-shore-technology-inc/302-S101/ED1543-ND/2178422" H 10750 4350 60  0001 L CNN "DK_Detail_Page"
F 10 "CONN HEADER VERT 10POS 2.54MM" H 10750 4450 60  0001 L CNN "Description"
F 11 "On Shore Technology Inc." H 10750 4550 60  0001 L CNN "Manufacturer"
F 12 "Active" H 10750 4650 60  0001 L CNN "Status"
	1    10550 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 4250 3300 4250
Wire Wire Line
	3200 4350 3300 4350
Wire Wire Line
	3200 4450 3300 4450
Wire Wire Line
	3200 4050 3300 4050
Wire Wire Line
	3200 4850 3300 4850
Wire Wire Line
	3200 4950 3300 4950
Wire Wire Line
	3200 5050 3300 5050
Wire Wire Line
	10350 3250 10250 3250
$Comp
L power:GND #PWR018
U 1 1 5F054809
P 10750 3650
F 0 "#PWR018" H 10750 3400 50  0001 C CNN
F 1 "GND" H 10755 3477 50  0000 C CNN
F 2 "" H 10750 3650 50  0001 C CNN
F 3 "" H 10750 3650 50  0001 C CNN
	1    10750 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	10750 3550 10750 3650
Connection ~ 10750 3650
Wire Wire Line
	10750 3550 10750 3450
Connection ~ 10750 3550
$Comp
L power:+3.3V #PWR020
U 1 1 5F05AFCE
P 10900 3200
F 0 "#PWR020" H 10900 3050 50  0001 C CNN
F 1 "+3.3V" H 10915 3373 50  0000 C CNN
F 2 "" H 10900 3200 50  0001 C CNN
F 3 "" H 10900 3200 50  0001 C CNN
	1    10900 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	10950 3200 10900 3200
Wire Wire Line
	10750 3250 10950 3250
Wire Wire Line
	10950 3250 10950 3200
Wire Wire Line
	10750 3350 10750 3250
Connection ~ 10750 3250
Wire Wire Line
	10250 3550 10350 3550
Wire Wire Line
	10250 3650 10350 3650
Wire Wire Line
	10250 3450 10350 3450
Wire Wire Line
	10250 3350 10350 3350
Text GLabel 3300 3650 0    50   Input ~ 0
V_BATT
$Comp
L Device:R R3
U 1 1 5F0B26D3
P 3150 2000
F 0 "R3" H 3220 2046 50  0000 L CNN
F 1 "500k" H 3220 1955 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3080 2000 50  0001 C CNN
F 3 "~" H 3150 2000 50  0001 C CNN
	1    3150 2000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 5F0B310D
P 3150 2350
F 0 "R4" H 3220 2396 50  0000 L CNN
F 1 "1M" H 3220 2305 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3080 2350 50  0001 C CNN
F 3 "~" H 3150 2350 50  0001 C CNN
	1    3150 2350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR07
U 1 1 5F0B670B
P 3150 2600
F 0 "#PWR07" H 3150 2350 50  0001 C CNN
F 1 "GND" H 3155 2427 50  0000 C CNN
F 2 "" H 3150 2600 50  0001 C CNN
F 3 "" H 3150 2600 50  0001 C CNN
	1    3150 2600
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C5
U 1 1 5F0B6B9D
P 2900 2400
F 0 "C5" H 3018 2446 50  0000 L CNN
F 1 "100n" H 3018 2355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 2938 2250 50  0001 C CNN
F 3 "~" H 2900 2400 50  0001 C CNN
	1    2900 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 2150 2900 2150
Wire Wire Line
	2900 2150 2900 2250
Wire Wire Line
	3150 2200 3150 2150
Connection ~ 3150 2150
Wire Wire Line
	3150 1850 3150 1750
Wire Wire Line
	3150 2600 3150 2550
Wire Wire Line
	2900 2550 3150 2550
Connection ~ 3150 2550
Wire Wire Line
	3150 2550 3150 2500
$Comp
L Device:LED D8
U 1 1 5F269D7B
P 6950 5100
F 0 "D8" V 6989 4982 50  0000 R CNN
F 1 "GREEN" V 6898 4982 50  0000 R CNN
F 2 "Diode_SMD:D_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6950 5100 50  0001 C CNN
F 3 "~" H 6950 5100 50  0001 C CNN
	1    6950 5100
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R18
U 1 1 5F269D81
P 10750 5400
F 0 "R18" H 10820 5446 50  0000 L CNN
F 1 "200" H 10820 5355 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 10680 5400 50  0001 C CNN
F 3 "~" H 10750 5400 50  0001 C CNN
	1    10750 5400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR019
U 1 1 5F269D87
P 10750 6250
F 0 "#PWR019" H 10750 6000 50  0001 C CNN
F 1 "GND" H 10755 6077 50  0000 C CNN
F 2 "" H 10750 6250 50  0001 C CNN
F 3 "" H 10750 6250 50  0001 C CNN
	1    10750 6250
	1    0    0    -1  
$EndComp
Wire Wire Line
	10750 5600 10750 5550
$Comp
L Device:R R17
U 1 1 5F269DA3
P 10450 6050
F 0 "R17" H 10520 6096 50  0000 L CNN
F 1 "100k" H 10520 6005 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 10380 6050 50  0001 C CNN
F 3 "~" H 10450 6050 50  0001 C CNN
	1    10450 6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	10750 6000 10750 6200
Wire Wire Line
	10450 6200 10750 6200
Connection ~ 10750 6200
Wire Wire Line
	10750 6200 10750 6250
$Comp
L Device:R R6
U 1 1 5F26E67F
P 6950 5400
F 0 "R6" H 7020 5446 50  0000 L CNN
F 1 "200" H 7020 5355 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6880 5400 50  0001 C CNN
F 3 "~" H 6950 5400 50  0001 C CNN
	1    6950 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 5600 6950 5550
Wire Wire Line
	6950 6000 6950 6200
Connection ~ 6950 6200
Wire Wire Line
	6950 6200 6950 6250
$Comp
L Device:R R8
U 1 1 5F272535
P 7700 5400
F 0 "R8" H 7770 5446 50  0000 L CNN
F 1 "200" H 7770 5355 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7630 5400 50  0001 C CNN
F 3 "~" H 7700 5400 50  0001 C CNN
	1    7700 5400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 5F27253B
P 7700 6250
F 0 "#PWR012" H 7700 6000 50  0001 C CNN
F 1 "GND" H 7705 6077 50  0000 C CNN
F 2 "" H 7700 6250 50  0001 C CNN
F 3 "" H 7700 6250 50  0001 C CNN
	1    7700 6250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 5600 7700 5550
$Comp
L Device:R R7
U 1 1 5F272557
P 7400 6050
F 0 "R7" H 7470 6096 50  0000 L CNN
F 1 "100k" H 7470 6005 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7330 6050 50  0001 C CNN
F 3 "~" H 7400 6050 50  0001 C CNN
	1    7400 6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 6000 7700 6200
Wire Wire Line
	7400 6200 7700 6200
Connection ~ 7700 6200
Wire Wire Line
	7700 6200 7700 6250
Text Label 3150 5250 0    50   ~ 0
5V_USB
Wire Wire Line
	1450 5950 1450 6350
Text GLabel 1450 5950 0    50   Input ~ 0
5V_USB
Wire Wire Line
	750  3100 800  3100
Connection ~ 1050 3100
Text GLabel 1550 4300 2    50   Input ~ 0
V_BAT_SWITCHED
$Comp
L power:+BATT #PWR02
U 1 1 5F291221
P 950 4300
F 0 "#PWR02" H 950 4150 50  0001 C CNN
F 1 "+BATT" H 965 4473 50  0000 C CNN
F 2 "" H 950 4300 50  0001 C CNN
F 3 "" H 950 4300 50  0001 C CNN
	1    950  4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	1850 6550 2100 6550
Wire Wire Line
	2100 6350 2100 6550
Connection ~ 2100 6550
Wire Wire Line
	2100 6550 2400 6550
Text GLabel 8500 4950 1    50   Input ~ 0
V_BATT_PROTECTED
Text GLabel 9250 4950 1    50   Input ~ 0
V_BATT_PROTECTED
Text GLabel 10000 4950 1    50   Input ~ 0
V_BATT_PROTECTED
Text GLabel 10750 4950 1    50   Input ~ 0
V_BATT_PROTECTED
Text GLabel 6950 4950 1    50   Input ~ 0
V_BATT_PROTECTED
Text GLabel 7700 4950 1    50   Input ~ 0
V_BATT_PROTECTED
$Comp
L dk_Transistors-FETs-MOSFETs-Single:DMG2305UX-13 Q2
U 1 1 5F29951E
P 7700 5800
F 0 "Q2" H 7808 5853 60  0000 L CNN
F 1 "MOS-N" H 7808 5747 60  0000 L CNN
F 2 "digikey-footprints:SOT-23-3" H 7900 6000 60  0001 L CNN
F 3 "https://www.diodes.com/assets/Datasheets/DMG2305UX.pdf" H 7900 6100 60  0001 L CNN
F 4 "DMG2305UX-13DICT-ND" H 7900 6200 60  0001 L CNN "Digi-Key_PN"
F 5 "DMG2305UX-13" H 7900 6300 60  0001 L CNN "MPN"
F 6 "Discrete Semiconductor Products" H 7900 6400 60  0001 L CNN "Category"
F 7 "Transistors - FETs, MOSFETs - Single" H 7900 6500 60  0001 L CNN "Family"
F 8 "https://www.diodes.com/assets/Datasheets/DMG2305UX.pdf" H 7900 6600 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/diodes-incorporated/DMG2305UX-13/DMG2305UX-13DICT-ND/4251589" H 7900 6700 60  0001 L CNN "DK_Detail_Page"
F 10 "MOSFET P-CH 20V 4.2A SOT23" H 7900 6800 60  0001 L CNN "Description"
F 11 "Diodes Incorporated" H 7900 6900 60  0001 L CNN "Manufacturer"
F 12 "Active" H 7900 7000 60  0001 L CNN "Status"
	1    7700 5800
	1    0    0    -1  
$EndComp
$Comp
L dk_Transistors-FETs-MOSFETs-Single:DMG2305UX-13 Q3
U 1 1 5F29A6F9
P 8500 5800
F 0 "Q3" H 8608 5853 60  0000 L CNN
F 1 "MOS-N" H 8608 5747 60  0000 L CNN
F 2 "digikey-footprints:SOT-23-3" H 8700 6000 60  0001 L CNN
F 3 "https://www.diodes.com/assets/Datasheets/DMG2305UX.pdf" H 8700 6100 60  0001 L CNN
F 4 "DMG2305UX-13DICT-ND" H 8700 6200 60  0001 L CNN "Digi-Key_PN"
F 5 "DMG2305UX-13" H 8700 6300 60  0001 L CNN "MPN"
F 6 "Discrete Semiconductor Products" H 8700 6400 60  0001 L CNN "Category"
F 7 "Transistors - FETs, MOSFETs - Single" H 8700 6500 60  0001 L CNN "Family"
F 8 "https://www.diodes.com/assets/Datasheets/DMG2305UX.pdf" H 8700 6600 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/diodes-incorporated/DMG2305UX-13/DMG2305UX-13DICT-ND/4251589" H 8700 6700 60  0001 L CNN "DK_Detail_Page"
F 10 "MOSFET P-CH 20V 4.2A SOT23" H 8700 6800 60  0001 L CNN "Description"
F 11 "Diodes Incorporated" H 8700 6900 60  0001 L CNN "Manufacturer"
F 12 "Active" H 8700 7000 60  0001 L CNN "Status"
	1    8500 5800
	1    0    0    -1  
$EndComp
$Comp
L dk_Transistors-FETs-MOSFETs-Single:DMG2305UX-13 Q4
U 1 1 5F29C0E8
P 9250 5800
F 0 "Q4" H 9358 5853 60  0000 L CNN
F 1 "MOS-N" H 9358 5747 60  0000 L CNN
F 2 "digikey-footprints:SOT-23-3" H 9450 6000 60  0001 L CNN
F 3 "https://www.diodes.com/assets/Datasheets/DMG2305UX.pdf" H 9450 6100 60  0001 L CNN
F 4 "DMG2305UX-13DICT-ND" H 9450 6200 60  0001 L CNN "Digi-Key_PN"
F 5 "DMG2305UX-13" H 9450 6300 60  0001 L CNN "MPN"
F 6 "Discrete Semiconductor Products" H 9450 6400 60  0001 L CNN "Category"
F 7 "Transistors - FETs, MOSFETs - Single" H 9450 6500 60  0001 L CNN "Family"
F 8 "https://www.diodes.com/assets/Datasheets/DMG2305UX.pdf" H 9450 6600 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/diodes-incorporated/DMG2305UX-13/DMG2305UX-13DICT-ND/4251589" H 9450 6700 60  0001 L CNN "DK_Detail_Page"
F 10 "MOSFET P-CH 20V 4.2A SOT23" H 9450 6800 60  0001 L CNN "Description"
F 11 "Diodes Incorporated" H 9450 6900 60  0001 L CNN "Manufacturer"
F 12 "Active" H 9450 7000 60  0001 L CNN "Status"
	1    9250 5800
	1    0    0    -1  
$EndComp
$Comp
L dk_Transistors-FETs-MOSFETs-Single:DMG2305UX-13 Q5
U 1 1 5F29CA05
P 10000 5800
F 0 "Q5" H 10108 5853 60  0000 L CNN
F 1 "MOS-N" H 10108 5747 60  0000 L CNN
F 2 "digikey-footprints:SOT-23-3" H 10200 6000 60  0001 L CNN
F 3 "https://www.diodes.com/assets/Datasheets/DMG2305UX.pdf" H 10200 6100 60  0001 L CNN
F 4 "DMG2305UX-13DICT-ND" H 10200 6200 60  0001 L CNN "Digi-Key_PN"
F 5 "DMG2305UX-13" H 10200 6300 60  0001 L CNN "MPN"
F 6 "Discrete Semiconductor Products" H 10200 6400 60  0001 L CNN "Category"
F 7 "Transistors - FETs, MOSFETs - Single" H 10200 6500 60  0001 L CNN "Family"
F 8 "https://www.diodes.com/assets/Datasheets/DMG2305UX.pdf" H 10200 6600 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/diodes-incorporated/DMG2305UX-13/DMG2305UX-13DICT-ND/4251589" H 10200 6700 60  0001 L CNN "DK_Detail_Page"
F 10 "MOSFET P-CH 20V 4.2A SOT23" H 10200 6800 60  0001 L CNN "Description"
F 11 "Diodes Incorporated" H 10200 6900 60  0001 L CNN "Manufacturer"
F 12 "Active" H 10200 7000 60  0001 L CNN "Status"
	1    10000 5800
	1    0    0    -1  
$EndComp
$Comp
L dk_Transistors-FETs-MOSFETs-Single:DMG2305UX-13 Q6
U 1 1 5F29D964
P 10750 5800
F 0 "Q6" H 10858 5853 60  0000 L CNN
F 1 "MOS-N" H 10858 5747 60  0000 L CNN
F 2 "digikey-footprints:SOT-23-3" H 10950 6000 60  0001 L CNN
F 3 "https://www.diodes.com/assets/Datasheets/DMG2305UX.pdf" H 10950 6100 60  0001 L CNN
F 4 "DMG2305UX-13DICT-ND" H 10950 6200 60  0001 L CNN "Digi-Key_PN"
F 5 "DMG2305UX-13" H 10950 6300 60  0001 L CNN "MPN"
F 6 "Discrete Semiconductor Products" H 10950 6400 60  0001 L CNN "Category"
F 7 "Transistors - FETs, MOSFETs - Single" H 10950 6500 60  0001 L CNN "Family"
F 8 "https://www.diodes.com/assets/Datasheets/DMG2305UX.pdf" H 10950 6600 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/diodes-incorporated/DMG2305UX-13/DMG2305UX-13DICT-ND/4251589" H 10950 6700 60  0001 L CNN "DK_Detail_Page"
F 10 "MOSFET P-CH 20V 4.2A SOT23" H 10950 6800 60  0001 L CNN "Description"
F 11 "Diodes Incorporated" H 10950 6900 60  0001 L CNN "Manufacturer"
F 12 "Active" H 10950 7000 60  0001 L CNN "Status"
	1    10750 5800
	1    0    0    -1  
$EndComp
Text Notes 900  4500 0    50   ~ 0
Switch for disabling the battery..\n
Text Notes 800  5800 0    50   ~ 0
Battery charge circuit..\n
Text Notes 2450 1500 0    50   ~ 0
Voltage divider for battery sense..\n
Text GLabel 3150 1750 2    50   Input ~ 0
V_BAT_SWITCHED
Text Notes 800  2850 0    50   ~ 0
MOLEX battery connector\n\n\n
Text GLabel 4100 4250 2    50   Input ~ 0
LED2
Text GLabel 3200 4250 0    50   Input ~ 0
LED3
Text GLabel 3150 4550 0    50   Input ~ 0
LED4
Text GLabel 3150 4650 0    50   Input ~ 0
LED5
Text GLabel 4100 4850 2    50   Input ~ 0
LED6
$Comp
L dk_Transistors-FETs-MOSFETs-Single:DMG2305UX-13 Q1
U 1 1 5F26E694
P 6950 5800
F 0 "Q1" H 7058 5853 60  0000 L CNN
F 1 "MOS-N" H 7058 5747 60  0000 L CNN
F 2 "digikey-footprints:SOT-23-3" H 7150 6000 60  0001 L CNN
F 3 "https://www.diodes.com/assets/Datasheets/DMG2305UX.pdf" H 7150 6100 60  0001 L CNN
F 4 "DMG2305UX-13DICT-ND" H 7150 6200 60  0001 L CNN "Digi-Key_PN"
F 5 "DMG2305UX-13" H 7150 6300 60  0001 L CNN "MPN"
F 6 "Discrete Semiconductor Products" H 7150 6400 60  0001 L CNN "Category"
F 7 "Transistors - FETs, MOSFETs - Single" H 7150 6500 60  0001 L CNN "Family"
F 8 "https://www.diodes.com/assets/Datasheets/DMG2305UX.pdf" H 7150 6600 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/diodes-incorporated/DMG2305UX-13/DMG2305UX-13DICT-ND/4251589" H 7150 6700 60  0001 L CNN "DK_Detail_Page"
F 10 "MOSFET P-CH 20V 4.2A SOT23" H 7150 6800 60  0001 L CNN "Description"
F 11 "Diodes Incorporated" H 7150 6900 60  0001 L CNN "Manufacturer"
F 12 "Active" H 7150 7000 60  0001 L CNN "Status"
	1    6950 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 6200 6950 6200
$Comp
L Device:R R5
U 1 1 5F26E6A1
P 6650 6050
F 0 "R5" H 6720 6096 50  0000 L CNN
F 1 "100k" H 6720 6005 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6580 6050 50  0001 C CNN
F 3 "~" H 6650 6050 50  0001 C CNN
	1    6650 6050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR011
U 1 1 5F26E685
P 6950 6250
F 0 "#PWR011" H 6950 6000 50  0001 C CNN
F 1 "GND" H 6955 6077 50  0000 C CNN
F 2 "" H 6950 6250 50  0001 C CNN
F 3 "" H 6950 6250 50  0001 C CNN
	1    6950 6250
	1    0    0    -1  
$EndComp
Text GLabel 6650 5900 1    50   Input ~ 0
LED1
Text GLabel 7400 5900 1    50   Input ~ 0
LED2
Text GLabel 8150 5900 1    50   Input ~ 0
LED3
Text GLabel 8950 5900 1    50   Input ~ 0
LED4
Text GLabel 9700 5900 1    50   Input ~ 0
LED5
Text GLabel 10450 5900 1    50   Input ~ 0
LED6
NoConn ~ 3200 4350
NoConn ~ 3200 4450
Text GLabel 4100 5050 2    50   Input ~ 0
EX5
Text GLabel 4100 5150 2    50   Input ~ 0
EX4
Text GLabel 4100 5250 2    50   Input ~ 0
EX3
Text GLabel 4100 5350 2    50   Input ~ 0
EX2
NoConn ~ 4100 4950
NoConn ~ 3200 4950
NoConn ~ 3200 5050
NoConn ~ 3200 4850
NoConn ~ 3300 5350
NoConn ~ 3300 5450
Text GLabel 10250 3250 0    50   Input ~ 0
EX1
Text GLabel 10250 3350 0    50   Input ~ 0
EX2
Text GLabel 10250 3450 0    50   Input ~ 0
EX3
Text GLabel 10250 3550 0    50   Input ~ 0
EX4
Text GLabel 10250 3650 0    50   Input ~ 0
EX5
Text GLabel 4100 5450 2    50   Input ~ 0
EX1
NoConn ~ 3300 3750
NoConn ~ 3300 3850
NoConn ~ 3300 3950
NoConn ~ 3300 5150
NoConn ~ 4100 3850
NoConn ~ 4100 3750
NoConn ~ 4100 3550
$Comp
L Switch:SW_DIP_x01 SW1
U 1 1 5F286A8B
P 1250 4300
F 0 "SW1" H 1250 4567 50  0000 C CNN
F 1 "SW_DIP_x01" H 1250 4476 50  0000 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_Pad1.0x1.5mm" H 1250 4300 50  0001 C CNN
F 3 "~" H 1250 4300 50  0001 C CNN
	1    1250 4300
	1    0    0    -1  
$EndComp
$Comp
L GettingStarted:touchPad P2
U 1 1 5F36A0AA
P 4850 2400
F 0 "P2" H 4904 2723 50  0000 C CNN
F 1 "touchPad" H 4904 2632 50  0000 C CNN
F 2 "GettingStarted:touchPad" H 4850 2400 50  0001 C CNN
F 3 "" H 4850 2400 50  0001 C CNN
	1    4850 2400
	1    0    0    -1  
$EndComp
Text Notes 4600 2000 0    50   ~ 0
Touch pad for back side.. \n
Text GLabel 5100 2400 2    50   Input ~ 0
TOUCHSENSE
Text GLabel 3150 4150 0    50   Input ~ 0
TOUCHSENSE
$Comp
L power:GND #PWR0113
U 1 1 5F3A93B9
P 750 3100
F 0 "#PWR0113" H 750 2850 50  0001 C CNN
F 1 "GND" H 755 2927 50  0000 C CNN
F 2 "" H 750 3100 50  0001 C CNN
F 3 "" H 750 3100 50  0001 C CNN
	1    750  3100
	1    0    0    -1  
$EndComp
$Comp
L power:+BATT #PWR0102
U 1 1 5F3A9A20
P 750 3400
F 0 "#PWR0102" H 750 3250 50  0001 C CNN
F 1 "+BATT" H 765 3573 50  0000 C CNN
F 2 "" H 750 3400 50  0001 C CNN
F 3 "" H 750 3400 50  0001 C CNN
	1    750  3400
	-1   0    0    1   
$EndComp
Wire Wire Line
	750  3400 750  3300
Text GLabel 1500 4700 2    50   Input ~ 0
V_BAT_SWITCHED
$Comp
L power:+BATT #PWR0114
U 1 1 5F431C4E
P 900 4700
F 0 "#PWR0114" H 900 4550 50  0001 C CNN
F 1 "+BATT" H 915 4873 50  0000 C CNN
F 2 "" H 900 4700 50  0001 C CNN
F 3 "" H 900 4700 50  0001 C CNN
	1    900  4700
	1    0    0    -1  
$EndComp
Text Notes 850  4900 0    50   ~ 0
Switch for disabling the battery..\n
$Comp
L Switch:SW_DIP_x01 SW2
U 1 1 5F431C55
P 1200 4700
F 0 "SW2" H 1200 4967 50  0000 C CNN
F 1 "SW_DIP_x01" H 1200 4876 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 1200 4700 50  0001 C CNN
F 3 "~" H 1200 4700 50  0001 C CNN
	1    1200 4700
	1    0    0    -1  
$EndComp
$Comp
L GettingStarted:MAX40403 U5
U 1 1 5F3ECA15
P 2600 5250
F 0 "U5" H 2600 5525 50  0000 C CNN
F 1 "MAX40403" H 2600 5434 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5_HandSoldering" H 2450 5250 50  0001 C CNN
F 3 "" H 2450 5250 50  0001 C CNN
	1    2600 5250
	1    0    0    -1  
$EndComp
Connection ~ 3150 5250
Wire Wire Line
	3150 5250 3300 5250
$Comp
L power:GND #PWR0115
U 1 1 5F3FCADA
P 2150 5350
F 0 "#PWR0115" H 2150 5100 50  0001 C CNN
F 1 "GND" H 2155 5177 50  0000 C CNN
F 2 "" H 2150 5350 50  0001 C CNN
F 3 "" H 2150 5350 50  0001 C CNN
	1    2150 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 5350 2250 5350
Wire Wire Line
	2950 5250 3050 5250
$Comp
L Device:R R19
U 1 1 5F40A975
P 1750 5350
F 0 "R19" H 1820 5396 50  0000 L CNN
F 1 "100k" H 1820 5305 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 1680 5350 50  0001 C CNN
F 3 "~" H 1750 5350 50  0001 C CNN
	1    1750 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 5200 1750 5200
Connection ~ 1750 5200
Wire Wire Line
	1750 5200 1500 5200
Wire Wire Line
	2250 5500 1750 5500
$Comp
L GettingStarted:TMP75AIDR U6
U 1 1 5F42B88B
P 7150 2450
F 0 "U6" H 7125 2715 50  0000 C CNN
F 1 "TMP75AIDR" H 7125 2624 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 7250 2450 50  0001 C CNN
F 3 "" H 7250 2450 50  0001 C CNN
	1    7150 2450
	1    0    0    -1  
$EndComp
Text Notes 6850 2150 0    50   ~ 0
Temperature sensor 
Text GLabel 6700 2450 0    50   Input ~ 0
SDA
Text GLabel 6700 2600 0    50   Input ~ 0
SCL
$Comp
L Device:R R20
U 1 1 5F431DA2
P 6450 2750
F 0 "R20" H 6520 2796 50  0000 L CNN
F 1 "1M" H 6520 2705 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6380 2750 50  0001 C CNN
F 3 "~" H 6450 2750 50  0001 C CNN
	1    6450 2750
	0    1    1    0   
$EndComp
Wire Wire Line
	6600 2750 6700 2750
Wire Wire Line
	7550 2450 7700 2450
$Comp
L power:GND #PWR01
U 1 1 5F43D83A
P 6700 2900
F 0 "#PWR01" H 6700 2650 50  0001 C CNN
F 1 "GND" H 6705 2727 50  0000 C CNN
F 2 "" H 6700 2900 50  0001 C CNN
F 3 "" H 6700 2900 50  0001 C CNN
	1    6700 2900
	1    0    0    -1  
$EndComp
Text GLabel 4200 3650 2    50   Input ~ 0
SCL
Text GLabel 4200 3950 2    50   Input ~ 0
SDA
Wire Wire Line
	4100 3950 4200 3950
Wire Wire Line
	4200 3650 4100 3650
$Comp
L power:GND #PWR021
U 1 1 5F452681
P 7800 2900
F 0 "#PWR021" H 7800 2650 50  0001 C CNN
F 1 "GND" H 7805 2727 50  0000 C CNN
F 2 "" H 7800 2900 50  0001 C CNN
F 3 "" H 7800 2900 50  0001 C CNN
	1    7800 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 2600 7550 2750
Wire Wire Line
	7550 2900 7800 2900
Connection ~ 7550 2900
Connection ~ 7550 2750
Wire Wire Line
	7550 2750 7550 2900
$Comp
L power:+3.3V #PWR0116
U 1 1 5F47FE0A
P 7700 2450
F 0 "#PWR0116" H 7700 2300 50  0001 C CNN
F 1 "+3.3V" H 7715 2623 50  0000 C CNN
F 2 "" H 7700 2450 50  0001 C CNN
F 3 "" H 7700 2450 50  0001 C CNN
	1    7700 2450
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0117
U 1 1 5F488FBF
P 6300 2550
F 0 "#PWR0117" H 6300 2400 50  0001 C CNN
F 1 "+3.3V" H 6315 2723 50  0000 C CNN
F 2 "" H 6300 2550 50  0001 C CNN
F 3 "" H 6300 2550 50  0001 C CNN
	1    6300 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 2550 6300 2750
Text GLabel 4100 4150 2    50   Input ~ 0
LED1
NoConn ~ 4100 4650
NoConn ~ 4100 4350
NoConn ~ 5200 3250
Text Notes 5400 4250 0    50   ~ 0
Reserved pins:\n4\n5\n23\n27\n26\n21 (I2C)\n22 (I2C)\n13
Text Notes 750  700  0    50   ~ 0
Reverse voltage protection
Text GLabel 1950 950  2    50   Input ~ 0
V_BATT_PROTECTED
$Comp
L Device:Q_PMOS_SDG Q7
U 1 1 5F3F51D3
P 1450 1050
F 0 "Q7" V 1792 1050 50  0000 C CNN
F 1 "Q_PMOS_SDG" V 1701 1050 50  0000 C CNN
F 2 "GettingStarted:SOT-23-MOSFET" H 1650 1150 50  0001 C CNN
F 3 "~" H 1450 1050 50  0001 C CNN
	1    1450 1050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1650 950  1950 950 
$Comp
L power:GND #PWR0106
U 1 1 5F404A07
P 1450 1250
F 0 "#PWR0106" H 1450 1000 50  0001 C CNN
F 1 "GND" H 1455 1077 50  0000 C CNN
F 2 "" H 1450 1250 50  0001 C CNN
F 3 "" H 1450 1250 50  0001 C CNN
	1    1450 1250
	1    0    0    -1  
$EndComp
Text GLabel 2900 2150 0    50   Input ~ 0
V_BATT
Text GLabel 1250 950  0    50   Input ~ 0
V_BAT_SWITCHED
Text GLabel 1500 5200 0    50   Input ~ 0
V_BATT_PROTECTED
Text GLabel 2100 6350 1    50   Input ~ 0
V_BATT_PROTECTED
NoConn ~ 4100 4750
Wire Wire Line
	3150 4550 3300 4550
Wire Wire Line
	3150 4650 3300 4650
$Comp
L Device:LED D3
U 1 1 5F26E679
P 9250 5100
F 0 "D3" V 9289 4982 50  0000 R CNN
F 1 "GREEN" V 9198 4982 50  0000 R CNN
F 2 "Diode_SMD:D_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 9250 5300 50  0001 C CNN
F 3 "~" H 9250 5100 50  0001 C CNN
	1    9250 5100
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D4
U 1 1 5F27252F
P 10000 5100
F 0 "D4" V 10039 4982 50  0000 R CNN
F 1 "GREEN" V 9948 4982 50  0000 R CNN
F 2 "Diode_SMD:D_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 10000 5100 50  0001 C CNN
F 3 "~" H 10000 5100 50  0001 C CNN
	1    10000 5100
	0    -1   -1   0   
$EndComp
$Comp
L Connector:TestPoint TP3
U 1 1 5F4524B8
P 3050 5250
F 0 "TP3" H 3108 5368 50  0000 L CNN
F 1 "TestPoint" H 3108 5277 50  0000 L CNN
F 2 "TestPoint:TestPoint_THTPad_1.0x1.0mm_Drill0.5mm" H 3250 5250 50  0001 C CNN
F 3 "~" H 3250 5250 50  0001 C CNN
	1    3050 5250
	1    0    0    -1  
$EndComp
Connection ~ 3050 5250
Wire Wire Line
	3050 5250 3150 5250
$Comp
L Connector:TestPoint TP2
U 1 1 5F459CB8
P 950 3300
F 0 "TP2" H 1008 3418 50  0000 L CNN
F 1 "TestPoint" H 1008 3327 50  0000 L CNN
F 2 "TestPoint:TestPoint_THTPad_1.0x1.0mm_Drill0.5mm" H 1150 3300 50  0001 C CNN
F 3 "~" H 1150 3300 50  0001 C CNN
	1    950  3300
	1    0    0    -1  
$EndComp
Connection ~ 950  3300
Wire Wire Line
	950  3300 1050 3300
$Comp
L Connector:TestPoint TP1
U 1 1 5F45AC65
P 800 3100
F 0 "TP1" H 858 3218 50  0000 L CNN
F 1 "TestPoint" H 858 3127 50  0000 L CNN
F 2 "TestPoint:TestPoint_THTPad_1.0x1.0mm_Drill0.5mm" H 1000 3100 50  0001 C CNN
F 3 "~" H 1000 3100 50  0001 C CNN
	1    800  3100
	1    0    0    -1  
$EndComp
Connection ~ 800  3100
Wire Wire Line
	800  3100 1050 3100
$Comp
L Device:CP C6
U 1 1 5F4733E8
P 8050 2600
F 0 "C6" H 8168 2646 50  0000 L CNN
F 1 "100n" H 8168 2555 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 8088 2450 50  0001 C CNN
F 3 "~" H 8050 2600 50  0001 C CNN
	1    8050 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 2450 7700 2450
Connection ~ 7700 2450
Wire Wire Line
	8050 2750 7800 2750
Wire Wire Line
	7800 2750 7800 2900
Connection ~ 7800 2900
$EndSCHEMATC

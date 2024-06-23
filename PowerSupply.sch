EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 3
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
L Device:Battery_Cell BT?
U 1 1 61E0C03C
P 5900 3650
AR Path="/61E0C03C" Ref="BT?"  Part="1" 
AR Path="/61E082C1/61E0C03C" Ref="BT1"  Part="1" 
F 0 "BT1" H 5782 3654 50  0000 R CNN
F 1 "CR2032_Battery" H 5782 3745 50  0000 R CNN
F 2 "" V 5900 3710 50  0001 C CNN
F 3 "~" V 5900 3710 50  0001 C CNN
	1    5900 3650
	-1   0    0    1   
$EndComp
Wire Wire Line
	4400 3900 4600 3900
$Comp
L Device:C_Small C?
U 1 1 61E0C043
P 4600 3700
AR Path="/61E0C043" Ref="C?"  Part="1" 
AR Path="/61E082C1/61E0C043" Ref="C5"  Part="1" 
F 0 "C5" H 4692 3746 50  0000 L CNN
F 1 "10uF" H 4692 3655 50  0000 L CNN
F 2 "" H 4600 3700 50  0001 C CNN
F 3 "~" H 4600 3700 50  0001 C CNN
	1    4600 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 61E0C049
P 5000 3700
AR Path="/61E0C049" Ref="C?"  Part="1" 
AR Path="/61E082C1/61E0C049" Ref="C6"  Part="1" 
F 0 "C6" H 5092 3746 50  0000 L CNN
F 1 "100nF" H 5092 3655 50  0000 L CNN
F 2 "" H 5000 3700 50  0001 C CNN
F 3 "~" H 5000 3700 50  0001 C CNN
	1    5000 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 61E0C04F
P 5400 3700
AR Path="/61E0C04F" Ref="C?"  Part="1" 
AR Path="/61E082C1/61E0C04F" Ref="C7"  Part="1" 
F 0 "C7" H 5492 3746 50  0000 L CNN
F 1 "100nF" H 5492 3655 50  0000 L CNN
F 2 "" H 5400 3700 50  0001 C CNN
F 3 "~" H 5400 3700 50  0001 C CNN
	1    5400 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 3900 5400 3800
Wire Wire Line
	5000 3800 5000 3900
Connection ~ 5000 3900
Wire Wire Line
	5000 3900 5400 3900
Wire Wire Line
	4600 3800 4600 3900
Connection ~ 4600 3900
Wire Wire Line
	4600 3900 5000 3900
Wire Wire Line
	4600 3600 4600 3500
Wire Wire Line
	4600 3500 5000 3500
Wire Wire Line
	5000 3500 5000 3600
Wire Wire Line
	5000 3500 5400 3500
Wire Wire Line
	5400 3500 5400 3600
Connection ~ 5000 3500
Wire Wire Line
	5900 3900 5900 3850
Wire Wire Line
	5900 3550 5900 3500
Text HLabel 4400 3900 0    50   BiDi ~ 0
VCC
Wire Wire Line
	5400 3500 5900 3500
Connection ~ 5400 3500
Wire Wire Line
	5400 3900 5900 3900
Connection ~ 5400 3900
Wire Wire Line
	4600 3500 4400 3500
Connection ~ 4600 3500
Text HLabel 4400 3500 0    50   BiDi ~ 0
GND
$EndSCHEMATC

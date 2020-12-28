# АСМОС протокол

``` mermaid
graph TD;
    st[Start byte]  -->     Addr;
    Addr            -->     Cmd_type{{Cmd_type}};
    Cmd_type        -->|Data packet| Addr_counter;
    Addr_counter    -->     Addr_arr[[<<Address array>><br/><br/>Addr 0<br/> Addr 1<br/>...<br/> Addr N]]

    Addr_arr        -->     Data_arr[[<<Data array>><br/><br/>Data 0<br/> Data 1<br/>...<br/>Data N]]

    Data_arr        -->     Data_len;
    Data_len        -->     Data_type;
    Data_type       -->     crc1[CRC16];

    Cmd_type        -->     |ACK packet|crc2[CRC16]; 
    


classDef green fill:#9f6,stroke:#333,stroke-width:2px;
classDef orange fill:#f96,stroke:#333,stroke-width:4px;

class Cmd_type green
```

---

## Global state register

Offset  | Const       | Description 
------- | ----------- | -----------
0       | LoRa_error
1       | Sensor_error
2       | Battary_warning
3       | PC_UART_error
4       | Packet_error 
5       | 
6       | 
7       | 
8       | 
9       | 
10      | 
11      | 
12      | 
13      | 
14      | 
15      | 
16      | 
17      | 
18      | 
19      | 
20      | 
21      | 
22      | 
23      | 
24      | 
25      | 
26      | 
27      | 
28      | 
29      | 
30      | 
31      | 

---

## Global variables

Size    | Name              | Description 
------- | ----------------- | -----------
uint32_t| LoRa_Error_counter|
uint32_t| TX_packet_counter |
uint32_t| RX_packet_counter |
uint32_t| PC_error_counter  |
uint32_t|  

---

## CMD Handler

Offset  | Name                             | Description 
------- | ---------------------------------| -----------
0       | LED1_REGISTER_Handler            | [0:3] - Led mode<br>            000 - N/U<br>            001 - N/U<br>            010 - LEDn_R init bin mode<br>            011 - LEDn_R init PWM mode<br>            100 - LEDn_G init bin mode<br>            101 - LEDn_G init PWM mode<br>            110 - LEDn_B init bin mode<br>            111 - LEDn_B init PWM mode<br>[3:8] - PWM duty cycle
1       | LED2_REGISTER_Handler            | see LED1_REGISTER_Handler
2       | LED3_REGISTER_Handler            | see LED1_REGISTER_Handler
3       | BUZZ_REGISTER1_Handler           | frequency(Hz) = 440 + cmd * 2
4       | BUZZ_REGISTER2_Handler           | buzz_duration(ms) = 10 + cmd * 4;
5       | BUZZ_REGISTER3_Handler           | sets volum[0:3] and play sound
6       | lora_struct.ADDH                 | 
7       | lora_struct.ADDL                 |
8       | lora_struct.SPED                 |
9       | lora_struct.CHAN                 |
10      | lora_struct.MODE                 |
11      | lora_struct.OPTION               |
12      | LORA_REGISTER1_Handler           | sends:<br> 0x00 - lora_struct(SRAM)<br> 0x01 - *0xC1 0xC1 0xC1*<br>0x02 - lora_struct(EEPROM)<br>0x03 - *0xC3 0xC3 0xC3*<br>0x05 - *0xC4 0xC4 0xC4*<br>0x06 - Sleep mode <br>0x07 - Transmit mode <br>0x08 - Write config to SRAM <br> 0x09 - Write config to EEPROM
13      | LORA_REGISTER2_Handler           | Get LoRa configuration (to PC)
14      | TMP1075_CONFIG_REGISTER_Handler  | Write TMP1075 config
15      | N/U                              |
16      | REQUEST_DATA                     | Makes answer to main station data packet request 
17      | PC_CONNECT                       | Sends PC self address and lora_struct
18      | FIFO_PUSH                        | Write to FIFO for retranslate cmd
19      | SEND_UNIC_CMD                    | At the beginning we should write all data to FIFO and after that we send any data to *SEND_UNIC_CMD* for transmit packet 
20      | LoRa_self_address                | Write station's address
21      | NVIC_SystemReset                 | Reset µC
22      | TIME_TO_RESET                    | (ms) before reset (if no packeges)
23      | ADC_REQUEST                      | Works like *REQUEST_DATA* but returns battary voltage
24      | 
25      | 
26      | 
27      | 
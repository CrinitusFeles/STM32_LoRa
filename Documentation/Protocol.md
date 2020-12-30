# АСМОС протокол {ignore}

---

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Packet struct](#packet-struct)
  - [CMD Packet](#cmd-packet)
  - [Data Packet](#data-packet)
- [Global state register](#global-state-register)
- [Global variables](#global-variables)
- [CMD Handler](#cmd-handler)

<!-- /code_chunk_output -->

--- 

## Packet struct

<!DOCTYPE html>
<html lang="en">
   <head>
	 <script src="https://cdnjs.cloudflare.com/ajax/libs/mermaid/8.0.0/mermaid.min.js"></script>
    </head>
	 
<body>
<div class="mermaid">
graph TD;
    st[0: Start byte]  -->     addr[1: Addr];
    addr            -->     Cmd_type{{2: Cmd_type}};
    Cmd_type        -->|Data packet| packet_len[3: Packet length];
    
    packet_len      -->     addr_counter[4: Addr_counter];
    addr_counter    -->     Addr_arr[[<<5...N: Address array>><br/><br/>Addr 0<br/> Addr 1<br/>...<br/> Addr N]];

    Addr_arr        -->     Data_arr[[<<Data array>><br/><br/>Data 0<br/> Data 1<br/>...<br/>Data N]]

    Data_arr        -->     Data_len; 
    Data_len        -->     Data_type;
    Data_type       -->     crc1[CRC16];    
    
    Cmd_type        -->|CMD packet| packet_len_cmd[3: Packet length];
    packet_len_cmd      -->     addr_counter_cmd[4: Addr_counter];
    addr_counter_cmd    -->     Addr_arr_cmd[[<<5...N: Address array>><br/><br/>Addr 0<br/> Addr 1<br/>...<br/> Addr N]];
    Addr_arr_cmd        -->     cmd_r["Command (4 bytes):<br>['r'] [uint8_t] ['b'] [uint8_t]"]
    cmd_r               -->     crc5[CRC16];


        Cmd_type        -->     |ACK packet|crc2[CRC16]; 

classDef green fill:#9f6,stroke:#333,stroke-width:2px;
classDef orange fill:#f96,stroke:#333,stroke-width:4px;

class Cmd_type green
</div>
	
</body>
<script>
var config = {
    startOnLoad:true,
    theme: 'forest',
    flowchart:{
            useMaxWidth:false,
            htmlLabels:true
        }
};
mermaid.initialize(config);
window.mermaid.init(undefined, document.querySelectorAll('.language-mermaid'));
</script>

</html>



---
### CMD Packet
Для примера рассмотрим пример пакета команды. Данный пакет содержит следующие данные:
```python {cmd=true, id="1"}

START_BYTE = 0xFF
TARGET_ADDRESS = 0x05
CMD_TYPE = 0x00
PACKET_LENGTH = 0x0E
ADDRESS_POINTER = 0x03
ADDRESS_0 = 0x01
ADDRESS_1 = 0x02
ADDRESS_2 = 0x03
ADDRESS_3 = 0x04
ADDRESS_4 = 0x05
CMD_BYTE_0 = 0x01
CMD_BYTE_1 = 0xAA
CRC16_BYTE_0 = 0x12
CRC16_BYTE_1 = 0x96
```
Из них видно что, инициатором передачи был адрес *0x01* пакет, а конечной целью адрес *0x05*. Данный пакет уже успел пройти 3 адреса. 

Выведем полученный пакет:
```python {cmd=true, continue="1",  id="2"}
rx_buffer = [START_BYTE, TARGET_ADDRESS, CMD_TYPE, PACKET_LENGTH, ADDRESS_POINTER,
             ADDRESS_0, ADDRESS_1, ADDRESS_2, ADDRESS_3, ADDRESS_4,
             CMD_BYTE_0, CMD_BYTE_1, CRC16_BYTE_0, CRC16_BYTE_1]

print(rx_buffer)
```

Введем переменные для удобного обращения к элементам массива:
```python {cmd=true, continue="2",  id="3"}
TARGET_ADDRESS_POS = 1
CMD_TYPE_POS = 2
PACKET_LENGTH_POS = 3
ADDRESS_POINTER_POS = 4
SOURCE_ADDRESS_POS = 5
CRC_SIZE = 2
CMD_SIZE = 2
PACKET_LENGTH_BYTE = rx_buffer[PACKET_LENGTH_POS]
CURRENT_ADDRESS_INDICATOR = rx_buffer[SOURCE_ADDRESS_POS + rx_buffer[ADDRESS_POINTER_POS]]
PACKET_LAST_INDEX = rx_buffer[PACKET_LENGTH_POS] - 1
LAST_ADDRESS_IN_LIST_POS = PACKET_LAST_INDEX - CRC_SIZE - CMD_SIZE
LAST_ADDRESS_IN_LIST = rx_buffer[LAST_ADDRESS_IN_LIST_POS]
ADRESS_AMOUNT = LAST_ADDRESS_IN_LIST_POS - SOURCE_ADDRESS_POS + 1
```

Проверим корректность некоторых из них:
```python {cmd=true, continue="3",  id="4"}
if CURRENT_ADDRESS_INDICATOR == ADDRESS_3:
    print("CURRENT_ADDRESS_INDICATOR ok")
if LAST_ADDRESS_IN_LIST == ADDRESS_4:
    print("LAST_ADDRESS_IN_LIST ok")
if ADRESS_AMOUNT == 5:
    print("ADRESS_AMOUNT ok")
```

---
### Data Packet

```python {cmd=true, id="5"}

START_BYTE = 0xFF
TARGET_ADDRESS = 0x05
CMD_TYPE = 0x00
PACKET_LENGTH = 0x18
ADDRESS_POINTER = 0x03
ADDRESS_0 = 0x01
ADDRESS_1 = 0x02
ADDRESS_2 = 0x03
ADDRESS_3 = 0x04
ADDRESS_4 = 0x05
DATA0_0 = 0xC1
DATA0_1 = 0xD4
DATA1_1 = 0xBB
DATA1_0 = 0x21
DATA2_1 = 0x06
DATA2_0 = 0xC1
DATA3_0 = 0xD4
DATA3_1 = 0xBB
DATA4_0 = 0x21
DATA4_1 = 0x06
DATA_TYPE = 0x00
DATA_LENGTH = 0x0A
CRC16_BYTE_0 = 0x12
CRC16_BYTE_1 = 0x96
```

Выведем полученный пакет:
```python {cmd=true, continue="5",  id="6"}
rx_buffer = [START_BYTE, TARGET_ADDRESS, CMD_TYPE, PACKET_LENGTH, ADDRESS_POINTER,
             ADDRESS_0, ADDRESS_1, ADDRESS_2, ADDRESS_3, ADDRESS_4, DATA0_0, 
             DATA0_1, DATA1_0, DATA1_1, DATA2_0, DATA2_1, DATA3_0, DATA3_1, 
             DATA4_0, DATA4_1, DATA_TYPE, DATA_LENGTH, CRC16_BYTE_0, CRC16_BYTE_1]

print(rx_buffer)
```

Введем переменные для удобного обращения к элементам массива:
```python {cmd=true, continue="6",  id="7"}
TARGET_ADDRESS_POS = 1
CMD_TYPE_POS = 2
PACKET_LENGTH_POS = 3
ADDRESS_POINTER_POS = 4
SOURCE_ADDRESS_POS = 5
CURRENT_ADDRESS_INDICATOR = rx_buffer[SOURCE_ADDRESS_POS + rx_buffer[ADDRESS_POINTER_POS]]

CRC16_SIZE = 2
PACKET_LAST_POS = rx_buffer[PACKET_LENGTH_POS] - 1 
DATA_LENGTH_POS = PACKET_LAST_POS - CRC16_SIZE
DATA_LENGTH_BYTE = rx_buffer[DATA_LENGTH_POS]

FIRST_DATA_BYTE_POS = DATA_LENGTH_POS - DATA_LENGTH_BYTE - 1
LAST_ADDRESS_IN_LIST_POS = FIRST_DATA_BYTE_POS - 1
LAST_ADDRESS_IN_LIST = rx_buffer[LAST_ADDRESS_IN_LIST_POS]
ADDRESS_AMOUNT = LAST_ADDRESS_IN_LIST_POS - SOURCE_ADDRESS_POS + 1
```

Проверим корректность некоторых из них:
```python {cmd=true, continue="7",  id="8"}
if CURRENT_ADDRESS_INDICATOR == ADDRESS_3:
    print("CURRENT_ADDRESS_INDICATOR ok")
if LAST_ADDRESS_IN_LIST == ADDRESS_4:
    print("LAST_ADDRESS_IN_LIST ok")
if ADDRESS_AMOUNT == 5:
    print("ADRESS_AMOUNT ok")
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



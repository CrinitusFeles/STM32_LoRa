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

## Global variables

Size    | Name              | Description 
------- | ----------------- | -----------
uint32_t| LoRa_Error_counter|
uint32_t| TX_packet_counter |
uint32_t| RX_packet_counter |
uint32_t| PC_error_counter  |
uint32_t|  
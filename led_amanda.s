
@ Constants for assembler - memory map associated
.equ    gpiobase, 0x3F000000     @ RPi 2,3,4, 400 peripherals
.equ    offset,  0x200000       @ start of GPIO device
.equ    prot_read, 0x1          @ can be read
.equ    prot_write,0x2          @ can be written
.equ    readwrite,prot_read|prot_write
.equ    mapshare,  0x01         @ share changes
.equ    nopref,    0
.equ    pagesize,  4096         @ memory size
 
.equ    O_RDWR,  00000002       @ open for read/write
.equ    O_DSYNC, 00010000       @ values are octal not hex
.equ    __O_SYNC,04000000
.equ    O_SYNC,__O_SYNC|O_DSYNC
.equ    openflags,O_RDWR|O_SYNC @ open file flags

@ Constants for Function Select
.equ    red_led, 19         @ pin set bit
.equ    green_led, 13       @ pin set bits
.equ    output,   1            @ use pin for ouput
.equ    pinfield, 0b111        @ 3 bits


@ Constants for assembler pinclear and pinset
.equ  pinbit,       1           @ 1 bit for pin
.equ  registerpins, 32
.equ  GPCLR0,   0x28            @ clear register offset
.equ  GPSET0,   0x1c            @ set register offset

@ addresses of messages
devicefile:     .word   device
openMode:       .word   openflags
gpio:           .word   gpiobase+offset
openerror:      .word   openstring1
memerror:       .word   memstring2
   

@ Constant program data
.section .rodata
.align  2
device:         .asciz  "/dev/gpiomem"
openstring1:    .asciz  "Didnt open /dev/gpiomem\n"
memstring2:     .asciz  "Didnt Map /dev/gpiomem \n"

@ Data section to store initialized data or constants
.data

@ Menu message
.balign 4 @ ensure variable is 4-byte aligned 
message1:
    .asciz "\n<< LED Display >>\n(1) Turn on the red LED light\n(2) Turn on the green LED light\n(3) Turn off both LED light\n(4) Exit the program\nEnter your option: "

@ User option message
.balign 4 @ ensure variable is 4-byte aligned 
message2:
    .asciz "\nYou have chosen option %d\n"

@ Invalid option message
message3:
    .asciz "\nYou have chosen an invalid option\n"

@ Option 1 message
message4:
    .asciz "\nRed LED light is turned on!\n"

@ Option 2 message
message5:
    .asciz "\nGreen LED light is turned on!\n"

@ Option 3 message
message6:
    .asciz "\nBoth LED lights are turned off!\n"

@ Format pattern for scanf
.balign 4 @ ensure variable is 4-byte aligned
scan_pattern: 
    .asciz "%d"

@ Where scanf will store the user input
.balign 4
number_read: .word 0

.text @ Text section
.global main @ Start of assembly code
.extern printf @ External function printf
.extern scanf @ External function scanf

main:
    PUSH {ip, lr} @ store return address, link register (lr) and intra procedural call (ip) register in stack

    @ Open /dev/gpiomem for read/write and syncing
    LDR     R0, devicefile  @ address of /dev/gpiomem string
    LDR     R1, openMode    @ flags for accessing device
    BL      open            @ call open
    MOVS    R4, R0		@ error check
    BPL     map_gpio		@ If positive, moveon
    LDR     R0, openerror   @ error, tell user
    BL      printf
    
LOOP:    
    LDR R0, address_menu_input @ load address_menu_input into R0
    BL printf @ branch and link to call the function printf

    LDR R0, address_of_scan_pattern @ load address_of_user_input into r0
    LDR R1, address_of_user_input @ load user input into r1
    BL scanf @ branch and link to call the function scanf

    LDR R0, address_option_message @ load address_option_message into r0
    LDR R1, address_of_user_input @ load address_of_user_input into r1
    LDR  R1, [R1] @ load the integer input into r1
    BL printf  @ branch and link to call the function printf

    LDR R0, address_of_user_input @ load address_of_user_input into r0
    LDR R0, [R0] @ load the integer input into r0

    @ If option is neither 1, 2, 3 or 4 it will be an invalid option
    CMP R0, #1 @ compare user input with 1
    CMPNE R0, #2 @ compare user input with 2
    CMPNE R0, #3 @ compare user input with 3
    CMPNE R0, #4 @ compare user input with 4
    BNE invalid_option @ print that option is invalid and loop the menu again

    @ If option is 1, turn on red LED light
    CMP R0, #1 @ compare user input with 1
    BEQ case_1 @ proceed to case 1

    @ If option is 2, turn on green LED light
    CMP R0, #2 @ compare user input with 2
    BEQ case_2 @ proceed to case 2

    @ If option is 3, turn off both LED lights
    CMP R0, #3 @ compare user input with 3
    BEQ case_3 @ proceed to case 3

    @ If option is 4, exit the program
    CMP R0, #4 @ compare user input with 4
    BEQ case_4 @ proceed to case 4

    B LOOP @ branch back and re-run the loop
 
END:
    POP 	{R8}        @ restore SP to entry level
    POP 	{R9}
    MOV 	R7, #1
    POP     {ip, pc}    @ pop ip and lr reg value to PC, return

invalid_option:
    LDR R0, address_of_invalid_option @ load address_of_invalid_option into r0
    BL printf @ branch and link to call the function printf
    B LOOP @ branch back and re-run the loop

case_1:
    LDR R0, address_option_one @ load address_option_one into r0
    BL printf @ branch and link to call the function printf

    @ Turn on red LED
    MOV     R0, R9              @ get memory address
    MOV     R1, #red_led        @ get pin number of green LED
    ADD     R4, R0, #GPSET0     @ point to GPSET regs in R4
    MOV     R7, R1              @ save pin number

	@ Compute address of GPSET register and pin field        
    MOV     R3, #registerpins   @ divisor
    UDIV    R0, R7, R3          @ GPSET number
    MUL     R1, R0, R3          @ compute remainder
    SUB     R1, R7, R1          @ for relative pin position
    LSL     R0, R0, #2          @ 4 bytes in a register
    ADD     R0, R0, R4          @ address of GPSETn

	@ Set up the gpio pin funtion register in programming memory
    LDR     R2, [R0]        @ get entire register
    MOV     R3, #pinbit     @ one pin
    LSL     R3, R3, R1      @ shift to pin position
    ORR     R2, R2, R3      @ set bit
    STR     R3, [R0]        @ update register

    B LOOP @ branch back and re-run the loop

case_2:
    LDR r0, address_option_two @ load address_option_two into r0
    BL printf @ branch and link to call the function printf

    @ Turn on green LED
    MOV     R0, R9              @ get memory address
    MOV     R1, #green_led		@ get pin number of green LED
    ADD     R4, R0, #GPSET0     @ point to GPSET regs in R4
    MOV     R5, R1              @ save pin number

	@ Compute address of GPSET register and pin field        
    MOV     R3, #registerpins   @ divisor
    UDIV    R0, R5, R3          @ GPSET number
    MUL     R1, R0, R3          @ compute remainder
    SUB     R1, R5, R1          @ for relative pin position
    LSL     R0, R0, #2          @ 4 bytes in a register
    ADD     R0, R0, R4          @ address of GPSETn

	@ Set up the gpio pin funtion register in programming memory
    LDR     R2, [R0]        @ get entire register
    MOV     R3, #pinbit     @ one pin
    LSL     R3, R3, R1      @ shift to pin position
    ORR     R2, R2, R3      @ set bit
    STR     R2, [R0]        @ update register

    B LOOP @ branch back and re-run the loop

case_3:
    LDR r0, address_option_three @ load address_option_three into r0
    BL printf @ branch and link to call the function printf

    @ Turn off green LED
    MOV     R0, R9			@ get gpio mapped address
    MOV     R1, #green_led	@ get pin number of green LED
    ADD     R4, R0, #GPCLR0	@ pointer to GPSET regs.
    MOV     R5, R1			@ save pin number

	@ Compute address of GPSET register and pin field        
    MOV     R3, #registerpins	@ divisor
    UDIV    R0, R5, R3          @ GPSET number
    MUL     R1, R0, R3          @ compute remainder
    SUB     R1, R5, R1          @ for relative pin position
    LSL     R0, R0, #2          @ 4 bytes in a register
    ADD     R0, R0, R4          @ address of GPSETn

	@ Set up the gpio pin funtion register in programming memory
    LDR     R2, [R0]         @ get entire register
    MOV     R3, #pinbit      @ one pin
    LSL     R3, R3, R1       @ shift to pin position
    ORR     R2, R2, R3       @ clear bit
    STR     R2, [R0]         @ update register


	@ Turn off red LED
    MOV     R0, R9			@ get gpio mapped address
    MOV     R1, #red_led	@ get pin number of red LED
    ADD     R4, R0, #GPCLR0	@ pointer to GPSET regs.
    MOV     R5, R1			@ save pin number

	@ Compute address of GPSET register and pin field        
    MOV     R3, #registerpins   @ divisor
    UDIV    R0, R5, R3          @ GPSET number
    MUL     R1, R0, R3          @ compute remainder
    SUB     R1, R5, R1          @ for relative pin position
    LSL     R0, R0, #2          @ 4 bytes in a register
    ADD     R0, R0, R4          @ address of GPSETn

	@ Set up the gpio pin funtion register in programming memory
    LDR		R2, [R0]		@ get entire register
    MOV     R3, #pinbit		@ one pin
    LSL     R3, R3, R1		@ shift to pin position
    ORR     R2, R2, R3		@ clear bit
    STR     R2, [R0]		@ update register

    B LOOP @ branch back and re-run the loop

case_4:
    @ Turn off any LED when user exits the program
    @ Turn off green LED
    MOV     R0, R9			@ get gpio mapped address
    MOV     R1, #green_led	@ get pin number of green LED
    ADD     R4, R0, #GPCLR0	@ pointer to GPSET regs.
    MOV     R5, R1			@ save pin number

	@ Compute address of GPSET register and pin field        
    MOV     R3, #registerpins	@ divisor
    UDIV    R0, R5, R3          @ GPSET number
    MUL     R1, R0, R3          @ compute remainder
    SUB     R1, R5, R1          @ for relative pin position
    LSL     R0, R0, #2          @ 4 bytes in a register
    ADD     R0, R0, R4          @ address of GPSETn

	@ Set up the gpio pin funtion register in programming memory
    LDR     R2, [R0]         @ get entire register
    MOV     R3, #pinbit      @ one pin
    LSL     R3, R3, R1       @ shift to pin position
    ORR     R2, R2, R3       @ clear bit
    STR     R2, [R0]         @ update register


	@ Turn off red LED
    MOV     R0, R9			@ get gpio mapped address
    MOV     R1, #red_led	@ get pin number of red LED
    ADD     R4, R0, #GPCLR0	@ pointer to GPSET regs.
    MOV     R5, R1			@ save pin number

	@ Compute address of GPSET register and pin field        
    MOV     R3, #registerpins   @ divisor
    UDIV    R0, R5, R3          @ GPSET number
    MUL     R1, R0, R3          @ compute remainder
    SUB     R1, R5, R1          @ for relative pin position
    LSL     R0, R0, #2          @ 4 bytes in a register
    ADD     R0, R0, R4          @ address of GPSETn

	@ Set up the gpio pin funtion register in programming memory
    LDR		R2, [R0]		@ get entire register
    MOV     R3, #pinbit		@ one pin
    LSL     R3, R3, R1		@ shift to pin position
    ORR     R2, R2, R3		@ clear bit
    STR     R2, [R0]		@ update register
    
    B END @ break the loop

map_gpio:
@ Map GPIO memory to a main memory location so we can access them
@ Keep a copy of the mapped memory address returned in R0 
@ this will be need later on.

    MOV     R4, R0          @ use r4 for file descriptor
    MOV     R8, R0          @ Save a copy of file descriptor
    
    LDR     R0, gpio        @ address of GPIO
    MOV     R9, R0          @ save a copy of GPIO address
    PUSH    {R9}            @ Copy on stack for mmmap
    PUSH    {R8}            @ file descriptor on stack for mmap
    MOV     R0, #nopref     @ let kernel pick memory
    MOV     R1, #pagesize   @ get 1 page of memory
    MOV     R2, #readwrite  @ read/write this memory
    MOV     R3, #mapshare   @ share with other processes
    BL      mmap            @ R0-R3+top of stack has info
    MOV     R9,R0           @ save mapped address
    CMP     R0, #-1         @ check for error
    BNE     set_pin         @ no error, continue
    LDR     R0, memerror    @ error, tell user
    BL      printf
    B       END 

set_pin:  
@  Select pin number and function. (green)
    MOV     R0, R9          @ programming memory
    MOV     R1, #green_led  @ pin number
    MOV     R2, #output     @ pin function
    MOV     R4, R0          @ save pointer to GPIO
    MOV     R5, R1          @ save pin number
    MOV     R6, R2          @ save function code

@ Compute address of GPFSEL register and pin field
    MOV     R3, #10         @ divisor
    UDIV    R0, R5, R3      @ GPFSEL number
    MUL     R1, R0, R3      @ compute remainder
    SUB     R1, R5, R1      @ for GPFSEL pin    
        
@ Set up the GPIO pin funtion register in programming memory
    LSL     R0, R0, #2      @4 bytes in a register
    ADD     R0, R4, R0      @ GPFSELn address
    LDR     R2, [R0]        @ get entire register
    
    MOV     R3, R1          @ need to multiply pin
    ADD     R1, R1, R3, lsl #1   @    position by 3
    MOV     R3, #pinfield   @ gpio pin field
    LSL     R3, R3, R1      @ shift to pin position
    BIC     R2, R2, R3      @ clear pin field

    LSL     R6, R6, R1      @ shift function code to pin position
    ORR     R2, R2, R6      @ enter function code
    STR     R2, [R0]        @ update register

@  Select pin number and function. (red)
    MOV     R0, R9          @programming memory
    MOV     R1, #red_led    @pin number for red LED
    MOV     R10, #output     @pin function
    MOV     R4, R0          @save pointer to GPIO
    MOV     R5, R1          @save pin number
    MOV     R6, R10          @save function code

@ Compute address of GPFSEL register and pin field
    MOV     R3, #10         @divisor
    UDIV    R0, R5, R3      @GPFSEL number
    MUL     R1, R0, R3      @compute remainder
    SUB     R1, R5, R1      @for GPFSEL pin    
        
@ Set up the GPIO pin funtion register in programming memory
    LSL     R0, R0, #2      @4 bytes in a register
    ADD     R0, R4, R0      @GPFSELn address
    LDR     R10, [R0]        @get entire register
    
    MOV     R3, R1                  @need to multiply pin
    ADD     R1, R1, R3, lsl #1      @position by 3
    MOV     R3, #pinfield           @gpio pin field
    LSL     R3, R3, R1              @shift to pin position
    BIC     R10, R10, R3              @clear pin field

    LSL     R6, R6, R1      @shift function code to pin position
    ORR     R10, R10, R6      @enter function code
    STR     R10, [R0]        @update register
    B       LOOP

address_menu_input : .word message1
address_option_message : .word message2
address_of_scan_pattern : .word scan_pattern
address_of_user_input : .word number_read
address_of_invalid_option: .word message3
address_option_one : .word message4
address_option_two : .word message5
address_option_three : .word message6

@ Reference
@ Title: Raspberry Pi Operating System Assembly Language                        
@ Author: Bruce Smith                                                           
@ Date published: March 18, 2021                                                
@ Date cited: November 2, 2022                                                  
@ Code version: 4th edition                                                     
@ Availability: https://www.brucesmith.info/raspberry-pi-assembly-language.html 


@ led.s - turn on and off LED lights connected to GPIO of RPi 4 (Question 2 Task 2)
@ Constants for assembler - memory map associated
.equ    gpiobase, 0x3F000000    @ Raspberry Pi 4 peripherals base address
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
.equ    output, 1           @ use pin for ouput
.equ    pinfield, 0b111     @ 3 bits


@ Constants for assembler pinclear and pinset
.equ  pinbit, 1         @ 1 bit for pin
.equ  registerpins, 32
.equ  GPCLR0, 0x28      @ clear register offset
.equ  GPSET0, 0x1c      @ set register offset

@ addresses of messages
devicefile: .word device
openMode: .word openflags
gpio: .word gpiobase+offset
openerror: .word openstring1
memerror: .word memstring2
   

@ Constant program data
.section .rodata
device: .asciz  "/dev/gpiomem"
openstring1: .asciz  "Didnt open /dev/gpiomem\n"
memstring2: .asciz  "Didnt Map /dev/gpiomem \n"


@ Data section to store initialized data or constants
.data
@ message to prompt for input
.balign 4
choice_menu: .asciz "\n=======Choices=======\n [1]turn on red LED\n [2]turn on green LED\n [3]turn off LEDs\n [0]exit\n\nEnter your choice: "

@ format pattern for scanf
.balign 4
choice_pattern: .asciz "%d"

@ store user input
.balign 4
choice_input: .word 5

@ check user input
.balign 4
choice_sel: .asciz "\n> Option [%d] selected\n"

@ invalid user input
.balign 4
choice_invalid: .asciz "\n> Invalid input, please enter 1, 2, 3 or 0.\n"

@ message when user select 1 
.balign 4 
on_red_msg: .asciz "\n> Turning on red LED...\n"

@ message when user select 2 
.balign 4 
on_green_msg: .asciz "\n> Turning on green LED...\n"

@ message when user select 3 
.balign 4 
off_led_msg: .asciz "\n> Turning off LEDs...\n"

@ message when user select 0 
.balign 4 
exit_msg: .asciz "\n> Exiting...\n"


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
    MOVS    R4, R0		    @ error check
    BPL     map_gpio		@ If positive, moveon
    LDR     R0, openerror   @ error, tell user
    BL      printf
    
LOOP:    
    LDR R0, address_choice_menu @ load address_choice_menu into R0
    BL printf @ branch and link to call the function printf

    LDR R0, address_choice_pattern @ load address_choice_input into r0
    LDR R1, address_choice_input @ load user input into r1
    BL scanf @ branch and link to call the function scanf

    LDR R0, address_choice_sel @ load address_choice_sel into r0
    LDR R1, address_choice_input @ load address_choice_input into r1
    LDR  R1, [R1] @ load the integer input into r1
    BL printf  @ branch and link to call the function printf

    LDR R0, address_choice_input @ load address_choice_input into r0
    LDR R0, [R0] @ load the integer input into r0

    @ If option is neither 1, 2, 3 or 4 it will be an invalid option
    CMP R0, #1 @ compare user input with 1
    CMPNE R0, #2 @ compare user input with 2
    CMPNE R0, #3 @ compare user input with 3
    CMPNE R0, #0 @ compare user input with 0
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

    @ If option is 0, exit the program
    CMP R0, #0 @ compare user input with 0
    BEQ case_0 @ proceed to case 0

    B LOOP @ branch back and re-run the loop
 
END:
    POP 	{R8}        @ restore SP to entry level
    POP 	{R9}
    MOV 	R7, #1
    POP     {ip, pc}    @ pop ip and lr reg value to PC, return

invalid_option:
    LDR R0, address_choice_invalid @ load address_choice_invalid into r0
    BL printf @ branch and link to call the function printf
    B LOOP @ branch back and re-run the loop

case_1:
    LDR R0, address_case_one_msg @ load address_case_one_msg into r0
    BL printf @ branch and link to call the function printf

    @ Turn on red LED
    MOV     R0, R9              @ get memory address
    MOV     R1, #red_led        @ get pin number of red LED
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
    LDR R0, address_case_two_msg @ load address_case_two_msg into r0
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
    LDR R0, address_case_three_msg @ load address_case_three_msg into r0
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
    MOV     R7, R1			@ save pin number

	@ Compute address of GPSET register and pin field        
    MOV     R3, #registerpins   @ divisor
    UDIV    R0, R7, R3          @ GPSET number
    MUL     R1, R0, R3          @ compute remainder
    SUB     R1, R7, R1          @ for relative pin position
    LSL     R0, R0, #2          @ 4 bytes in a register
    ADD     R0, R0, R4          @ address of GPSETn

	@ Set up the gpio pin funtion register in programming memory
    LDR		R2, [R0]		@ get entire register
    MOV     R3, #pinbit		@ one pin
    LSL     R3, R3, R1		@ shift to pin position
    ORR     R2, R2, R3		@ clear bit
    STR     R3, [R0]		@ update register

    B LOOP @ branch back and re-run the loop

case_0:
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
    MOV     R7, R1			@ save pin number

	@ Compute address of GPSET register and pin field        
    MOV     R3, #registerpins   @ divisor
    UDIV    R0, R7, R3          @ GPSET number
    MUL     R1, R0, R3          @ compute remainder
    SUB     R1, R7, R1          @ for relative pin position
    LSL     R0, R0, #2          @ 4 bytes in a register
    ADD     R0, R0, R4          @ address of GPSETn

	@ Set up the gpio pin funtion register in programming memory
    LDR		R2, [R0]		@ get entire register
    MOV     R3, #pinbit		@ one pin
    LSL     R3, R3, R1		@ shift to pin position
    ORR     R2, R2, R3		@ clear bit
    STR     R3, [R0]		@ update register

    LDR R0, address_exit @ load address_exit into r0
    BL printf @ branch and link to call the function printf
    
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
    @  Select pin number of green LED and function.
    MOV     R0, R9          @ programming memory
    MOV     R1, #green_led  @ GPIO pin number of green LED
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
    LSL     R0, R0, #2      @ 4 bytes in a register
    ADD     R0, R4, R0      @ GPFSELn address
    LDR     R2, [R0]        @ get entire register
    
    MOV     R3, R1              @ need to multiply pin
    ADD     R1, R1, R3, lsl #1  @ position by 3
    MOV     R3, #pinfield       @ gpio pin field
    LSL     R3, R3, R1          @ shift to pin position
    BIC     R2, R2, R3          @ clear pin field

    LSL     R6, R6, R1      @ shift function code to pin position
    ORR     R2, R2, R6      @ enter function code
    STR     R2, [R0]        @ update register

    @ Select pin number of red LED and function.
    MOV     R0, R9          @ programming memory
    MOV     R1, #red_led    @ GPIO pin number of red LED
    MOV     R10, #output    @ pin function
    MOV     R4, R0          @ save pointer to GPIO
    MOV     R5, R1          @ save pin number
    MOV     R6, R10         @ save function code

    @ Compute address of GPFSEL register and pin field
    MOV     R3, #10         @ divisor
    UDIV    R0, R5, R3      @ GPFSEL number
    MUL     R1, R0, R3      @ compute remainder
    SUB     R1, R5, R1      @ for GPFSEL pin    
        
    @ Set up the GPIO pin funtion register in programming memory
    LSL     R0, R0, #2      @ 4 bytes in a register
    ADD     R0, R4, R0      @ GPFSELn address
    LDR     R10, [R0]       @ get entire register
    
    MOV     R3, R1                  @ need to multiply pin
    ADD     R1, R1, R3, lsl #1      @ position by 3
    MOV     R3, #pinfield           @ gpio pin field
    LSL     R3, R3, R1              @ shift to pin position
    BIC     R10, R10, R3            @ clear pin field

    LSL     R6, R6, R1      @ shift function code to pin position
    ORR     R10, R10, R6    @ enter function code
    STR     R10, [R0]       @ update register
    B       LOOP

@ addresses of input and input messages
address_choice_menu : .word choice_menu
address_choice_sel : .word choice_sel
address_choice_pattern : .word choice_pattern
address_choice_input : .word choice_input
address_choice_invalid: .word choice_invalid
address_case_one_msg : .word on_red_msg
address_case_two_msg : .word on_green_msg
address_case_three_msg : .word off_led_msg
address_exit: .word exit_msg


@ Reference
@ Title: Raspberry Pi Operating System Assembly Language                        
@ Author: Bruce Smith                                                           
@ Date published: March 18, 2021                                                
@ Date cited: November 2, 2022                                                  
@ Code version: 4th edition                                                     
@ Availability: https://www.brucesmith.info/raspberry-pi-assembly-language.html 

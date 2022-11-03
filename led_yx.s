@ Constants for assembler - memory map associated
.equ    gpiobase,0x3F000000     @Rasberry Pi 4 peripherals base address
.equ    offset, 0x200000        @start of gpio device
.equ    prot_read, 0x1          @can be read
.equ    prot_write, 0x2         @can be written
.equ    readwrite, prot_read|prot_write
.equ    mapshare, 0x01          @share changes
.equ    nopref, 0
.equ    pagesize, 4096          @memory size
 
.equ    O_RDWR, 00000002        @open for read/write
.equ    O_DSYNC, 00010000       @values are octal not hex
.equ    __O_SYNC, 04000000
.equ    O_SYNC, __O_SYNC|O_DSYNC
.equ    openflags, O_RDWR|O_SYNC    @open file flags

@ Constants for Function Select
.equ    red_led, 19         @red LED gpio pin
.equ    green_led, 13       @green LED gpio pin
.equ    output, 1           @pins are for output (light up)
.equ    pinfield, 0b111     @3 bits

@ Constants for assembler pinclear and pinset
.equ    pinbit, 1           @1 bit for pin
.equ    registerpins, 32
.equ    GPCLR0, 0x28        @clear register offset
.equ    GPSET0, 0x1c        @set register offset

@ addresses of messages
devicefile: .word device
openMode: .word openflags
gpio: .word gpiobase+offset
openerror: .word openstring1
memerror: .word memstring2

@=================================================================@
@ Constant program data
.section .rodata
.align 2
device: .asciz "/dev/gpiomem"
openstring1: .asciz "Didnt open /dev/gpiomem\n"
memstring2: .asciz "Didnt Map /dev/gpiomem \n"

@=================================================================@
@ Data section
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


@=================================================================@
@ Text section - code starts here
.text
.global main
.extern printf
.extern scanf

main:
    PUSH    {ip, lr}    @store return address
    MOV     R10, #5      @default value R10=5   

    @ Open "/dev/gpiomem" for read/write and syncing
    LDR     R0, devicefile  @store address of /dev/gpiomem string in R0
    LDR     R1, openMode    @store flags for accessing device in R1
    BL      open            @call open
    MOVS    R4, R0          @check for errors
    BPL     map_gpio		@if positive, branch to map_gpio
    LDR     R0, openerror   @if there is error, display error message
    BL      printf   		@branch link to printf

loop:
    CMP     R10, #0     @compare R10 to 0
    BEQ     end         @end loop if R10=0
	
    @ Print all choices
    LDR     R0, =choice_menu    @load string into R0
    BL      printf              @branch link to printf

    @ Get user input
    LDR     R0, =choice_pattern @load choice_pattern into R0
    LDR     R1, =choice_input   @load choice_input into R1
    BL      scanf               @read user input
    
    @ Save user input
    LDR     R1, =choice_input   @load address of choice_input in R1
    LDR     R10, [R1]           @read value from choice_input into R10

    @ Print user input
    LDR     R0, =choice_sel     @load address of choice_sel in R0
    LDR     R1, =choice_input   @load address of choice_input in R1
    LDR     R1, [R1]            @read value from choice_input into R1
    BL      printf              @check input
    
    @ Check for valid user input
    CMP     R10, #3     @check choice_input>3 
    MOVGT   R2, #1      @if choice_input>3, R2=1 (true)
    MOVLE   R2, #0      @if choice_input<=3, R2=0 (false)

    CMP     R10, #0     @check choice_input<0
    MOVLT   R3, #1      @if choice_input<0, R3=1 (true)
    MOVGE   R3, #0      @if choice_input>=0, R3=0 (false)

    ORR     R2, R2, R3	@check if input is valid (0) or invalid (1)

    CMP     R2, #0      @check if R2==0 (R2 is valid)
    BEQ     case_0      @if R2==0, go to case_0

    LDR     R0, =choice_invalid     @load invalid message string into R0
    BL      printf                  @branch link to printf
    B       loop                    @re-loop to ask for input again

case_0:
    @ Option 0 - exit loop and end program
    CMP     R10, #0     @compare R10 to 0 (if user entered 0)
    MOVNE   R2, #0      @if R10!=0, R2=0
    MOVEQ   R2, #1      @if R10==0, R2=1

    CMP     R2, #0      @compare R2 to 0 (user input is not 0)
    BEQ     case_1      @branch to loop

    LDR     R0, =exit_msg       @load string into R0
    BL      printf              @branch link to printf (print msg)

    @ Turn off green LED if it is on
    MOV     R0, R9			@get gpio mapped address
    MOV     R1, #green_led	@get pin number of green LED
    ADD     R4, R0, #GPCLR0	@pointer to GPSET regs.
    MOV     R5, R1			@save pin number

    @ Compute address of GPSET register and pin field        
    MOV     R3, #registerpins	@divisor
    UDIV    R0, R5, R3          @GPSET number
    MUL     R1, R0, R3          @compute remainder
    SUB     R1, R5, R1          @for relative pin position
    LSL     R0, R0, #2          @4 bytes in a register
    ADD     R0, R0, R4          @address of GPSETn
        
    @ Set up the gpio pin funtion register in programming memory
    LDR     R2, [R0]         @get entire register
    MOV     R3, #pinbit      @one pin
    LSL     R3, R3, R1       @shift to pin position
    ORR     R2, R2, R3       @clear bit
    STR     R2, [R0]         @update register


    @ Turn off red LED if it is on
    MOV     R0, R9			@Get gpio mapped address
    MOV     R1, #red_led	@get pin number of red LED
    ADD     R4, R0, #GPCLR0	@pointer to GPSET regs.
    MOV     R5, R1			@save pin number

    @ Compute address of GPSET register and pin field        
    MOV     R3, #registerpins   @divisor
    UDIV    R0, R5, R3          @GPSET number
    MUL     R1, R0, R3          @compute remainder
    SUB     R1, R5, R1          @for relative pin position
    LSL     R0, R0, #2          @4 bytes in a register
    ADD     R0, R0, R4          @address of GPSETn
        
    @ Set up the gpio pin funtion register in programming memory
    LDR     R2, [R0]        @get entire register
    MOV     R3, #pinbit     @one pin
    LSL     R3, R3, R1      @shift to pin position
    ORR     R2, R2, R3      @clear bit
    STR     R2, [R0]        @update register
    
    B       end            @end case_0, branch back and rerun loop

case_1:
    @ Option 1 - turn on red LED
    CMP     R10, #1     @compare R10 to 1 (if user entered 1)
    MOVNE   R2, #0      @if R10!=1, R2=0
    MOVEQ   R2, #1      @if R10==1, R2=1

    CMP     R2, #0      @compare R2 to 0 (user input is not 1)
    BEQ     case_2      @branch to case_2

    LDR     R0, =on_red_msg     @load on_red_msg string into R0
    BL      printf              @branch link to printf

    @ Turn on red LED
    MOV     R0, R9              @get memory address
    MOV     R1, #red_led        @get pin number of green LED
    ADD     R4, R0, #GPSET0     @point to GPSET regs in R4
    MOV     R7, R1              @save pin number
    
    @ Compute address of GPSET register and pin field        
    MOV     R3, #registerpins   @divisor
    UDIV    R0, R7, R3          @GPSET number
    MUL     R1, R0, R3          @compute remainder
    SUB     R1, R7, R1          @for relative pin position
    LSL     R0, R0, #2          @4 bytes in a register
    ADD     R0, R0, R4          @address of GPSETn
        
    @ Set up the gpio pin funtion register in programming memory
    LDR     R2, [R0]        @get entire register
    MOV     R3, #pinbit     @one pin
    LSL     R3, R3, R1      @shift to pin position
    ORR     R2, R2, R3      @set bit
    STR     R3, [R0]        @update register

    B       loop            @end case_1, branch back and rerun loop
    
case_2:
    @ Option 2 - turn on green LED
    CMP     R10, #2     @compare R10 to 2 (if user entered 2)
    MOVNE   R2, #0      @if R10!=2, R2=0
    MOVEQ   R2, #1      @if R10==2, R2=1

    CMP     R2, #0      @compare R2 to 0 (user input is not 2)
    BEQ     case_3      @branch to case_3

    LDR     R0, =on_green_msg   @load om_green_msg string into R2
    BL      printf              @branch link to printf

    @ Turn on green LED
    MOV     R0, R9              @get memory address
    MOV     R1, #green_led		@get pin number of green LED
    ADD     R4, R0, #GPSET0     @point to GPSET regs in R4
    MOV     R5, R1              @save pin number
        
    @ Compute address of GPSET register and pin field        
    MOV     R3, #registerpins   @divisor
    UDIV    R0, R5, R3          @GPSET number
    MUL     R1, R0, R3          @compute remainder
    SUB     R1, R5, R1          @for relative pin position
    LSL     R0, R0, #2          @4 bytes in a register
    ADD     R0, R0, R4          @address of GPSETn
        
    @ Set up the gpio pin funtion register in programming memory
    LDR     R2, [R0]        @get entire register
    MOV     R3, #pinbit     @one pin
    LSL     R3, R3, R1      @shift to pin position
    ORR     R2, R2, R3      @set bit
    STR     R2, [R0]        @update register
        
    B       loop			@end case_2, branch back and rerun loop

case_3:
    @ Option 3 - turn off all LEDs
    CMP     R10, #3     @compare R10 to 3 (if user entered 3)
    MOVNE   R2, #0      @if R10!=3, R2=0
    MOVEQ   R2, #1      @if R10==3, R2=1

    CMP     R2, #0      @compare R2 to 0 (user input is not 3)
    BEQ     loop      @branch to loop

    LDR     R0, =off_led_msg    @load off_led_msg string into R2
    BL      printf              @branch link to printf

    @ Turn off green LED
    MOV     R0, R9			@get gpio mapped address
    MOV     R1, #green_led	@get pin number of green LED
    ADD     R4, R0, #GPCLR0	@pointer to GPSET regs.
    MOV     R5, R1			@save pin number

    @ Compute address of GPSET register and pin field        
    MOV     R3, #registerpins	@divisor
    UDIV    R0, R5, R3          @GPSET number
    MUL     R1, R0, R3          @compute remainder
    SUB     R1, R5, R1          @for relative pin position
    LSL     R0, R0, #2          @4 bytes in a register
    ADD     R0, R0, R4          @address of GPSETn
        
    @ Set up the gpio pin funtion register in programming memory
    LDR     R2, [R0]         @get entire register
    MOV     R3, #pinbit      @one pin
    LSL     R3, R3, R1       @shift to pin position
    ORR     R2, R2, R3       @clear bit
    STR     R2, [R0]         @update register


    @ Turn off red LED
    MOV     R0, R9			@Get gpio mapped address
    MOV     R1, #red_led	@get pin number of red LED
    ADD     R4, R0, #GPCLR0	@pointer to GPSET regs.
    MOV     R5, R1			@save pin number

    @ Compute address of GPSET register and pin field        
    MOV     R3, #registerpins   @divisor
    UDIV    R0, R5, R3          @GPSET number
    MUL     R1, R0, R3          @compute remainder
    SUB     R1, R5, R1          @for relative pin position
    LSL     R0, R0, #2          @4 bytes in a register
    ADD     R0, R0, R4          @address of GPSETn
        
    @ Set up the gpio pin funtion register in programming memory
    LDR     R2, [R0]        @get entire register
    MOV     R3, #pinbit     @one pin
    LSL     R3, R3, R1      @shift to pin position
    ORR     R2, R2, R3      @clear bit
    STR     R2, [R0]        @update register
    
    B       loop            @end case_3, branch back and rerun loop

end:
    POP 	{R8}        @restore SP to entry level
    POP 	{R9}
    MOV 	R7, #1
    POP     {ip, pc}            @pop ip reg, return

map_gpio:
@ Map gpio memory to a main memory location so we can access them.
@ Keep a copy of the mapped memory address returned in R0 this will be need later on.

    MOV     R4, R0          @use r4 for file descriptor
    MOV     R8, R0          @save a copy of file descriptor
    
    LDR     R0, gpio        @address of gpio
    MOV     R9, R0          @save a copy of gpio address
    PUSH    {R9}            @gpio address on stack for mmap
    PUSH    {R8}            @file descriptor on stack for mmap
    MOV     R0, #nopref     @let kernel pick memory
    MOV     R1, #pagesize   @get 1 page of memory
    MOV     R2, #readwrite  @read/write this memory
    MOV     R3, #mapshare   @share with other processes
    BL      mmap            @R0-R3+top of stack has info
    MOV     R9, R0          @save mapped address

    CMP     R0, #-1         @check for error
    BNE     setup_pins		@if no error, go to setup_pins
    LDR     R0, memerror    @if there is error, display error message
    BL      printf			@branch link to printf
    B       end 			@branch to end (end program)

setup_pins:  
    @ Select gpio pin number of green LED and function.
    MOV     R0, R9          @programming memory
    MOV     R1, #green_led  @gpio pin for green LED
    MOV     R2, #output     @pin function (output)
    MOV     R4, R0          @save pointer to gpio
    MOV     R5, R1          @save gpio pin number for green LED
    MOV     R6, R2          @save function code

    @ Compute address of GPFSEL register and pin field
    MOV     R3, #10         @divisor
    UDIV    R0, R5, R3      @GPFSEL number
    MUL     R1, R0, R3      @compute remainder
    SUB     R1, R5, R1      @for GPFSEL pin    
        
    @ Set up the gpio pin funtion register in programming memory
    LSL     R0, R0, #2      @4 bytes in a register
    ADD     R0, R4, R0      @GPFSELn address
    LDR     R2, [R0]        @get entire register
    
    MOV     R3, R1                  @need to multiply pin
    ADD     R1, R1, R3, lsl #1      @position by 3
    MOV     R3, #pinfield           @gpio pin field
    LSL     R3, R3, R1              @shift to pin position
    BIC     R2, R2, R3              @clear pin field

    LSL     R6, R6, R1      @shift function code to pin position
    ORR     R2, R2, R6      @enter function code
    STR     R2, [R0]        @update register

 
    @ Select gpio pin number of red LED and function. @* changed R12 back to R2, see if have error
    MOV     R0, R9			@programming memory
    MOV     R1, #red_led    @gpio pin for red LED
    MOV     R2, #output		@pin function
    MOV     R4, R0          @save pointer to gpio
    MOV     R5, R1          @save gpio pin of red LED
    MOV     R6, R2			@save function code

    @ Compute address of GPFSEL register and pin field
    MOV     R3, #10         @divisor
    UDIV    R0, R5, R3      @GPFSEL number
    MUL     R1, R0, R3      @compute remainder
    SUB     R1, R5, R1      @for GPFSEL pin    
        
    @ Set up the gpio pin funtion register in programming memory
    LSL     R0, R0, #2      @4 bytes in a register
    ADD     R0, R4, R0      @GPFSELn address
    LDR     R2, [R0]        @get entire register
    
    MOV     R3, R1                  @need to multiply pin
    ADD     R1, R1, R3, lsl #1      @position by 3
    MOV     R3, #pinfield           @gpio pin field
    LSL     R3, R3, R1              @shift to pin position
    BIC     R2, R2, R3              @clear pin field

    LSL     R6, R6, R1      @shift function code to pin position
    ORR     R2, R2, R6      @enter function code
    STR     R2, [R0]        @update register
    B       loop            @branch to loop after setup complete



@======================================================================================@                                                                                           
@ Citation                                                                                  
                                                                                          
@ Title: Raspberry Pi Operating System Assembly Language                                    
@ Author: Bruce Smith                                                                       
@ Date published: March 18, 2021                                                            
@ Date cited: November 2, 2022                                                              
@ Code version: 4th edition                                                                 
@ Availability: https://www.brucesmith.info/raspberry-pi-assembly-language.html             
                                                                                           
@ Title: ARM assembler in Raspberry Pi – Chapter 9                                          
@ Author: Roger Ferrer Ibáñez                                                               
@ Date published: February 2, 2013                                                          
@ Date cited: November 2, 2022                                                              
@ Availability: https://thinkingeek.com/2013/02/02/arm-assembler-raspberry-pi-chapter-9/    
                                                                                         
@ Title: Introduction To MIPS Assembly Language Programming                                 
@ Author: Charles W. Kann III                                                               
@ Date published: August 2, 2021                                                            
@ Date cited: November 2, 2022                                                              
@ Availability: https://eng.libretexts.org/Bookshelves/Computer_Science/Programming_Languages/Introduction_To_MIPS_Assembly_Language_Programming_(Kann)

@======================================================================================@

@ data section
.data
@ message to prompt for input
.balign 4 @not sure about this
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


@ code starts here
.text
.global main
.extern printf
.extern scanf

main:
    PUSH    {ip, lr}    @store return address
    MOV     R10, #5     @default value R10=5   
    @todo: setup gpio    

loop:
    CMP     R10, #0     @compare R10 to 0
    BEQ     end         @end loop if R10=0
	
    @ print all choices
    LDR     R0, =choice_menu    @load string into R0
	BL      printf              @branch link to printf (print msg)

    @ get user input
	LDR     R0, =choice_pattern @load choice_pattern into R0
	LDR     R1, =choice_input   @load choice_input into R1
	BL      scanf               @read user input
    
    @ save user input
    LDR     R1, =choice_input   @load address of choice_input in R1
    LDR     R10, [R1]           @read value from choice_input into R10

    @ print user input
    LDR     R0, =choice_sel     @load address of choice_sel in R0
	LDR     R1, =choice_input   @load address of choice_input in R1
	LDR     R1, [R1]            @read value from choice_input into R1
	BL      printf              @check input
    
    @ check for valid user input
    CMP     R10, #3     @check choice_input > 3 
    MOVGT   R2, #1      @if choice_input > 3, R2 = 1 (true)
    MOVLE   R2, #0      @if choice_input <= 3, R2 = 0 (false)

    CMP     R10, #0     @check choice_input < 0
    MOVLT   R3, #1      @if choice_input < 0, R3 = 1 (true)
    MOVGE   R3, #0      @if choice_input >= 0, R3 = 0 (false)

    ORR     R2, R2, R3

    CMP     R2, #0      @check if R2 == 0
    BEQ     case_1      @if R2==0, go to case_1

    LDR     R0, =choice_invalid     @load string into R0
    BL      printf                  @branch link to printf
    B       end_if                  @end loop to ask for input again

case_1:
    @ option 1 - turn on red LED
    CMP     R10, #1     @compare R10 to 1
    MOVNE   R2, #0      @if R10!=1, R2=0
    MOVEQ   R2, #1      @if R10==1, R2=1

    CMP     R2, #0      @compare R2 to 0
    BEQ     case_2      @branch to case_2

    LDR     R0, =on_red_msg     @load string into R2
    BL      printf              @branch link to printf
    @todo: turn on red LED
    B       end_if              @end case_1
    
case_2:
    @ option 2 - turn on green LED
    CMP     R10, #2     @compare R10 to 2
    MOVNE   R2, #0      @if R10!=2, R2=0
    MOVEQ   R2, #1      @if R10==2, R2=1

    CMP     R2, #0      @compare R2 to 0
    BEQ     case_3      @branch to case_3

    LDR     R0, =on_green_msg   @load string into R2
    BL      printf              @branch link to printf
    @todo: turn on green LED
    B       end_if              @end case_2

case_3:
    @ option 3 - turn off all LEDs
    CMP     R10, #3     @compare R10 to 3
    MOVNE   R2, #0      @if R10!=3, R2=0
    MOVEQ   R2, #1      @if R10==3, R2=1

    CMP     R2, #0      @compare R2 to 0
    BEQ     end_if      @branch to end_if

    LDR     R0, =off_led_msg    @load string into R2
    BL      printf              @branch link to printf
    @todo: turn off LEDs
    B       end_if              @end case_3

end_if:
    @ end if-else loop 
    B       loop        @branch back and rerun loop

end:
    LDR     R0, =exit_msg       @load string into R0
	BL      printf              @branch link to printf (print msg)
 	POP     {ip, pc}            @pop ip reg, return

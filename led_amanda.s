
@; Data section to store initialized data or constants
.data

@; Menu message
.balign 4 @; Ensure variable is 4-byte aligned 
message1:
    .asciz "\n << LED Display >> \n (1) Turn on the red LED light \n (2) Turn on the green LED light \n (3) Turn off both LED light \n (4) Exit the program \n Enter your option: "

@; User option message
.balign 4 @; Ensure variable is 4-byte aligned 
message2:
    .asciz "\n You have chosen option %d \n"

@; Invalid option message
message3:
    .asciz "\n You have chosen an invalid option \n"

@; Option 1 message
message4:
    .asciz "\n Red LED light is turned on! \n"

@; Option 2 message
message5:
    .asciz "\n Green LED light is turned on! \n"

@; Option 3 message
message6:
    .asciz "\n Both LED lights are turned off! \n"

@; Format pattern for scanf
.balign 4 @; Ensure variable is 4-byte aligned
scan_pattern: 
    .asciz "%d"

@; Where scanf will store the user input
.balign 4
number_read: .word 0

.text @; Text section
.global main @; Start of assembly code
.extern printf @; External function printf
.extern scand @; External function scanf

main:
    PUSH {ip, lr} @; Store return address, link register (lr) and intra procedural call (ip) register in stack
    
LOOP:    
    LDR r0, address_menu_input @; Load address_menu_input into r0
    BL printf @; Branch and link to call the function printf

    LDR r0, address_of_scan_pattern @; Load address_of_user_input into r0
    LDR r1, address_of_user_input @; Load user input into r1
    BL scanf @; Branch and link to call the function scanf

    LDR r0, address_option_message @; Load address_option_message into r0
    LDR r1, address_of_user_input @; Load address_of_user_input into r1
    LDR  r1, [r1] @; Load the integer input into r1
    BL printf  @; @; Branch and link to call the function printf

    LDR r0, address_of_user_input @; Load address_of_user_input into r0
    LDR r0, [r0] @; Load the integer input into r0

    @; If option is neither 1, 2, 3 or 4 it will be an invalid option
    CMP R0, #1 @; Compare user input with 1
    CMPNE R0, #2 @; Compare user input with 2
    CMPNE R0, #3 @; Compare user input with 3
    CMPNE R0, #4 @; Compare user input with 4
    BNE invalid_option

    @; If option is 1, turn on red LED light
    CMP r0, #1 @; Compare user input with 1
    BEQ case_1 @; Proceed to case 1

    @; If option is 2, turn on green LED light
    CMP r0, #2 @; Compare user input with 2
    BEQ case_2 @; Proceed to case 2

    @; If option is 3, turn off both LED lights
    CMP r0, #3 @; Compare user input with 3
    BEQ case_3 @; Proceed to case 3

    @; If option is 4, exit the program
    CMP r0, #4 @; Compare user input with 4
    BEQ END @; Break the loop

    B LOOP @; Branch back and re-run the loop
 
END:
    POP {ip, pc} @; Pop ip and lr reg value to PC, return

invalid_option:
    LDR r0, address_of_invalid_option @; Load address_of_invalid_option into r0
    BL printf @; Branch and link to call the function printf
    B LOOP @; Branch back and re-run the loop

case_1:
    LDR r0, address_option_one @; Load address_option_one into r0
    BL printf @; Branch and link to call the function printf
    B LOOP @; Branch back and re-run the loop

case_2:
    LDR r0, address_option_two @; Load address_option_two into r0
    BL printf @; Branch and link to call the function printf
    B LOOP @; Branch back and re-run the loop

case_3:
    LDR r0, address_option_three @; Load address_option_three into r0
    BL printf @; Branch and link to call the function printf
    B LOOP @; Branch back and re-run the loop


address_menu_input : .word message1
address_option_message : .word message2
address_of_scan_pattern : .word scan_pattern
address_of_user_input : .word number_read
address_of_invalid_option: .word message3
address_option_one : .word message4
address_option_two : .word message5
address_option_three : .word message6



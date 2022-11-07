.section .text

.data
/* Prompt User input */
.balign 4
user_input: .asciz "Please enter an integer: "

/* Print the retrieved memeory address */
.balign 4
display_output: .asciz "The value is stored in the memory address of %p\n"

/* Format input for scanf */
.balign 4
scan_input: .asciz "%d"

/* Print the retrieved memeory address value */
.balign 4
display_value: .asciz "The value retrieved from the stack is %d\n"

/* Formats the output memory address for scanf
.balign 4
scan_output: .asciz "%p" */

/* Store the memory address of integer read */
.balign 4
integer_read: .word 0

.balign 4
return: .word 0


.global main
main:

        LDR R1, address_of_return
        STR LR, [R1] /* store the address whose in R1 into LR */

        /* Prompt user input by printing the user input message */
        LDR R0, address_of_userInput
        BL printf

        LDR R0, address_of_scanInput
        LDR R1, address_of_integer_read
        BL scanf

        LDR R1, address_of_integer_read

        /* PUSH R1 write the address of the integer into the stack */
        PUSH {R1}

        /* Prints out the value from the RAM */
        LDR R0, address_of_displayValue
        POP {R1}
        LDR R1, [R1]
        BL printf

        /* Prints out the memory address */
        LDR R0, address_of_displayOutput
        POP {R1}
        BL printf

        /* LDR R0, address_of_integer_read
        LDR R0, [R0] */

        LDR LR, address_of_return
        LDR LR, [LR]
        BX LR

address_of_userInput: .word user_input
address_of_displayOutput: .word display_output
address_of_displayValue: .word display_value
address_of_scanInput: .word scan_input
/* address_of_scan_Output: .word scan_output */
address_of_integer_read: .word integer_read
address_of_return: .word return

/* External */
.global printf
.global scanf

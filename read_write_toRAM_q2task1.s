section .text                                                              @ text section is used to physically store the actual assembly language code

.data                                                                       @ declare variable that is needed to be initialised before the main program starts 

/* Prompt User input */
.balign 4                                                                  @ specifies the user input expected must be aligned to 4 bytes, an indication to restricted the user input expected to be an integer
user_input: .asciz "Please enter an integer: "                             @ for printf calls, by GNU assembler's ".asciz", to print prompt statement for prompting user input 

/* Print the retrieved memory address */
.balign 4                                                                  @ specifies the memory address expected must be aligned to 4 bytes, an indication to restricted the statement expected to be a memory address
display_output: .asciz "The value is stored in the memory address of %p\n" @ for printf calls, by GNU assembler's ".asciz", statement to prints out the RAM address

/* Format input for scanf */
.balign 4                                                                  @ specifies the value formatted must be aligned to 4 bytes, restrict the value to be formatted to be an integer
scan_input: .asciz "%d"                                                    @ use %d format specifier to take integer input from the user

/* Print the retrieved memory address value */
.balign 4                                                                  @ specifies the value returned is expected to aligned to 4 bytes, an indication to restricted the statement expected to be a memory address
display_value: .asciz "The value retrieved from the stack is %d\n"         @ use %d format specifier to print out the value retrieved from the RAM address

/* Store the memory address of integer read */
.balign 4                                                                 @ specifies the variable to be read must be aligned to 4 bytes, indicates that input read must be 4 bytes
integer_read: .word 0                                                     @ initialised the variable value to 0 for the 32 memory address bit (4 byte)

/*To store the return address at the beginning and end of the program*/ 
.balign 4                                                                 @specifies the varaible to be read must be aligned to 4 bytes
return: .word 0                                                           @ initialised the variable value to 0 for the 32 memory address bit (4 byte)


.global main                                                              @ indicate the starting point of the assembly language code
main:                                                                     @ the beginning and the start of the main function
        
                                                                          @ all instructions starts here

        LDR R1, address_of_return                                         @ load the return address into R1
        STR LR, [R1]                                                      @ store the return address whose in R1 into link registers

        /* Prompt user input by printing the user input message */
        LDR R0, address_of_userInput                                      @ load the statement for prompting user input into R0
        BL printf                                                         @ store the return address of the next instructionn and branch link to printf function to print the user input and display the result to the user

        /*Format user input*/
        LDR R0, address_of_scanInput                                      @ load the format specifier to take in integer input from the user
        LDR R1, address_of_integer_read                                   @ load the variable to store the integer input into R1
        BL scanf                                                          @ store the return address of the next instruction and branch link to scanf function 

        /*Load memory address of formatted input into R1*/
        LDR R1, address_of_integer_read                                   @ load the memory address of the user input into R1 to store into RAM later

        /* PUSH R1 write the address of the integer into the stack */
        PUSH {R1}                                                         @ save R1 to stack memory

        /* Prints out the value from the RAM */
        LDR R0, address_of_displayValue                                   @ load the printing statement to print the value into R0
        POP {R1}                                                          @ restore R1 from stack
        LDR R1, [R1]                                                      @ load value of mem[R1] to register R1
        BL printf                                                         @ store the return address of the next instruction and branch link to printf function to display ("The value retrieved from the stack is %d\n") out the value retrieved from R1 for verification

        /* Prints out the memory address */
        LDR R0, address_of_displayOutput                                  @ load the statement for display memory address to R0
        POP {R1}                                                          @ restore R1 from stack
        BL printf                                                         @ branch link to printf, like 'printf("The value is stored in the memory address of %p\n")' to print out the memory address


        LDR LR, address_of_return                                         @ load the return address into the link register
        LDR LR, [LR]                                                      @ load the value of the return address into the link register
        BX LR                                                             @ finish the program

address_of_userInput: .word user_input                                    @ assign the to the label from user_input to address_of_userInput
address_of_displayOutput: .word display_output                            @ assign the to the label from label display_output to address_of_displayOutput
address_of_displayValue: .word display_value                              @ assign the to the label from label display_value to address_of_displayValue
address_of_scanInput: .word scan_input                                    @ assign the to the label from label scan_input to address_of_scanInput
address_of_integer_read: .word integer_read                               @ assign the to the label from label integer_read to address_of_integer_read
address_of_return: .word return                                           @ assign the to the label from label return to address_of_return

/* External */
.global printf                                                            @ External Function for printf
.global scanf                                                             @ External Function for scanf

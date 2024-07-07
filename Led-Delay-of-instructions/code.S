
.section .data
L1 : .word 10000000
.section .text
.global main

main:
    la x3 ,L1
	lw x4 ,0(x3)    // contains delay count
    // actual code starts here

    lui x5, 0x10012   // x5 contains 10012000
	//sw x0, 0(x5)
    addi x6, x0, 0    // x6 contains off_val
    addi x7, x0, 32   // x7 contains on_val
    sw x6, 4(x5)      // storing off_val in 10012004
    sw x7, 8(x5)      // storing on_val in 10012008




    function_on_off:
        sw x7, 12(x5)  // LED on
        addi x8 ,x0,0  // counter register

        delay_for_on:   // loop for delay LED on
            beq x8 ,x4, LED_off
            addi x8, x8, 1
            beq x0, x0, delay_for_on

        LED_off:   // LED off
            sw x6, 12(x5)
            addi x8, x0, 0

            delay_for_off:  //loop for delay LED off
                beq x8, x4, function_on_off
                addi x8, x8, 1
                beq x0, x0, delay_for_off


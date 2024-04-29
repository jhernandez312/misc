! Written by Jackson McGill
! Storing Test
main:   lea $sp, initsp                 ! initialize stack pointer
        lw $sp, 0($sp)                  ! finish initialization

        lea $a0, TWO                    ! load first number address
        lw $a0, 0($a0)                  ! load first number

        lea $a1, FIVE                   ! load second number address
        lw $a1, 0($a1)                  ! load second number

        lea $a2, addr
        lw $a2, 0($a2)
        sw $a1, 0($a2)                  ! Store second number at address

        halt

TWO:    .fill 2
FIVE:   .fill 5
TEN:    .fill 10

initsp: .fill 0xA000
addr:   .fill 0xB000
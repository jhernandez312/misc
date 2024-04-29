! Spring 2023 Revisions by Bijan Nikain,Samy Amer

! This program executes pow as a test program using the LC 5500 calling convention
! Check your registers ($v0) and memory to see if it is consistent with this program



        ! vector table
vector0:
        .fill 0x00000000                        ! device ID 0
        .fill 0x00000000                        ! device ID 1
        .fill 0x00000000                        ! ...
        .fill 0x00000000
        .fill 0x00000000
        .fill 0x00000000
        .fill 0x00000000
        .fill 0x00000000                        ! device ID 7
        ! end vector table
main:   lea $sp, initsp                         ! initialize the stack pointer
        lw $sp, 0($sp)                          ! finish initialization

                                                ! TODO FIX ME: Install timer interrupt handler into vector table
        add $zero, $zero, $zero
        lea $t0, vector0
        lea $t1, timer_handler
        sw $t1, 0($t0)                          ! DONE

                                                ! TODO FIX ME: Install distance tracker interrupt handler into vector table
        add $zero, $zero, $zero
        lea $t1, distance_tracker_handler
        sw $t1, 1($t0)                          ! DONE

        lea $t0, minval
        lw  $t0, 0($t0)
        addi $t1, $zero, 65535                  ! store 0000ffff into minval (to make comparisons easier)
        sw  $t1, 0($t0)

        ei                                      ! Enable interrupts

        lea $a0, BASE                           ! load base for pow
        lw $a0, 0($a0)
        lea $a1, EXP                            ! load power for pow
        lw $a1, 0($a1)
        lea $at, POW                            ! load address of pow
        jalr $ra, $at                           ! run pow
        lea $a0, ANS                            ! load base for pow
        sw $v0, 0($a0)

        halt                                    ! stop the program here
        addi $v0, $zero, -1                     ! load a bad value on failure to halt

BASE:   .fill 2
EXP:    .fill 8
ANS:	.fill 0                                 ! should come out to 256 (BASE^EXP)

POW:    push $fp                                ! saves the old frame pointer

        addi $fp, $sp, 0                        ! set new frame pointer

        blt $zero, $a1, BASECHK                 ! check if $a1 is zero
        br RET1                                 ! if the exponent is 0, return 1

BASECHK:
        blt $zero, $a0, WORK
        br RET0

WORK:
        addi $a1, $a1, -1                       ! decrement the power

        lea $at, POW                            ! load the address of POW
        push $ra                                ! saves return address onto stack
        push $a0                                ! saves arg 0 onto stack
        jalr $ra, $at                           ! recursively call POW
        add $a1, $v0, $zero                     ! store return value in arg 1
        lw $a0, -2($fp)                         ! load the base into arg 0
        lea $at, MULT                           ! load the address of MULT
        jalr $ra, $at                           ! multiply arg 0 (base) and arg 1 (running product)
        pop $a0
        pop $ra

        br FIN                                  ! unconditional branch to FIN

RET1:   add $v0, $zero, $zero                   ! return a value of 0
	addi $v0, $v0, 1                        ! increment and return 1
        br FIN                                  ! unconditional branch to FIN

RET0:   add $v0, $zero, $zero                   ! return a value of 0

FIN:	pop $fp                                 ! restore old frame pointer
        jalr $zero, $ra

MULT:   add $v0, $zero, $zero                   ! return value = 0
        addi $t0, $zero, 0                      ! sentinel = 0
AGAIN:  add $v0, $v0, $a0                       ! return value += argument0
        addi $t0, $t0, 1                        ! increment sentinel
        blt $t0, $a1, AGAIN                     ! while sentinel < argument, loop again
        jalr $zero, $ra                         ! return from mult


timer_handler:
        add $zero, $zero, $zero                 ! TODO FIX ME
        push $k0                                ! saving address so we can return
        ei

        push $t0                                ! saving registers
        push $t1
	lea $t0, ticks                          ! loading ticks to increase
	lw $t0, 0($t0)
	lw $t1, 0($t0)
	addi $t1, $t1, 1                        ! actually increasing ticks
	sw $t1, 0($t0)
	pop $t1                                 ! restoring registers
        pop $t0

        di                                      ! disable interrupt
        pop $k0
        reti


distance_tracker_handler:
        add $zero, $zero, $zero                 ! TODO FIX ME
        push $k0                                ! saving address
        ei                                      ! now we can enable interrupts

        push $t0                                ! saving temp registers
        push $t1
        push $t2

        in $t0, 0x1
        lea $t1, minval
        lw $t1, 0($t1)
        lw $t2, 0($t1)
        blt $t2, $t0, maxnum                    ! branch if t2 < t0
        sw $t0, 0($t1)

maxnum:
        lea $t1, maxval
        lw $t1, 0($t1)
        lw $t2, 0($t1)
        blt $t0, $t2, end
        sw $t0, 0($t1)

end:
        lea $t0, maxval                         ! maxval
        lw $t0, 0($t0)
        lw $t0, 0($t0)

        lea $t1, minval                         ! minval
        lw $t1, 0($t1)
        lw $t1, 0($t1)

        lea $t2, range                          ! range
        lw $t2, 0($t2)
        nand $t1, $t1, $t1
        addi $t1, $t1, 1
        add $t0, $t0, $t1
        sw $t0, 0($t2)

        pop $t2                                 ! restoring registers
        pop $t1
        pop $t0

        di
        pop $k0
        reti





initsp: .fill 0xA000
ticks:  .fill 0xFFFF
range:  .fill 0xFFFE
maxval: .fill 0xFFFD
minval: .fill 0xFFFC

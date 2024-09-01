SETI r3, 9
BL Fibonacci
SYSCALL HALT

Fibonacci:
    STW r0, r0, -24
    SUBI r0, r0, 24
    STW r31, r0, 12
    XOR r31, r31, r31
    OR r31, r3, r3
    CMPI r3, 1
    BLE Exit
    MFLR r1
    STW r1, r0, 20
    STW r30, r0, 8
    SUBI r3, r3, 1
    BL Fibonacci
    XOR r30, r30, r30
    OR r30, r3, r3
    SUBI r3, r31, 2
    BL Fibonacci
    ADD r3, r30, r3
    LDW r30, r0, 8
    LDW r1, r0, 20
    MTLR r1
Exit:
    LDW r31, r0, 12
    ADDI r0, r0, 24
    BLR

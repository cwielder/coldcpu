SETI r0, 'h'
SYSCALL PRINT, r0
SETI r0, 'e'
SYSCALL PRINT, r0
SETI r0, 'l'
SYSCALL PRINT, r0
SETI r0, 'l'
SYSCALL PRINT, r0
SETI r0, 'o'
SYSCALL PRINT, r0
SETI r0, ' '
SYSCALL PRINT, r0
SETI r0, 'w'
SYSCALL PRINT, r0
SETI r0, 'o'
SYSCALL PRINT, r0
SETI r0, 'r'
SYSCALL PRINT, r0
SETI r0, 'l'
SYSCALL PRINT, r0
SETI r0, 'd'
SYSCALL PRINT, r0
SETI r0, 10
SYSCALL PRINT, r0
SETI r0, '`'
SHIFTL r0, r0, 16
SETI r1, '4'
SHIFTL r1, r1, 16
SETI r2, 'f'
OR r1, r1, r2
FMUL f2, f0, f1
SYSCALL HALT

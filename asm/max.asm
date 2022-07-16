.data
first:  .word   5
second: .word   8   
.text
.global _start
_start:
   ldr   r0, =first    
   ldr   r0, [r0]      
   ldr   r1, =second   
   ldr   r1, [r1]            
   cmp   r1, r0        
   bgt   _greater      
   b     _exit         
_greater:
   mov    r0, r1        
_exit:
   mov    r7, #1
   swi    0

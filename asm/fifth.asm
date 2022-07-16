.data
msg:	.ascii "Hello World!\n"
end_msg:
.align 4


.equ STDOUT, 1
.equ SVC_WRITE, 64
.equ SVC_EXIT, 93
.equ LENGTH, end_msg - msg
 
.text
.global main
 
main:
	stp x29, x30, [sp, -16]!
	mov x0, #STDOUT
	ldr x1, =msg
	mov x2, LENGTH
	mov x8, #SVC_WRITE
	mov x29, sp
	svc #0 // write(stdout, msg, 13);
	ldp x29, x30, [sp], 16
	mov x0, #3
	mov x8, #SVC_EXIT
	svc #0 // exit(0);


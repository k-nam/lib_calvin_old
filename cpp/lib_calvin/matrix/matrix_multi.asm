; AddTwoSum_64.asm - Chapter 3 example.

ExitProcess proto
.data
sum qword 0


.code
doAssemblyGigaOp PROC
	; params:
	; rcx = src
	; rdx = target
	; r8 = size
	; r9 = # iter

	; locals:
	; r10 = index1 ( r10 < size, innerloop)
	; r11 = index2 ( r11 < #iter, outerloop)

	; vars:
	; ymm1: multiplier
	; 

	sub rsp, 40h

	vzeroall
	mov qword ptr[rsp + 8h], r12 ; push
	
	;mov qword ptr[rsp], rcx
	;mov rax, rsp
	vbroadcastsd ymm1, qword ptr[rdx]	

	mov	r11, 0
outerloop:	
	cmp r11, r9
	jge outerloopend
	mov	r10, 0
innerloop:
	cmp r10, r8
	jge innerloopend
	imul r12, r10, 8
	vmovapd ymm2, ymmword ptr[rcx + r12]
	vmovapd ymm3, ymmword ptr[rcx + r12 + 32]
	vmulpd ymm2, ymm2, ymm1
	vmulpd ymm3, ymm3, ymm1
	vmovapd ymmword ptr[rdx + r12], ymm2
	vmovapd ymmword ptr[rdx + r12 + 32], ymm3
	add r10, 8 ; ymm register contain 4 double values, using two ymm registers
	jmp innerloop
innerloopend:
	add r11, 1
	jmp outerloop
outerloopend:	

	mov rax, qword ptr[rsp]
	mov r12, qword ptr[rsp + 8h] ; pop
	add rsp, 40h		
	ret
doAssemblyGigaOp ENDP
End
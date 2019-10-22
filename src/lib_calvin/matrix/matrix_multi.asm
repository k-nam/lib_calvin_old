; AddTwoSum_64.asm - Chapter 3 example.

ExitProcess proto
.data
sum qword 0


.code
doAssembly PROC
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

	mov rax, qword ptr[rcx]
	vzeroall
	mov qword ptr[rsp + 8h], r12 ; push
	
	;mov qword ptr[rsp], rcx

	vbroadcastsd ymm1, qword ptr[rcx]	

	mov	r11, 0
outerloop:	
	cmp r11, r9
	jge outerloopend
	mov	r10, 0
innerloop:
	cmp r10, r8
	jge innerloopend
	imul r12, r10, 8

	;vmulpd ymm2, ymm2, ymm1

	vmovapd ymm2, ymmword ptr[rdx + r12] 
	vmovapd ymm3, ymmword ptr[rdx + r12 + 32]
	vmovapd ymm4, ymmword ptr[rdx + r12 + 64]
	vmovapd ymm5, ymmword ptr[rdx + r12 + 96]


	vfmadd231pd ymm2, ymm1, ymmword ptr[rcx + r12] 
	vfmadd231pd ymm3, ymm1, ymmword ptr[rcx + r12 + 32]  
	vfmadd231pd ymm4, ymm1, ymmword ptr[rcx + r12 + 64] 
	vfmadd231pd ymm5, ymm1, ymmword ptr[rcx + r12 + 96]  

	vmovapd ymmword ptr[rdx + r12], ymm2
	vmovapd ymmword ptr[rdx + r12 + 32], ymm3
	vmovapd ymmword ptr[rdx + r12 + 64], ymm4
	vmovapd ymmword ptr[rdx + r12 + 96], ymm5

	vmovapd ymm6, ymmword ptr[rdx + r12 + 128] 
	vmovapd ymm7, ymmword ptr[rdx + r12 + 160]
	vmovapd ymm8, ymmword ptr[rdx + r12 + 192]
	vmovapd ymm9, ymmword ptr[rdx + r12 + 224]

	vfmadd231pd ymm6, ymm1, ymmword ptr[rcx + r12 + 128]  
	vfmadd231pd ymm7, ymm1, ymmword ptr[rcx + r12 + 160]  
	vfmadd231pd ymm8, ymm1, ymmword ptr[rcx + r12 + 192]  
	vfmadd231pd ymm9, ymm1, ymmword ptr[rcx + r12 + 224]  

	vmovapd ymmword ptr[rdx + r12 + 128], ymm6
	vmovapd ymmword ptr[rdx + r12 + 160], ymm7
	vmovapd ymmword ptr[rdx + r12 + 192], ymm8
	vmovapd ymmword ptr[rdx + r12 + 224], ymm9

	add r10, 32 ; ymm register contain 4 double values, using two ymm registers
	jmp innerloop
innerloopend:
	add r11, 1
	jmp outerloop
outerloopend:	

	
	mov r12, qword ptr[rsp + 8h] ; pop
	add rsp, 40h		
	ret
doAssembly ENDP
End
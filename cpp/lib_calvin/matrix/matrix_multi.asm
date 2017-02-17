; AddTwoSum_64.asm - Chapter 3 example.

ExitProcess proto
.data
sum qword 0
.code
doAssemblyGigaOp proc
   mov  rax,5
   add  rax,6
   mov  sum,rax

   mov  ecx,0
   ret
doAssemblyGigaOp endp
end
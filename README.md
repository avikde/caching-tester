
10 "runs" per stride, stride increments by 4 bytes.

output:
https://docs.google.com/spreadsheets/d/1Hr4u52INS3Dqp-nlQ81L-MxZcmFWv9_naqdooUigKpM/edit?usp=sharing

release ASM with mingw:

```
    for (size_t i = 0; i < ARRAY_SIZE / MAX_STRIDE; i++)
    {
        sink += data[readIndex];
        readIndex += stride;
    }
```

```
movq	$0, -16(%rbp)
	jmp	.L14
.L15:
	movl	-8(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	leaq	data(%rip), %rax
	movss	(%rdx,%rax), %xmm0
	movss	-4(%rbp), %xmm1
	addss	%xmm1, %xmm0
	movss	%xmm0, -4(%rbp)
	movq	16(%rbp), %rax
	movl	%eax, %edx
	movl	-8(%rbp), %eax
	addl	%edx, %eax
	movl	%eax, -8(%rbp)
	addq	$1, -16(%rbp)
.L14:
	cmpq	$999999, -16(%rbp)
	jbe	.L15
```
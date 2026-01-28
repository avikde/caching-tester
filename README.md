
10 "runs" per stride, stride increments by 4 bytes.

output:
https://docs.google.com/spreadsheets/d/1Hr4u52INS3Dqp-nlQ81L-MxZcmFWv9_naqdooUigKpM/edit?usp=sharing

release ASM for the entire file with mingw:


```
	.linkonce discard

std::ctype<char>::do_widen(char) const:
	movl	%edx, %eax
	ret

warmup():
	ret
.LC2:
	.ascii "\0"

testStride(unsigned long long):
	pushq	%rsi
	pushq	%rbx
	subq	$56, %rsp
	movaps	%xmm6, 32(%rsp)
	pxor	%xmm6, %xmm6
	movq	%rcx, %rsi
	call	std::chrono::_V2::steady_clock::now()
	movl	$1000000, %edx
	leaq	data(%rip), %r8
	movq	%rax, %rbx
	xorl	%eax, %eax
	cmpq	$1, %rsi
	jne	.L6
.L5:
	movslq	%eax, %rcx
	addl	$1, %eax
	addss	(%r8,%rcx,4), %xmm6
	subq	$1, %rdx
	jne	.L5
.L7:
	call	std::chrono::_V2::steady_clock::now()
	ucomiss	.LC1(%rip), %xmm6
	movq	%rax, %rsi
	jp	.L8
	je	.L14
.L8:
	subq	%rbx, %rsi
	movaps	32(%rsp), %xmm6
	movabsq	$2361183241434822607, %rax
	imulq	%rsi
	movq	%rsi, %rcx
	sarq	$63, %rcx
	sarq	$7, %rdx
	movq	%rdx, %rax
	subq	%rcx, %rax
	addq	$56, %rsp
	popq	%rbx
	popq	%rsi
	ret
.L6:
	movslq	%eax, %r9
	addl	%esi, %eax
	addss	(%r8,%r9,4), %xmm6
	subq	$1, %rdx
	jne	.L6
	jmp	.L7
.L14:
	movq	.refptr.std::cout(%rip), %rcx
	xorl	%r8d, %r8d
	leaq	.LC2(%rip), %rdx
	call	std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long long)
	jmp	.L8
.LC6:
	.ascii "B stride\11time: \0"
.LC7:
	.ascii " us\11ratio: \0"

main:
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbp
	pushq	%rdi
	pushq	%rsi
	pushq	%rbx
	subq	$104, %rsp
	movaps	%xmm6, 32(%rsp)
	movaps	%xmm7, 48(%rsp)
	movaps	%xmm8, 64(%rsp)
	movaps	%xmm9, 80(%rsp)
	pxor	%xmm9, %xmm9
	movl	$128, %ebp
	leaq	data(%rip), %r14
	movabsq	$2361183241434822607, %rdi
	leaq	std::ctype<char>::do_widen(char) const(%rip), %r13
	call	__main
	movl	$1, %ecx
	call	testStride(unsigned long long)
	movss	.LC1(%rip), %xmm8
	cvtsi2sdq	%rax, %xmm9
.L16:
	movl	%ebp, %r15d
	movl	$10, %ebx
	pxor	%xmm7, %xmm7
.L26:
	call	std::chrono::_V2::steady_clock::now()
	pxor	%xmm6, %xmm6
	movl	$1000000, %edx
	movq	%rax, %rsi
	xorl	%eax, %eax
.L17:
	movslq	%eax, %rcx
	addl	%r15d, %eax
	addss	(%r14,%rcx,4), %xmm6
	subq	$1, %rdx
	jne	.L17
	call	std::chrono::_V2::steady_clock::now()
	ucomiss	%xmm8, %xmm6
	movq	%rax, %r12
	jp	.L18
	je	.L31
.L18:
	movq	%r12, %r9
	pxor	%xmm0, %xmm0
	subq	%rsi, %r9
	movq	%r9, %rax
	sarq	$63, %r9
	imulq	%rdi
	sarq	$7, %rdx
	subq	%r9, %rdx
	cvtsi2sdq	%rdx, %xmm0
	addsd	%xmm0, %xmm7
	subq	$1, %rbx
	jne	.L26
	movq	.refptr.std::cout(%rip), %rcx
	leaq	0(,%rbp,4), %rdx
	divsd	.LC5(%rip), %xmm7
	call	std::basic_ostream<char, std::char_traits<char> >& std::basic_ostream<char, std::char_traits<char> >::_M_insert<unsigned long long>(unsigned long long)
	movl	$15, %r8d
	leaq	.LC6(%rip), %rdx
	movq	%rax, %rbx
	movq	%rax, %rcx
	call	std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long long)
	movapd	%xmm7, %xmm1
	movq	%rbx, %rcx
	call	std::basic_ostream<char, std::char_traits<char> >& std::basic_ostream<char, std::char_traits<char> >::_M_insert<double>(double)
	movl	$11, %r8d
	leaq	.LC7(%rip), %rdx
	movq	%rax, %rcx
	movq	%rax, %rbx
	call	std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long long)
	movapd	%xmm7, %xmm1
	movq	%rbx, %rcx
	divsd	%xmm9, %xmm1
	call	std::basic_ostream<char, std::char_traits<char> >& std::basic_ostream<char, std::char_traits<char> >::_M_insert<double>(double)
	movq	%rax, %rsi
	movq	(%rax), %rax
	movq	-24(%rax), %rax
	movq	240(%rsi,%rax), %rbx
	testq	%rbx, %rbx
	je	.L30
	cmpb	$0, 56(%rbx)
	je	.L23
	movsbl	67(%rbx), %edx
.L24:
	movq	%rsi, %rcx
	call	std::basic_ostream<char, std::char_traits<char> >::put(char)
	movq	%rax, %rcx
	call	std::basic_ostream<char, std::char_traits<char> >::flush()
	subq	$1, %rbp
	jne	.L16
	movaps	32(%rsp), %xmm6
	movaps	48(%rsp), %xmm7
	xorl	%eax, %eax
	movaps	64(%rsp), %xmm8
	movaps	80(%rsp), %xmm9
	addq	$104, %rsp
	popq	%rbx
	popq	%rsi
	popq	%rdi
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
.L31:
	movq	.refptr.std::cout(%rip), %rcx
	xorl	%r8d, %r8d
	leaq	.LC2(%rip), %rdx
	call	std::basic_ostream<char, std::char_traits<char> >& std::__ostream_insert<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*, long long)
	jmp	.L18
.L23:
	movq	%rbx, %rcx
	call	std::ctype<char>::_M_widen_init() const
	movq	(%rbx), %rax
	movl	$10, %edx
	movq	48(%rax), %rax
	cmpq	%r13, %rax
	je	.L24
	movq	%rbx, %rcx
	call	*%rax
	movsbl	%al, %edx
	jmp	.L24

main.cold:
.L30:
	call	std::__throw_bad_cast()
	nop
	.bss

data:
	.space 512000000
.LC1:
	.long	-1082130432
.LC5:
	.long	0
	.long	1076101120
	.linkonce	discard

.refptr.std::cout:
	.quad	std::cout
```
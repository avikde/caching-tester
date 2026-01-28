
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
.L5:
	movslq	%eax, %rcx
	addl	$1, %eax
	addss	(%r8,%rcx,4), %xmm6
	subq	$1, %rdx
	jne	.L5
.L7:
```
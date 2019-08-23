; SPDX-License-Identifier: GPL-2.0
	segment	.bss
	struc	node
.next	resq	1
.data	resq	1
	endstruc
list	resq	1
	segment	.text
	extern	malloc
insert	push	rbp
	mov	rbp, rsp
	push	rbx
	push	r12
	mov	rbx, rdi
	mov	r12, rsi
	mov	rdi, node_size
	call	malloc
	cmp	rax, 0
	jz	.err
	mov	[rax+node.data], r12
	mov	[rax+node.next], rbx
.err	pop	r12
	pop	rbx
	leave
	ret
	segment	.data
fmt	db	"%s", 0xa, 0
	segment	.test
	extern	printf
dump	push	rbp
	mov	rbp, rsp
	push	rbx
	push	r12
	mov	rbx, rdi
.more	cmp	rbx, 0
	jz	.done
	lea	rdi, [fmt]
	mov	rsi, [rbx+node.data]
	xor	eax, eax
	call	printf
	mov	rbx, [rbx+node.next]
	jmp	.more
.done	pop	r12
	pop	rbx
	leave
	ret
	segment	.text
	global	main
main	push	rbp
	mov	rbp, rsp
	push	r12
	push	rbx
	mov	rbx, rsi
	mov	rdi, 0
	mov	rsi, [rbx]
.more	call	insert
	cmp	rax, 0
	jz	.out
	mov	[list], rax
	add	rbx, 8
	mov	rsi, [rbx]
	cmp	rsi, 0
	jz	.out
	mov	rdi, rax
	jmp	.more
.out	mov	rdi, [list]
	call	dump
	xor	eax, eax
	pop	rbx
	pop	r12
	leave
	ret

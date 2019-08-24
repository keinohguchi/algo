; SPDX-License-Identifier: GPL-2.0

; struct node {
;	const char	*data;
;	struct node	*next;
;	struct node	*prev;
; }
	struc	node
.data	resq	1
.next	resq	1
.prev	resq	1
	align	16
	endstruc

; struct node *init(void);
	segment	.bss
head	resq	1
	segment	.text
	extern	malloc
init	push	rbp
	mov	rbp, rsp
	mov	rdi, node_size
	call	malloc
	cmp	rax, 0
	jz	.done
	mov	dword [rax+node.data], 0
	mov	[rax+node.next], rax
	mov	[rax+node.prev], rax
	mov	[head], rax
.done	leave
	ret

; void term(struct node *n);
	extern	free
term	push	rbp
	mov	rbp, rsp
	push	rbx
	push	r12
	mov	r12, [head]
	mov	rdi, [r12+node.next]
.more	cmp	rdi, r12
	jz	.done
	mov	rbx, [rdi+node.next]
	call	free
	mov	rdi, rbx
	jmp	.more
.done	mov	rdi, r12
	call	free
	pop	r12
	pop	rbx
	leave
	ret

; struct node *insert(struct node *prev, const char *data);
insert	push	rbp
	mov	rbp, rsp
	leave
	ret

; void dump(struct node *head);
	segment	.data
fmt	db	"%s ", 0
newline	db	0xa, 0
	extern	printf
	segment	.text
dump	push	rbp
	mov	rbp, rsp
	push	rbx
	push	r12
	mov	r12, rdi
	mov	rbx, [rdi+node.next]
	cmp	rbx, r12
	jz	.done
.more	lea	rdi, [fmt]
	mov	rsi, [rbx+node.data]
	xor	eax, eax
	call	printf
	mov	rbx, [rbx+node.next]
	cmp	rbx, r12
	jnz	.more
.done	lea	rdi, [newline]
	xor	eax, eax
	call	printf
	pop	r12
	pop	rbx
	leave
	ret

; int main(int argc, char *const argv[]);
	global	main
main	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	[rsp], rbx
	mov	rbx, rsi
	call	init
	cmp	qword [head], 0
	jz	.err
.more	mov	rsi, [rbx]
	cmp	rsi, 0
	jz	.done
	add	rbx, 8
	jmp	.more
.done	mov	rdi, [head]
	call	dump
	call	term
.err	mov	rbx, [rsp]
	leave
	ret

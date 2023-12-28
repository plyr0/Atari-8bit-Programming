;
; File generated by cc65 v 2.19
;
	.fopt		compiler,"cc65 v 2.19"
	.setcpu		"6502"
	.smart		on
	.autoimport	on
	.case		on
	.debuginfo	off
	.importzp	sp, sreg, regsave, regbank
	.importzp	tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
	.macpack	longbranch
	.forceimport	__STARTUP__
	.import		_printf
	.import		_scanf
	.import		_rand
	.import		_srand
	.export		_number
	.export		_i
	.export		_guess
	.export		_atariClock
	.export		_main

.segment	"RODATA"

S0006:
	.byte	$4C,$65,$74,$73,$20,$70,$6C,$61,$79,$20,$67,$75,$65,$73,$73,$69
	.byte	$6E,$67,$20,$6E,$75,$6D,$62,$65,$72,$20,$67,$61,$6D,$65,$20,$30
	.byte	$2D,$39,$39,$9B,$00
S0001	:=	S0006+0
S0004:
	.byte	$49,$6E,$63,$6F,$72,$72,$65,$63,$74,$3A,$20,$25,$69,$20,$74,$72
	.byte	$79,$20,$73,$6D,$61,$6C,$6C,$65,$72,$9B,$00
S0008:
	.byte	$59,$6F,$75,$20,$6C,$6F,$73,$74,$2C,$20,$6E,$75,$6D,$62,$65,$72
	.byte	$20,$77,$61,$73,$3A,$20,$25,$69,$9B,$00
S0003:
	.byte	$49,$6E,$63,$6F,$72,$72,$65,$63,$74,$3A,$20,$25,$69,$20,$74,$72
	.byte	$79,$20,$62,$69,$67,$67,$65,$72,$9B,$00
S0005:
	.byte	$43,$6F,$72,$72,$65,$63,$74,$3A,$20,$25,$69,$20,$59,$6F,$75,$20
	.byte	$77,$6F,$6E,$21,$9B,$00
S0007:
	.byte	$25,$69,$00
S0002	:=	S0007+0

.segment	"BSS"

_number:
	.res	1,$00
_i:
	.res	1,$00
_guess:
	.res	1,$00
_atariClock:
	.res	1,$00

; ---------------------------------------------------------------
; void __near__ main (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_main: near

.segment	"CODE"

L0005:	lda     #<(S0001)
	ldx     #>(S0001)
	jsr     pushax
	ldy     #$02
	jsr     _printf
	lda     #<(S0002)
	ldx     #>(S0002)
	jsr     pushax
	lda     #<(_guess)
	ldx     #>(_guess)
	jsr     pushax
	ldy     #$04
	jsr     _scanf
	lda     $0014
	sta     _atariClock
	ldx     #$00
	lda     _atariClock
	jsr     _srand
	jsr     _rand
	jsr     pushax
	ldx     #$00
	lda     #$64
	jsr     tosmoda0
	sta     _number
	lda     #$00
	sta     _i
L000F:	lda     _i
	cmp     #$02
	bcs     L0007
	lda     _guess
	cmp     _number
	bcs     L0010
	lda     #<(S0003)
	ldx     #>(S0003)
	jsr     pushax
	lda     _guess
	jsr     pusha0
	ldy     #$04
	jmp     L000E
L0010:	lda     _guess
	cmp     _number
	bcc     L000C
	beq     L000C
	lda     #<(S0004)
	ldx     #>(S0004)
	jsr     pushax
	lda     _guess
	jsr     pusha0
	ldy     #$04
	jmp     L000E
L000C:	lda     #<(S0005)
	ldx     #>(S0005)
	jsr     pushax
	lda     _number
	jsr     pusha0
	ldy     #$04
	jsr     _printf
	lda     #<(S0006)
	ldx     #>(S0006)
	jsr     pushax
	ldy     #$02
L000E:	jsr     _printf
	lda     #<(S0007)
	ldx     #>(S0007)
	jsr     pushax
	lda     #<(_guess)
	ldx     #>(_guess)
	jsr     pushax
	ldy     #$04
	jsr     _scanf
	inc     _i
	jmp     L000F
L0007:	lda     #<(S0008)
	ldx     #>(S0008)
	jsr     pushax
	lda     _number
	jsr     pusha0
	ldy     #$04
	jsr     _printf
	jmp     L0005

.endproc

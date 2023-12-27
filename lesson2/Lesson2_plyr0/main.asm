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
	.import		_memcpy
	.export		_xpos
	.export		_missile_xpos
	.export		_ufo_xpos
	.export		_stick
	.export		_move_type
	.export		_ch
	.export		_i
	.export		_counter
	.export		_internal_string
	.export		_flags
	.export		_antic4_display_list
	.export		_sprite_data
	.export		_sprite_ufo
	.export		_sprite_missile
	.export		_wait_for_vblank
	.export		_erase_sprite
	.export		_draw_sprite
	.export		_printinternal
	.export		_update_sprite
	.export		_handle_input
	.export		_init_antic
	.export		_init_vars
	.export		_main

.segment	"DATA"

_antic4_display_list:
	.byte	$70
	.byte	$70
	.byte	$70
	.byte	$42
	.byte	$00
	.byte	$50
	.byte	$04
	.byte	$04
	.byte	$04
	.byte	$04
	.byte	$04
	.byte	$04
	.byte	$04
	.byte	$04
	.byte	$04
	.byte	$04
	.byte	$04
	.byte	$04
	.byte	$04
	.byte	$04
	.byte	$04
	.byte	$04
	.byte	$04
	.byte	$04
	.byte	$04
	.byte	$04
	.byte	$04
	.byte	$04
	.byte	$04
	.byte	$41
	.byte	$00
	.byte	$40
_sprite_data:
	.byte	$3C
	.byte	$3C
	.byte	$14
	.byte	$14
	.byte	$14
	.byte	$96
	.byte	$AA
	.byte	$AA
_sprite_ufo:
	.byte	$3C
	.byte	$D7
	.byte	$D7
	.byte	$EB
	.byte	$EB
	.byte	$D7
	.byte	$EB
	.byte	$3C
_sprite_missile:
	.byte	$3C
	.byte	$14
	.byte	$3C
	.byte	$14
	.byte	$3C
	.byte	$14
	.byte	$28
	.byte	$3C

.segment	"RODATA"

S0005:
	.byte	$5A,$6E,$69,$73,$63,$7A,$20,$55,$46,$4F,$20,$20,$20,$20,$20,$20
	.byte	$20,$20,$20,$20,$20,$20,$20,$20,$20,$00
S0004:
	.byte	$47,$72,$61,$74,$75,$6C,$61,$63,$6A,$65,$20,$55,$46,$4F,$20,$7A
	.byte	$6E,$69,$73,$7A,$63,$7A,$6F,$6E,$65,$00

.segment	"BSS"

_xpos:
	.res	2,$00
_missile_xpos:
	.res	2,$00
_ufo_xpos:
	.res	2,$00
_stick:
	.res	1,$00
_move_type:
	.res	1,$00
_ch:
	.res	1,$00
_i:
	.res	1,$00
_counter:
	.res	1,$00
_internal_string:
	.res	26,$00
_flags:
	.res	1,$00

; ---------------------------------------------------------------
; void __near__ wait_for_vblank (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_wait_for_vblank: near

.segment	"CODE"

	lda     $14
L0002:	cmp     $14
	beq     L0002
	rts

.endproc

; ---------------------------------------------------------------
; void __near__ erase_sprite (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_erase_sprite: near

.segment	"CODE"

	clc
	lda     _xpos
	sta     ptr1
	lda     #$50
	adc     _xpos+1
	sta     ptr1+1
	lda     #$00
	tay
	sta     (ptr1),y
	lda     _flags
	and     #$01
	beq     L0002
	clc
	lda     _missile_xpos
	sta     ptr1
	lda     #$50
	adc     _missile_xpos+1
	sta     ptr1+1
	tya
	sta     (ptr1),y
L0002:	lda     _flags
	and     #$02
	beq     L0003
	clc
	lda     _ufo_xpos
	sta     ptr1
	lda     #$50
	adc     _ufo_xpos+1
	sta     ptr1+1
	tya
	sta     (ptr1),y
L0003:	rts

.endproc

; ---------------------------------------------------------------
; void __near__ draw_sprite (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_draw_sprite: near

.segment	"CODE"

	clc
	lda     _xpos
	sta     ptr1
	lda     #$50
	adc     _xpos+1
	sta     ptr1+1
	lda     #$21
	ldy     #$00
	sta     (ptr1),y
	lda     _flags
	and     #$01
	beq     L0002
	clc
	lda     _missile_xpos
	sta     ptr1
	lda     #$50
	adc     _missile_xpos+1
	sta     ptr1+1
	lda     #$7C
	sta     (ptr1),y
L0002:	lda     _flags
	and     #$02
	beq     L0003
	clc
	lda     _ufo_xpos
	sta     ptr1
	lda     #$50
	adc     _ufo_xpos+1
	sta     ptr1+1
	lda     #$54
	sta     (ptr1),y
L0003:	rts

.endproc

; ---------------------------------------------------------------
; void __near__ printinternal (unsigned char *mem, char *str)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_printinternal: near

.segment	"CODE"

	jsr     pushax
	jsr     ldax0sp
	sta     ptr1
	stx     ptr1+1
	ldy     #$00
	lda     (ptr1),y
	sta     _ch
	sty     _i
	jmp     L0004
L0012:	lda     _ch
	cmp     #$20
	bcc     L0015
	cmp     #$80
	bcc     L0016
	cmp     #$A0
	bcs     L0016
L0015:	lda     #$40
	clc
	adc     _ch
	jmp     L0011
L0016:	lda     _ch
	cmp     #$20
	bcc     L0017
	cmp     #$60
	bcc     L001A
L0017:	lda     _ch
	cmp     #$A0
	bcc     L000A
	cmp     #$E0
	bcs     L000A
L001A:	lda     _ch
	sec
	sbc     #$20
L0011:	sta     _ch
L000A:	ldy     #$03
	jsr     ldaxysp
	sta     ptr1
	stx     ptr1+1
	ldy     _i
	lda     _ch
	sta     (ptr1),y
	inc     _i
	jsr     ldax0sp
	ldy     _i
	sta     ptr1
	stx     ptr1+1
	lda     (ptr1),y
	sta     _ch
L0004:	lda     _ch
	bne     L0012
	jmp     incsp4

.endproc

; ---------------------------------------------------------------
; void __near__ update_sprite (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_update_sprite: near

.segment	"CODE"

	lda     #$02
	cmp     _move_type
	bne     L0020
	lda     _xpos+1
	cmp     #$03
	bne     L0004
	lda     _xpos
	cmp     #$BF
L0004:	bcs     L0020
	inc     _xpos
	bne     L0009
	inc     _xpos+1
	jmp     L0009
L0020:	lda     #$01
	cmp     _move_type
	bne     L0009
	lda     _xpos
	cmp     #$99
	lda     _xpos+1
	sbc     #$03
	bcc     L0009
	ldx     _xpos
	bne     L000D
	dec     _xpos+1
L000D:	dex
	stx     _xpos
L0009:	lda     _flags
	and     #$01
	beq     L0011
	lda     _missile_xpos+1
	cmp     #$00
	bne     L0010
	lda     _missile_xpos
	cmp     #$50
L0010:	bcs     L000F
	lda     _flags
	and     #$FE
	sta     _flags
	jmp     L0011
L000F:	lda     _missile_xpos
	sec
	sbc     #$28
	sta     _missile_xpos
	bcs     L0011
	dec     _missile_xpos+1
L0011:	lda     _ufo_xpos+1
	bne     L0013
	lda     _ufo_xpos
	cmp     #$4F
	bne     L0013
	ldx     #$00
	lda     #$28
	sta     _ufo_xpos
	stx     _ufo_xpos+1
	jmp     L0016
L0013:	lda     _counter
	jsr     pusha0
	lda     #$03
	jsr     tosumoda0
	stx     tmp1
	ora     tmp1
	bne     L0016
	inc     _ufo_xpos
	bne     L0016
	inc     _ufo_xpos+1
L0016:	lda     _flags
	and     #$01
	beq     L0019
	lda     _ufo_xpos
	ldx     _ufo_xpos+1
	cpx     _missile_xpos+1
	bne     L0019
	cmp     _missile_xpos
	beq     L001A
L0019:	rts
L001A:	lda     _flags
	and     #$FD
	sta     _flags
	and     #$FE
	sta     _flags
	ldx     #$50
	lda     #$00
	jsr     pushax
	lda     #<(S0004)
	ldx     #>(S0004)
	jmp     _printinternal

.endproc

; ---------------------------------------------------------------
; void __near__ handle_input (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_handle_input: near

.segment	"CODE"

	lda     $0278
	sta     _stick
	lda     #$07
	cmp     _stick
	bne     L000C
	lda     #$02
	jmp     L000B
L000C:	lda     #$0F
	cmp     _stick
	bne     L000D
	lda     #$00
	jmp     L000B
L000D:	lda     #$0B
	cmp     _stick
	bne     L000E
	lda     #$01
L000B:	sta     _move_type
L000E:	lda     #$00
	cmp     $0284
	bne     L0010
	lda     _flags
	and     #$01
	beq     L0011
L0010:	rts
L0011:	lda     _flags
	ora     #$01
	sta     _flags
	lda     _xpos
	ldx     _xpos+1
	ldy     #$28
	jsr     decaxy
	sta     _missile_xpos
	stx     _missile_xpos+1
	rts

.endproc

; ---------------------------------------------------------------
; void __near__ init_antic (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_init_antic: near

.segment	"CODE"

	ldx     #$40
	lda     #$00
	jsr     pushax
	lda     #<(_antic4_display_list)
	ldx     #>(_antic4_display_list)
	jsr     pushax
	ldx     #$00
	lda     #$20
	jsr     _memcpy
	ldx     #$40
	lda     #$00
	sta     $0230
	stx     $0230+1
	lda     #$A3
	sta     $02C5
	lda     #$26
	sta     $02C6
	lda     #$E1
	sta     $02C7
	lda     #$00
	sta     $02C8
	ldx     #$30
	jsr     pushax
	ldx     #$E0
	jsr     pushax
	ldx     #$04
	jsr     _memcpy
	ldx     #$31
	lda     #$08
	jsr     pushax
	lda     #<(_sprite_data)
	ldx     #>(_sprite_data)
	jsr     pushax
	ldx     #$00
	lda     #$08
	jsr     _memcpy
	ldx     #$32
	lda     #$A0
	jsr     pushax
	lda     #<(_sprite_ufo)
	ldx     #>(_sprite_ufo)
	jsr     pushax
	ldx     #$00
	lda     #$08
	jsr     _memcpy
	ldx     #$33
	lda     #$E0
	jsr     pushax
	lda     #<(_sprite_missile)
	ldx     #>(_sprite_missile)
	jsr     pushax
	ldx     #$00
	lda     #$08
	jsr     _memcpy
	lda     #$30
	sta     $02F4
	rts

.endproc

; ---------------------------------------------------------------
; void __near__ init_vars (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_init_vars: near

.segment	"CODE"

	clc
	lda     _xpos
	sta     ptr1
	lda     #$50
	adc     _xpos+1
	sta     ptr1+1
	lda     #$00
	tay
	sta     (ptr1),y
	sta     _counter
	ldx     #$03
	lda     #$AB
	sta     _xpos
	stx     _xpos+1
	sty     _move_type
	lda     _flags
	and     #$FE
	sta     _flags
	ora     #$02
	sta     _flags
	ldx     #$00
	lda     #$28
	sta     _ufo_xpos
	stx     _ufo_xpos+1
	ldx     #$50
	tya
	jsr     pushax
	lda     #<(S0005)
	ldx     #>(S0005)
	jmp     _printinternal

.endproc

; ---------------------------------------------------------------
; void __near__ main (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_main: near

.segment	"CODE"

	jsr     _init_antic
	jsr     _init_vars
L0005:	jsr     _wait_for_vblank
	lda     _flags
	and     #$02
	beq     L0006
	jsr     _handle_input
	jsr     _erase_sprite
	jsr     _update_sprite
	jsr     _draw_sprite
	jmp     L0009
L0006:	lda     #$06
	cmp     $D01F
	bne     L0009
	jsr     _init_vars
L0009:	inc     _counter
	jmp     L0005

.endproc


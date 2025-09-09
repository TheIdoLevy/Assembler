 
; test 3

.entry TEST3
.extern L

mcro2
MAIN: add #70, r7
	mcro main_macro
	dec r2
	mcroend 
prn r6
	clr r2

add #1027, r6
jsr TEST3
main_macro

TEST3: sub #-7, r5
mcro mcro2
	rts
	add #20, r6
mcroend
add L, r5

sub L, r6

.entry MAIN

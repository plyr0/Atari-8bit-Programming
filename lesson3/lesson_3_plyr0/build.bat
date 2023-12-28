del game.xex

cl65 -t atari -O -S main.c
move main.s main.asm

cl65 --debug-info -Wl --dbgfile,game.lab -m game.map -Ln game.lbl -t atari -Oi main.c -o game.xex -C atari.cfg

game.xex

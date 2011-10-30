@echo off
md bin
cl /Febin\zhttpd.exe *.c
del *.obj
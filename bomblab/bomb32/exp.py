#! /usr/bin/python

from pwn import *
import sys

#context.terminal = ["tmux","sp","-h"]
context.log_level = "debug"

DEBUG = 1

Local = True
BIN = './bomb'
HOST = '124.126.19.106'
PORT = 31347

def exploit(sh):
    
    ans = ["Public speaking is very easy.","1 2 6 24 120 720","0 q 777","9","opekma"]
    sh.sendlineafter("Have a nice day!",ans[0])
    sh.sendlineafter("Phase 1 defused. How about the next one?",ans[1])
    sh.sendlineafter("Keep going!",ans[2])
    sh.sendlineafter("Halfway there!",ans[3])
    sh.sendlineafter("Try this one.",ans[4])
    sh.interactive()

    return


if __name__ == "__main__":
    elf = ELF(BIN)
    if len(sys.argv)>1:
        LOCAL = False
        p = remote(HOST,PORT)
        exploit(p)
    else:
        LOCAL = True
        p = process(BIN)
        log.info('PID:'+str(proc.pidof(p)[0]))
        pause()
#        if DEBUG:
#            gdb.attach(p)
        exploit(p)
   

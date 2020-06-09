***
CSAPP LAB   
Log during solving it    
Environment:    
sys:kali 2019-3
***

# [datalab](./datalab)
## 1.1.Questions
- make failed like this:    
fatal error: bits/libc-header-start.h:      
solution:apt-get install gcc-multilibs

# bomblab
工具：pwndebug      
> 简单来说就是达成程序要求，不让explode函数执行     
首先要明确这是一个32位程序，所以它的参数是压在栈里的    

## phase_1
对该函数进行反编译以后发现它调用了string_not_equal函数      
根据语义，猜测应该是要eax指向的字符串和0x80497c0指向的字符串相同，这个eax其实就是我们输入的第一个字符串
```
    <main>
   0x08048a5a <+170>:	push   eax ; 压入函数phase_1的参数
   0x08048a5b <+171>:	call   0x8048b20 <phase_1>
   0x08048a60 <+176>:	call   0x804952c <phase_defused>

   <phase_1>
   0x08048b26 <+6>:	mov    eax,DWORD PTR [ebp+0x8] ;这里实际上就是将上面的eax继续赋给了eax
   0x08048b29 <+9>:	add    esp,0xfffffff8
   0x08048b2c <+12>:	push   0x80497c0
   0x08048b31 <+17>:	push   eax 
   0x08048b32 <+18>:	call   0x8049030 <strings_not_equal> ;猜测该函数应该是比较了eax和0x80497c0两个字符串的大小 
   0x08048b37 <+23>:	add    esp,0x10
   0x08048b3a <+26>:	test   eax,eax
   0x08048b3c <+28>:	je     0x8048b43 <phase_1+35> ;如果返回值不是0，炸弹就会爆炸，这里进一步验证了我们的猜想
   0x08048b3e <+30>:	call   0x80494fc <explode_bomb>
```
思路:   
在push 0x80497c0处下断点，查看其值为Public speaking is very easy.验证成功。     
或者x/30cb 0x80497c0，直接查看rodata的值

## phase_2
做到这里突然对phase_defused这个函数产生了些许疑惑，它是用来干嘛的？     
偷了个懒，用ida反编译了一下，它应该是在所有炸弹全部被拆除以后才会被触发的，那就先放到一边吧
```
    <main>
    /* 这里与上面一致 */
   0x08048aa0 <+240>:	push   eax
   0x08048aa1 <+241>:	call   0x8048b98 <phase_3>

    <phase_2>

```


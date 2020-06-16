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
   0x08048b48 <+0>:	push   ebp
   0x08048b49 <+1>:	mov    ebp,esp
   0x08048b4b <+3>:	sub    esp,0x20
   0x08048b4e <+6>:	push   esi
   0x08048b4f <+7>:	push   ebx
   0x08048b50 <+8>:	mov    edx,DWORD PTR [ebp+0x8]
   0x08048b53 <+11>:	add    esp,0xfffffff8
   0x08048b56 <+14>:	lea    eax,[ebp-0x18]
   0x08048b59 <+17>:	push   eax
   0x08048b5a <+18>:	push   edx
   0x08048b5b <+19>:	call   0x8048fd8 <read_six_numbers> ;调用这个函数，这个函数如果通不过也会引爆炸弹
   0x08048b60 <+24>:	add    esp,0x10 ;
   /* 这一段的代码逻辑为输入的第一个数字为1 后面的每个数字是前一个数字*2 *3 ...依此类推 */
   /* 不满足条件炸弹就会引爆 */
   0x08048b63 <+27>:	cmp    DWORD PTR [ebp-0x18],0x1
   0x08048b67 <+31>:	je     0x8048b6e <phase_2+38>
   0x08048b69 <+33>:	call   0x80494fc <explode_bomb>
   0x08048b6e <+38>:	mov    ebx,0x1
   0x08048b73 <+43>:	lea    esi,[ebp-0x18]
   0x08048b76 <+46>:	lea    eax,[ebx+0x1]
   0x08048b79 <+49>:	imul   eax,DWORD PTR [esi+ebx*4-0x4]
   0x08048b7e <+54>:	cmp    DWORD PTR [esi+ebx*4],eax
   0x08048b81 <+57>:	je     0x8048b88 <phase_2+64>
   0x08048b83 <+59>:	call   0x80494fc <explode_bomb>
   0x08048b88 <+64>:	inc    ebx
   0x08048b89 <+65>:	cmp    ebx,0x5
   0x08048b8c <+68>:	jle    0x8048b76 <phase_2+46>
   /* 一直到这里其实是一个循环 */
   0x08048b8e <+70>:	lea    esp,[ebp-0x28]
   0x08048b91 <+73>:	pop    ebx
   0x08048b92 <+74>:	pop    esi
   0x08048b93 <+75>:	mov    esp,ebp
   0x08048b95 <+77>:	pop    ebp
   0x08048b96 <+78>:	ret    

   <read_six_numbers>
   0x08048fd8 <+0>:	push   ebp
   0x08048fd9 <+1>:	mov    ebp,esp
   0x08048fdb <+3>:	sub    esp,0x8
   0x08048fde <+6>:	mov    ecx,DWORD PTR [ebp+0x8]
   0x08048fe1 <+9>:	mov    edx,DWORD PTR [ebp+0xc]
   0x08048fe4 <+12>:	lea    eax,[edx+0x14]
   0x08048fe7 <+15>:	push   eax
   0x08048fe8 <+16>:	lea    eax,[edx+0x10]
   0x08048feb <+19>:	push   eax
   0x08048fec <+20>:	lea    eax,[edx+0xc]
   0x08048fef <+23>:	push   eax
   0x08048ff0 <+24>:	lea    eax,[edx+0x8]
   0x08048ff3 <+27>:	push   eax
   0x08048ff4 <+28>:	lea    eax,[edx+0x4]
   0x08048ff7 <+31>:	push   eax
   0x08048ff8 <+32>:	push   edx
   0x08048ff9 <+33>:	push   0x8049b1b
   0x08048ffe <+38>:	push   ecx 
   0x08048fff <+39>:	call   0x8048860 <sscanf@plt>
   0x08049004 <+44>:	add    esp,0x20
   0x08049007 <+47>:	cmp    eax,0x5
   0x0804900a <+50>:	jg     0x8049011 <read_six_numbers+57>
   0x0804900c <+52>:	call   0x80494fc <explode_bomb> 
   0x08049011 <+57>:	mov    esp,ebp
   0x08049013 <+59>:	pop    ebp
   0x08049014 <+60>:	ret 

```
要求输入六个数字，然后满足一定的规则
调用sscanf前的栈帧状态如下:
地址 | 值
---|---
0xffffd2a8  | main栈帧的ebp
0xffffd2a4  | 
0xffffd2a0  | 
0xffffd29c  | 
0xffffd298  | 
0xffffd294  | 
0xffffd290  |
...         |
0xffffd240  | 0xffff2da4
0xffffd240  | ...
0xffffd240  | ...
0xffffd240  | ...
0xffffd240  | ...
0xffffd240  | 0xffff2d90
0xffffd244  | "%d %d %d %d %d %d %d" 
0xffffd240  | 输入的字符串

即输入六个数字到对应的六个地址，应该是phase_2的局部变量。

## phase_3
格式化字符串为%d %c % d。
一个switch语句  第一个数字应该是swicth的跳转条件
后面的一个字符，一个数字对应限制条件(本阶段答案不唯一)
0 q 777 和 7 b 524都可，还有其他答案，我没试

## phase_4
输入一个数字，对其计算func(n)，使func(n)==0x37,观察func(n)得逻辑可知它是一个Fibonacci数列
故数字为9

## phase_5
输入6个字母,有一个array，取字母ascii码得低4位为其索引，得到另外6个字母，使其为giants
输入的字符串为opekma

## phase_6
输入6个数字，有一个长度为全局链表，结构大概是
```
struct node{
    int;
    int;
    struct node* next;
}
```
输入的六个字母让链表重新排序，让链表的第一个域按从大到小排列即可
答案：4 2 6 3 1 5
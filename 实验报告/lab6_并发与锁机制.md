自旋锁

定义一个共享变量bolt，初始化为0。bolt为0时可以进入临界区，为1时不能进入

设锁

```C
void SpinLock::lock()
{
    uint32 key = 1;

    do
    {
        asm_atomic_exchange(&key, &bolt);
    } while (key);
}
```

```asm
; void asm_atomic_exchange(uint32 *register, uint32 *memeory);
asm_atomic_exchange:
    push ebp
    mov ebp, esp
    pushad

    mov ebx, [ebp + 4 * 2] ; register
    mov eax, [ebx]      ; 取出register指向的变量的值
    mov ebx, [ebp + 4 * 3] ; memory
    xchg [ebx], eax      ; 原子交换指令
    mov ebx, [ebp + 4 * 2] ; memory
    mov [ebx], eax      ; 将memory指向的值赋值给register指向的变量

    popad
    pop ebp
    ret
```

**作了一个重要的假设——形式参数register指向的变量不是一个共享变量。**


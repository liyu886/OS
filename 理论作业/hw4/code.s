.main

mov $3, %bx    # %bx中的值初始化为3
sub $1, %bx    # 给bx寄存器中的值减1
mov %bx, 2000  # 将bx寄存器中的值存入内存地址为2000的内存单元
mov 2000, %ax  # get 'value' at address 2000
add $1, %ax    # increment it
mov %ax, 2000  # store it back	

halt
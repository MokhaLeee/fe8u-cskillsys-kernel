/\/\* LynJump! \*\// {
    getline next_line;                              # 读取下一行（函数声明）
    if (match(next_line, /([a-zA-Z_][a-zA-Z0-9_]*)\(/, arr)) {  # 提取函数名
        print "LYN_REPLACE_CHECK(" arr[1] ");"       # 打印替换后的行
    }
    print next_line;                                # 打印原始函数声明行
    next;                                           # 跳过默认打印 LynJump 行
}
{ print }                                           # 默认打印其他行

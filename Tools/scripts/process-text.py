#!/bin/python3

import re

# 输入的文本数据
with open('input.txt', 'r', encoding='utf-8') as file:
    text_data = file.read()

# 正则表达式用于提取文件名和十六进制数
pattern = r'#incbin\s+"Source\/\.TextEntries\/(.*?)\.fetxt\.dmp"\s+setText\(\$(\w+),'

# 匹配并提取信息
matches = re.findall(pattern, text_data)

# 格式化并打印结果
for basename, msg_idx in matches:
    print(f'TEXTADV:0x{msg_idx}={basename}.txt')

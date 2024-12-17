#!/usr/bin/env python3

import sys, os

def show_exception_and_exit(exc_type, exc_value, tb):
	import traceback

	traceback.print_exception(exc_type, exc_value, tb)
	sys.exit(-1)

def gen_tokens(string):
	pos = 0

	string = string.strip()

	while len(string) > 0:
		if string[0] == '"':
			pos = string.find('"', 1)

			if pos < 0:
				yield string[1:]
				break

			yield string[1:pos]
			string = string[(pos+2):]

		else:
			pos = string.find(' ', 1)

			if pos < 0:
				yield string[:]
				break

			yield string[0:pos]
			string = string[(pos+1):]

		string = string.strip()

class MugEntry:

	def __init__(self, line):
		self.fPath = ''
		self.index = ''
		self.indexDefinition = None

		self.xMouth = 0
		self.yMouth = 0
		self.xEyes  = 0
		self.yEyes  = 0

		tokens = gen_tokens(line)

		try:
			self.fPath = next(tokens)
			self.index = next(tokens)

			self.xMouth = next(tokens)
			self.yMouth = next(tokens)

		except StopIteration:
			sys.exit("Not enough components in line `{0}`.".format(line))

		try:
			self.xEyes = next(tokens)
			self.yEyes = next(tokens)
			self.indexDefinition = next(tokens)

		except StopIteration:
			pass # those are optional

	def list_data_files(self, parentPath):
		realPath = os.path.join(os.path.dirname(parentPath), self.fPath)
		incBase = os.path.splitext(realPath)[0]

		yield incBase + '_mug.dmp'
		yield incBase + '_palette.dmp'
		yield incBase + '_minimug.dmp'
		yield incBase + '_frames.dmp'

	def gen_event_lines(self, parentPath):
		realPath = os.path.join(os.path.dirname(parentPath), self.fPath)

		if not os.path.exists(realPath):
			sys.exit("File `{0}` doesn't exist.".format(realPath))

		incBase = os.path.splitext(self.fPath)[0]

		yield '{\n'
		yield '_MugGfx:\n#incbin "{0}"\n\n'.format(incBase + '_mug.dmp')
		yield '_MugPal:\n#incbin "{0}"\n\n'.format(incBase + '_palette.dmp')
		yield '_MugMiniGfx:\n#incbin "{0}"\n\n'.format(incBase + '_minimug.dmp')
		yield '_MugFramesGfx:\n#incbin "{0}"\n\n'.format(incBase + '_frames.dmp')
		yield 'PUSH\nORG PortraitTable+(0x1C*{0})\n'.format(self.index)
		yield 'POIN _MugGfx\n'
		yield 'POIN _MugMiniGfx\n'
		yield 'POIN _MugPal\n'
		yield 'POIN _MugFramesGfx\n'
		yield 'WORD 0\n'
		yield 'BYTE {0} {1} {2} {3}\n'.format(self.xMouth, self.yMouth, self.xEyes, self.yEyes)
		yield 'WORD 1\n'
		yield 'POP\n}\n\n'

	def gen_header_lines(self):
		if self.indexDefinition != None:
			yield '#define {0} {1}\n\n'.format(self.indexDefinition, self.index)

def gen_header():
	yield '// File generated from by portrait-process\n'
	yield '#include "PortraitDef.h"\n'
	yield '#include "Tools/Tool Helpers.txt"\n\n'

if __name__ == '__main__':
	sys.excepthook = show_exception_and_exit

	import argparse

	argParser = argparse.ArgumentParser(description = "")

	argParser.add_argument("input", help = "input list file")

	argParser.add_argument("--list-files", action = 'store_true', help = "print installer dependencies")
	argParser.add_argument("--installer", help = "output installer filename")
	argParser.add_argument("--definition", help = "output filename header")

	arguments = argParser.parse_args()

	if not os.path.exists(arguments.input):
		sys.exit("File `{0}` doesn't exist.".format(arguments.input))

	mugs = []

	with open(arguments.input, 'r') as f:
		for line in f.readlines():
			line = line.strip()

			if (len(line) == 0):
				continue

			if line[0] == '#':
				continue

			mugs.append(MugEntry(line))

	if arguments.list_files:
		for mug in mugs:
			for filename in mug.list_data_files(arguments.input):
				print(filename)

	else:
		with open(arguments.installer, 'w') as f:
			f.writelines(gen_header())

			for mug in mugs:
				f.writelines(mug.gen_event_lines(arguments.installer))
	
		with open(arguments.definition, 'w') as fdef:
			fdef.writelines("#ifndef PORTRAIT_DEFINITIONS_H\n")
			fdef.writelines("#define PORTRAIT_DEFINITIONS_H\n\n")
			for mug in mugs:
				fdef.writelines(mug.gen_header_lines())

			fdef.writelines("#endif /* PORTRAIT_DEFINITIONS_H */\n")

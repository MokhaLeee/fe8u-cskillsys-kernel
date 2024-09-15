FE8_ELF = fireemblem8.elf
BASE_REF = base_ref.s
FE8_REF = fireemblem8.ref.s
FE8_SYM = fireemblem8.sym

all: $(FE8_REF) $(FE8_SYM)

$(FE8_REF): $(FE8_ELF) $(BASE_REF)
	python3 elf2ref.py $< > $@
	cat $(BASE_REF) >> $@

$(FE8_SYM): $(FE8_ELF)
	python3 elf2sym.py $< > $@

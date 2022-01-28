TRUE=$(shell which true)
WHOAMI=$(shell which whoami)

CFLAGS=-Wall -DTRUE='"${TRUE}"' -DWHOAMI='"${WHOAMI}"'

.PHONY: all
all: dry-run-cve-2021-4034

.PHONY: clean
clean:
	rm -rf dry-run-cve-2021-4034 pwnkit-dry-run.so_data.h pwnkit-dry-run.so

%.so: %.c
	$(CC) $(CFLAGS) --shared -fPIC -o $@ $<

%.so_data.h: %.so
	echo "#ifndef __PWNKIT_SO_DATA_H"  >$@
	echo "#define __PWNKIT_SO_DATA_H" >>$@
	xxd -i $<                         >>$@
	echo "#endif"                     >>$@

dry-run-cve-2021-4034: dry-run-cve-2021-4034.c pwnkit-dry-run.so_data.h
	$(CC) $(CFLAGS) -o $@ $<

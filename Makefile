.SUFFIXES:

framework: framework/marmoset.c framework/marmoset

framework/marmoset.c: src/marmoset.c
	mkdir -p framework
	cp src/marmoset.c framework/

framework/marmoset: src/marmoset src/t.c process.sed
	mkdir -p framework
	cat src/marmoset | sed -f ./process.sed > framework/marmoset
	chmod +x framework/marmoset

clean:
	rm framework/marmoset
	rm framework/marmoset.c
	rmdir framework/ --ignore-fail-on-non-empty
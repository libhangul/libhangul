doc: Doxyfile hangulkeyboards.dox
	doxygen

hangulkeyboards.dox: gen_hangulkeyboards.py
	./gen_hangulkeyboards.py > $@

Doxyfile: Doxyfile.in
	cd .. && ./config.status doc/$@

clean:
	rm -rf html

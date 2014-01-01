.PHONY: all install dist clean backup deinstall check
.SUFFIXES: .ps .pdf .py

BASE=/usr/local
MANDIR=$(BASE)/man
BINDIR=$(BASE)/bin
PYSITE!=python -c 'import site; print site.getsitepackages()[0]'

install: ${ALL}
	@if [ `id -u` != 0 ]; then \
		echo "You must be root to install the program!"; \
		exit 1; \
	fi
# Let Python do most of the install work.
	python setup.py install
# Lose the extension; this is UNIX. :-)
	mv $(BINDIR)/stl2ps.py $(BINDIR)/stl2ps
	mv $(BINDIR)/stl2pov.py $(BINDIR)/stl2pov
	mv $(BINDIR)/stl2pdf.py $(BINDIR)/stl2pdf
	mv $(BINDIR)/stlinfo.py $(BINDIR)/stlinfo
	rm -rf build
#Install the manual pages.
	gzip -c stl2ps.1 >stl2ps.1.gz
	gzip -c stl2pov.1 >stl2pov.1.gz
	gzip -c stl2pdf.1 >stl2pdf.1.gz
	gzip -c stlinfo.1 >stlinfo.1.gz
	install -m 644 stl2ps.1.gz stl2pov.1.gz stl2pdf.1.gz stlinfo.1.gz $(MANDIR)/man1
	rm -f stl2ps.1.gz stl2pov.1.gz stl2pdf.1.gz stlinfo.1.gz

deinstall::
	@if [ `id -u` != 0 ]; then \
		echo "You must be root to deinstall the program!"; \
		exit 1; \
	fi
	rm -f ${PYSITE}/stl.py*
	rm -f ${PYSITE}/xform.py*
	rm -f $(BINDIR)/stl2ps $(BINDIR)/stl2pov $(BINDIR)/stl2pdf $(BINDIR)/stlinfo
	rm -f $(MANDIR)/man1/stl2ps.1* $(MANDIR)/man1/stl2pdf.1*
	rm -f $(MANDIR)/man1/stlinfo.1* $(MANDIR)/man1/stl2pov.1*


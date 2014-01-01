#
# Makefile for source programs and the libraries in use
#

opt:
	$(MAKE) target "TARGET=$@"

debug:
	$(MAKE) target "TARGET=$@"

clean:
	$(MAKE) target "TARGET=$@"

target: 
	cd freeglut; $(MAKE) $(TARGET)
	cd gaps; $(MAKE) $(TARGET)
	cd apps; $(MAKE) $(TARGET)




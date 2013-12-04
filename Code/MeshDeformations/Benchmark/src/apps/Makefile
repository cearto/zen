#
# Makefile for applications
#

opt:
	$(MAKE) target "TARGET=$@"

debug:
	$(MAKE) target "TARGET=$@"

clean:
	$(MAKE) target "TARGET=$@"

target: 
	cd segEval; $(MAKE) $(TARGET)
	cd mshview; $(MAKE) $(TARGET)
	cd groupSegEval; $(MAKE) $(TARGET)
	cd segPreprocess; $(MAKE) $(TARGET)
	cd mshinfo; $(MAKE) $(TARGET)
	cd outputSegments; $(MAKE) $(TARGET)
	cd iseg; $(MAKE) $(TARGET)
	cd segAnalysis; $(MAKE) $(TARGET)
	cd msh2msh; $(MAKE) $(TARGET)
	


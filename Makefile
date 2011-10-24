OUTDIR:=out
FLAGS:=-Wall -std=c++0x -g3 -O0

UNITSFILES:=bits/types.hpp bits/defs.hpp bits/units.hpp bits/units.cpp



.PHONY: all
all: si



.PHONY: si
si: $(OUTDIR)/si.o

$(OUTDIR)/si.o: bits/units.cpp $(OUTDIR)/si.deps
	g++ $(FLAGS) -c $< -o $@

$(OUTDIR)/si.deps: bits/units.cpp $(UNITSFILES)
	@ mkdir -p $(OUTDIR)
	g++ $(FLAGS) $< -MM -MT '$(OUTDIR)/si.o $@' > $@	

$(UNITSFILES): bits/units.py
	cd bits ; python units.py

-include $(OUTDIR)/si.deps



.PHONY: test
test: $(OUTDIR)/test
	@ $<

$(OUTDIR)/test:  $(OUTDIR)/si.o $(OUTDIR)/test.o $(OUTDIR)/test_dummy.o
	g++ $(FLAGS) $+ -o $@

$(OUTDIR)/test.o: test.cpp $(OUTDIR)/test.deps
	g++ $(FLAGS) -c $< -o $@

$(OUTDIR)/test_dummy.o: test_dummy.cpp $(OUTDIR)/test_dummy.deps
	g++ $(FLAGS) -c $< -o $@

$(OUTDIR)/test.deps: test.cpp $(UNITSFILES)
	@ mkdir -p $(OUTDIR)
	g++ $(FLAGS) $< -MM -MT '$(OUTDIR)/test.o $@' > $@	

$(OUTDIR)/test_dummy.deps: test_dummy.cpp $(UNITSFILES)
	@ mkdir -p $(OUTDIR)
	g++ $(FLAGS) $< -MM -MT '$(OUTDIR)/test_dummy.o $@' > $@	

-include $(OUTDIR)/test*.deps



.PHONY: docs
docs:
	doxygen


.PHONY: clean
clean:
	rm -rf $(UNITSFILES) $(OUTDIR)/ docs/

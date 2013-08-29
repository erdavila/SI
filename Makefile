UNITSFILES := bits/types.hpp bits/defs.hpp bits/units.hpp bits/units.cpp
OUTDIR := out
OBJS := $(OUTDIR)/si.o $(OUTDIR)/test.o $(OUTDIR)/test_dummy.o

FLAGS  := -Wall -std=c++0x -g3 -O0

MAKEDEPS = @ g++ $(FLAGS) -MM $< -o $(@:.o=.d) -MT $@ -MP
COMPILE  =   g++ $(FLAGS) -c $< -o $@
LINK     =   g++ $(FLAGS) $+ -o $@


.PHONY: all
all: si

.PHONY: docs
docs:
	doxygen

.PHONY: test
test: $(OUTDIR)/test
	@ $<

.PHONY: clean
clean:
	rm -rf $(UNITSFILES) $(OUTDIR)/ docs/

.PHONY: si
si: $(OUTDIR)/si.o


$(OUTDIR)/si.o: bits/units.cpp
	@ mkdir -p $(OUTDIR)
	$(MAKEDEPS)
	$(COMPILE)

$(UNITSFILES): bits/units.py
	cd bits ; python units.py


$(OUTDIR)/test:  $(OUTDIR)/si.o $(OUTDIR)/test.o $(OUTDIR)/test_dummy.o
	$(LINK)

$(OUTDIR)/%.o: %.cpp
	@ mkdir -p $(OUTDIR)
	$(MAKEDEPS)
	$(COMPILE)


-include $(OBJS:.o=.d)

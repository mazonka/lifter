

all: $(HEAD) main.exe

$(HEAD):
	@echo "Error: build $(HEAD)"
	@exit 1

main.exe: o/bigun.$(OEXT) $(RT)/bigun.h $(obj1) $(obj2) $(LDF1)
	$(CL) $(obj1) $(obj2) o/bigun.$(OEXT) $(LDF1) $(LDFS) $(EOUT)main.exe

main: main.exe
	./main.exe

$(obj1): o/%.$(OEXT):%.cpp *.h
	@mkdir -p o
	$(CL) -c $< $(OOUT)$@

$(obj2): o/%.$(OEXT):../%.cpp ../*.h
	@mkdir -p o
	$(CL) -c $< $(OOUT)$@

o/bigun.$(OEXT): $(RT)/bigun.cpp $(RT)/bigun.h
	@mkdir -p o
	$(CL) -c $< $(OOUT)$@

c clean:
	rm -f *.$(OEXT) *.exe
	rm -rf o o.1 o.i o.0

s:
	cmd /C "$(APP)/run/style.bat *.cpp *.h"

check:
	cmp z_1 z_0
	cmp z_i z_0
	cmp z_i z_m
	rm z_1 z_0 z_i z_m

checkout:
	cmp z_i output.txt

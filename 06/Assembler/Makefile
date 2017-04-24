CC = c++ -std=c++11

srcfile := parser tokernizer assembler
out = Assembler
all: clean objs gen
objs:
	$(foreach src,$(srcfile), c++ -std=c++11 -g -c  $(src).cpp &)

gen:
	c++ -std=c++11 -g -o $(out)  $(foreach src,$(srcfile), $(src).o )
clean:
	del .*
	del *.o
	del $(out).exe
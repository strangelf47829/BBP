
.PHONY: %.cpp

%.cpp:
	@cppcheck ../../Source/$@ --dump -v -I ../../Source/include --force
	@cp ../../Source/$@.dump output.dump
	@rm ../../Source/$@.dump

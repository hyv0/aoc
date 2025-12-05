.PHONY: phony_explicit
.SECONDARY:

clean:
	rm -rf _build

_build/day%.out: day%/*.cpp day%/*.h
	@echo "======= Compiling Day $* ======="
	@mkdir -p ./_build/
	if [ ! -L _build/_inputs ]; then ln -sf `pwd`/_inputs _build/_inputs; fi
	g++ -Wall -O2 -march=native -funroll-loops -flto -fno-exceptions  -fno-rtti $(filter %.cpp, $^) -o ./_build/$(@F)


_inputs/day%.txt: 
	@echo "======= Downloading $(@F) ======="
	@$(eval SESSION = $(shell cat cookieFile.txt))
	curl https://adventofcode.com/2025/day/$*/input --cookie 'session=$(SESSION)' > $@

day%: phony_explicit _build/day%.out _inputs/day%.txt
	@echo
	@echo "=======  Running Day $*  ======="
	@echo
	@./_build/$@.out
	@echo

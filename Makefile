debug ?= false

COMMON_FLAGS += -fstack-protector-strong
ifeq (${debug}, true)
	COMMON_FLAGS += -g3 -ggdb -ftrapv -fno-omit-frame-pointer \
		-fsanitize=address -fsanitize=leak -fsanitize=undefined
else
	COMMON_FLAGS += -O2 -D_FORTIFY_SOURCE=2 -DNDEBUG
endif

profile ?= false
ifeq (${profile}, true)
	COMMON_FLAGS += -pg
endif

coverage ?= false
# todo

clang ?= false
ifeq (${clang}, true)
CC := clang
endif

SOURCES := $(wildcard src/*.c)
OBJECTS := $(patsubst %.c,%.o,${SOURCES})
DEPENDENCIES := $(patsubst %.c,%.d,${SOURCES})

INCLUDE_FLAGS := -I./include
WARNING_FLAGS := -Wextra -Wall -Wshadow -Wdouble-promotion \
		-Wformat=2 -fno-common -Wconversion
ifeq (${clang}, false)
WARNING_FLAGS += -Wformat-truncation -fanalyzer
endif

CFLAGS += ${WARNING_FLAGS} ${INCLUDE_FLAGS} ${COMMON_FLAGS}
export

.PHONY: clean tests coverage library

${OBJECTS}: %.o: %.c
	${CC} -Werror ${CFLAGS} -MMD -c $< -o $@

tests: ${OBJECTS}
	${MAKE} -C tests tests
	LSAN_OPTIONS=verbosity=1:log_threads=1 ./tests/tests

library: ${OBJECTS} | lib
	ar -rc lib/libdictionary.a $^

lib:
	mkdir lib

-include $(DEPENDENCIES)

clean:
	rm -rf ${OBJECTS} ${DEPENDENCIES} lib
	-$(MAKE) -C tests clean

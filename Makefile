PROGNM = hglc

CC ?= gcc
CFLAGS ?= -g -ggdb -O3 -fPIE -flto -fstack-protector-strong --param=ssp-buffer-size=1 -Wno-reserved-id-macro -Wall -Wextra -Wpedantic -Werror -std=gnu18 -fsanitize=undefined
LDFLAGS ?= `pkg-config --libs-only-l readline pcq` -lm
VER = r`git rev-list --count HEAD`.`git rev-parse --short HEAD`
FMFLAGS = -wp -then -wp -wp-rte

ifneq ($(CC), tcc)
CFLAGS += -pie -D_FORTIFY_SOURCE=2
LDFLAGS += -Wl,-z,relro,-z,now
endif

ifeq ($(CC), clang)
CFLAGS += -Weverything -fsanitize-trap=undefined
endif

CFLAGS += -Wno-disabled-macro-expansion

CONFIGURATION ?= debug
ifneq ($(CONFIGURATION), release)
BLDDIR ?= dist/debug
CFLAGS += -g -ggdb -O0 -U_FORTIFY_SOURCE
else
BLDDIR ?= dist/release
CFLAGS += -DNDEBUG -O3
endif

include mke/rules

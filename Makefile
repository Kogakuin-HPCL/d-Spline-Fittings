#ライブラリを使用する際に使用するMakefile

ROOT=$(shell pwd -P)

CC=gcc

INC=./dspline/src
CFLAGS= -shared -O3 -lm -g

OBJDIR=./dspline/work
LIBDIR=./dspline/lib
SRC_DIR=./dspline/src
TOOL_DIR=./dspline/tool

OBJ=dsp_qr.o dsp_abic.o dsp_set.o dsp_ind.o idou.o dspline_fitting.o

all: ${LIBDIR}/libdspline.so
	echo "#!/bin/sh\\n" > setup_env.sh
	echo "export PATH=\$$PATH:$(ROOT)/dspline/bin" >> setup_env.sh
	echo "export C_INCLUDE_PATH=\$$C_INCLUDE_PATH:$(ROOT)/dspline/include" >> setup_env.sh
	echo "export LIBRARY_PATH=\$$LIBRARY_PATH:$(ROOT)/dspline/lib" >> setup_env.sh
	echo "export LD_LIBRARY_PATH=\$$LD_LIBRARY_PATH:$(ROOT)/dspline/lib" >> setup_env.sh

${LIBDIR}/libdspline.so: $(addprefix ./dspline/work/, ${OBJ})
	${CC} -o $@ $^ ${CFLAGS}

${OBJDIR}/%.o: ${SRC_DIR}/%.c
	${CC} -c -o $@ $^ -I${INC} ${CFLAGS}

clean:
	rm -f ${LIBDIR}/*.a
	rm -f ${OBJDIR}/*.o


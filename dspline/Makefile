#ツールを使用する際に使用するMakefile

CC=gcc

INC=./src
CFLAGS= -O3 -lm -fopenmp -g

OBJDIR=./work
LIBDIR=./lib
SRC_DIR=./src
TOOL_DIR=./tool

OBJ=dsp_qr.o dsp_abic.o dsp_set.o dsp_ind.o idou.o dspline_fitting.o

all: ${LIBDIR}/libdspline.a
	gcc ${TOOL_DIR}/main.c -g -O0  -fopenmp -L ./lib -I./include -o ./bin/fitting  -ldspline -lm
	rm -f ${LIBDIR}/libdspline.a

${LIBDIR}/libdspline.a: $(addprefix ./work/, ${OBJ})
	ar rcs $@ $^

${OBJDIR}/%.o: ${SRC_DIR}/%.c
	${CC} ${CFLAGS} -c -o $@ $^ -I${INC}

clean:
	rm -f ${LIBDIR}/*.a
	rm -f ${OBJDIR}/*.o


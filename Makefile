#ライブラリを使用する際に使用するMakefile

USER = user.c #ユーザが使用するmain.cの名前を右辺に入力する。

CC=gcc

INC=./dspline/src
CFLAGS= -O3 -lm -fopenmp -g

OBJDIR=./dspline/work
LIBDIR=./dspline/lib
SRC_DIR=./dspline/src
TOOL_DIR=./dspline/tool

OBJ=dsp_qr.o dsp_abic.o dsp_set.o dsp_ind.o idou.o dspline_fitting.o

all: ${LIBDIR}/libdspline.a
	rm -f abic
	gcc ${USER} -g -O0  -fopenmp -L ./dspline/lib -I./dspline/include -o fitting  -ldspline -lm

${LIBDIR}/libdspline.a: $(addprefix ./dspline/work/, ${OBJ})
	ar rcs $@ $^

${OBJDIR}/%.o: ${SRC_DIR}/%.c
	${CC} ${CFLAGS} -c -o $@ $^ -I${INC}

clean:
	rm -f ${LIBDIR}/*.a
	rm -f ${OBJDIR}/*.o


CC = nvcc
CFLAGS = -std=c++11
INCLUDES =
LDFLAGS = -lGL -lglut -lGLU --expt-relaxed-constexpr
SOURCES = main.cu
OUTF = main.exe
OBJS = main.o

$(OUTF): $(OBJS)
        $(CC) $(CFLAGS) -o $(OUTF) $< $(LDFLAGS)

$(OBJS): $(SOURCES)
        $(CC) $(CFLAGS) -c $<

rebuild: clean $(OUTF)

clean:
        rm *.o $(OUTF)

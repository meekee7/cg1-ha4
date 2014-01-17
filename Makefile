EXECUTABLE = cg1_ex4
CC = g++
CPPFLAGS = -O3 -Iglm-0.9.4.0 -std=c++0x
DEPS = glm-0.9.4.0/glm/glm.hpp glm-0.9.4.0/glm/gtc/constants.inl Context.h Texture.h Image.hpp GLSLShader.h Model.cpp
OBJ = cg1_ex4.o Context.o Texture.o Image.o GLSLShader.o Model.o
LIBS = -lGLEW -lGLU -lGL -lglut

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CPPFLAGS)

$(EXECUTABLE): $(OBJ)
	$(CC) -o $@ $^ $(CPPFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o *~ $(EXECUTABLE)		
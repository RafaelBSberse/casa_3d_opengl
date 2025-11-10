# Makefile para Casa 3D - Trabalho T2
# Compilação multiplataforma

# Compilador
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Nome do executável
TARGET = casa3d

# Arquivos fonte
SOURCES = main.cpp

# Detecta o sistema operacional
ifeq ($(OS),Windows_NT)
    # Windows
    LIBS = -lglfw3 -lglew32 -lopengl32 -lgdi32
    RM = del /Q
    MKDIR = if not exist
    TARGET := $(TARGET).exe
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        # Linux
        LIBS = -lGLEW -lglfw -lGL -lm
        RM = rm -f
        MKDIR = mkdir -p
    endif
    ifeq ($(UNAME_S),Darwin)
        # macOS
        LIBS = -lGLEW -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
        CXXFLAGS += -I/opt/homebrew/include -L/opt/homebrew/lib
        RM = rm -f
        MKDIR = mkdir -p
    endif
endif

# Regra padrão
all: $(TARGET)

# Compila o executável
$(TARGET): $(SOURCES)
	@echo "Compilando $(TARGET)..."
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LIBS)
	@echo "Compilação concluída!"

# Limpa arquivos gerados
clean:
	@echo "Limpando arquivos..."
	$(RM) $(TARGET)
	@echo "Limpeza concluída!"

# Executa o programa
run: $(TARGET)
	@echo "Executando $(TARGET)..."
	./$(TARGET)

# Regras phony
.PHONY: all clean run

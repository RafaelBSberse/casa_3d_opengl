#!/bin/bash
# Script de verificação de dependências - Linux/macOS

echo "========================================"
echo "Verificação de Dependências - Casa 3D"
echo "========================================"
echo ""

echo "Verificando compilador C++..."
if command -v g++ &> /dev/null; then
    echo "[OK] g++ encontrado"
    g++ --version | head -n 1
else
    echo "[ERRO] g++ não encontrado. Instale build-essential (Linux) ou Xcode (macOS)."
fi
echo ""

echo "Verificando bibliotecas..."
echo ""

# Teste de compilação
cat > test_deps.cpp << EOF
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
int main() { return 0; }
EOF

if g++ -c test_deps.cpp -o test_deps.o 2>/dev/null; then
    echo "[OK] Headers encontrados (GLEW, GLFW, GLM)"
    rm -f test_deps.o
else
    echo "[ERRO] Alguns headers não foram encontrados"
    echo ""
    echo "Instale as dependências:"
    echo "  Linux (Ubuntu/Debian): sudo apt install libglfw3-dev libglew-dev libglm-dev"
    echo "  Linux (Fedora):       sudo dnf install glfw-devel glew-devel glm-devel"
    echo "  macOS:                brew install glfw glew glm"
fi

rm -f test_deps.cpp

echo ""
echo "Verificando OpenGL..."
if command -v glxinfo &> /dev/null; then
    GL_VERSION=$(glxinfo | grep "OpenGL version" | head -n 1)
    echo "[OK] $GL_VERSION"
else
    echo "[INFO] glxinfo não encontrado (instale mesa-utils para verificar OpenGL)"
fi

echo ""
echo "========================================"
echo "Verificação completa"
echo "========================================"
echo ""
echo "Se todas as verificações passaram, execute:"
echo "  make"
echo "ou"
echo "  g++ -std=c++17 main.cpp -o casa3d -lGLEW -lglfw -lGL -lm"
echo ""

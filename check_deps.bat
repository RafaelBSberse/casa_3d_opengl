@echo off
REM Script de verificação de dependências - Windows

echo ========================================
echo Verificação de Dependências - Casa 3D
echo ========================================
echo.

echo Verificando compilador C++...
where g++ >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [OK] g++ encontrado
    g++ --version | findstr /C:"g++"
) else (
    echo [ERRO] g++ não encontrado. Instale MinGW ou MSYS2.
)
echo.

echo Verificando bibliotecas...
echo.

echo Tentando compilar teste...
echo #include ^<GL/glew.h^> > test_deps.cpp
echo #include ^<GLFW/glfw3.h^> >> test_deps.cpp
echo #include ^<glm/glm.hpp^> >> test_deps.cpp
echo int main() { return 0; } >> test_deps.cpp

g++ -c test_deps.cpp -o test_deps.o >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [OK] Headers encontrados (GLEW, GLFW, GLM)
) else (
    echo [ERRO] Alguns headers não foram encontrados
    echo.
    echo Instale as dependências:
    echo   MSYS2: pacman -S mingw-w64-x86_64-glfw mingw-w64-x86_64-glew mingw-w64-x86_64-glm
)

del test_deps.cpp >nul 2>&1
del test_deps.o >nul 2>&1

echo.
echo ========================================
echo Verificação completa
echo ========================================
echo.
echo Se todas as verificações passaram, execute:
echo   make
echo ou
echo   g++ -std=c++17 main.cpp -o casa3d -lglfw3 -lglew32 -lopengl32 -lgdi32
echo.
pause

# Roteiro de Apresentação - Casa 3D OpenGL
**Duração: 8-10 minutos | Foco: Código e Implementação**

---

## 📋 ROTEIRO PASSO A PASSO

### **INTRODUÇÃO (30 segundos)**
- "Projeto de computação gráfica: Casa 3D com OpenGL moderno"
- "Vou mostrar a arquitetura orientada a objetos e as principais técnicas implementadas"

---

### **1. ESTRUTURA DO PROJETO (1 min)**

📂 **Abra o VS Code na pasta raiz**
- **Mostre a árvore de arquivos**
- **Fale:** "Arquitetura modular com headers separados por responsabilidade"

```
main.cpp      → Aplicação principal e criação da cena
Camera.h      → Sistema de câmera primeira pessoa
Shader.h      → Compilação de shaders GLSL
Mesh.h        → Gerenciamento de geometria (VAO/VBO/IBO)
Model.h       → Representação de objetos 3D
Geometry.h    → Geração de formas básicas
Texture.h     → Carregamento de texturas
shaders/      → Vertex e Fragment shaders
```

---

### **2. GEOMETRIA E VAO/VBO/IBO (2 min)**

**`Mesh.h` linha 1-20**
```cpp
struct Vertex {
    glm::vec3 position;   // Posição 3D
    glm::vec3 normal;     // Para iluminação
    glm::vec2 texCoords;  // Coordenadas de textura
};
```
- "Cada vértice tem posição, normal para iluminação, e UV para texturas"

**`Mesh.h` linha 30-50**
```cpp
unsigned int VAO, VBO, IBO;
```
- "VAO guarda a configuração, VBO os vértices, IBO os índices para reutilizar vértices"

**Vá até: `Geometry.h` linha 10-50**
- **Mostre a função `createCube`**
- **Fale:** "Geramos 24 vértices (4 por face) com normais calculadas"
- **Destaque:** "Índices permitem reutilizar vértices - cubo tem 8 posições únicas, mas 36 referências"

---

### **3. PIPELINE MVP (1.5 min)**

**Vá até: `main.cpp` linha 400-420 (loop de renderização)**
- **Mostre as três matrizes:**
```cpp
glm::mat4 model = glm::translate(...) * glm::rotate(...) * glm::scale(...);
glm::mat4 view = camera.getViewMatrix();
glm::mat4 projection = glm::perspective(...);
```
- **Fale:** "Pipeline MVP transforma coordenadas: Local → Mundo → Câmera → Tela"
- **Destaque:** "Model transforma o objeto, View posiciona a câmera, Projection faz perspectiva"

**Vá até: `shaders/phong.vert` linha 15-20**
- **Mostre:**
```glsl
gl_Position = projection * view * model * vec4(aPos, 1.0);
```
- **Fale:** "Multiplicação ocorre da direita para esquerda: vértice local vira posição na tela"

---

### **4. ILUMINAÇÃO PHONG (2 min)**

**Vá até: `shaders/phong.frag` linha 1-10**
- **Fale:** "Fragment shader calcula a cor de cada pixel com modelo Phong"

**Linha 15-18 (Ambiente)**
```glsl
vec3 ambient = ambientStrength * lightColor;
```
- **Fale:** "Componente ambiente: luz base constante, mesmo em sombra"

**Linha 20-24 (Difusa)**
```glsl
vec3 norm = normalize(Normal);
vec3 lightDir = normalize(lightPos - FragPos);
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * lightColor;
```
- **Fale:** "Componente difusa: quanto mais perpendicular a luz, mais iluminado"
- **Destaque:** "Dot product entre normal e direção da luz calcula o ângulo"

**Linha 26-30 (Especular)**
```glsl
vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm);
float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
vec3 specular = spec * lightColor;
```
- **Fale:** "Componente especular: reflexo brilhante dependendo do ângulo de visão"

**Linha 33**
```glsl
vec3 result = (ambient + diffuse + specular) * objectColor;
```
- **Fale:** "Soma das três componentes multiplica a cor do objeto"

---

### **5. CÂMERA PRIMEIRA PESSOA (1.5 min)**

**Vá até: `Camera.h` linha 30-40**
- **Mostre os vetores:**
```cpp
glm::vec3 Position;
glm::vec3 Front;
glm::vec3 Up;
glm::vec3 Right;
```
- **Fale:** "Câmera usa sistema de coordenadas local: frente, cima, direita"

**Linha 50-55 (getViewMatrix)**
```cpp
glm::mat4 getViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
}
```
- **Fale:** "LookAt cria matriz View: câmera olha para Position + Front"

**Linha 80-90 (processMouseMovement)**
```cpp
Yaw += xoffset;
Pitch += yoffset;
// Atualiza Front usando trigonometria
Front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
Front.y = sin(glm::radians(Pitch));
Front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
```
- **Fale:** "Mouse move ângulos Yaw e Pitch, convertidos para vetor direção 3D"

---

### **6. CONSTRUÇÃO DA CENA (1.5 min)**

**Vá até: `main.cpp` linha 150-180**
- **Mostre exemplos de criação:**
```cpp
// Parede (cubo esticado)
objects.push_back(Model(
    Geometry::createCube(1.0f),
    glm::vec3(0.0f, 2.0f, -10.0f),   // posição
    glm::vec3(0.0f, 0.0f, 0.0f),      // rotação
    glm::vec3(20.0f, 4.0f, 0.1f)      // escala
));
```
- **Fale:** "Objetos simples: pego um cubo básico e aplico transformações"

**Linha 200-230 (sofá ou mesa)**
```cpp
// Sofá = composição de cubos
// Base
objects.push_back(...);
// Encosto
objects.push_back(...);
// Braços
objects.push_back(...);
```
- **Fale:** "Objetos complexos: composição de formas simples posicionadas"
- **Destaque:** "68 objetos na cena usando apenas cubos, cilindros e esferas"

---

### **7. EXECUÇÃO (30 segundos)**

💻 **Compile e execute:**
```bash
g++ -std=c++17 main.cpp -o casa3d.exe -lglfw3 -lglew32 -lopengl32 -lgdi32
.\casa3d.exe
```
- **Mostre a cena rodando**
- **Fale:** "WASD para mover, mouse para olhar, 68 objetos renderizados em tempo real"

---

## 🎯 CONCLUSÃO (30 segundos)

**Recapitule:**
- ✅ Arquitetura orientada a objetos modular
- ✅ Pipeline MVP completo (Model-View-Projection)
- ✅ Iluminação Phong (ambiente + difusa + especular)
- ✅ Câmera em primeira pessoa com mouse
- ✅ Geometria eficiente com VAO/VBO/IBO
- ✅ OpenGL 4.0 Core Profile moderno

**Fale:** "Projeto demonstra fundamentos sólidos de computação gráfica: geometria, transformações, iluminação e interação"

---

## ⚠️ DICAS DE APRESENTAÇÃO

- **Navegue rapidamente entre arquivos** (Ctrl+P no VS Code)
- **Destaque as linhas importantes** enquanto fala
- **Não leia o código todo** - explique o conceito
- **Mantenha ritmo rápido** - 1-2 minutos por seção
- **Prepare o projeto compilado** antes de apresentar
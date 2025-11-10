#version 400 core

// Atributos de entrada (do VBO)
layout (location = 0) in vec3 aPos;      // Posição do vértice
layout (location = 1) in vec3 aNormal;   // Normal do vértice
layout (location = 2) in vec2 aTexCoord; // Coordenadas de textura

// Variáveis de saída para o Fragment Shader
out vec3 FragPos;    // Posição do fragmento no espaço world
out vec3 Normal;     // Normal no espaço world
out vec2 TexCoord;   // Coordenadas de textura

// Matrizes de transformação (Uniforms)
uniform mat4 model;      // Matriz de modelo (local -> world)
uniform mat4 view;       // Matriz de visualização (world -> view)
uniform mat4 projection; // Matriz de projeção (view -> clip)

void main()
{
    // Transforma a posição do vértice para o espaço world
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // Transforma a normal para o espaço world
    // Usa a matriz normal (transpose da inversa) para lidar com escalas não-uniformes
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // Passa as coordenadas de textura para o fragment shader
    TexCoord = aTexCoord;
    
    // Pipeline MVP completo: Vclip = Projection * View * Model * Vlocal
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

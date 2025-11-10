#version 400 core

// Variáveis de entrada do Vertex Shader
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

// Saída: cor final
out vec4 FragColor;

// Uniforms
uniform vec3 objectColor;
uniform bool useTexture;
uniform sampler2D texture1;

void main()
{
    // VERSÃO SUPER SIMPLES - SEM ILUMINAÇÃO
    // Apenas mostra a cor do objeto
    
    vec3 color;
    if (useTexture) {
        color = vec3(texture(texture1, TexCoord));
    } else {
        color = objectColor;
    }
    
    // Cor final sem iluminação
    FragColor = vec4(color, 1.0);
}

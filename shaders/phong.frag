#version 400 core

// Variáveis de entrada do Vertex Shader
in vec3 FragPos;   // Posição do fragmento no espaço world
in vec3 Normal;    // Normal no espaço world
in vec2 TexCoord;  // Coordenadas de textura

// Saída: cor final do fragmento
out vec4 FragColor;

// Uniforms para iluminação
uniform vec3 lightPos;     // Posição da fonte de luz pontual
uniform vec3 lightColor;   // Cor da luz
uniform vec3 viewPos;      // Posição da câmera

// Uniforms para material
uniform vec3 objectColor;  // Cor base do objeto (se não usar textura)
uniform bool useTexture;   // Se deve usar textura ou cor sólida
uniform sampler2D texture1; // Textura do objeto

// Componentes de iluminação Phong
uniform float ambientStrength;  // Intensidade da luz ambiente
uniform float specularStrength; // Intensidade da reflexão especular
uniform int shininess;          // Brilho especular (potência)

void main()
{
    // Determina a cor base do objeto
    vec3 baseColor;
    if (useTexture) {
        baseColor = vec3(texture(texture1, TexCoord));
    } else {
        baseColor = objectColor;
    }
    
    // ========== ILUMINAÇÃO PHONG CORRIGIDA ==========
    
    // 1. Componente Ambiente (sempre visível)
    vec3 ambient = ambientStrength * lightColor * baseColor;
    
    // 2. Componente Difusa (baseada na normal)
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * baseColor;
    
    // 3. Componente Especular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;
    
    // 4. Combina: SEMPRE multiplica baseColor no final
    vec3 result = ambient + diffuse + specular;
    
    // Cor final do fragmento
    FragColor = vec4(result, 1.0);
}

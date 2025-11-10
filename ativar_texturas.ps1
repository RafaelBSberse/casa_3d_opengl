# Script PowerShell para ativar texturas automaticamente
# Execute este script DEPOIS de colocar as imagens em res/textures/

Write-Host "`n🎨 ATIVADOR DE TEXTURAS - Trabalho T2`n" -ForegroundColor Cyan

# Verifica se as texturas existem
$texturesPath = "res\textures"
$requiredTextures = @("wood.jpg", "wall.jpg", "fabric.jpg", "tile.jpg", "metal.jpg")
$missingTextures = @()

Write-Host "📁 Verificando texturas em $texturesPath...`n" -ForegroundColor Yellow

foreach ($texture in $requiredTextures) {
    $fullPath = Join-Path $texturesPath $texture
    if (Test-Path $fullPath) {
        Write-Host "  ✅ $texture" -ForegroundColor Green
    } else {
        Write-Host "  ❌ $texture (NÃO ENCONTRADA)" -ForegroundColor Red
        $missingTextures += $texture
    }
}

if ($missingTextures.Count -gt 0) {
    Write-Host "`n⚠️  AVISO: Faltam $($missingTextures.Count) textura(s)!" -ForegroundColor Red
    Write-Host "Por favor, baixe as texturas faltantes e coloque em $texturesPath" -ForegroundColor Yellow
    Write-Host "`nConsulte COMO_ADICIONAR_TEXTURAS.md para instruções completas.`n" -ForegroundColor Cyan
    exit 1
}

Write-Host "`n✨ Todas as texturas encontradas!`n" -ForegroundColor Green

# Backup do main.cpp
Write-Host "💾 Criando backup do main.cpp..." -ForegroundColor Yellow
Copy-Item "main.cpp" "main.cpp.backup" -Force
Write-Host "  ✅ Backup salvo: main.cpp.backup`n" -ForegroundColor Green

# Lê o conteúdo do main.cpp
$content = Get-Content "main.cpp" -Raw

# Ativa o carregamento de texturas (descomenta os try-catch)
Write-Host "🔧 Ativando carregamento de texturas no código..." -ForegroundColor Yellow

# Usa strings com escape adequado para evitar conflitos com PowerShell
$woodCode = @"
Texture* woodTexture = nullptr;
    try {
        woodTexture = new Texture("res/textures/wood.jpg");
        std::cout << "Textura wood.jpg carregada!" << std::endl;
    } catch(...) {
        std::cout << "Falha ao carregar wood.jpg" << std::endl;
    }
"@

$wallCode = @"
Texture* wallTexture = nullptr;
    try {
        wallTexture = new Texture("res/textures/wall.jpg");
        std::cout << "Textura wall.jpg carregada!" << std::endl;
    } catch(...) {
        std::cout << "Falha ao carregar wall.jpg" << std::endl;
    }
"@

$fabricCode = @"
Texture* fabricTexture = nullptr;
    try {
        fabricTexture = new Texture("res/textures/fabric.jpg");
        std::cout << "Textura fabric.jpg carregada!" << std::endl;
    } catch(...) {
        std::cout << "Falha ao carregar fabric.jpg" << std::endl;
    }
"@

$tileCode = @"
Texture* tileTexture = nullptr;
    try {
        tileTexture = new Texture("res/textures/tile.jpg");
        std::cout << "Textura tile.jpg carregada!" << std::endl;
    } catch(...) {
        std::cout << "Falha ao carregar tile.jpg" << std::endl;
    }
"@

$metalCode = @"
Texture* metalTexture = nullptr;
    try {
        metalTexture = new Texture("res/textures/metal.jpg");
        std::cout << "Textura metal.jpg carregada!" << std::endl;
    } catch(...) {
        std::cout << "Falha ao carregar metal.jpg" << std::endl;
    }
"@

$content = $content -replace 'Texture\* woodTexture = nullptr;', $woodCode
$content = $content -replace 'Texture\* wallTexture = nullptr;', $wallCode
$content = $content -replace 'Texture\* fabricTexture = nullptr;', $fabricCode
$content = $content -replace 'Texture\* tileTexture = nullptr;', $tileCode
$content = $content -replace 'Texture\* metalTexture = nullptr;', $metalCode

# Substitui nullptr por texturas nos objetos
Write-Host "🎨 Aplicando texturas aos objetos..." -ForegroundColor Yellow

# Mapeia onde cada textura deve ser usada
$replacements = @{
    'createCube\(nullptr\).*\/\/ Chão' = 'createCube(woodTexture)); // Chão'
    'createCube\(nullptr\).*\/\/ Parede' = 'createCube(wallTexture)); // Parede'
    'createCube\(nullptr\).*\/\/ Teto' = 'createCube(wallTexture)); // Teto'
    'createCube\(nullptr\).*\/\/ Divisória' = 'createCube(wallTexture)); // Divisória'
    'createCube\(nullptr\).*\/\/ Sofá' = 'createCube(fabricTexture)); // Sofá'
    'createCube\(nullptr\).*\/\/ Mesa' = 'createCube(woodTexture)); // Mesa'
    'createCube\(nullptr\).*\/\/ Perna' = 'createCube(woodTexture)); // Perna'
    'createCube\(nullptr\).*\/\/ Cama' = 'createCube(fabricTexture)); // Cama'
    'createCube\(nullptr\).*\/\/ Base' = 'createCube(woodTexture)); // Base'
    'createCube\(nullptr\).*\/\/ Guarda-roupa' = 'createCube(woodTexture)); // Guarda-roupa'
    'createCube\(nullptr\).*\/\/ Vaso' = 'createCube(tileTexture)); // Vaso'
    'createCube\(nullptr\).*\/\/ Pia' = 'createCube(tileTexture)); // Pia'
    'createCube\(nullptr\).*\/\/ Banheira' = 'createCube(tileTexture)); // Banheira'
    'createCylinder\([^,]+,\s*nullptr\).*\/\/ Torneira' = 'createCylinder($1, metalTexture)); // Torneira'
    'createCylinder\([^,]+,\s*nullptr\).*\/\/ Poste' = 'createCylinder($1, metalTexture)); // Poste'
}

foreach ($pattern in $replacements.Keys) {
    $replacement = $replacements[$pattern]
    $content = $content -replace $pattern, $replacement
}

# Salva o arquivo modificado
$content | Set-Content "main.cpp" -NoNewline

Write-Host "  ✅ Código modificado com sucesso!`n" -ForegroundColor Green

# Compila o projeto
Write-Host "🔨 Compilando projeto com texturas..." -ForegroundColor Yellow
$compileResult = & g++ -std=c++17 main.cpp -o casa3d.exe -lglfw3 -lglew32 -lopengl32 -lgdi32 2>&1

if ($LASTEXITCODE -eq 0) {
    Write-Host "  ✅ Compilação bem-sucedida!`n" -ForegroundColor Green
    
    Write-Host "🚀 Executando projeto com texturas...`n" -ForegroundColor Cyan
    Write-Host "═══════════════════════════════════════`n" -ForegroundColor Gray
    
    & .\casa3d.exe
    
} else {
    Write-Host "  ❌ Erro na compilação:`n" -ForegroundColor Red
    Write-Host $compileResult -ForegroundColor Red
    Write-Host "`n💡 Restaurando backup..." -ForegroundColor Yellow
    Copy-Item "main.cpp.backup" "main.cpp" -Force
    Write-Host "  ✅ Backup restaurado`n" -ForegroundColor Green
    exit 1
}

Write-Host "`n═══════════════════════════════════════" -ForegroundColor Gray
Write-Host "✨ Processo concluído!" -ForegroundColor Green
Write-Host "`n📝 Nota: Se precisar voltar ao modo sem texturas, restaure o backup:" -ForegroundColor Cyan
Write-Host "   Copy-Item main.cpp.backup main.cpp -Force`n" -ForegroundColor Gray

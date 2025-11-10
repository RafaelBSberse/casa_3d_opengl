# Script PowerShell para verificar texturas
# Execute este script para verificar se as texturas foram colocadas corretamente

Write-Host "`n==================================" -ForegroundColor Cyan
Write-Host "  VERIFICADOR DE TEXTURAS - T2" -ForegroundColor Cyan
Write-Host "==================================`n" -ForegroundColor Cyan

# Verifica se as texturas existem
$texturesPath = "res\textures"
$requiredTextures = @("wood.jpg", "wall.jpg", "fabric.jpg", "tile.jpg", "metal.jpg")
$missingTextures = @()
$foundCount = 0

Write-Host "Verificando texturas em: $texturesPath`n" -ForegroundColor Yellow

foreach ($texture in $requiredTextures) {
    $fullPath = Join-Path $texturesPath $texture
    if (Test-Path $fullPath) {
        Write-Host "  [OK] $texture" -ForegroundColor Green
        $foundCount++
    } else {
        Write-Host "  [X]  $texture - NAO ENCONTRADA" -ForegroundColor Red
        $missingTextures += $texture
    }
}

Write-Host "`n----------------------------------" -ForegroundColor Gray
Write-Host "Resultado: $foundCount de $($requiredTextures.Count) texturas encontradas" -ForegroundColor White
Write-Host "----------------------------------`n" -ForegroundColor Gray

if ($missingTextures.Count -gt 0) {
    Write-Host "AVISO: Faltam $($missingTextures.Count) textura(s)!`n" -ForegroundColor Red
    Write-Host "Texturas faltando:" -ForegroundColor Yellow
    foreach ($tex in $missingTextures) {
        Write-Host "  - $tex" -ForegroundColor Red
    }
    Write-Host "`nPor favor, baixe as texturas e coloque em: $texturesPath" -ForegroundColor Yellow
    Write-Host "Consulte: COMO_ADICIONAR_TEXTURAS.md para instrucoes`n" -ForegroundColor Cyan
} else {
    Write-Host "SUCESSO! Todas as texturas foram encontradas!" -ForegroundColor Green
    Write-Host "`nProximo passo: Edite main.cpp manualmente para ativar as texturas" -ForegroundColor Yellow
    Write-Host "Consulte: COMO_ADICIONAR_TEXTURAS.md para instrucoes detalhadas`n" -ForegroundColor Cyan
}

$ErrorActionPreference = 'Stop'
$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'
$vs = & $vswhere -latest -requires Microsoft.Component.MSBuild -property installationPath
if (-not $vs) { throw 'Visual Studio C++ build tools were not found.' }
$project = Join-Path $PSScriptRoot '26311042_Leejunseo_GameCraft/26311042_Leejunseo_GameProject.vcxproj'
$output = Join-Path $PSScriptRoot '26311042_Leejunseo_GameCraft/x64/Release/KoreanUI/'
& (Join-Path $vs 'MSBuild/Current/Bin/MSBuild.exe') $project "/p:OutDir=$output" /p:Configuration=Release /p:Platform=x64 /p:WholeProgramOptimization=false /v:minimal /nologo
if ($LASTEXITCODE -ne 0) { throw 'Game build failed.' }

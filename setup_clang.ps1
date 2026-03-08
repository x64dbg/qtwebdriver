# Setup MSVC environment and configure with clang/clang++
$vcvars = 'C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat'

# Extract env vars from vcvars64
$tempBat = [System.IO.Path]::GetTempFileName() + '.bat'
"@echo off`r`ncall `"$vcvars`"`r`nset" | Out-File -FilePath $tempBat -Encoding ASCII
$envLines = cmd /c $tempBat 2>&1
Remove-Item $tempBat -Force

foreach ($line in $envLines) {
    if ($line -match '^([^=]+)=(.*)$') {
        [System.Environment]::SetEnvironmentVariable($Matches[1], $Matches[2], 'Process')
    }
}

Write-Host "MSVC environment loaded"
Write-Host "Running cmake with clang..."

& cmake `
    -S 'C:\CodeBlocks\qtwebdriver' `
    -B 'C:\CodeBlocks\qtwebdriver\build_clang' `
    -G Ninja `
    -DCMAKE_C_COMPILER='C:/Program Files/LLVM/bin/clang.exe' `
    -DCMAKE_CXX_COMPILER='C:/Program Files/LLVM/bin/clang++.exe' `
    -DQT_VERSION=5 `
    "-DCMAKE_PREFIX_PATH=C:/Qt/Qt5.12.12/5.12.12/msvc2017_64" `
    -DCMAKE_BUILD_TYPE=Release

Write-Host "cmake exit: $LASTEXITCODE"

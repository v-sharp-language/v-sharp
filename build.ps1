<# 
.SYNOPSIS 
    Build script for the project. 
.DESCRIPTION 
    Automates building VSharp, running tests, and packaging binaries. 
.PARAMETER Configuration 
    Build configuration: Debug or Release (default: Release). 
.PARAMETER RunTests
    Run unit tests after building.
.EXAMPLE
    pwsh ./build.ps1 -Configuration Debug -Verbose
    Builds VSharp in Debug mode with verbose logging.
.EXAMPLE
    pwsh ./build.ps1 -RunTests
    Builds VSharp and runs the unit tests.
.NOTES 
    File Name: build.ps1 
    Author: Codezz-ops (codezz-ops@protonmail.com) 
    Created: 2025-12-26 
#>

[CmdletBinding()]
param (
    [ValidateSet("Debug", "Release")]
    [string]$Configuration = "Release",
    [switch]$RunTests
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

function Fail { 
    param([string]$Message, [int]$Code = 1)
    Write-Error $Message
    exit $Code 
}

function Success { 
    param(
        [string]$Message
    ) 
    Write-Host $Message 
}

function Info {
    param(
        [string]$Message
    )
    Write-Host $Message
}

function VerboseLog { 
    param(
        [string]$Message
    ) 
    Write-Verbose $Message 
}

$StartTime = Get-Date

$IsWindowsOS = ($PSVersionTable.OS -join "") -match "Windows"
VerboseLog "Operating System: $($IsWindowsOS ? 'Windows' : 'Unix/Linux')"

$ProjectRoot = (Resolve-Path "$PSScriptRoot/") | Select-Object -First 1
$ProjectRoot = $ProjectRoot.ProviderPath
$SourceDir = Join-Path $ProjectRoot "source"
$IncludeDir = Join-Path $SourceDir "include"
$BuildDir = Join-Path $ProjectRoot "build"
$BinDir = Join-Path $BuildDir "bin"
$LibDir = Join-Path $BuildDir "lib"
$TestDir = Join-Path $ProjectRoot "tests"

VerboseLog "Project directories:"
VerboseLog " - Root: $ProjectRoot"
VerboseLog " - Source: $SourceDir"
VerboseLog " - Include: $IncludeDir"
VerboseLog " - Build: $BuildDir"
VerboseLog " - Bin: $BinDir"
VerboseLog " - Lib: $LibDir"
VerboseLog " - Tests: $TestDir"

$dirs = @($BuildDir, $BinDir, $LibDir)
foreach ($dir in $dirs) {
    if (Test-Path $dir) { 
        VerboseLog "Directory exists: $dir" 
    }
    else { 
        try { 
            New-Item -ItemType Directory -Path $dir -Force | Out-Null
            VerboseLog "Created directory: $dir" 
        } 
        catch { 
            Fail "Failed to create directory: $dir" 
        }
    }
}

if ($IsWindowsOS) {
    $Compiler = "cl.exe"
    $CFlags = "/W4 /EHsc" + ($Configuration -eq "Release" ? " /O2" : " /Zi /Od")
}
else {
    $Compiler = "g++"
    $CFlags = "-Wall -Wextra -pedantic -std=c++17" + ($Configuration -eq "Release" ? " -O2" : " -g -O0")
}

try { 
    Get-Command $Compiler -ErrorAction Stop | Out-Null 
}
catch { 
    Fail "Compiler not found: $Compiler" 
}

VerboseLog "Compiler: $Compiler"
VerboseLog "Flags: $CFlags"

$Sources = @(Get-ChildItem -Path $SourceDir -Recurse -Filter *.cxx)
if ($Sources.Count -eq 0) { 
    Fail "No source files found in $SourceDir" 
}

VerboseLog "Found source files ($($Sources.Count)):"
foreach ($src in $Sources) { 
    VerboseLog " - $src" 
}

$BinaryName = $IsWindowsOS ? "vsharp.exe" : "vsharp"
$OutputPath = Join-Path $BinDir $BinaryName

$ObjectFiles = @()
foreach ($src in $Sources) {
    $ObjName = [System.IO.Path]::ChangeExtension($src.Name, ".o")
    $ObjPath = Join-Path $BuildDir $ObjName
    $ObjectFiles += $ObjPath

    if ((-not (Test-Path $ObjPath)) -or ($src.LastWriteTime -gt (Get-Item $ObjPath).LastWriteTime)) {
        $CompileArgs = @()
        if ($IsWindowsOS) {
            $CompileArgs += $CFlags.Split()
            $CompileArgs += "/c", $src.FullName, "/Fo$ObjPath"
        } else {
            $CompileArgs += $CFlags.Split()
            $CompileArgs += "-c", $src.FullName, "-o", $ObjPath
        }

        if ($PSCmdlet.ShouldProcess("$($src.FullName)", "Compiling")) {
            & $Compiler @CompileArgs
        }
    } else {
        Write-Host "Skipping up-to-date source: $($src.Name)"
    }
}

$LinkArgs = @()
if ($IsWindowsOS) {
    $LinkArgs += $ObjectFiles
    $LinkArgs += "/Fe$OutputPath"
} else {
    $LinkArgs += $ObjectFiles
    $LinkArgs += "-o", $OutputPath
}

VerboseLog "Linking object files into binary"
VerboseLog "Link command: $Compiler $($LinkArgs -join ' ')"

& $Compiler @LinkArgs

if (-not (Test-Path $OutputPath)) { 
    Fail "Build succeeded but output binary not found." 
}

if ($RunTests) {
    $TestSources = @(
        Join-Path $TestDir "lexer_tests.cxx",
        Join-Path $SourceDir "lexer.cxx"
    )

    foreach ($file in $TestSources) { 
        if (-not (Test-Path $file)) { 
            Fail "Test source file not found: $file" 
        }
        VerboseLog "Test source: $file"
    }

    $TestExe = Join-Path $BinDir ($IsWindowsOS ? "lexer_tests.exe" : "lexer_tests")

    $TestCmd = if ($IsWindowsOS) {
        "$Compiler $CFlags /I`"$IncludeDir`" $($TestSources | ForEach-Object { '"' + $_ + '"' } -join ' ') /Fe`"$TestExe`""
    }
    else {
        "$Compiler $CFlags -I`"$IncludeDir`" $($TestSources | ForEach-Object { '"' + $_ + '"' } -join ' ') -o `"$TestExe`""
    }

    VerboseLog "Compiling tests"
    VerboseLog "Test compile command: $TestCmd"

    try { 
        Invoke-Expression $TestCmd 
    }
    catch { 
        Fail "Test compilation failed." 
    }

    VerboseLog "Running tests: $TestExe"
    try { 
        & $TestExe 
    }
    catch { 
        Fail "Tests failed." 
    }
}

$EndTime = Get-Date
$Duration = [math]::Round(($EndTime - $StartTime).TotalSeconds, 2)
$FileInfo = Get-Item $OutputPath
$RelativePath = Resolve-Path -Relative $OutputPath
VerboseLog "Binary size: $([math]::Round($FileInfo.Length / 1KB, 2)) KB"
Success "Build completed successfully in $Duration seconds. Binary: $RelativePath"

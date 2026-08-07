<#
.SYNOPSIS
    Builds NESContra (DirectX 11, C++20) with MSVC via MSBuild.

.DESCRIPTION
    Locates MSBuild (via vswhere, with hard-coded fallbacks) and runs it on
    NESContra.sln.

    The parallelism that matters here comes from the vcxproj, not from this
    script: /MP (MultiProcessorCompilation) is what spreads the project's ~175
    translation units across cores. MSBuild's own /m:4 only parallelises across
    PROJECTS, and this solution has exactly one, so it is passed for form's sake.
    /MP is also the reason the vcxproj sets /FS (ForceSynchronousPDBWrites - note
    the all-caps PDB, which is the spelling cl.xml defines; anything else is
    silently ignored by MSBuild): concurrent cl instances share one PDB, and
    without /FS they can corrupt it.

    FXC compiles the four .hlsl files to .cso in the output directory, and a
    post-build target copies Resources/ + the shader sources alongside them.

.USAGE
    .\build.ps1                  # Debug|x64 build
    .\build.ps1 -Config Release  # Release|x64 build
    .\build.ps1 -Platform x86    # Debug|x86 build
    .\build.ps1 -Rebuild         # full rebuild instead of an incremental build

    Run from the project root: D:\revamped\SE102.N21

    -Platform takes a SOLUTION platform name. NESContra.sln defines x64 and
    x86; the vcxproj spells the 32-bit one "Win32", but MSBuild rejects that
    when the target is the .sln ("MSB4126: the specified solution
    configuration is invalid"), so "Win32" is accepted here only as an alias
    and translated to x86 before it reaches MSBuild.
#>

param(
    [ValidateSet("Debug", "Release")]
    [string]$Config = "Debug",
    [ValidateSet("x64", "x86", "Win32")]
    [string]$Platform = "x64",
    [switch]$Rebuild
)

$ErrorActionPreference = "Stop"

# The solution has no "Win32" platform - that is the project's spelling.
if ($Platform -eq "Win32") { $Platform = "x86" }

$projectDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
Set-Location $projectDir

# --- Locate MSBuild ---
$msbuild = $null

$vswhere = $null
foreach ($programFiles in @(${env:ProgramFiles(x86)}, $env:ProgramFiles)) {
    if ([string]::IsNullOrEmpty($programFiles)) { continue }

    $candidate = Join-Path $programFiles "Microsoft Visual Studio\Installer\vswhere.exe"
    if (Test-Path $candidate) { $vswhere = $candidate; break }
}

if ($vswhere) {
    $found = & $vswhere -latest -requires Microsoft.Component.MSBuild `
                        -find "MSBuild\**\Bin\MSBuild.exe" 2>$null | Select-Object -First 1
    if ($found) { $msbuild = $found }
}

if (-not $msbuild) {
    foreach ($edition in @("Community", "Professional", "Enterprise", "BuildTools")) {
        $candidate = "C:\Program Files\Microsoft Visual Studio\2022\$edition\MSBuild\Current\Bin\MSBuild.exe"
        if (Test-Path $candidate) { $msbuild = $candidate; break }
    }
}

if (-not $msbuild) {
    Write-Host "MSBuild.exe not found. Install Visual Studio 2022 (or the Build Tools) with the C++ workload." -ForegroundColor Red
    exit 1
}

Write-Host "Using MSBuild: $msbuild" -ForegroundColor Cyan

# --- Build ---
$target = if ($Rebuild) { "Rebuild" } else { "Build" }
$solution = "NESContra.sln"

Write-Host "`nBuilding $solution ($Config|$Platform) / $target..." -ForegroundColor Cyan
Write-Host "  /MP (parallel compile, from the vcxproj)  /FS (safe PDB writes)  /v:minimal`n"

$previousErrorAction = $ErrorActionPreference
$ErrorActionPreference = "Continue"

& $msbuild $solution `
    /p:Configuration=$Config `
    /p:Platform=$Platform `
    /m:4 `
    /t:$target `
    /v:minimal `
    | Tee-Object -Variable buildOutput | ForEach-Object { $_ }

$exitCode = $LASTEXITCODE
$ErrorActionPreference = $previousErrorAction

Write-Host "`n--- Build Summary ---" -ForegroundColor Cyan
if ($exitCode -eq 0) {
    Write-Host "  BUILD SUCCEEDED" -ForegroundColor Green
}
else {
    Write-Host "  BUILD FAILED (exit code $exitCode)" -ForegroundColor Red
    $errors = $buildOutput | Where-Object { $_ -match "error" }
    if ($errors) {
        Write-Host "  Errors:" -ForegroundColor Yellow
        $errors | ForEach-Object { Write-Host "    $_" }
    }
    else {
        Write-Host "  (no 'error' lines captured - see MSBuild output above)" -ForegroundColor Yellow
    }
}

# Check output (only meaningful when the build actually succeeded, otherwise
# we would report a stale executable left over from a previous successful build)
if ($exitCode -eq 0) {
    # The project sets no explicit OutDir, so MSBuild's defaults for a classic
    # VC++ project apply: 64-bit platforms build into $(Platform)\$(Configuration)\
    # while Win32 builds into $(Configuration)\ with no platform folder at all.
    # (The previous mapping assumed "Win32\$Config" and so never found the exe.)
    #
    # $Platform is a SOLUTION platform here (x64 / x86), but the output folder is
    # named after the PROJECT platform - which is "Win32" for a 32-bit build.
    $candidates = if ($Platform -eq "x86") {
        @("$Config", "Win32\$Config")
    } else {
        @("$Platform\$Config", "$Config")
    }

    $exePath = $candidates |
        ForEach-Object { Join-Path $projectDir "$_\NESContra.exe" } |
        Where-Object { Test-Path $_ } |
        Select-Object -First 1

    if ($exePath) {
        $size = (Get-Item $exePath).Length
        Write-Host "  Output: $exePath ($size bytes)" -ForegroundColor Green
    }
    else {
        Write-Host "  Warning: build succeeded but NESContra.exe was not found under $($candidates -join ' or ')" -ForegroundColor Yellow
    }
}

exit $exitCode

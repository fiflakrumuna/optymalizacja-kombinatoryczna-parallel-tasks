$projectRoot = "C:\Users\adamg\Desktop\studia\PROJEKT OPTYMALIZACJA\optymalizacja-kombinatoryczna-parallel-tasks"
$cppDir = Join-Path $projectRoot "cpp"
$checkerPath = Join-Path $projectRoot "chk-pata.exe"
$resultPath = Join-Path $cppDir "result.txt"
$swfPath = Join-Path $projectRoot "Testing\test-n10k-m128-r0-0.txt"
$taskCount = 10000

Write-Host "Compiling..."
& g++ -std=c++17 (Join-Path $cppDir "main.cpp") (Join-Path $cppDir "data_scraper.cpp") (Join-Path $cppDir "greedy.cpp") -o (Join-Path $cppDir "app.exe")

if (-not $?) {
    Write-Error "Compilation failed."
    exit 1
}

Write-Host "Generating schedule..."
& (Join-Path $cppDir "app.exe") $swfPath $taskCount | Out-File -Encoding ASCII $resultPath

if (-not $?) {
    Write-Error "Schedule generation failed."
    exit 1
}

Write-Host "Running validator..."
& $checkerPath $swfPath $resultPath q

if (-not $?) {
    exit 1
}

@echo off
setlocal

REM 현재 bat 파일 폴더 기준으로 경로 계산
set "HERE=%~dp0"

REM 출력 위치(절대경로). 먼저 C:\Temp 폴더 만들어 둬!
set "OUT=C:\Temp\snap.png"

REM 원하는 해상도/장치 인자
"%HERE%SnapCam.exe" --device 0 --width 1280 --height 720 --warmup 2 --timeout_ms 1500 --out "%OUT%"

echo.
echo DONE. Saved to: %OUT%
pause
@echo off
echo ========================================
echo     运行基于栈的表达式计算器
echo ========================================

REM 检查可执行文件是否存在
if not exist calculator.exe (
    echo 可执行文件不存在，正在编译...
    call build.bat
    echo.
)

REM 运行程序
if exist calculator.exe (
    echo 启动计算器...
    echo.
    calculator.exe
) else (
    echo 错误：无法找到或创建可执行文件！
    pause
)

echo.
echo 程序已退出。
pause
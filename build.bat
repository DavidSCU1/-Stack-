@echo off
echo ========================================
echo     基于栈的表达式计算器编译脚本
echo ========================================

REM 检查是否存在g++编译器
where g++ >nul 2>nul
if %errorlevel% neq 0 (
    echo 错误：未找到g++编译器！
    echo 请确保已安装MinGW或其他C++编译器
    pause
    exit /b 1
)

echo 正在编译项目...

REM 编译项目
g++ -std=c++17 -Wall -Wextra -O2 -o calculator.exe main.cpp ExpressionCalculator.cpp

if %errorlevel% equ 0 (
    echo 编译成功！
    echo 可执行文件：calculator.exe
    echo.
    echo 运行程序请执行：calculator.exe
    echo 或者直接运行：run.bat
) else (
    echo 编译失败！请检查代码错误。
)

echo ========================================
pause
#include "ExpressionCalculator.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>

/**
 * 显示程序标题和欢迎信息
 */
void showWelcome() {
    std::cout << "========================================" << std::endl;
    std::cout << "    基于栈的表达式计算器 v1.0" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "欢迎使用表达式计算器！" << std::endl;
    std::cout << "输入 'help' 查看使用帮助" << std::endl;
    std::cout << "输入 'quit' 退出程序" << std::endl;
    std::cout << "========================================" << std::endl;
}

/**
 * 显示简单的使用提示
 */
void showQuickHelp() {
    std::cout << "\n快速提示：" << std::endl;
    std::cout << "- 支持运算符：+ - * / % ^ & ( )" << std::endl;
    std::cout << "- 支持负数和小数" << std::endl;
    std::cout << "- 表达式以 '=' 结束" << std::endl;
    std::cout << "- ^ 表示乘方，& 表示开方" << std::endl;
    std::cout << "- 示例：2 + 3 * 4 = 或 (2 + 3) * 4 =" << std::endl;
}

/**
 * 处理用户输入的命令
 */
bool processCommand(const std::string& input, ExpressionCalculator& calculator) {
    // 转换为小写进行比较
    std::string command = input;
    std::transform(command.begin(), command.end(), command.begin(), ::tolower);
    
    if (command == "help") {
        ExpressionCalculator::showHelp();
        return true;
    }
    else if (command == "quit" || command == "exit") {
        std::cout << "感谢使用表达式计算器，再见！" << std::endl;
        return false;
    }
    else if (command == "clear" || command == "cls") {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
        showWelcome();
        return true;
    }
    else if (command.empty()) {
        return true; // 空输入，继续
    }
    
    // 如果不是命令，则作为表达式处理
    return true;
}

/**
 * 验证表达式格式
 */
bool validateExpression(const std::string& expr) {
    if (expr.empty()) {
        std::cout << "错误：表达式不能为空！" << std::endl;
        return false;
    }
    
    // 检查是否以等号结尾
    if (expr.back() != '=') {
        std::cout << "错误：表达式必须以 '=' 结尾！" << std::endl;
        return false;
    }
    
    // 检查是否只有等号
    if (expr.length() == 1) {
        std::cout << "错误：表达式内容为空！" << std::endl;
        return false;
    }
    
    return true;
}

/**
 * 格式化输出结果
 */
void displayResult(double result) {
    std::cout << std::fixed << std::setprecision(6);
    
    // 如果结果是整数，则不显示小数部分
    if (result == static_cast<long long>(result)) {
        std::cout << "计算结果：" << static_cast<long long>(result) << std::endl;
    } else {
        std::cout << "计算结果：" << result << std::endl;
    }
}

/**
 * 主程序函数
 */
int main() {
    // 设置控制台输出编码（Windows）
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif
    
    showWelcome();
    showQuickHelp();
    
    ExpressionCalculator calculator;
    std::string input;
    
    while (true) {
        std::cout << "\n请输入表达式: ";
        std::getline(std::cin, input);
        
        // 去除输入字符串两端的空格
        size_t start = input.find_first_not_of(" \t");
        size_t end = input.find_last_not_of(" \t");
        
        if (start == std::string::npos) {
            input = "";
        } else {
            input = input.substr(start, end - start + 1);
        }
        
        // 处理命令
        if (input == "help" || input == "quit" || input == "exit" || 
            input == "clear" || input == "cls" || input.empty()) {
            if (!processCommand(input, calculator)) {
                break; // 退出程序
            }
            continue;
        }
        
        // 验证表达式格式
        if (!validateExpression(input)) {
            continue;
        }
        
        // 移除表达式末尾的等号，因为计算器内部会处理
        std::string expression = input.substr(0, input.length() - 1);
        
        // 计算表达式
        double result;
        std::cout << "\n正在计算: " << input << std::endl;
        
        if (calculator.calculate(expression + "=", result)) {
            displayResult(result);
            
            // 显示计算过程提示
            std::cout << "提示：计算使用双栈算法完成" << std::endl;
        } else {
            std::cout << "计算失败，请检查表达式格式！" << std::endl;
            std::cout << "输入 'help' 查看使用帮助" << std::endl;
        }
    }
    
    return 0;
}

/**
 * 程序入口点的替代实现（用于某些编译器）
 */
#ifdef ALTERNATIVE_MAIN
int wmain() {
    return main();
}
#endif
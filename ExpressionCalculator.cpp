#include "ExpressionCalculator.h"
#include <cmath>
#include <cctype>
#include <iomanip>

// 运算符优先级表定义
const OperatorPriority ExpressionCalculator::priority_table[] = {
    {'#', 0, 0},   // 表达式结束符
    {'(', 1, 8},   // 左括号
    {')', 8, 1},   // 右括号
    {'+', 3, 2},   // 加法
    {'-', 3, 2},   // 减法
    {'*', 5, 4},   // 乘法
    {'/', 5, 4},   // 除法
    {'%', 5, 4},   // 求模
    {'^', 7, 6},   // 乘方
    {'&', 7, 6}    // 开方
};

const int ExpressionCalculator::PRIORITY_TABLE_SIZE = sizeof(priority_table) / sizeof(OperatorPriority);

/**
 * 构造函数
 */
ExpressionCalculator::ExpressionCalculator() : currentPos(0) {
    // 初始化运算符栈，压入结束符
    optr.push('#');
}

/**
 * 析构函数
 */
ExpressionCalculator::~ExpressionCalculator() {
    // 栈的析构函数会自动清理内存
}

/**
 * 计算表达式主函数
 */
bool ExpressionCalculator::calculate(const std::string& expr, double& result) {
    // 重置计算器状态
    reset();
    expression = expr;
    currentPos = 0;
    
    // 检查表达式是否为空
    if (expression.empty()) {
        showError("表达式不能为空！");
        return false;
    }
    
    // 检查括号是否匹配
    if (!checkParentheses(expression)) {
        showError("括号不匹配！");
        return false;
    }
    
    char ch;
    GetNextchar(ch);
    
    while (ch != '=' && ch != '\0') {
        if (isdigit(ch) || ch == '.') {
            // 回退一个位置，让parseNumber处理完整数字
            currentPos--;
            double number;
            if (!parseNumber(number)) {
                showError("数字格式错误！");
                return false;
            }
            opnd.push(number);
        }
        else if (IsOperator(ch)) {
            char topOp;
            if (!optr.topValue(topOp)) {
                showError("运算符栈错误！");
                return false;
            }
            
            int stackPriority = isp(topOp);
            int inputPriority = osp(ch);
            
            if (stackPriority < inputPriority) {
                // 栈内优先级低于栈外优先级，运算符入栈
                optr.push(ch);
            }
            else if (stackPriority > inputPriority) {
                // 栈内优先级高于栈外优先级，执行运算
                double x, y, r;
                if (!Get2Operands(opnd, x, y)) {
                    showError("操作数不足！");
                    return false;
                }
                
                if (!cal(topOp, y, x, r)) {
                    showError("计算错误！");
                    return false;
                }
                
                opnd.push(r);
                optr.pop();
                currentPos--; // 重新处理当前运算符
            }
            else {
                // 优先级相等，处理括号
                if (ch == ')' && topOp == '(') {
                    optr.pop(); // 弹出左括号
                }
                else {
                    showError("表达式语法错误！");
                    return false;
                }
            }
        }
        else if (ch == ' ') {
            // 跳过空格
        }
        else {
            showError("非法字符: " + std::string(1, ch));
            return false;
        }
        
        GetNextchar(ch);
    }
    
    // 处理剩余的运算符
    char topOp;
    while (optr.topValue(topOp) && topOp != '#') {
        double x, y, r;
        if (!Get2Operands(opnd, x, y)) {
            showError("操作数不足！");
            return false;
        }
        
        if (!cal(topOp, y, x, r)) {
            showError("计算错误！");
            return false;
        }
        
        opnd.push(r);
        optr.pop();
    }
    
    // 检查最终结果
    if (opnd.getSize() != 1) {
        showError("表达式格式错误！");
        return false;
    }
    
    return opnd.topValue(result);
}

/**
 * 获取运算符的栈内优先级
 */
int ExpressionCalculator::isp(char ch) {
    for (int i = 0; i < PRIORITY_TABLE_SIZE; i++) {
        if (priority_table[i].op == ch) {
            return priority_table[i].isp;
        }
    }
    return -1; // 未找到
}

/**
 * 获取运算符的栈外优先级
 */
int ExpressionCalculator::osp(char ch) {
    for (int i = 0; i < PRIORITY_TABLE_SIZE; i++) {
        if (priority_table[i].op == ch) {
            return priority_table[i].osp;
        }
    }
    return -1; // 未找到
}

/**
 * 执行二元运算
 */
bool ExpressionCalculator::cal(char op, double x, double y, double& r) {
    switch (op) {
        case '+':
            r = x + y;
            break;
        case '-':
            r = x - y;
            break;
        case '*':
            r = x * y;
            break;
        case '/':
            if (y == 0) {
                showError("除数不能为零！");
                return false;
            }
            r = x / y;
            break;
        case '%':
            if (y == 0) {
                showError("模运算的除数不能为零！");
                return false;
            }
            r = fmod(x, y);
            break;
        case '^':
            r = pow(x, y);
            break;
        case '&':
            if (y < 0) {
                showError("开方运算的被开方数不能为负数！");
                return false;
            }
            if (x <= 0) {
                showError("开方运算的根指数必须为正数！");
                return false;
            }
            r = pow(y, 1.0 / x);
            break;
        default:
            showError("未知运算符: " + std::string(1, op));
            return false;
    }
    return true;
}

/**
 * 获取表达式中的下一个字符
 */
void ExpressionCalculator::GetNextchar(char& ch) {
    if (currentPos >= expression.length()) {
        ch = '\0';
    } else {
        ch = expression[currentPos++];
    }
}

/**
 * 判断字符是否为数字
 */
bool ExpressionCalculator::isdigit(char ch) {
    return std::isdigit(ch);
}

/**
 * 判断字符是否为运算符
 */
bool ExpressionCalculator::IsOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || 
           ch == '%' || ch == '^' || ch == '&' || ch == '(' || ch == ')';
}

/**
 * 从操作数栈中取出两个操作数
 */
bool ExpressionCalculator::Get2Operands(LStack<double>& opnd, double& x, double& y) {
    if (!opnd.topValue(x)) {
        return false;
    }
    opnd.pop();
    
    if (!opnd.topValue(y)) {
        return false;
    }
    opnd.pop();
    
    return true;
}

/**
 * 解析数字（包括小数和负数）
 */
bool ExpressionCalculator::parseNumber(double& number) {
    std::string numStr;
    char ch;
    
    // 处理负号
    GetNextchar(ch);
    if (ch == '-') {
        numStr += ch;
        GetNextchar(ch);
    }
    
    // 解析整数部分
    if (!isdigit(ch)) {
        return false;
    }
    
    while (isdigit(ch)) {
        numStr += ch;
        GetNextchar(ch);
    }
    
    // 解析小数部分
    if (ch == '.') {
        numStr += ch;
        GetNextchar(ch);
        
        if (!isdigit(ch)) {
            return false; // 小数点后必须有数字
        }
        
        while (isdigit(ch)) {
            numStr += ch;
            GetNextchar(ch);
        }
    }
    
    // 回退一个字符
    currentPos--;
    
    try {
        number = std::stod(numStr);
        return true;
    } catch (...) {
        return false;
    }
}

/**
 * 检查括号是否匹配
 */
bool ExpressionCalculator::checkParentheses(const std::string& expr) {
    LStack<char> parenStack;
    
    for (char ch : expr) {
        if (ch == '(') {
            parenStack.push(ch);
        } else if (ch == ')') {
            if (parenStack.isEmpty()) {
                return false; // 右括号多于左括号
            }
            parenStack.pop();
        }
    }
    
    return parenStack.isEmpty(); // 所有括号都匹配
}

/**
 * 重置计算器状态
 */
void ExpressionCalculator::reset() {
    opnd.clear();
    optr.clear();
    optr.push('#'); // 重新压入结束符
    expression.clear();
    currentPos = 0;
}

/**
 * 显示帮助信息
 */
void ExpressionCalculator::showHelp() {
    std::cout << "\n=== 表达式计算器使用帮助 ===" << std::endl;
    std::cout << "支持的运算符：" << std::endl;
    std::cout << "  +  : 加法" << std::endl;
    std::cout << "  -  : 减法" << std::endl;
    std::cout << "  *  : 乘法" << std::endl;
    std::cout << "  /  : 除法" << std::endl;
    std::cout << "  %  : 求模" << std::endl;
    std::cout << "  ^  : 乘方 (例: 2^3 = 8)" << std::endl;
    std::cout << "  &  : 开方 (例: 3&8 = 2, 表示8的3次方根)" << std::endl;
    std::cout << "  () : 括号，改变运算优先级" << std::endl;
    std::cout << "\n支持特性：" << std::endl;
    std::cout << "  - 负数 (例: -5 + 3)" << std::endl;
    std::cout << "  - 小数 (例: 3.14 * 2)" << std::endl;
    std::cout << "  - 括号嵌套 (例: ((2+3)*4))" << std::endl;
    std::cout << "\n使用方法：" << std::endl;
    std::cout << "  1. 输入表达式，以 '=' 结束" << std::endl;
    std::cout << "  2. 输入 'help' 查看帮助" << std::endl;
    std::cout << "  3. 输入 'quit' 退出程序" << std::endl;
    std::cout << "\n示例：" << std::endl;
    std::cout << "  2 + 3 * 4 =" << std::endl;
    std::cout << "  (2 + 3) * 4 =" << std::endl;
    std::cout << "  2 ^ 3 =" << std::endl;
    std::cout << "  3 & 27 =" << std::endl;
    std::cout << "  -5.5 + 2.3 =" << std::endl;
    std::cout << "=========================" << std::endl;
}

/**
 * 显示错误信息
 */
void ExpressionCalculator::showError(const std::string& message) {
    std::cout << "错误: " << message << std::endl;
}
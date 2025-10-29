#ifndef EXPRESSION_CALCULATOR_H
#define EXPRESSION_CALCULATOR_H

#include "LStack.h"
#include <string>
#include <iostream>

/**
 * 运算符优先级结构
 */
struct OperatorPriority {
    char op;
    int isp;  // 栈内优先级
    int osp;  // 栈外优先级
};

/**
 * 表达式计算器核心类
 * 使用双栈算法实现中缀表达式计算
 */
class ExpressionCalculator {
private:
    LStack<double> opnd;    // 操作数栈
    LStack<char> optr;      // 运算符栈
    std::string expression; // 当前表达式
    size_t currentPos;      // 当前解析位置
    
    // 运算符优先级表
    static const OperatorPriority priority_table[];
    static const int PRIORITY_TABLE_SIZE;

public:
    /**
     * 构造函数
     */
    ExpressionCalculator();
    
    /**
     * 析构函数
     */
    ~ExpressionCalculator();
    
    /**
     * 计算表达式
     * @param expr 要计算的表达式字符串
     * @param result 计算结果
     * @return 计算成功返回true，失败返回false
     */
    bool calculate(const std::string& expr, double& result);
    
    /**
     * 获取运算符的栈内优先级
     * @param ch 运算符字符
     * @return 栈内优先级值
     */
    int isp(char ch);
    
    /**
     * 获取运算符的栈外优先级
     * @param ch 运算符字符
     * @return 栈外优先级值
     */
    int osp(char ch);
    
    /**
     * 执行二元运算
     * @param op 运算符
     * @param x 第一个操作数
     * @param y 第二个操作数
     * @param r 计算结果引用
     * @return 计算成功返回true，失败返回false
     */
    bool cal(char op, double x, double y, double& r);
    
    /**
     * 获取表达式中的下一个字符
     * @param ch 字符引用，用于存储获取的字符
     */
    void GetNextchar(char& ch);
    
    /**
     * 判断字符是否为数字
     * @param ch 要判断的字符
     * @return 是数字返回true，否则返回false
     */
    bool isdigit(char ch);
    
    /**
     * 判断字符是否为运算符
     * @param ch 要判断的字符
     * @return 是运算符返回true，否则返回false
     */
    bool IsOperator(char ch);
    
    /**
     * 从操作数栈中取出两个操作数
     * @param opnd 操作数栈引用
     * @param x 第一个操作数引用
     * @param y 第二个操作数引用
     * @return 成功返回true，失败返回false
     */
    bool Get2Operands(LStack<double>& opnd, double& x, double& y);
    
    /**
     * 解析数字（包括小数和负数）
     * @param number 解析出的数字
     * @return 解析成功返回true，失败返回false
     */
    bool parseNumber(double& number);
    
    /**
     * 检查括号是否匹配
     * @param expr 表达式字符串
     * @return 匹配返回true，不匹配返回false
     */
    bool checkParentheses(const std::string& expr);
    
    /**
     * 重置计算器状态
     */
    void reset();
    
    /**
     * 显示帮助信息
     */
    static void showHelp();
    
    /**
     * 显示错误信息
     * @param message 错误信息
     */
    static void showError(const std::string& message);
};

#endif // EXPRESSION_CALCULATOR_H
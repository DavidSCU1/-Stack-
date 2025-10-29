#ifndef LSTACK_H
#define LSTACK_H

#include <iostream>

/**
 * 链式栈数据结构实现
 * 支持泛型，可存储任意类型的数据
 */
template<typename T>
class LStack {
private:
    // 栈节点结构
    struct Node {
        T data;
        Node* next;
        Node(const T& item, Node* ptr = nullptr) : data(item), next(ptr) {}
    };
    
    Node* top;      // 栈顶指针
    int size;       // 栈大小

public:
    // 构造函数
    LStack() : top(nullptr), size(0) {}
    
    // 析构函数
    ~LStack() {
        clear();
    }
    
    // 拷贝构造函数
    LStack(const LStack& other) : top(nullptr), size(0) {
        *this = other;
    }
    
    // 赋值运算符
    LStack& operator=(const LStack& other) {
        if (this != &other) {
            clear();
            if (!other.isEmpty()) {
                // 使用辅助栈来保持元素顺序
                LStack<T> temp;
                Node* current = other.top;
                while (current != nullptr) {
                    temp.push(current->data);
                    current = current->next;
                }
                // 从辅助栈弹出元素到当前栈
                T item;
                while (!temp.isEmpty()) {
                    temp.topValue(item);
                    push(item);
                    temp.pop();
                }
            }
        }
        return *this;
    }
    
    /**
     * 入栈操作
     * @param item 要入栈的元素
     * @return 成功返回true，失败返回false
     */
    bool push(const T& item) {
        try {
            Node* newNode = new Node(item, top);
            top = newNode;
            size++;
            return true;
        } catch (const std::bad_alloc&) {
            return false;
        }
    }
    
    /**
     * 出栈操作
     * @return 成功返回true，栈空返回false
     */
    bool pop() {
        if (isEmpty()) {
            return false;
        }
        Node* temp = top;
        top = top->next;
        delete temp;
        size--;
        return true;
    }
    
    /**
     * 获取栈顶元素值（不删除）
     * @param item 用于存储栈顶元素的引用
     * @return 成功返回true，栈空返回false
     */
    bool topValue(T& item) const {
        if (isEmpty()) {
            return false;
        }
        item = top->data;
        return true;
    }
    
    /**
     * 判断栈是否为空
     * @return 空返回true，非空返回false
     */
    bool isEmpty() const {
        return top == nullptr;
    }
    
    /**
     * 获取栈的大小
     * @return 栈中元素的个数
     */
    int getSize() const {
        return size;
    }
    
    /**
     * 清空栈
     */
    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }
    
    /**
     * 打印栈内容（用于调试）
     */
    void print() const {
        std::cout << "Stack (top to bottom): ";
        Node* current = top;
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};

#endif // LSTACK_H
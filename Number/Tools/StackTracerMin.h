#pragma once
#include <list>
#include <string>
#include "Console.h"


struct ubiq;

struct StackTracer
{
    static int stack_level;
    static StackTracer& Instance()
    {
        static StackTracer instance;
        return instance;
    }
    void push(std::string& func)
    {
        m_functions.push_back(func);
    }
    void pop()
    {
        if (m_functions.empty())
            return;
        m_functions.pop_back();
    }
    const std::string& top()
    {
        if (m_functions.empty())
            return "";
        return m_functions.back();
    }
    private:
        std::list<std::string> m_functions;
        StackTracer() {};
        StackTracer(const StackTracer&) = delete;
        StackTracer operator = (const StackTracer&) = delete;
};

#define $ auto __var =  ubiq();
#define $1 auto __var =  ubiq(__FUNCSIG__);
#define MARGIN_AND_WHITE WHITE << Margin(StackTracer::stack_level)
#define MARGIN Margin(StackTracer::stack_level)
#define $PRNT\
    std::cout << MARGIN_AND_WHITE << __FUNCSIG__ ;\
    struct __brackets{__brackets(){std::cout<<"{\n";}~__brackets(){std::cout << MARGIN_AND_WHITE << "}\n";}}__br;
#define $_  StackTracer::stack_level++;
#define $$_ StackTracer::stack_level--;

struct ubiq
{
    bool flag = 0;
    ubiq(std::string func_proto)
    { 
        flag = 1;
        StackTracer::stack_level++;
        StackTracer::Instance().push(func_proto);
    }

    ubiq()
    {
        flag = 0;
        StackTracer::stack_level++;
    }

    ~ubiq()
    {
        StackTracer::stack_level--;
        if(flag)
            StackTracer::Instance().pop();
    }
};
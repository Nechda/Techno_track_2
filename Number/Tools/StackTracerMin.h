#pragma once

extern int __stack_level;

struct ubiq
{
    ubiq() { __stack_level++; }
    ~ubiq() { __stack_level--; }
};

#define CONCAT(a,b) a##b
#define $ auto CONCAT(__var,__LINE__) =  ubiq();
#define $_  __stack_level++;
#define $$_ __stack_level--;
#include "Tools/Types.h"
#include "Dot.h"
#include "Number.h"
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;
namespace dot
{
    static void printOperationSequence();
    static string dotFileOutput;


    /**
        \brief Функций записывает в буфер строку
        \note  Данная функция не добавляет перевода строки
    */
    static void pushLineIntoDot(const string& line)
    {
        dotFileOutput.append(line);
    }


    /**
        \brief  Функция генерирует dot-файл, на основе данных из буфера
        \param  [in]  filename  Имя файла, в который будет записываться граф
        \note   Расширение файла указывать НУЖНО
    */
    void saveDotFile(const string& filename)
    {
        ofstream myfile;
        myfile.open(filename);
        myfile << "digraph{" << endl;
        myfile << dotFileOutput;
        myfile << "}" << endl;
        myfile.close();
        dotFileOutput = "";
    }

    /**
        \brief  Функция записывает сгенерированный граф в файл
        \param  [in]  filename  Имя файла, в который будет сохранен граф
        \note   Расширение файла указывать НУЖНО
    */
    void generateGraph(string filename)
    {
        printOperationSequence();
        saveDotFile(filename);
    }


    //первый аргумент --- узел, второй --- функция
    //Вектор пар для хранения информации о том, в какой функции была вызвана та или иная операция
    //
    //   имя узла в dot --+       +-- имя функции, в которой данная операция была вызвана
    //                    |       |
    //                    V       V
    static vector<pair<string, string>> nodes;


    /**
        \brief Функция добавляет на граф стрелки, задающие последовательность
               операций, которая реализуется при выполнении программы
           
    */
    static void printOperationSequence()
    {
        if (!nodes.size())
            return;
        stringstream myString;
        for (ui32 i = 0; i < nodes.size() - 1; i++)
        {
            myString << nodes[i].first << " -> " << nodes[i + 1].first
                     << " [ label = \"" << (i + 1) << "\", color = \"red\"]\n";
        }
        pushLineIntoDot(myString.str());

        myString.str("");
        string currentFunctionName = "";
        int clusterNum = 0;
        for (const auto& node : nodes)
        {

            if (currentFunctionName != node.second)
            {
                if (currentFunctionName.size())
                    myString << "}\n";
                currentFunctionName = node.second;
                myString << "subgraph cluster_" << clusterNum << "{\n";
                myString << "style=filled;\n color = lightgrey;\n";
                myString << "label = \"" << currentFunctionName << "\";\n";
                clusterNum++;
            }
            myString << node.first << "\n";
        }
        myString << "}\n";
        pushLineIntoDot(myString.str());
    }


    /**
        \brief   Функция генерирует на графе историю объекта
        \details Функция генерирует на графе последовательность стрелочек, отображающая
                 историю объекта. Все начинается на конструкторе, а заканчивается на деструкторе.
                 В историю объекта входят операции, меняющие внутреннее состояние объекта, например
                 оператор присваивания, инкременты и декременты.
        \param   [in]  num  Ссылка на объект типа Number, историю которого требуется создать
    */
    void createHistory(const Number& num)
    {
        #define MAKE_NAME_BY_PARAMS(p)\
            "NODE_" + to_string(num.m_id) + "_" + to_string(p)

        stringstream myString;
        for (int i = 0; i < num.m_nStage; i++)
            myString << MAKE_NAME_BY_PARAMS(i)
                     << " -> " MAKE_NAME_BY_PARAMS(i + 1)
                     << "[ style = \"dotted\"]\n";
        pushLineIntoDot(myString.str());

        #undef MAKE_NAME_BY_PARAMS
    }

    #define MAKE_OBJECT_NODE(obj) "NODE_" + to_string(obj.m_id) + "_" + to_string(obj.m_nStage)
    #define MAKE_OPERATOR_NODE() "OPERATION_"+to_string(Number::nOpration) + "_0"


    /**
        \brief Функция генерит новый узел графа
        \param [in]  num   Ссылка на объект типа Number, на основе которого создается узел
        \param [in]  func  Имя функции-члена, которая вызывается
    */
    void createNode(const Number& num, const string& func)
    {
        nodes.push_back({ MAKE_OBJECT_NODE(num), StackTracer::Instance().top() });
        stringstream myString;
        myString << MAKE_OBJECT_NODE(num) <<
            "[label = \"{";
        if (num.m_name.size() == 0)
            myString << func;
        else if (func.size() == 0)
            myString << num.m_name;
        else
            myString << num.m_name << "." << func;

        myString << "| 0x" << uppercase << hex
                 << &num << dec << "| m_number: "
                 << num.m_number << "}\","
                 << "shape = record]\n";
        pushLineIntoDot(myString.str());
    }


    /**
        \brief  Функция генерит линию между узлами
        \param  [in]  src    Ссылка на объект типа Number, узел которого является начальным
        \param  [in]  dst    Ссылка на объект типа Number, узел которого является конечным
        \param  [in]  label  Подпись к стрелке
        \param  [in]  style  Тип стрелки; может быть "dotted", "dashed", "solid"
    */
    void createLine(const Number& src, const Number& dst, string label, string style)
    {
        stringstream myString;
        myString << MAKE_OBJECT_NODE(src) << " -> " MAKE_OBJECT_NODE(dst)
                 << "[ label = \"" << label<< "\","
                 << "style = " << style << "]\n";
        pushLineIntoDot(myString.str());
    }


    /**
        \brief  Функция генерит линию между узлами
        \param  [in]  src    Имя узла, который считается начальным
        \param  [in]  dst    Ссылка на объект типа Number, узел которого является конечным
        \param  [in]  label  Подпись к стрелке
        \param  [in]  style  Тип стрелки; может быть "dotted", "dashed", "solid"
    */
    void createLine(const string& src, const Number& dst, string label, string style)
    {
        stringstream myString;
        myString << src << "->" MAKE_OBJECT_NODE(dst)
                 << "[ label = \"" << label << "\","
                 << "style = " << style << "]\n";
        pushLineIntoDot(myString.str());
    }


    /**
        \brief   Функция генерит часть графа, реализующую бинарный оператор
        \details Функция генерирует часть графа, реализующую бинарный оператор.
                 Генерируются две стрелки и узел самого бинарного оператора.
        \param   [in]  lhs          Ссылка на объект типа Number, являющийся левым операндом
        \param   [in]  rhs          Ссылка на объект типа Number, являющийся правым операндом
        \param   [in]  operatorStr  Строка с именем операции
    */
    void createOperator(const Number& lhs, const Number& rhs, string operatorStr)
    {
        nodes.push_back({ MAKE_OPERATOR_NODE(), StackTracer::Instance().top() });
        stringstream myString;
        myString << MAKE_OPERATOR_NODE() << "[ label = \""
            "{{<lhs>Number& lhs|<rhs>Number& rhs}|" << operatorStr << "}" <<
            "\", shape = record]\n";

        myString << MAKE_OBJECT_NODE(lhs) << " -> " MAKE_OPERATOR_NODE() << ":<lhs>[ style=\"dashed\"]\n";
        myString << MAKE_OBJECT_NODE(rhs) << " -> " MAKE_OPERATOR_NODE() << ":<rhs>[ style=\"dashed\"]\n";

        Number::nOpration++;
        pushLineIntoDot(myString.str());
    }
}
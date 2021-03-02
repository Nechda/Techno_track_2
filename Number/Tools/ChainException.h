#pragma once
#include <stdexcept>
#include <string>

/**
    \brief Структура реализует понятие "места" в коде
*/
struct DebugLocation
{
    const char* m_file; ///< Название файла
    const char* m_func; ///< Имя функции
    unsigned m_line;    ///< Номер строки

    //конструктор
    DebugLocation(
        const char* file,
        const char* func,
        unsigned line
    ) :
        m_line(line),
        m_func(func),
        m_file(file)
    {}
private:
    DebugLocation();
};

#if _MSC_VER
    #define DEBUG_LOCATION_GEN DebugLocation(__FILE__, __FUNCSIG__, __LINE__)
#else
    #define DEBUG_LOCATION_GEN(__FILE__, __FUNCTION__, __LINE__)
#endif


/**
    \brief Класс, реализующий цепные исключения
    \note  Является наследником от std::exception
*/
class ChainException : public std::exception
{
    private:
        const char* m_msg; ///< Информация об исключении
        DebugLocation m_location; ///< Место в коде, откуда было прошено исключение
        std::exception* m_prev_exception; ///< Указатель на исключение - причину
        std::string m_what_buffer; ///< Буффер для генерации полной строки с инфой об исключении
    public:
        ChainException(
            const char* msg,
            const DebugLocation location,
            std::exception* reason = nullptr
        ) :
            m_msg(msg),
            m_location(location),
            m_prev_exception(reason),
            m_what_buffer()
        {}

        ~ChainException()
        {
            if (m_prev_exception)
                m_prev_exception->~exception();
            delete m_prev_exception;
        }

        /// \brief Реализация метода what() std::exception
        const char* what()
        {
            if(m_what_buffer.size())
                return m_what_buffer.c_str();
            
            m_what_buffer.reserve(128);
            m_what_buffer.append("[ File: ");
            m_what_buffer.append(m_location.m_file);
            m_what_buffer.append("; Function: ");
            m_what_buffer.append(m_location.m_func);
            m_what_buffer.append("; Line: ");
            m_what_buffer.append(std::to_string(m_location.m_line));
            m_what_buffer.append("] : ");
            m_what_buffer.append(m_msg);
            if (m_prev_exception)
            {
                m_what_buffer.append("\n");
                m_what_buffer.append(m_prev_exception->what());
            }
            return m_what_buffer.c_str();
        }
};

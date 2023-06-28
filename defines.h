//
// Created by a6c0424fa083 on 15.06.23.
//

#ifndef DEFINES_H
#define DEFINES_H

#include <cstdarg>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

#ifdef LOG_PROCESS
#define LOG_FILE_PATH "./process_info.log"

clock_t start_time;

void WRITE_LOG_INIT()
{
    start_time = clock();
    time_t t   = std::time(nullptr);
    tm     tm  = *std::localtime(&t);

    std::ofstream log_file;
    log_file.open(LOG_FILE_PATH, std::ios::out | std::ios::app);
    if (log_file.is_open())
    {
        log_file << "\n\n\n\n\nN E W   L O G     " << std::put_time(&tm, "%Y-%m-%d   %H:%M:%S") << "\n---------------------------------------\n"
                 << std::endl;

        log_file.close();
    }
}

std::string GET_ELAPSED_CYCLES()
{
    clock_t now_time       = clock();
    clock_t elapsed_cycles = now_time - start_time;

    std::stringstream converter;
    converter << (uint64_t)(elapsed_cycles);

    std::string _elapsed_cycles;
    converter >> _elapsed_cycles;

    uint8_t cycle_length = _elapsed_cycles.length();

    _elapsed_cycles += " cycles -->   ";
    for (uint8_t i = 0; i < 20 /*max digits of uint64_t*/ - cycle_length; i++)
    {
        _elapsed_cycles += " ";
    }
    return _elapsed_cycles;
}

void WRITE_LOG(std::string message, ...)
{
    std::ofstream log_file;
    log_file.open(LOG_FILE_PATH, std::ios::out | std::ios::app);

    if (log_file.is_open())
    {
        log_file << GET_ELAPSED_CYCLES();

        va_list args;
        va_start(args, message);

        bool  ignore_next = false;
        char *str;

        for (uint64_t i = 0; i < message.length(); i++)
        {
            if (message.at(i) != '%')
            {
                if (!ignore_next)
                    log_file << message.at(i);

                ignore_next = false;
            }
            else
            {
                ignore_next = true;

                switch (message.at(i + 1))
                {
                case 's':
                    str = va_arg(args, char *);
                    for (uint16_t j = 0; *(str + j) != '\0'; j++)
                    {
                        log_file << *(str + j);
                    }
                    break;
                case 'i':
                    log_file << va_arg(args, int);
                    break;
                case 'd':
                    log_file << va_arg(args, double);
                    break;
                default:
                    break;
                }
            }
            va_end(args);
        }
        log_file << std::endl;
        log_file.close();
    }
}

void EXIT_FUNCTION()
{
    time_t t  = std::time(nullptr);
    tm     tm = *std::localtime(&t);

    std::string       message = "EXITING PROGRAM! (";
    std::stringstream conversion;
    conversion << std::put_time(&tm, "%Y-%m-%d   %H:%M:%S");
    message += conversion.str();
    message += ")";
    WRITE_LOG(message);
}
#else  //LOG_PROCESS
void WRITE_LOG_INIT() {}
void WRITE_LOG(std::string message, ...) {}
void EXIT_FUNCTION() {}
#endif //LOG_PROCESS

#endif //DEFINES_H

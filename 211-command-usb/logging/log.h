#pragma once 

#include <stdio.h>



void log_version (void);
void log_prefix (const char *level, const char *function, int line);


#define LOG_LEVEL_ERR 1 //ошибка 

#define LOG_ERR(...)                                \
    do                                              \
    {                                               \
        if (LOG_LEVEL >= LOG_LEVEL_ERR)             \
        {                                           \
            log_prefix("err", __func__, __LINE__);  \
            printf(__VA_ARGS__);                    \
        }                                           \
    } while (0)


#define LOG_LEVEL_INF 2 //сведения

#define LOG_INF(...)                                \
    do                                              \
    {                                               \
        if (LOG_LEVEL >= LOG_LEVEL_INF)             \
        {                                           \
            log_prefix("inf", __func__, __LINE__);  \
            printf(__VA_ARGS__);                    \
        }                                           \
    } while (0)

#define LOG_LEVEL_DBG 3 // отладка


#define LOG_DBG(...)                                \
    do                                              \
    {                                               \
        if (LOG_LEVEL >= LOG_LEVEL_DBG)             \
        {                                           \
            log_prefix("dbg", __func__, __LINE__);  \
            printf(__VA_ARGS__);                    \
        }                                           \
    } while (0)



#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DBG
#endif





#pragma once

// Deletes a buffer
#define RELEASE( x ) \
    {									  \
       if( x != nullptr )   \
       {						      \
         delete x;                  \
	     x = nullptr;             \
       }                      \
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
	{                              \
       if( x != nullptr )              \
       {                            \
           delete[] x;                \
	       x = nullptr;                    \
		 }                            \
                              \
	 }

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#ifdef _DEBUG
#define DEBUG_EXP(exp)              exp
#ifdef _WIN32
#define ASSERT_INVALID()            __debugbreak()
#else
#define ASSERT_INVALID()            { *(int*)0 = 0; }
#endif
#define ASSERT(exp, ...)            if (!(exp)) ASSERT_INVALID()
#define ASSERT_OR_ALERT(exp, ...)   ([&](){ if (!!(exp)) return (exp); ALERT("ALERT", __VA_ARGS__); exit(-1); })()

#else
#define OUTPUT_MSG(...)             void(0)
#define LOG_ERROR(tag, ...)         void(0)
#define LOG_WARNING(tag, ...)       void(0)
#define LOG_INFO(tag, ...)          void(0)
#define ASSERT_INVALID()            void(0)
#define ALERT(...)                  void(0)
#define DEBUG_EXP(exp)
#define ASSERT(exp, ...)            void(0)
#define ASSERT_OR_ALERT(exp,...)    void(0)

#endif

enum class UpdateStatus
{
    UPDATE_CONTINUE,
    UPDATE_STOP,
    UPDATE_ERROR
};
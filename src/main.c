#include "Core/Platform.h"
#include "Core/Log.h"

#include "Backend/Token.h"
#include "Backend/Lexer.h"


int main(void)
{
    EnableConsoleColors();

    LOG_INFO("This is an info message");
    LOG_WARN("This is an warning message");
    LOG_ERROR("This is an error message");
    LOG_DEBUG("This is a debug message");
}

#include "Exception.h"

char* Exception::get_message() {
    switch(e) {
        case ERR_MEMORY: 
            return "Not Enough Memory";
        case ERR_FILE_NOT_FOUND:
            return "File Not Found";
        case ERR_FILE_WRITE:
            return "File Write Error";
        case ERR_FILE_ACCESS:
            return "File Access Error";
    }
    return "An Exception occured!";
}
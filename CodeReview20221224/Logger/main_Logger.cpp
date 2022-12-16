#include "Logger.h"


int main(int, char* []) {
    logger::Logger logger;
    logger.Alert(__FILE__, __LINE__, "alert");
}
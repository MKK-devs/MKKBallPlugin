#include "MKKLogger.h"
#include <ctime>

/*
Statuikus v�ltoz�k
*/
bool MKKLogger::fileBusy = false;
int MKKLogger::lastID = BASIC_INFO;
std::string MKKLogger::logFilePath = "MKKLog.txt";

/*
A Napl�bejegyz�s szintj�nek �t�r�sa a napl�ba.
*/
std::string LevelToString(LogLevel level) {
	switch (level) {
	default:
	case LL_ERROR:
		return "ERROR";
	case WARNING:
		return "WARNING";
	case INFO:
		return "INFO";
	case UPDATE:
		return "UPDATE";
	}
}

/*
�j napl� f�jl l�trehoz�sa
*/
void MKKLogger::generateLogFile(std::string filePath, int intervals)
{
	interval = intervals;
	logFilePath = filePath;
	logFileStream.open(logFilePath, std::fstream::out | std::fstream::app);
	logFileStream.close();
}



/*
�j napl�bejegyz�s �r�sa
*/
void MKKLogger::createLog(LogLevel level, std::string message, int groupID)
{

	while (fileBusy) {
		//Do nothing
	}

	fileBusy = true;
	std::time_t t = time(0);
	std::tm now;
	localtime_s(&now, &t);



	if((groupID != lastID) && (abs(lastLog - t) < interval)) return;

	logFileStream.open(logFilePath, std::fstream::out | std::fstream::app);
	logFileStream << " [" << now.tm_hour << ":" << now.tm_min << ":" << now.tm_sec << "] " << "[" << LevelToString(level).c_str() << "] " << message.c_str() << std::endl;
	logFileStream.close();

	lastLog = t;
	lastID = groupID;
	fileBusy = false;
}

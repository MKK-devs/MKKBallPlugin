#pragma once
#include <iostream>
#include <fstream>

/*
Enumer�ci� a k�l�nb�z� napl�bejegyz�sek szintjeihez
WARNING: figyelmen k�v�l hagyhat� (vesz�lyes) hiba
INFO: alapvet� inform�ci�
UPDATE: st�tuszv�ltoz�s, metainform�ci�
LL_ERROR: v�gzetes hiba
*/
enum LogLevel {
	WARNING,
	INFO,
	UPDATE,
	LL_ERROR
};


enum G_ID {
	BASIC_INFO,
	TRACK_INFO
};

/*
A Napl�z� oszt�ly fel�p�t�se
Ez az oszt�ly k�pes napl�zni f�ljba a pluginnal kapcsolatos alapvet� inform�ci�kat
Jelenleg napl�zza:
	A plugin bels� m�k�d�s�t, �s a st�tuszok v�ltoz�s�t
	A lej�tszott zenesz�mokat egy k�l�n f�jlba
*/
class MKKLogger
{
public:
	int generateLogFile(std::string filePath, int intervals = 1);
	void createLog(LogLevel level, std::string message, int groupID = BASIC_INFO);
	void logNowPlaying(std::string trackName);
	bool compareLastPlayed(std::string trackName);

private:
	
	static std::string logFilePath;
	static std::string musicFilePath;
	static bool fileBusy;

	std::fstream logFileStream;
	int interval;
	time_t lastLog;
	static int lastID;

	std::string last_played;
};
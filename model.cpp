#include "stdafx.h"
#include "model.h"
#include "utils.h"


namespace {
	
	SYSTEMTIME getLocalTime() {
		SYSTEMTIME time;
		GetLocalTime(&time);
		return time;
	}

	void insertInDB(std::string event, SYSTEMTIME &time) {

		// event length is 4. All lines have length of at most 25.

		std::ofstream file("log.txt", std::ofstream::app);
		file << event << ";" << time.wYear << "-" << time.wMonth << "-" << time.wDay << "-" << time.wHour << "-" << time.wMinute << "-" << time.wSecond << '\n';
		file.flush();
	}

	me::Optional<SYSTEMTIME> readLastLocalTime() {
		std::ifstream file("log.txt");
		if (file) {
			// TODO check note in Remarks in http://msdn.microsoft.com/query/dev12.query?appId=Dev12IDEF1&l=EN-US&k=k(fstream%2Fstd%3A%3Abasic_ifstream%3A%3Aseekg);k(istream%2Fstd%3A%3Abasic_istream%3A%3Aseekg);k(std%3A%3Abasic_ifstream%3A%3Aseekg);k(std%3A%3Abasic_istream%3A%3Aseekg);k(seekg);k(DevLang-C%2B%2B);k(TargetOS-Windows)&rd=true
			file.seekg(-26, file.end);
			// TODO: check what to do if file smaller than 26 characters.
			char buffer[26];
			// TODO: simplify all that.
			file.read(buffer, 26);
			for (int i = 24; i >= 0; i--) {
				if (buffer[i] == ';') {
					std::string datetimestr = std::string(buffer + i+1, buffer + 25);
					std::vector<std::string> itemsAsStr = me::split(datetimestr, '-');
					std::vector<unsigned short> items;
					std::transform(itemsAsStr.begin(), itemsAsStr.end(), std::back_inserter(items), [](const std::string &str) { return std::stoi(str); });
					SYSTEMTIME time;
					time.wYear = items[0];
					time.wMonth = items[1];
					time.wDay = items[2];
					time.wHour = items[3];
					time.wMinute = items[4];
					time.wSecond = items[5];
					return me::Optional<SYSTEMTIME>(time);
				}
			}
		}
		return me::Optional<SYSTEMTIME>();
	}

	me::Optional<SYSTEMTIME> lastLocalTime;
}

namespace model {
	
	void pong(){
		SYSTEMTIME time = getLocalTime();
		insertInDB("PONG", time);
		lastLocalTime = time;
	}

	void failPong(){
		SYSTEMTIME time = getLocalTime();
		insertInDB("NPNG", time);
		lastLocalTime = time;
	}

	LPSYSTEMTIME getLastLocalTime() {
		if (!lastLocalTime) {
			lastLocalTime = readLastLocalTime();
		}

		if (!lastLocalTime) {
			return 0;
		}

		return lastLocalTime.pointer();
	}
}
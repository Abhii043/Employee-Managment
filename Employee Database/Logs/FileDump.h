#ifndef _dump_
#define _dump_

#include<iostream>
#include<fstream>

namespace systemLog {
	class fileDump {
	private:
		std::ofstream writeFile{};
	public:
		fileDump() : writeFile{ "C:/Users/ZTI/Downloads/without controller/Employee Database/Logs/DatabaseLog.txt", std::ios::app } {}
		fileDump(const std::string& fileName) : writeFile{ fileName , std::ios::app} {}
		~fileDump() = default;

		void flush1(const std::string& buffer) {
			writeFile << buffer;
			writeFile.flush();
		}
	};
}

#endif
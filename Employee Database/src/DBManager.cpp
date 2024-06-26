#include <iostream>
#include <string>
#include <iomanip>
#include <exception>
#include "Config.h"
#include "DBManager.h"
#include "Salary.h"
#include "EmployeeController.h"
#include "../Logs/Logger.h"

using EmployeeDB::DBManager;
using EmployeeDB::Model::Salary;
using EmployeeDB::Controller::EmployeeController;

DBManager& DBManager::instance() {
	static DBManager dbManager;
	return dbManager;
}

int DBManager::openConnection() {
	m_ResultCode = sqlite3_open(EmployeeDB::Config::dbFilePath.string().c_str(), &m_DB);

	if (m_ResultCode == SQLITE_OK) {
		systemLog::Info("[SUCCESS]", "Successfully opened DB connection");
	}
	else {
		systemLog::Error("[FAIL]", "Error opening DB connection");

	}
	return m_ResultCode;
}

int DBManager::closeConnection() {
	m_ResultCode = sqlite3_close(m_DB);

	if (m_ResultCode == SQLITE_OK) {
		systemLog::Info("[SUCCESS]", "Successfully closed DB connection");
	}
	else {
		systemLog::Error("[FAIL]", "Error closing DB connection");
	}
	return m_ResultCode;
}

int DBManager::executeQuery(const char* queryString) {
	m_ResultCode = sqlite3_exec(m_DB, queryString, 0, 0, &m_ErrorMessage);
	if (m_ResultCode == SQLITE_OK) {
		systemLog::Info("[SUCCESS]", "Successfully executed Query ->" , queryString);
	}
	else {
		systemLog::Error("[FAIL]", m_ErrorMessage,"->", queryString);
		throw std::runtime_error{ m_ErrorMessage };
	}

	return m_ResultCode;
}

int DBManager::executeCustomQuery(const char* queryString, int (*callback)(void*, int, char**, char**), void* arg) {
	m_ResultCode = sqlite3_exec(m_DB, queryString, callback, arg, &m_ErrorMessage);

	if (m_ResultCode == SQLITE_OK) {
		systemLog::Info("[SUCCESS]", "Successfully executed Query ->", queryString);
	}
	else {
		systemLog::Error("[FAIL]", m_ErrorMessage, "->", queryString);
		throw std::runtime_error{ m_ErrorMessage };
	}

	return m_ResultCode;
}

int DBManager::selectCallback(void* arg, int argc, char** argv, char** azColName) {
	int* rowCount = static_cast<int*>(arg);
	(*rowCount)++;
	std::cout << "|--------------------|----------------------------------------|\n";
	int i;
	for (i = 0; i < argc; i++) {
		std::cout << "|" << std::setw(20) << std::left << azColName[i] << "|" << std::setw(40) << std::left << (argv[i] ? argv[i] : "NULL") << "|\n";
	}
	std::cout << "|--------------------|----------------------------------------|\n";
	std::cout << std::endl;
	return 0;
}

int DBManager::executeSelectQuery(const char* queryString) {
	int rowCount{ 0 };
	m_ResultCode = sqlite3_exec(m_DB, queryString, selectCallback, &rowCount, &m_ErrorMessage);

	if (m_ResultCode == SQLITE_OK) {
		systemLog::Info("[SUCCESS]", "Successfully executed Query ->", queryString);

	}
	else {
		systemLog::Error("[FAIL]", m_ErrorMessage, "->", queryString);
		throw std::runtime_error{ m_ErrorMessage };
	}
	return rowCount;
}

int DBManager::selectSalaryCallback(void* arg, int argc, char** argv, char** azColName) {
	int* rowCount = static_cast<int*>(arg);
	(*rowCount)++;

	Salary obj;

	std::cout << "|--------------------|----------------------------------------|\n";
	int i;
	for (i = 0; i < argc; i++) {
		if (!strcmp(azColName[i], "employeeID")) {
			obj.setEmployeeID(std::stoi(argv[i]));
		}
		std::cout << "|" << std::setw(20) << std::left << azColName[i] << "|" << std::setw(40) << std::left << (argv[i] ? argv[i] : "NULL") << "|\n";
	}
	EmployeeController::getSalaryDetails(obj);
	double totalSalary = Employee::computeSalary(obj);
	std::cout << "|" << std::setw(20) << std::left << "Total Salary" << "|" << std::setw(40) << std::left << totalSalary << "|\n";
	std::cout << "|--------------------|----------------------------------------|\n";
	std::cout << std::endl;
	return 0;
}

int DBManager::executeSelectSalaryQuery(const char* queryString) {
	int rowCount{ 0 };
	m_ResultCode = sqlite3_exec(m_DB, queryString, selectSalaryCallback, &rowCount, &m_ErrorMessage);

	if (m_ResultCode == SQLITE_OK) {
		systemLog::Info("[SUCCESS]", "Successfully executed Query ->", queryString);
	}
	else {
		systemLog::Error("[FAIL]", m_ErrorMessage, "->", queryString);
		throw std::runtime_error{ m_ErrorMessage };
	}
	return rowCount;
}

int DBManager::rowCountCallback(void* arg, int argc, char** argv, char** azColName) {
	int* rowCount = static_cast<int*>(arg);
	(*rowCount)++;
	return 0;
}

int DBManager::executeRowCountQuery(const char* queryString) {
	int rowCount{ 0 };
	m_ResultCode = sqlite3_exec(m_DB, queryString, rowCountCallback, &rowCount, &m_ErrorMessage);

	if (m_ResultCode == SQLITE_OK) {
		systemLog::Info("[SUCCESS]", "Successfully executed Query ->", queryString);
	}
	else {
		systemLog::Error("[FAIL]", m_ErrorMessage, "->", queryString);
		throw std::runtime_error{ m_ErrorMessage };
	}
	return rowCount;
}

void DBManager::executeConfigQuery() {
	std::string queryString = "PRAGMA foreign_keys = ON;";

	try {
		instance().executeQuery(queryString.c_str());
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		systemLog::Error("[FAIL]", e.what(), "->", queryString);
	}

	queryString = "PRAGMA case_sensitive_like = ON;";

	try {
		instance().executeQuery(queryString.c_str());
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		systemLog::Error("[FAIL]", e.what(), "->", queryString);
	}
}

void DBManager::executeTruncateQuery(const char* tableName) {
	std::string queryString = std::string{ "DELETE FROM " } + tableName + ";";

	try {
		instance().executeQuery(queryString.c_str());
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
	}
}
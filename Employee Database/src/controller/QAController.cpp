#include <exception>
#include "DBManager.h"
#include "QAController.h"
#include "EmployeeController.h"
#include "DepartmentController.h"

using EmployeeDB::Controller::EmployeeController;
using EmployeeDB::Controller::QAController;
using EmployeeDB::Controller::DepartmentController;
using EmployeeDB::DBManager;

bool QAController::insertQA(QA& obj) {
	int departmentID = DepartmentController::getDepartmentIDbyName("QA");

	if (departmentID == -1) {
		std::cerr << "\x1B[31mQA department not found. Please insert a department named QA.\033[0m\n";
		return false;
	}

	obj.setDepartmentID(departmentID);

	bool employeeResult = EmployeeController::insertEmployee(obj);

	if (!employeeResult) {
		std::cerr << "\x1B[31mQA could not be inserted.\033[0m\n";
		return false;
	}

	int employeeID = EmployeeController::getEmployeeIDbyEmail(obj.getEmail());

	std::string queryString = "INSERT INTO QA (employeeID, testingTool) VALUES (" +
		std::to_string(employeeID) + ", " +
		"\"" + obj.getTestingTool() + "\");";

	try {
		DBManager::instance().executeQuery(queryString.c_str());
		std::cout << "\x1B[32mSuccessfully inserted a QA.\033[0m\n";
	}
	catch (const std::exception& e) {
		std::cerr << "\x1B[31m" << e.what() << "\033[0m\n";
		std::cerr << "\x1B[31mQA could not be inserted.\033[0m\n";
		return false;
	}
	return true;
}

bool QAController::selectQA(const std::string& attributeName, const std::string& attributeValue) {
	std::string queryString = "SELECT * FROM Employee NATURAL JOIN QA " + ((attributeName.size() != 0) ? "WHERE " + attributeName + " = \"" + attributeValue + "\"  COLLATE NOCASE" : "") + ";";

	try {
		int rowCount = DBManager::instance().executeSelectQuery(queryString.c_str());
		std::cout << "\x1B[33m" << rowCount << std::string{ " record" } + (rowCount > 1 ? "s" : "") + " found\033[0m\n";
	}
	catch (const std::exception& e) {
		std::cerr << "\x1B[31m" << e.what() << "\033[0m\n";
		return false;
	}
	return true;
};

bool QAController::deleteQAByID(int ID) {
	int deleteResult = EmployeeController::deleteEmployeeByID(ID);

	if (deleteResult) {
		std::cout << "\x1B[32mSuccessfully deleted a QA.\033[0m\n";
	}
	else {
		std::cerr << "\x1B[31mQA could not be deleted.\033[0m\n";
	}
	return deleteResult;
}

std::string QAController::getUpdateQueryCondition(QA& obj) {
	std::string updateQueryCondition{ "" };

	if (obj.getTestingTool() != "#") {
		updateQueryCondition = "testingTool = \"" + obj.getTestingTool() + "\"";
	}

	return updateQueryCondition;
}

bool QAController::updateQA(QA& obj) {

	bool employeeResult = EmployeeController::updateEmployee(obj);

	if (!employeeResult) {
		std::cerr << "\x1B[31mQA could not be updated.\033[0m\n";
		return false;
	}

	std::string updateQueryCondition = getUpdateQueryCondition(obj);

	if (updateQueryCondition.size() != 0) {
		std::string queryString = "UPDATE QA SET " + updateQueryCondition + " WHERE employeeID = " + std::to_string(obj.getEmployeeID()) + ";";

		try {
			DBManager::instance().executeQuery(queryString.c_str());
			std::cout << "\x1B[32mSuccessfully updated a QA.\033[0m\n";
		}
		catch (const std::exception& e) {
			std::cerr << "\x1B[31m" << e.what() << "\033[0m\n";
			std::cerr << "\x1B[31mQA could not be updated.\033[0m\n";
			return false;
		}
	}
	return true;
}
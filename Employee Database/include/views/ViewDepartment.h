#ifndef _viewDepartment_h_
#define _viewDepartment_h_
#include "Department.h"

namespace EmployeeDB::Console {

	void departmentMenu() noexcept;
	void operationOfDept(const char& input) noexcept;
	bool insertDept();
	bool deleteDept();
	bool updateDept();
	bool viewDept();
}

#endif
#ifndef __ManagerControllerTest_h__
#define __ManagerControllerTest_h__

#include "pch.h"
#include "ManagerController.h"
#include "Manager.h"

class ManagerControllerTest : public EmployeeDB::Controller::ManagerController {
public:
	static std::string getUpdateQueryCondition(EmployeeDB::Model::Manager& man) {
		return ManagerController::getUpdateQueryCondition(man);
	}
};

#endif
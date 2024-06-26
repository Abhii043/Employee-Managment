#ifndef _viewQA_h_
#define _viewQA_h_
#include "QA.h"

namespace EmployeeDB::Console {

	void QAMenu() noexcept;
	void operationOfQA(const char& input) noexcept;
	bool insertInQA();
	bool deleteInQA();
	bool updateInQA();
	bool viewInQA();
}


#endif
#include "SqlTools.h"

bool SqlTools::Login_Account_Password_Check(const std::string& account, const std::string& password)
{
	return false;
}

bool SqlTools::Account_Exist(const std::string& account, const std::string& password)
{
	return false;
}

bool SqlTools::Login_Email_Code_Check(const std::string& email, const std::string& code)
{
	return false;
}

bool SqlTools::Register_Account_Password_Check(const std::string& account, const std::string& password)
{
	return false;
}

bool SqlTools::Register_Email_Code_Check(const std::string& email, const std::string& code)
{
	return false;
}

std::string SqlTools::SearchProductTable_Kind_Price_Name(const std::string& kind, const std::string& price, const std::string& name)
{
	return std::string();
}

std::string SqlTools::SearchOrderTable_State_Price_ID(const std::string& state, const std::string& price, const std::string& id)
{
	return std::string();
}

std::string SqlTools::Search_Sale_Information(const std::string& account, const std::string& password)
{
	return std::string();
}

std::string SqlTools::Search_SaleTable_State_Price_ID(const std::string& state, const std::string& price, const std::string& id)
{
	return std::string();
}

std::string SqlTools::Search_StaffTable_Role_Name(const std::string& role, const std::string& name)
{
	return std::string();
}

std::string SqlTools::Search_CustomerTable_Name(const std::string& name)
{
	return std::string();
}

bool SqlTools::Change_OrderTable_State(const std::string& account)
{
	return false;
}

bool SqlTools::Change_OrderTable(const std::string& changeInformation)
{
	return false;
}

bool SqlTools::Add_Order(const std::string& orderInformation)
{
	return false;
}

bool SqlTools::Delete_Order(const std::string& orderInformation)
{
	return false;
}

bool SqlTools::Change_StaffTable(const std::string& changeInformation)
{
	return false;
}

bool SqlTools::Add_Staff(const std::string& staffInformation)
{
	return false;
}

bool SqlTools::Delete_Staff(const std::string& staffInformation)
{
	return false;
}

bool SqlTools::Change_CustomerTable(const std::string& changeInformation)
{
	return false;
}

bool SqlTools::Add_Customer(const std::string& customerInformation)
{
	return false;
}

bool SqlTools::Delete_Customer(const std::string& customerInformation)
{
	return false;
}

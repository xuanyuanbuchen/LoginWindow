#pragma once
#include <string>
class SqlTools
{
public:
	//登陆判断
	bool Login_Account_Password_Check(const std::string& account, const std::string& password);
	bool Account_Exist(const std::string& account, const std::string& password);
	bool Login_Email_Code_Check(const std::string& email, const std::string& code);
	bool Register_Account_Password_Check(const std::string& account, const std::string& password);
	bool Register_Email_Code_Check(const std::string& email, const std::string& code);

	//查表操作
	std::string SearchProductTable_Kind_Price_Name(const std::string& kind = " ", const std::string& price = " ", const std::string& name = " ");
	std::string SearchOrderTable_State_Price_ID(const std::string& state = " ", const std::string& price = " ", const std::string& id = " ");
	std::string Search_Sale_Information(const std::string& account, const std::string& password);
	std::string Search_SaleTable_State_Price_ID(const std::string& state = " ", const std::string& price = " ", const std::string& id = " ");
	std::string Search_StaffTable_Role_Name(const std::string& role = " ", const std::string& name = " ");
	std::string Search_CustomerTable_Name(const std::string& name = " ");

	//改表操作
	bool Change_OrderTable_State(const std::string& account);
	bool Change_OrderTable(const std::string& changeInformation);
	bool Add_Order(const std::string& orderInformation);
	bool Delete_Order(const std::string& orderInformation);

	bool Change_StaffTable(const std::string& changeInformation);
	bool Add_Staff(const std::string& staffInformation);
	bool Delete_Staff(const std::string& staffInformation);

	bool Change_CustomerTable(const std::string& changeInformation);
	bool Add_Customer(const std::string& customerInformation);
	bool Delete_Customer(const std::string& customerInformation);


};
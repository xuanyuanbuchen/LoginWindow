#pragma once
#include <string>
class SqlTools
{
public:
	//登陆判断
	static int Login_Account_Password_Check(const std::string& account, const std::string& password);
	static int Login_Email_Code_Check(const std::string& email, const std::string& code);
	static bool Account_Exist(const std::string& account, const std::string& password);
	static bool Register_Account_Password_Check(const std::string& account, const std::string& password);
	static bool Register_Email_Code_Check(const std::string& email, const std::string& code);
	static bool Register_Account_Password_Email_Wirte(const std::string& account, const std::string& password, const std::string& email);

	//查表操作
	static std::string SearchProductTable_Kind_Price_Name(const std::string& kind = " ", const std::string& price = " ", const std::string& name = " ");
	static std::string SearchOrderTable_State_Price_ID(const std::string& state = " ", const std::string& price = " ", const std::string& id = " ");
	static std::string Search_Sale_Information(const std::string& account, const std::string& password);
	static std::string Search_SaleTable_State_Price_ID(const std::string& state = " ", const std::string& price = " ", const std::string& id = " ");
	static std::string Search_StaffTable_Role_Name(const std::string& role = " ", const std::string& name = " ");
	static std::string Search_CustomerTable_Name(const std::string& name = " ");

	//写表操作
	static bool Change_OrderTable_State(const std::string& account);
	static bool Change_OrderTable(const std::string& changeInformation);
	static bool Add_Order(const std::string& orderInformation);
	static bool Delete_Order(const std::string& orderInformation);

	static bool Change_StaffTable(const std::string& changeInformation);
	static bool Add_Staff(const std::string& staffInformation);
	static bool Delete_Staff(const std::string& staffInformation);

	static bool Change_CustomerTable(const std::string& changeInformation);
	static bool Add_Customer(const std::string& customerInformation);
	static bool Delete_Customer(const std::string& customerInformation);


};
#pragma once
#include <string>
#include <tuple>
#include <vector>
struct ProductDetail 
{
	std::string path;
	std::string ID;
	std::string name;
	std::string price;
	std::string category;
	std::string count;
	std::string description;

	ProductDetail
	(
		std::string path = NULL, std::string ID = NULL, std::string name = NULL,
		std::string price = NULL, std::string category = NULL, std::string count = NULL,
		std::string description = NULL
	)
		: path(std::move(path)), ID(std::move(ID)), name(std::move(name)),
		price(std::move(price)), category(std::move(category)), count(std::move(count)),
		description(std::move(description)) 
	{

	}
};

struct OrderDetail
{
	std::string order_ID;
	std::string customer_ID;
	std::string date;
	std::string price;
	std::string state;

	OrderDetail
	(
		std::string path = NULL, std::string ID = NULL, std::string name = NULL,
		std::string price = NULL, std::string category = NULL, std::string count = NULL,
		std::string description = NULL
	)
		: order_ID(std::move(order_ID)), customer_ID(std::move(customer_ID)), date(std::move(date)),
		price(std::move(price)), state(std::move(state))
	{

	}
	
};

struct SalesDetail
{
	std::string max_sales_amount;
	std::string max_sales_count;
	ProductDetail product_detail;

	SalesDetail
	(
		std::string max_sales_amount = NULL, std::string max_sales_count = NULL,
		ProductDetail product_detail = ProductDetail()
	)
		: max_sales_amount(std::move(max_sales_amount)), max_sales_count(std::move(max_sales_count)),
		product_detail(std::move(product_detail))
	{
	}
};


class SqlTools
{
public:
	//登陆判断
	static int Login_Account_Password_Check(const std::string& account, const std::string& password);
	static int Login_Email_Code_Check(const std::string& email, const std::string& code);
	static std::pair<std::string, std::string> Login_Get_Account_Password(const std::string& email, const std::string& code);
	static bool Account_Exist(const std::string& account, const std::string& password);
	static bool Register_Account_Password_Check(const std::string& account, const std::string& password);
	static bool Register_Email_Code_Check(const std::string& email, const std::string& code);
	static bool Register_Account_Password_Email_Wirte(const std::string& account, const std::string& password, const std::string& email);

	//查表操作
	static std::pair<std::vector<ProductDetail>, int> SearchProductTable_Kind_Price_Name
	(
		const std::string& kind = " ",
		const std::string& price = " ",
		const std::string& name = " ",
		const int& return_count = 9,
		const int& offset = 0
	);
	static std::pair<std::vector<OrderDetail>, int> SearchOrderTable_State_Price_ID
	(
		const std::string& kind = " ",
		const std::string& price = " ",
		const std::string& id = " ",
		const int& return_count = 9,
		const int& offset = 0
	);
	static SalesDetail Search_Sale_Information(const std::string& account, const std::string& password);
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
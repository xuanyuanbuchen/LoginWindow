#pragma once
#include<iostream>
#include<vector>
#include <qvector.h>
#include <qtimer.h>
#include <qdatetime.h>

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
		std::string path = "", std::string ID = "", std::string name = "",
		std::string price = "", std::string category = "", std::string count = "",
		std::string description = ""
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
		std::string order_ID = "",
		std::string customer_ID = "",
		std::string date = "",
		std::string price = "",
		std::string state = ""
	)
		: order_ID(std::move(order_ID)),
		customer_ID(std::move(customer_ID)),
		date(std::move(date)),
		price(std::move(price)),
		state(std::move(state))
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
		std::string max_sales_amount = "", std::string max_sales_count = "",
		ProductDetail product_detail = ProductDetail()
	)
		: max_sales_amount(std::move(max_sales_amount)), max_sales_count(std::move(max_sales_count)),
		product_detail(std::move(product_detail))
	{
	}
};
struct SaleTableLine
{
	std::string sale_ID;
	std::string order_ID;
	std::string goods_ID;
	std::string count;
	std::string date;
	SaleTableLine
	(
		std::string sale_ID = "", std::string order_ID = "", std::string goods_ID = "",
		std::string count = "", std::string date = ""
	)
		: sale_ID(std::move(sale_ID)), order_ID(std::move(order_ID)), goods_ID(std::move(goods_ID)),
		count(std::move(count)), date(std::move(date))
	{
	}
};
struct StaffDetail
{
	std::string staff_ID;
	std::string name;
	std::string email;
	std::string password;
	std::string join_date;
	std::string role;

	StaffDetail(
		std::string staff_ID = "",
		std::string name = "",
		std::string email = "",
		std::string password = "",
		std::string join_date = "",
		std::string role = ""
	)
		: staff_ID(std::move(staff_ID)),
		name(std::move(name)),
		email(std::move(email)),
		password(std::move(password)),
		join_date(std::move(join_date)),
		role(std::move(role))
	{
	}
};
struct CustomerDetail
{
	std::string profile_picture;
	std::string customer_ID;
	std::string birth_date;
	std::string note;
	std::string register_date;
	std::string email;
	std::string password;

	CustomerDetail(
		std::string customer_ID = "",
		std::string birth_date = "",
		std::string note = "",
		std::string register_date = "",
		std::string email = "",
		std::string password = ""
	)
		: customer_ID(std::move(customer_ID)),
		birth_date(std::move(birth_date)),
		note(std::move(note)),
		register_date(std::move(register_date)),
		email(std::move(email)),
		password(std::move(password))
	{
	}
};
struct FinancialData
{
	std::string timestamp;
	double income;
	double expense;

	FinancialData(
		std::string timestamp = "",
		double income = 0.0,
		double expense = 0.0
	)
		: timestamp(std::move(timestamp)),
		income(income),
		expense(expense)
	{
	}
};
struct StockDetail
{
	std::string goods_ID;
	std::string stock_ID;
	std::string count;
	std::string date;

	StockDetail(
		std::string goods_ID = "",
		std::string stock_ID = "",
		std::string count = "",
		std::string date = ""
	)
		: goods_ID(std::move(goods_ID)),
		stock_ID(std::move(stock_ID)),
		count(std::move(count)),
		date(std::move(date))
	{
	}
};
struct order
{
	std::string ProductID;//下单的商品ID
	std::string count;//下单的商品个数
	std::string customerID;//下单的顾客ID
};
struct search {
	std::string ProductID;     // 可忽略
	std::string prise;         // 价格区间（"小于10元"、"10元到50元"、"50元到100元"、"大于100元"）
	std::string category;      // 商品种类
	std::string name;          // 商品名称（模糊匹配）
	bool re;                   // 是否价格逆序
};
std::string toLower(const std::string& input);
class SqlTools
{
public:
	
	//用户登录界面
	static int Login_Account_Password_Check(const std::string& account, const std::string& password);
	static int Login_Email_Code_Check(const std::string& email, const std::string& code);//李帅彤实现
	static std::pair<std::string, std::string> Login_Get_Account_Password(const std::string& email, const std::string& code);
	static bool Account_Exist(const std::string& account, const std::string& password);
	static bool Register_Account_Password_Check(const std::string& account, const std::string& password);
	static bool Register_Email_Code_Check(const std::string& email, const std::string& code);
	static bool Register_Account_Password_Email_Wirte(const std::string& account, const std::string& password, const std::string& email);

	//收银员
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
		const int& offset = 0,
		bool reverse_order_ID = false,
		bool reverse_customer_ID = false,
		bool reverse_date = false,
		bool reverse_price = false,
		bool reverse_state = false
	);
	static bool Change_OrderTable_State(const std::vector<OrderDetail>& changeData,const std::string& account);
	static SalesDetail Search_Sale_Information(const std::string& account, const std::string& password);

	//管理员
	static bool Stock_Goods(const std::string& id, const std::string& counts);
	static bool Add_Goods
	(
		const std::string& path,
		const std::string& id,
		const std::string& name,
		const std::string& price,
		const std::string& category,
		const std::string& count,
		const std::string& description
	);
	static bool Delete_Goods(const std::string& id);


	//4
	static std::pair<std::vector<SaleTableLine>, int> Search_SaleTable_State_Price_ID(
		const std::string& kind,
		const std::string& price,
		const std::string& id,
		const int& return_count,
		const int& offset,
		bool reverse_sale_ID,
		bool reverse_order_ID,
		bool reverse_goods_ID,
		bool reverse_count,
		bool reverse_date
	);
	static bool Change_SaleTable(const std::vector<SaleTableLine>& changeData);
	static bool Add_Sale(const SaleTableLine& addData);//不可调用
	static bool Delete_Sale(const std::string& id);
	//5
	static std::pair<std::vector<StockDetail>, int> Search_StockTable_State_Price_ID(
		const std::string& kind,
		const std::string& price,
		const std::string& id,
		const int& return_count,
		const int& offset,
		bool reverse_goods_ID,
		bool reverse_stock_ID,
		bool reverse_count,
		bool reverse_date);
	static bool Change_StockTable_State(const std::vector<StockDetail>& changeData);//已经删除，不然无法实现
	static bool Add_Stock(const StockDetail& addData);
	static bool Delete_Stock(const std::string& id);
	//6
	static std::pair<std::vector<StaffDetail>, int> Search_StaffTable_Role_Name(
		const std::string& role,
		const std::string& name,
		const int& return_count,
		const int& offset,
		bool reverse_staff_ID,
		bool reverse_name,
		bool reverse_email,
		bool reverse_password,
		bool reverse_join_date,
		bool reverse_role
	);
	static bool Change_StaffTable(const std::vector<StaffDetail>& changeData);
	static bool Add_Staff(const StaffDetail& addData);
	static bool Delete_Staff(const std::string& id);

	static const QVector<QPair<QDateTime, double>> Get_Income_Date
	(
		const std::string& start_date = " ",
		const std::string& end_date = " "
	);

	static bool Add_VerificationCode(const std::string& email, const std::string& code);


	//客户管理
	static std::pair<std::vector<CustomerDetail>, int> Search_CustomerTable_Name//仅可生日和注册日期逆序
	(
		const std::string& name = " ",
		const int& return_count = 9,
		const int& offset = 0,
		bool reverse_birth_date = false,
		bool reverse_register_date = false
	);
	static bool Change_CustomerTable(const std::vector<CustomerDetail>& changeData);
	static bool Add_Customer(const CustomerDetail& customer);
	static bool Delete_Customer(const std::string& id);
	//下单
	static bool Create_Order_From_Html(const order& fromHt);
	static bool CustormerLogin(const std::string& email, const std::string& password);
	std::pair<std::vector<ProductDetail>, int> SearchProductTable_BySearchStruct(const search& s);

};

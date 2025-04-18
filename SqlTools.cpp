#include "SqlTools.h"

int SqlTools::Login_Account_Password_Check(const std::string& account, const std::string& password)
{
	return 2;
}

bool SqlTools::Account_Exist(const std::string& account, const std::string& password)
{
	return false;
}

int SqlTools::Login_Email_Code_Check(const std::string& email, const std::string& code)
{
	return 1;
}

std::pair<std::string, std::string> SqlTools::Login_Get_Account_Password(const std::string& email, const std::string& code)
{
	return std::pair<std::string, std::string>();
}

bool SqlTools::Register_Account_Password_Check(const std::string& account, const std::string& password)
{
	return false;
}

bool SqlTools::Register_Email_Code_Check(const std::string& email, const std::string& code)
{
	return false;
}

bool SqlTools::Register_Account_Password_Email_Wirte(const std::string& account, const std::string& password, const std::string& email)
{
	return false;
}



std::pair<std::vector<ProductDetail>, int> SqlTools::SearchProductTable_Kind_Price_Name
(
	const std::string& kind,
	const std::string& price,
	const std::string& name,
	const int& return_count,
	const int& offset
	)
{
	return std::pair<std::vector<ProductDetail>, int>();
}

std::pair<std::vector<OrderDetail>, int> SqlTools::SearchOrderTable_State_Price_ID
(
	const std::string& kind,
	const std::string& price,
	const std::string& id,
	const int& return_count,
	const int& offset,
	bool reverse_order_ID,
	bool reverse_customer_ID,
	bool reverse_date,
	bool reverse_price,
	bool reverse_state
)
{
	return std::pair<std::vector<OrderDetail>, int>();
}

bool SqlTools::Change_OrderTable_State(const std::vector<OrderDetail>& changeData)
{
	return false;
}

SalesDetail SqlTools::Search_Sale_Information(const std::string& account, const std::string& password)
{
	return SalesDetail();
}

bool SqlTools::Stock_Goods(const std::string& id, const std::string& counts)
{
	return false;
}

bool SqlTools::Add_Goods(const std::string& path, const std::string& id, const std::string& name, const std::string& price, const std::string& category, const std::string& count, const std::string& description)
{
	return false;
}

bool SqlTools::Delete_Goods(const std::string& id)
{
	return false;
}

std::pair<std::vector<SaleTableLine>, int> SqlTools::Search_SaleTable_State_Price_ID(const std::string& kind, const std::string& price, const std::string& id, const int& return_count, const int& offset, bool reverse_sale_ID, bool reverse_order_ID, bool reverse_goods_ID, bool reverse_count, bool reverse_date)
{
	return std::pair<std::vector<SaleTableLine>, int>();
}

bool SqlTools::Change_SaleTable(const std::vector<SaleTableLine>& changeData)
{
	return false;
}

bool SqlTools::Add_Sale(const SaleTableLine& addData)
{
	return false;
}

bool SqlTools::Delete_Sale(const std::string& id)
{
	return false;
}

std::pair<std::vector<StockDetail>, int> SqlTools::Search_StockTable_State_Price_ID(const std::string& kind, const std::string& price, const std::string& id, const int& return_count, const int& offset, bool reverse_goods_ID, bool reverse_stock_ID, bool reverse_count, bool reverse_date)
{
	return std::pair<std::vector<StockDetail>, int>();
}

bool SqlTools::Change_StockTable_State(const std::vector<StockDetail>& changeData)
{
	return false;
}

bool SqlTools::Add_Stock(const StockDetail& addData)
{
	return false;
}

bool SqlTools::Delete_Stock(const std::string& id)
{
	return false;
}

std::pair<std::vector<StaffDetail>, int> SqlTools::Search_StaffTable_Role_Name(const std::string& role, const std::string& name, const int& return_count, const int& offset, bool reverse_staff_ID, bool reverse_name, bool reverse_email, bool reverse_password, bool reverse_join_date, bool reverse_role)
{
	return std::pair<std::vector<StaffDetail>, int>();
}

bool SqlTools::Change_StaffTable(const std::vector<StaffDetail>& changeData)
{
	return false;
}

bool SqlTools::Add_Staff(const StaffDetail& addData)
{
	return false;
}

bool SqlTools::Delete_Staff(const std::string& id)
{
	return false;
}

std::pair<std::vector<CustomerDetail>, int> SqlTools::Search_CustomerTable_Name(const std::string& name, const int& return_count, const int& offset, bool reverse_profile_picture, bool reverse_customer_ID, bool reverse_birth_date, bool reverse_note, bool reverse_register_date, bool reverse_email, bool reverse_password)
{
	return std::pair<std::vector<CustomerDetail>, int>();
}

bool SqlTools::Change_CustomerTable(const std::vector<CustomerDetail>& changeData)
{
	return false;
}

bool SqlTools::Add_Customer(const CustomerDetail& addData)
{
	return false;
}

bool SqlTools::Delete_Customer(const std::string& id)
{
	return false;
}

const QVector<QPair<QDateTime, double>> SqlTools::Get_Income_Date(const std::string& start_date, const std::string& end_date)
{
	return QVector<QPair<QDateTime, double>>();
}

const QVector<QPair<QDateTime, double>> SqlTools::Get_Expense_Date(const std::string& start_date, const std::string& end_date)
{
	return QVector<QPair<QDateTime, double>>();
}

//std::pair<std::vector<ProductRecord>, int> SqlTools::SearchProductTable(int offset, int limit)
//{
//    std::vector<ProductRecord> records;
//    int totalCount = 0;
//
//    // 执行分页查询
//    QSqlQuery query;
//    query.prepare("SELECT * FROM products LIMIT :limit OFFSET :offset");
//    query.bindValue(":limit", limit);
//    query.bindValue(":offset", offset);
//
//    if (query.exec()) {
//        while (query.next()) {
//            ProductRecord record;
//            record.ID = query.value("id").toString().toStdString();
//            record.name = query.value("name").toString().toStdString();
//            record.price = query.value("price").toString().toStdString();
//            record.category = query.value("category").toString().toStdString();
//            record.count = query.value("count").toString().toStdString();
//            record.description = query.value("description").toString().toStdString();
//            record.path = query.value("image_path").toString().toStdString();
//            records.push_back(record);
//        }
//    }
//
//    // 获取总商品数量
//    if (query.exec("SELECT COUNT(*) FROM products")) {
//        if (query.next()) {
//            totalCount = query.value(0).toInt();
//        }
//    }
//
//    return { records, totalCount };
//}

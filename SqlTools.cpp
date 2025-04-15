#include "SqlTools.h"

int SqlTools::Login_Account_Password_Check(const std::string& account, const std::string& password)
{
	return 1;
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
	const int& offset
)
{
	return std::pair<std::vector<OrderDetail>, int>();
}

SalesDetail SqlTools::Search_Sale_Information(const std::string& account, const std::string& password)
{
	return SalesDetail();
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

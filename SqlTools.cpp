#include"SqlTools.h"
#include"OracleDB.h"
#include<string>
#include<algorithm>

std::string toLower(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}
//登录界面
int SqlTools::Login_Account_Password_Check(const std::string& account, const std::string& password) {
    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();
        if (!conn) {
            std::cerr << "Database connection is NULL!" << std::endl;
            return -1;
        }

        std::string sql = "SELECT Role FROM EMPLOYEE WHERE email = :1 AND password = :2";
        oracle::occi::Statement* stmt = conn->createStatement(sql);
        stmt->setString(1, account);
        stmt->setString(2, password);
        oracle::occi::ResultSet* rs = stmt->executeQuery();

        int result = 0;
        if (rs->next()) {
            std::string role = rs->getString(1);
            if (role == "Admister") {
                result = 1;
            }
            else if (role == "Normal") {
                result = 2;
            }
        }

        stmt->closeResultSet(rs);
        conn->terminateStatement(stmt);
        return result;
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "Login_Account_Password_Check error: " << e.getMessage() << std::endl;
        return -1;
    }
}
//完成
int SqlTools::Login_Email_Code_Check(const std::string& email, const std::string& code) 
{
   
    int result = -1;

    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();
        oracle::occi::Statement* stmt = nullptr;
        oracle::occi::ResultSet* res = nullptr;
        // 合并查询：验证邮箱和验证码，并获取角色
        std::string sql =
            "SELECT e.Role "
            "FROM Employee e "
            "JOIN Email_Verification ev ON e.Email = ev.email "
            "WHERE ev.email = :email "
            "AND ev.code = :code "
            "AND ev.expire_time > SYSTIMESTAMP";

        stmt = conn->createStatement(sql);
        stmt->setString(1, email);
        stmt->setString(2, code);

        res = stmt->executeQuery();

        if (res->next()) {
            std::string role = res->getString(1);
            if (role == "Admister") 
            { // 确认拼写是否正确
                result = 1;
            }
            else if (role == "Normal") 
            {
                result = 2;
            }
        }

        delete res;
        delete stmt;

    }
    catch (oracle::occi::SQLException& e) {
        // 处理SQL异常
        qDebug() << "SQL Error:" << e.getMessage();
        // 根据需要可以设置一个错误码或抛出异常
    }

    return result;
}
bool SqlTools::Register_Account_Password_Check(const std::string& account, const std::string& password)
{
    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();
        std::string check_sql = "SELECT COUNT(*) FROM Employee WHERE Email = :1";
        oracle::occi::Statement* check_stmt = conn->createStatement(check_sql);
        check_stmt->setString(1, account);
        oracle::occi::ResultSet* rs = check_stmt->executeQuery();
        rs->next();
        int count = rs->getInt(1);
        conn->terminateStatement(check_stmt);
        if (count > 0) {
            return false;
        }
       
        return true;
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "SQL Error: " << e.getMessage() << std::endl;
        return false;
    }
}
std::pair<std::string, std::string> SqlTools::Login_Get_Account_Password(const std::string& email, const std::string& code)
{
    oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();
    oracle::occi::Statement* stmt = nullptr;
    oracle::occi::ResultSet* res = nullptr;
    std::pair<std::string, std::string> result;

    try {
        // 准备 SQL 查询语句，根据 email 获取 Email 和 Password
        std::string sql = "SELECT Email, Password FROM Employee WHERE Email = :email";

        stmt = conn->createStatement(sql);
        stmt->setString(1, email);

        res = stmt->executeQuery();

        if (res->next()) {
            std::string empEmail = res->getString(1);
            std::string empPassword = res->getString(2);
            result = std::make_pair(empEmail, empPassword);
        }

        // 清理资源
        stmt->closeResultSet(res);
        conn->terminateStatement(stmt);
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "登录查询失败: " << e.what() << std::endl;
        // 根据需要，可以选择抛出异常或返回特定的错误对
        result = std::make_pair("", "");
    }

    // 注意：在实际应用中，连接的释放应在更高层级管理，避免在函数内关闭连接
    // 这里假设连接由外部管理

    return result;
};
bool SqlTools::Account_Exist(const std::string& account, const std::string& password)
{
    return true;//所有写入都直接完成，退出也不需要数据库
};
bool SqlTools::Register_Email_Code_Check(const std::string& email, const std::string& code)
{
    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();

        // 查询SQL，检查email和code是否匹配且未过期
        std::string query_sql = R"(
            SELECT 1 FROM Email_Verification 
            WHERE email = :1 
            AND code = :2 
            AND expire_time > SYSTIMESTAMP
        )";

        oracle::occi::Statement* stmt = conn->createStatement(query_sql);
        stmt->setString(1, email);
        stmt->setString(2, code);

        oracle::occi::ResultSet* rs = stmt->executeQuery();
        bool exists = rs->next(); // 如果有结果返回true

        stmt->closeResultSet(rs);
        conn->terminateStatement(stmt);

        return exists;
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "验证邮箱验证码失败: " << e.getMessage() << std::endl;
        return false;
    }
}
bool SqlTools::Register_Account_Password_Email_Wirte(const std::string& account, const std::string& password, const std::string& email)
{
    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();

        // 3. 插入新员工记录
        std::string insert_sql = R"(
            INSERT INTO Employee (EmployeeID, Name, Email, Password, Role)
            VALUES (:1, :2, :3, :4, 'Normal')
        )";

        oracle::occi::Statement* insert_stmt = conn->createStatement(insert_sql);
        insert_stmt->setString(1, account);
        insert_stmt->setString(2, account);  // 假设Name和Account相同，可根据需求修改
        insert_stmt->setString(3, email);
        insert_stmt->setString(4, password);

        int rows = insert_stmt->executeUpdate();
        conn->commit();
        conn->terminateStatement(insert_stmt);

        return rows > 0;
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "注册账号失败: " << e.getMessage() << std::endl;
        return false;
    }
};

//收银界面SQL
//和顾客查看重用

std::pair<std::vector<ProductDetail>, int> SqlTools::SearchProductTable_Kind_Price_Name(
    const std::string& kind,
    const std::string& price,
    const std::string& name,
    const int& return_count,
    const int& offset)
{
    std::vector<ProductDetail> results;
    int total_count = 0;
    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();
        std::string base_sql =
            "SELECT * FROM ("
            "  SELECT p.*, ROWNUM rn FROM ("
            "    SELECT * FROM PRODUCT WHERE 1=1";

        if (kind != "" && kind != "all")
            base_sql += " AND Category = :kind";
        if (price != "") {
            if (price == "less10")
                base_sql += " AND Price < 10";
            else if (price == "10to50")
                base_sql += " AND Price >= 10 AND Price <= 50";
            else if (price == "50to100")
                base_sql += " AND Price > 50 AND Price <= 100";
            else if (price == "than100")
                base_sql += " AND Price > 100";
        }
        if (name != "")
            base_sql += " AND TO_CHAR(ProductID) LIKE :id";

        base_sql += " ORDER BY ProductID ) p WHERE ROWNUM <= :end_row"
            ") WHERE rn > :start_row";

        std::string count_sql = "SELECT COUNT(*) FROM Product WHERE 1=1";
        if (kind != "" && kind != "all")
            count_sql += " AND Category = :kind";
        if (price != "") {
            if (price == "less10")
                count_sql += " AND Price < 10";
            else if (price == "10to50")
                count_sql += " AND Price >= 10 AND Price <= 50";
            else if (price == "50to100")
                count_sql += " AND Price > 50 AND Price <= 100";
            else if (price == "than100")
                count_sql += " AND Price > 100";
        }
        if (name != "")
            count_sql += " AND TO_CHAR(ProductID) LIKE :id";

        auto* count_stmt = conn->createStatement(count_sql);
        int bind_index = 1;
        if (kind != "" && kind != "all")
            count_stmt->setString(bind_index++, kind);
        if (name != "")
            count_stmt->setString(bind_index++, "%" + name + "%");

        auto* count_rs = count_stmt->executeQuery();
        if (count_rs->next()) {
            total_count = count_rs->getInt(1);
        }
        conn->terminateStatement(count_stmt);

        auto* stmt = conn->createStatement(base_sql);
        bind_index = 1;
        if (kind != "" && kind != "all")
            stmt->setString(bind_index++, kind);
        if (name != "")
            stmt->setString(bind_index++, "%" + name + "%");

        int end_row = offset + return_count;
        int start_row = offset;

        stmt->setInt(bind_index++, end_row);
        stmt->setInt(bind_index++, start_row);

        auto* rs = stmt->executeQuery();
        while (rs->next()) {
            ProductDetail pd;
            pd.ID = std::to_string(rs->getInt(1));
            pd.name = rs->getString(2);
            pd.category = rs->getString(3);
            pd.price = std::to_string(rs->getFloat(4));
            pd.count = std::to_string(rs->getInt(5));
            pd.description = rs->getString(6);
            pd.path = rs->getString(7);
            results.emplace_back(std::move(pd));
        }
        conn->terminateStatement(stmt);
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "SearchProductTable_Kind_Price_ID SQL Error: " << e.getMessage() << std::endl;
    }
    return { results, total_count };
}



std::pair<std::vector<OrderDetail>, int>SqlTools::SearchOrderTable_State_Price_ID(
    const std::string& kind,
    const std::string& price,
    const std::string& id,
    const int& return_count,
    const int& offset,
    bool reverse_order_ID,
    bool reverse_customer_ID,
    bool reverse_date,
    bool reverse_price,
    bool reverse_state)
{
    std::vector<OrderDetail> results;
    int total_count = 0;

    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();

        // ========== 1. 构建分页查询 SQL ==========
        std::string base_sql =
            "SELECT * FROM ("
            "  SELECT o.*, ROWNUM rn FROM ("
            "    SELECT * FROM Orders WHERE 1=1";

        if (kind != "" && kind != "all")
            base_sql += " AND Status = :status";

        if (price != "") {
            if (price == "less10")
                base_sql += " AND TotalAmount < 10";
            else if (price == "10to50")
                base_sql += " AND TotalAmount >= 10 AND TotalAmount < 50";
            else if (price == "50to100")
                base_sql += " AND TotalAmount >= 50 AND TotalAmount <= 100";
            else if (price == "than100")
                base_sql += " AND TotalAmount > 100";
        }

        if (id != "")
            base_sql += " AND OrderID LIKE :id";

        base_sql += " ORDER BY ";
        if (reverse_order_ID)
            base_sql += "OrderID DESC";
        else if (reverse_customer_ID)
            base_sql += "CustomerID DESC";
        else if (reverse_date)
            base_sql += "OrderTime DESC";
        else if (reverse_price)
            base_sql += "TotalAmount DESC";
        else if (reverse_state)
            base_sql += "Status DESC";
        else
            base_sql += "OrderID ASC";

        base_sql += ") o WHERE ROWNUM <= :end_row) WHERE rn > :start_row";
        std::string count_sql = "SELECT COUNT(*) FROM Orders WHERE 1=1";
        if (kind != "" && kind != "all")
            count_sql += " AND Status = :status";

        if (price != "") {
            if (price == "less10")
                count_sql += " AND TotalAmount < 10";
            else if (price == "10to50")
                count_sql += " AND TotalAmount >= 10 AND TotalAmount < 50";
            else if (price == "50to100")
                count_sql += " AND TotalAmount >= 50 AND TotalAmount <= 100";
            else if (price == "than100")
                count_sql += " AND TotalAmount > 100";
        }

        if (id != "")
            count_sql += " AND OrderID LIKE :id";
        auto* count_stmt = conn->createStatement(count_sql);
        int count_index = 1;
        if (kind != "" && kind != "all")
            count_stmt->setString(count_index++, kind);
        if (id != "")
            count_stmt->setString(count_index++, "%" + id + "%");
        std::cout << count_sql << std::endl;
        auto* count_rs = count_stmt->executeQuery();
        if (count_rs->next()) {
            total_count = count_rs->getInt(1);
        }
        conn->terminateStatement(count_stmt);
        auto* stmt = conn->createStatement(base_sql);
        int bind_index = 1;
        if (kind != "" && kind != "all")
            stmt->setString(bind_index++, kind);
        if (id != "")
            stmt->setString(bind_index++, "%" + id + "%");

        stmt->setInt(bind_index++, offset + return_count);  // end_row
        stmt->setInt(bind_index++, offset);                // start_row

        auto* rs = stmt->executeQuery();
        while (rs->next()) {
            OrderDetail od;
            od.order_ID = std::to_string(rs->getInt(1));
            od.customer_ID = rs->getString(2);
            od.date = rs->getString(3);
            od.price = std::to_string(rs->getFloat(4));
            od.state = rs->getString(5);
            results.push_back(std::move(od));
        }
        conn->terminateStatement(stmt);
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "SearchOrderTable_State_Price_ID SQL Error: " << e.getMessage() << std::endl;
    }

    return { results, total_count };
}

bool SqlTools::Change_OrderTable_State(const std::vector<OrderDetail>& changeData,const std::string & account)//SalesRecordID系统自动填充
{
    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();
        for (const auto& order : changeData) {
            std::cout << "!!!" << order.order_ID << std::endl;
            std::string check_sql = "SELECT Status FROM ORDERS WHERE ORDERID = :1";
            oracle::occi::Statement* check_stmt = conn->createStatement(check_sql);
            check_stmt->setString(1, order.order_ID);
            oracle::occi::ResultSet* rs = check_stmt->executeQuery();
            if (!rs->next()) {
                conn->terminateStatement(check_stmt);
                return false;
            }
            std::string current_state = rs->getString(1);
            conn->terminateStatement(check_stmt);
            if (current_state != "未处理") {
                return false; 
            }
            std::string update_sql = "UPDATE ORDERS SET STATUS = '已出库' WHERE ORDERID = :1";
            oracle::occi::Statement* update_stmt = conn->createStatement(update_sql);
            update_stmt->setString(1, order.order_ID);
            update_stmt->executeUpdate();
            conn->terminateStatement(update_stmt);
            std::string get_time_sql = "SELECT OrderTime FROM Orders WHERE OrderID = :1";
            oracle::occi::Statement* time_stmt = conn->createStatement(get_time_sql);
            time_stmt->setString(1, order.order_ID);
            rs = time_stmt->executeQuery();
            oracle::occi::Timestamp order_time;
            if (rs->next()) {
                order_time = rs->getTimestamp(1);
            }
            conn->terminateStatement(time_stmt);
            std::string id_sql = "SELECT NVL(MAX(SalesRecordID), 0) + 1 FROM SalesRecord";
            oracle::occi::Statement* id_stmt = conn->createStatement(id_sql);
            rs = id_stmt->executeQuery();
            int new_sales_id = 1;
            if (rs->next()) {
                new_sales_id = rs->getInt(1);
            }
            conn->terminateStatement(id_stmt);

            std::string insert_sales_sql = R"(
                INSERT INTO SalesRecord (SalesRecordID, OrderID, EmployeeID, OrderTime, SalesTime, TotalAmount)
                VALUES (:1, :2, :3, :4, SYSTIMESTAMP, :5)
            )";
            oracle::occi::Statement* insert_stmt = conn->createStatement(insert_sales_sql);
            insert_stmt->setInt(1, new_sales_id);
            insert_stmt->setString(2, order.order_ID);
            insert_stmt->setString(3, account); // 员工账号
            insert_stmt->setTimestamp(4, order_time);
            insert_stmt->setInt(5, std::stoi(order.price));
            insert_stmt->executeUpdate();
            conn->terminateStatement(insert_stmt);
        }
        conn->commit();
        return true;
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "SQL Error: " << e.getMessage() << std::endl;
        return false;
    }
}
SalesDetail SqlTools::Search_Sale_Information(const std::string& account, const std::string& password)
{
    SalesDetail result;

    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();
        std::string amount_sql = R"(
            SELECT NVL(SUM(TotalAmount), 0)
            FROM SalesRecord
            WHERE EmployeeID = :1 AND TRUNC(SalesTime) = TRUNC(SYSDATE)
        )";
        oracle::occi::Statement* amount_stmt = conn->createStatement(amount_sql);
        amount_stmt->setString(1, account);
        oracle::occi::ResultSet* rs = amount_stmt->executeQuery();

        if (rs->next()) {
            result.max_sales_amount = std::to_string(rs->getDouble(1));
        }
        conn->terminateStatement(amount_stmt);

        std::string count_sql = R"(
            SELECT NVL(SUM(OI.Quantity), 0)
            FROM SalesRecord SR
            JOIN OrderItem OI ON SR.OrderID = OI.OrderID
            WHERE SR.EmployeeID = :1 AND TRUNC(SR.SalesTime) = TRUNC(SYSDATE)
        )";
        oracle::occi::Statement* count_stmt = conn->createStatement(count_sql);
        count_stmt->setString(1, account);
        rs = count_stmt->executeQuery();

        if (rs->next()) {
            result.max_sales_count = std::to_string(rs->getInt(1));
        }
        conn->terminateStatement(count_stmt);

        std::string top_sql = R"(
            SELECT * FROM (
            SELECT OI.ProductID
            FROM SalesRecord SR
            JOIN OrderItem OI ON SR.OrderID = OI.OrderID
            WHERE SR.EmployeeID = :1 AND TRUNC(SR.SalesTime) = TRUNC(SYSDATE)
            GROUP BY OI.ProductID
            ORDER BY SUM(OI.Quantity * OI.Price) DESC
            ) WHERE ROWNUM = 1
        )";
        oracle::occi::Statement* top_stmt = conn->createStatement(top_sql);
        top_stmt->setString(1, account);
        rs = top_stmt->executeQuery();

        std::string top_product_id;
        if (rs->next()) {
            top_product_id = rs->getString(1);
        }
        conn->terminateStatement(top_stmt);

        if (!top_product_id.empty()) {
            // Step 5: 查询对应商品详细信息
            std::string product_sql = R"(
                SELECT ProductID, Name, Category, Price, StockQuantity, Description, Path
                FROM Product WHERE ProductID = :1
            )";
            oracle::occi::Statement* product_stmt = conn->createStatement(product_sql);
            product_stmt->setString(1, top_product_id);
            rs = product_stmt->executeQuery();

            if (rs->next()) {
                ProductDetail detail;
                detail.ID = std::to_string(rs->getInt(1));
                detail.name = rs->getString(2);
                detail.category = rs->getString(3);

                float raw_price = rs->getFloat(4);
                if (raw_price < 10)
                    detail.price = "less10";
                else if (raw_price >= 10 && raw_price <= 50)
                    detail.price = "10to50";
                else if (raw_price > 50 && raw_price <= 100)
                    detail.price = "50to100";
                else
                    detail.price = "than100";

                detail.count = std::to_string(rs->getInt(5));
                detail.description = rs->getString(6);
                detail.path = rs->getString(7);
                result.product_detail = detail;
            }
            conn->terminateStatement(product_stmt);
        }
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "SQL Error: " << e.getMessage() << std::endl;
    }

    return result;
}

//管理员管理商品
bool SqlTools::Stock_Goods(const std::string& id, const std::string& counts)//默认商品已经存在，RestockID，RestockTime系统自动填充
{
    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();
        std::string insert_sql = R"(
            INSERT INTO RestockRecord 
            (RecordID, ProductID, Quantity, RestockTime)
            VALUES (RestockRecord_SEQ.NEXTVAL, :1, :2, CURRENT_TIMESTAMP)
        )";
        oracle::occi::Statement* stmt = conn->createStatement(insert_sql);
        stmt->setInt(1, std::stoi(id));
        stmt->setInt(2, std::stoi(counts));
        stmt->executeUpdate();
        conn->commit();
        conn->terminateStatement(stmt);
        return true;
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "进货失败: " << e.getMessage() << std::endl;
        return false;
    }
}

bool SqlTools::Add_Goods(
    const std::string& path,
    const std::string& id,
    const std::string& name,
    const std::string& price,
    const std::string& category,
    const std::string& count,
    const std::string& description)
{
    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();
        std::string insert_product_sql = R"(
            INSERT INTO Product 
            (ProductID, Name, Category, Price, StockQuantity, Description, PATH)
            VALUES (SEQ_PRODUCT_ID.NEXTVAL, :1, :2, :3, :4, :5, :6)
        )";
        oracle::occi::Statement* stmt = conn->createStatement(insert_product_sql);
        stmt->setString(1, name);
        stmt->setString(2, category);
        stmt->setFloat(3, std::stof(price));
        stmt->setInt(4, 0); // 初始库存为0
        stmt->setString(5, description);
        stmt->setString(6, path);
        stmt->executeUpdate();
        conn->terminateStatement(stmt);
        std::string get_id_sql = "SELECT SEQ_PRODUCT_ID.CURRVAL FROM dual";
        stmt = conn->createStatement(get_id_sql);
        auto* rs = stmt->executeQuery();
        int product_id = 0;
        if (rs->next()) {
            product_id = rs->getInt(1);
        }
        conn->terminateStatement(stmt);
        std::string insert_restock_sql = R"(
            INSERT INTO RestockRecord 
            (RecordID, ProductID, Quantity, RestockTime)
            VALUES (RestockRecord_SEQ.NEXTVAL, :1, :2, CURRENT_TIMESTAMP)
        )";
        stmt = conn->createStatement(insert_restock_sql);
        stmt->setInt(1, product_id);
        stmt->setInt(2, std::stoi(count));  // 实际进货数量
        stmt->executeUpdate();
        conn->terminateStatement(stmt);
        conn->commit();

        return true;
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "添加商品失败: " << e.getMessage() << std::endl;
        return false;
    }
}
bool SqlTools::Delete_Goods(const std::string& id)
{
    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();
        std::string delete_sql = R"(
            DELETE FROM Product WHERE ProductID = :1
        )";
        oracle::occi::Statement* stmt = conn->createStatement(delete_sql);
        stmt->setString(1, id);
        int rows = stmt->executeUpdate();
        conn->commit();
        conn->terminateStatement(stmt);
        return rows > 0;
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "删除商品失败: " << e.getMessage() << std::endl;
        return false;
    }
}

//管理员管理销售记录

std::pair<std::vector<SaleTableLine>, int> SqlTools::Search_SaleTable_State_Price_ID(
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
) {
    std::vector<SaleTableLine> results;
    int total_count = 0;
    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();
        std::string base_sql =
            "SELECT * FROM ( "
            "  SELECT t.*, ROWNUM rn FROM ( "
            "    SELECT sr.SalesRecordID, sr.OrderID, oi.ProductID, oi.Quantity, sr.SalesTime "
            "    FROM SalesRecord sr "
            "    JOIN OrderItem oi ON sr.OrderID = oi.OrderID "
            "    JOIN Product p ON oi.ProductID = p.ProductID "
            "    WHERE 1=1 ";
        if (kind != "" && kind != "all")
            base_sql += " AND p.Category = :kind ";
        if (price != "") {
            if (price == "less10")
                base_sql += " AND oi.Price < 10 ";
            else if (price == "10to50")
                base_sql += " AND oi.Price >= 10 AND oi.Price <= 50 ";
            else if (price == "50to100")
                base_sql += " AND oi.Price > 50 AND oi.Price <= 100 ";
            else if (price == "than100")
                base_sql += " AND oi.Price > 100 ";
        }
        if (id != "")
            base_sql += " AND TO_CHAR(sr.SalesRecordID) LIKE :id ";
        std::vector<std::pair<std::string, bool>> order_fields = {
            {"sr.SalesRecordID", reverse_sale_ID},
            {"sr.OrderID", reverse_order_ID},
            {"oi.ProductID", reverse_goods_ID},
            {"oi.Quantity", reverse_count},
            {"sr.SalesTime", reverse_date}
        };
        base_sql += " ORDER BY ";
        for (size_t i = 0; i < order_fields.size(); ++i) {
            base_sql += order_fields[i].first + (order_fields[i].second ? " DESC" : " ASC");
            if (i != order_fields.size() - 1)
                base_sql += ", ";
        }
        base_sql += " ) t WHERE ROWNUM <= :end_row "
            ") WHERE rn > :start_row";
        std::string count_sql =
            "SELECT COUNT(*) FROM SalesRecord sr "
            "JOIN OrderItem oi ON sr.OrderID = oi.OrderID "
            "JOIN Product p ON oi.ProductID = p.ProductID WHERE 1=1 ";
        if (kind != "" && kind != "all")
            count_sql += " AND p.Category = :kind ";
        if (price != "") {
            if (price == "less10")
                count_sql += " AND oi.Price < 10 ";
            else if (price == "10to50")
                count_sql += " AND oi.Price >= 10 AND oi.Price <= 50 ";
            else if (price == "50to100")
                count_sql += " AND oi.Price > 50 AND oi.Price <= 100 ";
            else if (price == "than100")
                count_sql += " AND oi.Price > 100 ";
        }
        if (id != "")
            count_sql += " AND TO_CHAR(sr.SalesRecordID) LIKE :id ";
        auto* count_stmt = conn->createStatement(count_sql);
        int bind_index = 1;
        if (kind != "" && kind != "all")
            count_stmt->setString(bind_index++, kind);
        if (id != "")
            count_stmt->setString(bind_index++, "%" + id + "%");

        auto* count_rs = count_stmt->executeQuery();
        if (count_rs->next()) {
            total_count = count_rs->getInt(1);
        }
        conn->terminateStatement(count_stmt);
        auto* stmt = conn->createStatement(base_sql);
        bind_index = 1;
        if (kind != "" && kind != "all")
            stmt->setString(bind_index++, kind);
        if (id != "")
            stmt->setString(bind_index++, "%" + id + "%");

        int end_row = offset + return_count;
        stmt->setInt(bind_index++, end_row);
        stmt->setInt(bind_index++, offset);

        auto* rs = stmt->executeQuery();
        while (rs->next()) {
            SaleTableLine line;
            line.sale_ID = std::to_string(rs->getInt(1));
            line.order_ID = std::to_string(rs->getInt(2));
            line.goods_ID = std::to_string(rs->getInt(3));
            line.count = std::to_string(rs->getInt(4));
            line.date = (rs->getTimestamp(5)).toText("YYYY-MM-DD HH24:MI:SS",0,"");
            results.emplace_back(std::move(line));
        }
        conn->terminateStatement(stmt);
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "Search_SaleTable_State_Price_ID SQL Error: " << e.getMessage() << std::endl;
    }
    return { results, total_count };
}
// 只能修改时间
bool SqlTools::Change_SaleTable(const std::vector<SaleTableLine>& changeData)
{
    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();

        for (const auto& data : changeData) {
            std::string check_sql = "SELECT COUNT(*) FROM SalesRecord WHERE SalesRecordID = :1";
            auto* check_stmt = conn->createStatement(check_sql);
            check_stmt->setInt(1, std::stoi(data.sale_ID));
            auto* rs = check_stmt->executeQuery();

            if (rs->next() && rs->getInt(1) == 0) {
                std::cerr << "销售记录 ID " << data.sale_ID << " 不存在，跳过修改。" << std::endl;
                conn->terminateStatement(check_stmt);
                continue;
            }
            conn->terminateStatement(check_stmt);
            std::string update_sql =
                "UPDATE SalesRecord SET SalesTime = TO_TIMESTAMP(:1, 'YYYY-MM-DD HH24:MI:SS') "
                "WHERE SalesRecordID = :2";
            auto* update_stmt = conn->createStatement(update_sql);
            update_stmt->setString(1, data.date);  // 格式需为 "YYYY-MM-DD HH24:MI:SS"
            update_stmt->setInt(2, std::stoi(data.sale_ID));
            update_stmt->executeUpdate();
            conn->terminateStatement(update_stmt);
        }

        conn->commit();
        return true;
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "Change_SaleTable SQL 错误: " << e.getMessage() << std::endl;
        return false;
    }
}

//系统自动完成，不能修改,调用将返回false
bool SqlTools::Add_Sale(const SaleTableLine& addData) { return false; }

bool SqlTools::Delete_Sale(const std::string& id)
{
    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();

        std::string check_sql = "SELECT COUNT(*) FROM SalesRecord WHERE SalesRecordID = :1";
        oracle::occi::Statement* check_stmt = conn->createStatement(check_sql);
        check_stmt->setInt(1, std::stoi(id));
        auto* check_rs = check_stmt->executeQuery();

        if (check_rs->next() && check_rs->getInt(1) == 0) {
            std::cerr << "销售记录不存在，无法删除。" << std::endl;
            conn->terminateStatement(check_stmt);
            return false;
        }

        conn->terminateStatement(check_stmt);

        std::string delete_sql = "DELETE FROM SalesRecord WHERE SalesRecordID = :1";
        oracle::occi::Statement* stmt = conn->createStatement(delete_sql);
        stmt->setInt(1, std::stoi(id));
        stmt->executeUpdate();
        conn->commit();
        conn->terminateStatement(stmt);
        return true;
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "Delete_Sale SQL Error: " << e.getMessage() << std::endl;
        return false;
    }
}

//管理员进货
std::pair<std::vector<StockDetail>, int> SqlTools::Search_StockTable_State_Price_ID(
    const std::string& kind,
    const std::string& price,
    const std::string& id,
    const int& return_count,
    const int& offset,
    bool reverse_goods_ID,
    bool reverse_stock_ID,
    bool reverse_count,
    bool reverse_date)
{
    std::vector<StockDetail> results;
    int total_count = 0;

    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();

        std::string base_sql =
            "SELECT * FROM ("
            "  SELECT r.*, ROWNUM rn FROM ("
            "    SELECT rr.RecordID, rr.ProductID, rr.Quantity, rr.RestockTime, p.Category "
            "    FROM RestockRecord rr JOIN Product p ON rr.ProductID = p.ProductID WHERE 1=1";

        if (kind != "" && kind != "all")
            base_sql += " AND p.Category = :kind";

        if (price != "") {
            if (price == "less10")
                base_sql += " AND p.Price < 10";
            else if (price == "10to50")
                base_sql += " AND p.Price >= 10 AND p.Price < 50";
            else if (price == "50to100")
                base_sql += " AND p.Price >= 50 AND p.Price <= 100";
            else if (price == "than100")
                base_sql += " AND p.Price > 100";
        }

        if (!id.empty())
            base_sql += " AND rr.RecordID LIKE :id";

        base_sql += " ORDER BY ";
        if (reverse_goods_ID)
            base_sql += "rr.ProductID DESC";
        else if (reverse_stock_ID)
            base_sql += "rr.RecordID DESC";
        else if (reverse_count)
            base_sql += "rr.Quantity DESC";
        else if (reverse_date)
            base_sql += "TRUNC(rr.RestockTime) DESC";
        else
            base_sql += "rr.RecordID ASC";

        base_sql += ") r WHERE ROWNUM <= :end_row) WHERE rn > :start_row";

        std::string count_sql =
            "SELECT COUNT(*) FROM RestockRecord rr JOIN Product p ON rr.ProductID = p.ProductID WHERE 1=1";

        if (kind != "" && kind != "all")
            count_sql += " AND p.Category = :kind";

        if (price != "") {
            if (price == "less10")
                count_sql += " AND p.Price < 10";
            else if (price == "10to50")
                count_sql += " AND p.Price >= 10 AND p.Price < 50";
            else if (price == "50to100")
                count_sql += " AND p.Price >= 50 AND p.Price <= 100";
            else if (price == "than100")
                count_sql += " AND p.Price > 100";
        }

        if (!id.empty())
            count_sql += " AND rr.RecordID LIKE :id";

        auto* count_stmt = conn->createStatement(count_sql);
        int bind_index = 1;
        if (kind != "" && kind != "all")
            count_stmt->setString(bind_index++, kind);
        if (!id.empty())
            count_stmt->setString(bind_index++, "%" + id + "%");
        auto* count_rs = count_stmt->executeQuery();
        if (count_rs->next()) {
            total_count = count_rs->getInt(1);
        }
        conn->terminateStatement(count_stmt);

        auto* stmt = conn->createStatement(base_sql);
        bind_index = 1;
        if (kind != "" && kind != "all")
            stmt->setString(bind_index++, kind);
        if (!id.empty())
            stmt->setString(bind_index++, "%" + id + "%");
        stmt->setInt(bind_index++, offset + std::min(return_count, total_count));
        stmt->setInt(bind_index++, offset);

        auto* rs = stmt->executeQuery();
        while (rs->next()) {
            StockDetail sd;
            sd.stock_ID = std::to_string(rs->getInt(1));   // RecordID
            sd.goods_ID = std::to_string(rs->getInt(2));   // ProductID
            sd.count = std::to_string(rs->getInt(3));   // Quantity
            sd.date = rs->getString(4);                // RestockTime
            // 第5列为 Category，可忽略或使用
            results.push_back(std::move(sd));
        }

        conn->terminateStatement(stmt);
    }
    catch (oracle::occi::SQLException& e) {
        qDebug() << "Search_StockTable_State_Price_ID SQL Error: " << e.getMessage();
    }

    return { results, total_count };
}

bool SqlTools::Change_StockTable_State(const std::vector<StockDetail>& changeData)//进货记录不能修改
{
    return false;
}
bool SqlTools::Add_Stock(const StockDetail& addData)//根据你传入的参数，该商品一定存在。RecordID,RestockTime由系统自动填充
{
    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();

        std::string insert_sql = R"(
            INSERT INTO RestockRecord (RecordID, ProductID, Quantity, RestockTime)
            VALUES (RESTOCKRECORD_SEQ.NEXTVAL, :1, :2, CURRENT_TIMESTAMP)
        )";
        oracle::occi::Statement* stmt = conn->createStatement(insert_sql);
        stmt->setInt(1, std::stoi(addData.goods_ID));
        stmt->setInt(2, std::stoi(addData.count));
        stmt->executeUpdate();
        conn->commit();
        return true;
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "添加进货记录失败: " << e.getMessage() << std::endl;
        return false;
    }
}//
bool SqlTools::Delete_Stock(const std::string& id)
{
    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();
        std::string delete_sql = R"(
            DELETE FROM RestockRecord WHERE RecordID = :1
        )";
        oracle::occi::Statement* stmt = conn->createStatement(delete_sql);
        stmt->setString(1, id);
        int rows = stmt->executeUpdate();
        conn->commit();
        conn->terminateStatement(stmt);
        return rows > 0;
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "删除进货记录失败: " << e.getMessage() << std::endl;
        return false;
    }
}

//员工管理
std::pair<std::vector<StaffDetail>, int> SqlTools::Search_StaffTable_Role_Name(//只能名字或入职时间或角色逆序
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
) {
    std::vector<StaffDetail> results;
    int total_count = 0;

    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();

        // 限制：只允许一个可选字段逆序（name、join_date、role）
        std::string order_field = "EmployeeID";
        std::string order_direction = "ASC";  // 默认按照ID升序

        if (reverse_name) {
            order_field = "Name";
            order_direction = "DESC";
        }
        else if (reverse_join_date) {
            order_field = "JoinDate";
            order_direction = "DESC";
        }
        else if (reverse_role) {
            order_field = "Role";
            order_direction = "DESC";
        }

        // 构建分页 SQL
        std::string base_sql =
            "SELECT * FROM ( "
            "  SELECT e.*, ROWNUM rn FROM ( "
            "    SELECT * FROM Employee WHERE 1=1 ";

        if (role != "" && role != "全部角色")
            base_sql += " AND Role = :role ";
        if (name != "")
            base_sql += " AND LOWER(Name) LIKE :name ";

        base_sql += " ORDER BY " + order_field + "" + order_direction;

        base_sql += " ) e WHERE ROWNUM <= :end_row "
            ") WHERE rn > :start_row";

        // 统计总数 SQL
        std::string count_sql = "SELECT COUNT(*) FROM Employee WHERE 1=1 ";
        if (role != "" && role != "全部角色")
            count_sql += " AND Role = :role ";
        if (name != "")
            count_sql += " AND LOWER(Name) LIKE :name ";

        auto* count_stmt = conn->createStatement(count_sql);
        int bind_index = 1;
        if (role != "" && role != "全部角色")
            count_stmt->setString(bind_index++, role);
        if (name != "")
            count_stmt->setString(bind_index++, "%" + toLower(name) + "%");

        auto* count_rs = count_stmt->executeQuery();
        if (count_rs->next()) {
            total_count = count_rs->getInt(1);
        }
        conn->terminateStatement(count_stmt);

        // 执行分页查询
        auto* stmt = conn->createStatement(base_sql);
        bind_index = 1;
        if (role != "" && role != "全部角色")
            stmt->setString(bind_index++, role);
        if (name != "")
            stmt->setString(bind_index++, "%" + toLower(name) + "%");

        int end_row = offset + return_count;
        stmt->setInt(bind_index++, end_row);
        stmt->setInt(bind_index++, offset);

        auto* rs = stmt->executeQuery();
        while (rs->next()) {
            StaffDetail sd;
            sd.staff_ID = rs->getString(1);
            sd.name = rs->getString(2);
            sd.email = rs->getString(3);
            sd.password = rs->getString(4);
            sd.join_date = rs->getString(5);
            sd.role = rs->getString(6);
            results.emplace_back(std::move(sd));
        }

        conn->terminateStatement(stmt);
    }
    catch (oracle::occi::SQLException& e) {
        qDebug() << "Search_StaffTable_Role_Name SQL Error: " << e.getMessage() ;
    }

    return { results, total_count };
}

bool SqlTools::Change_StaffTable(const std::vector<StaffDetail>& changeData) {
    if (changeData.empty()) return true;  // 没有数据直接返回 true

    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();

        for (const auto& staff : changeData) {
            std::string update_sql = "UPDATE Employee SET ";

            bool first = true;
            if (!staff.name.empty()) {
                update_sql += (first ? "" : ", ") + std::string("Name = :name");
                first = false;
            }
            if (!staff.email.empty()) {
                update_sql += (first ? "" : ", ") + std::string("Email = :email");
                first = false;
            }
            if (!staff.password.empty()) {
                update_sql += (first ? "" : ", ") + std::string("Password = :password");
                first = false;
            }
            if (!staff.role.empty()) {
                update_sql += (first ? "" : ", ") + std::string("Role = :role");
                first = false;
            }
            if (first) continue;

            update_sql += " WHERE EmployeeID = :employee_id";

            auto* stmt = conn->createStatement(update_sql);

            int param_index = 1;
            if (!staff.name.empty()) {
                stmt->setString(param_index++, staff.name);
            }
            if (!staff.email.empty()) {
                stmt->setString(param_index++, staff.email);
            }
            if (!staff.password.empty()) {
                stmt->setString(param_index++, staff.password);
            }
            if (!staff.role.empty()) {
                stmt->setString(param_index++, staff.role);
            }

            // 设置 WHERE 子句中的员工 ID
            stmt->setString(param_index, staff.staff_ID);

            stmt->executeUpdate();
            conn->terminateStatement(stmt);
        }
        conn->commit();
        return true;
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "Change_StaffTable SQL Error: " << e.getMessage() << std::endl;
        return false;
    }
}
bool SqlTools::Add_Staff(const StaffDetail& addData) {
    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();

        std::string insert_sql =
            "INSERT INTO Employee (EmployeeID, Name, Email, Password, Role) "
            "VALUES (:id, :name, :email, :password, :role)";

        auto* stmt = conn->createStatement(insert_sql);
        stmt->setString(1, addData.staff_ID);
        stmt->setString(2, addData.name);
        stmt->setString(3, addData.email);
        stmt->setString(4, addData.password);
        stmt->setString(5, addData.role);
        stmt->executeUpdate();
        conn->terminateStatement(stmt);
        conn->commit();
        return true;
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "Add_Staff SQL Error: " << e.getMessage() << std::endl;
        return false;
    }
}
bool SqlTools::Delete_Staff(const std::string& id) {
    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();
        std::string delete_sql = "DELETE FROM Employee WHERE EmployeeID = :id";
        auto* stmt = conn->createStatement(delete_sql);
        stmt->setString(1, id);
        int rows_affected = stmt->executeUpdate();
        conn->terminateStatement(stmt);
        conn->commit();
        return rows_affected > 0;
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "Delete_Staff SQL Error: " << e.getMessage() << std::endl;
        return false;
    }
}

const QVector<QPair<QDateTime, double>> SqlTools::Get_Income_Date(const std::string& start_date, const std::string& end_date)
{
    QVector<QPair<QDateTime, double>> result; // 存储结果的向量
    try 
    {
        // 获取数据库连接
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();
        // 构造 SQL 查询语句
        // 使用 JOIN 将 SalesRecord 和 Orders 表关联
        // 按天汇总 TotalAmount
        std::string sql = R"(
            SELECT 
                TRUNC(o.OrderTime) AS OrderDate, 
                SUM(o.TotalAmount) AS TotalIncome
            FROM 
                Orders o
            JOIN 
                SalesRecord sr ON o.OrderID = sr.OrderID
            WHERE 
                TRUNC(o.OrderTime) >= TO_DATE(:startDate, 'YYYY-MM-DD')
                AND TRUNC(o.OrderTime) <= TO_DATE(:endDate, 'YYYY-MM-DD')
            GROUP BY 
                TRUNC(o.OrderTime)
            ORDER BY 
                TRUNC(o.OrderTime)
        )";
        // 创建 Statement 对象
        auto* stmt = conn->createStatement(sql);
        // 绑定参数
        stmt->setString(1, start_date); // 绑定开始日期
        stmt->setString(2, end_date);   // 绑定结束日期
        // 执行查询
        auto* res = stmt->executeQuery();
        // 遍历结果集
        while (res->next()) 
        {
            // 获取日期（TRUNC 后的日期）
            std::string dateStr = res->getString(1); // 获取日期字符串
            QDateTime orderDate = QDateTime::fromString(QString::fromStdString(dateStr), "YYYY-MM-DD");
            // 获取总收入
            double totalIncome = res->getDouble(2);
            // 将结果添加到 QVector 中
            result.append(qMakePair(orderDate, totalIncome));
        }
        // 释放资源
        stmt->closeResultSet(res);
        conn->terminateStatement(stmt);
        return result;
    }
    catch (oracle::occi::SQLException& e) 
    {
        // 捕获并输出 SQL 异常信息
        std::cerr << "Get_Income_Date SQL Error: " << e.getMessage() << std::endl;
        // 如果需要，可以在这里记录日志或执行其他异常处理逻辑
        return result; // 返回空结果
    }
}

bool SqlTools::Add_VerificationCode(const std::string& email, const std::string& code) {
    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();

        // Step 1: 先检查邮箱是否存在
        std::string check_sql = "SELECT COUNT(*) FROM Email_Verification WHERE email = :1";
        auto* check_stmt = conn->createStatement(check_sql);
        check_stmt->setString(1, email);
        auto* rs = check_stmt->executeQuery();

        bool exists = false;
        if (rs->next()) {
            exists = rs->getInt(1) > 0;
        }

        check_stmt->closeResultSet(rs);
        conn->terminateStatement(check_stmt);

        if (exists) {
            // Step 2: 存在则更新验证码和过期时间（5分钟后）
            std::string update_sql = R"(
                UPDATE Email_Verification 
                SET code = :1, expire_time = SYSTIMESTAMP + INTERVAL '5' MINUTE 
                WHERE email = :2
            )";
            auto* update_stmt = conn->createStatement(update_sql);
            update_stmt->setString(1, code);
            update_stmt->setString(2, email);
            update_stmt->executeUpdate();
            conn->terminateStatement(update_stmt);
        }
        else {
            // Step 3: 不存在则插入记录，过期时间为30分钟后
            std::string insert_sql = R"(
                INSERT INTO Email_Verification (email, code, expire_time)
                VALUES (:1, :2, SYSTIMESTAMP + INTERVAL '30' MINUTE)
            )";
            auto* insert_stmt = conn->createStatement(insert_sql);
            insert_stmt->setString(1, email);
            insert_stmt->setString(2, code);
            insert_stmt->executeUpdate();
            conn->terminateStatement(insert_stmt);
        }

        conn->commit();
        return true;
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "Add_VerificationCode SQL Error: " << e.getMessage() << std::endl;
        return false;
    }
}

//客户管理，在客户端
std::pair<std::vector<CustomerDetail>, int> SqlTools::Search_CustomerTable_Name//仅可生日和注册日期逆序
(
    const std::string& name,
    const int& return_count,
    const int& offset,
    bool reverse_birth_date,
    bool reverse_register_date
) {
    std::vector<CustomerDetail> results;
    int total_count = 0;

    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();
        std::string base_sql =
            "SELECT * FROM ("
            "  SELECT c.*, ROWNUM rn FROM ("
            "    SELECT * FROM Customer WHERE 1=1";

        if (name != "" && name != "") {
            base_sql += " AND LOWER(Name) LIKE :name";
        }
        if (reverse_birth_date) {
            base_sql += " ORDER BY BirthDate DESC";
        }
        else {
            base_sql += " ORDER BY BirthDate ASC";
        }

        if (reverse_register_date) {
            base_sql += ", RegisterTime DESC";
        }
        else {
            base_sql += ", RegisterTime ASC";
        }
        base_sql += " ) c WHERE ROWNUM <= :end_row"
            ") WHERE rn > :start_row";
        std::string count_sql = "SELECT COUNT(*) FROM Customer WHERE 1=1";
        if (name != "" && name != "") {
            count_sql += " AND LOWER(Name) LIKE :name";
        }

        auto* count_stmt = conn->createStatement(count_sql);
        int bind_index = 1;
        if (name != "" && name != "") {
            count_stmt->setString(bind_index++, "%" + toLower(name) + "%");
        }

        auto* count_rs = count_stmt->executeQuery();
        if (count_rs->next()) {
            total_count = count_rs->getInt(1);
        }
        conn->terminateStatement(count_stmt);
        auto* stmt = conn->createStatement(base_sql);
        bind_index = 1;
        if (name != "" && name != "") {
            stmt->setString(bind_index++, "%" + toLower(name) + "%");
        }

        int end_row = offset + return_count;
        int start_row = offset;

        stmt->setInt(bind_index++, end_row);
        stmt->setInt(bind_index++, start_row);

        auto* rs = stmt->executeQuery();
        while (rs->next()) {
            CustomerDetail cd;
            cd.profile_picture = rs->getString(1);  // 顾客头像路径
            cd.customer_ID = rs->getString(2);      // 顾客ID
            cd.birth_date = rs->getString(3);       // 顾客出生日期
            cd.note = rs->getString(4);             // 顾客自我描述
            cd.register_date = rs->getString(5);    // 顾客注册日期
            cd.email = rs->getString(6);            // 顾客邮箱
            cd.password = rs->getString(7);         // 顾客密码

            results.emplace_back(std::move(cd));
        }
        conn->terminateStatement(stmt);
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "Search_CustomerTable_Name SQL Error: " << e.getMessage() << std::endl;
    }
    return { results, total_count };
}
bool SqlTools::Add_Customer(const CustomerDetail& customer) {
    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();

        std::string insert_sql =
            "INSERT INTO Customer (CustomerID, Avatar, BirthDate, Note, RegisterTime, Email, Password) "
            "VALUES (:customerID, :profilePicture, TO_DATE(:birthDate, 'YYYY-MM-DD'), :note, CURRENT_TIMESTAMP, :email, :password)";

        auto* stmt = conn->createStatement(insert_sql);
        stmt->setString(1, customer.customer_ID);
        stmt->setString(2, customer.profile_picture);
        stmt->setString(3, customer.birth_date); // Assume birth_date is in 'YYYY-MM-DD' format
        stmt->setString(4, customer.note);
        stmt->setString(5, customer.email);
        stmt->setString(6, customer.password);

        stmt->executeUpdate();
        conn->terminateStatement(stmt);
        conn->commit();

        return true;
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "Add_Customer SQL Error: " << e.getMessage() << std::endl;
        return false;
    }
}
bool SqlTools::Delete_Customer(const std::string& id) {
    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();

        // SQL 查询，删除对应顾客的记录
        std::string delete_sql =
            "DELETE FROM Customer WHERE CustomerID = :customerID";

        auto* stmt = conn->createStatement(delete_sql);
        stmt->setString(1, id);  // 使用传入的顾客ID

        int rows_affected = stmt->executeUpdate();
        conn->terminateStatement(stmt);

        // 提交事务
        if (rows_affected > 0) {
            conn->commit();  // 删除成功后提交事务
            return true;
        }
        else {
            return false;  // 顾客ID不存在，删除失败
        }
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "Delete_Customer SQL Error: " << e.getMessage() << std::endl;
        return false;
    }
}
bool SqlTools::Change_CustomerTable(const std::vector<CustomerDetail>& changeData) {
    if (changeData.empty()) return true;  // 如果没有数据，直接返回 true

    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();

        for (const auto& customer : changeData) {
            std::string update_sql = "UPDATE Customer SET ";

            bool first = true;  // 用于判断是否是第一个修改的字段

            if (customer.profile_picture!="") {
                update_sql += (first ? "" : ", ");
                update_sql += "Avatar = :avatar";
                first = false;
            }
            if (customer.birth_date!="") {
                update_sql += (first ? "" : ", ");
                update_sql += "BirthDate = TO_DATE(:birth_date, 'YYYY-MM-DD')";
                first = false;
            }
            if (customer.note !="") {
                update_sql += (first ? "" : ", ");
                update_sql += "Note = :note";
                first = false;
            }
            if (customer.email !="") {
                update_sql += (first ? "" : ", ");
                update_sql += "Email = :email";
                first = false;
            }
            if (customer.password !="") {
                update_sql += (first ? "" : ", ");
                update_sql += "Password = :password";
                first = false;
            }

            update_sql += " WHERE CustomerID = :customer_id";  // WHERE 子句，确保更新指定顾客

            auto* stmt = conn->createStatement(update_sql);

            // 设置参数
            int num = 1;
            if (customer.profile_picture != "") 
            {
                stmt->setString(num, customer.profile_picture);num++;
            }
            if (customer.birth_date != "")
            {
                stmt->setString(num, customer.birth_date);num++;
            }
            if (customer.note != "")
            {
                stmt->setString(num, customer.note);
                num++;
            }
            if (customer.email != "")
            {
                stmt->setString(num, customer.email);
                num++;
            }
            if (customer.password != "")
            {
                stmt->setString(num, customer.password);
                num++;
            }
            stmt->setString(num, customer.customer_ID);  // 顾客ID作为WHERE条件

            stmt->executeUpdate();
            conn->terminateStatement(stmt);
            conn->commit();  // 提交事务
        }

        return true;
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "Change_CustomerTable SQL Error: " << e.getMessage() << std::endl;
        return false;
    }
}
bool Make_Order(const std::vector<std::pair<ProductDetail, int>>& items, const std::string& customerID)
{
    if (items.empty()) return true;
    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();

        for (const auto& item : items) {
            const ProductDetail& product = item.first;
            int quantity = item.second;
            if (product.ID.empty() || quantity <= 0) continue;

            auto* seq_stmt = conn->createStatement("SELECT order_seq.NEXTVAL FROM dual");
            auto* seq_rs = seq_stmt->executeQuery();
            int new_order_id = 0;
            if (seq_rs->next()) {
                new_order_id = seq_rs->getInt(1);
            }
            conn->terminateStatement(seq_stmt);
            std::string insert_order_sql = "INSERT INTO Orders(OrderID, CustomerID, OrderTime, TotalAmount, Status) "
                "VALUES(:1, :2, SYSTIMESTAMP, :3, '未处理')";
            auto* order_stmt = conn->createStatement(insert_order_sql);

            double unit_price = std::stod(product.price);
            double total_price = unit_price * quantity;

            order_stmt->setInt(1, new_order_id);
            order_stmt->setString(2, customerID);
            order_stmt->setDouble(3, total_price);
            order_stmt->executeUpdate();
            conn->terminateStatement(order_stmt);
            std::string insert_item_sql = "INSERT INTO OrderItem(OrderID, ProductID, Quantity, Price) "
                "VALUES(:1, :2, :3, :4)";
            auto* item_stmt = conn->createStatement(insert_item_sql);
            item_stmt->setInt(1, new_order_id);
            item_stmt->setInt(2, std::stoi(product.ID));
            item_stmt->setInt(3, quantity);
            item_stmt->setDouble(4, total_price);
            item_stmt->executeUpdate();
            conn->terminateStatement(item_stmt);
            conn->commit();
        }

        return true;
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "Make_Order SQL Error: " << e.getMessage() << std::endl;
        return false;
    }
}
//用户相关
bool Create_Order_From_Html(const order& fromHt)
{
    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();

        // 1. 查询商品信息
        std::string query_sql = R"(
            SELECT ProductID, Name, Category, Price, StockQuantity, Description, PATH
            FROM Product
            WHERE ProductID = :1
        )";
        auto* stmt = conn->createStatement(query_sql);
        stmt->setInt(1, std::stoi(fromHt.ProductID));

        oracle::occi::ResultSet* rs = stmt->executeQuery();
        if (!rs->next()) {
            std::cerr << "Create_Order_From_Html Error: Product not found." << std::endl;
            conn->terminateStatement(stmt);
            return false;
        }

        // 封装 ProductDetail
        ProductDetail product;
        product.ID = rs->getString(1);
        product.name = rs->getString(2);
        product.category = rs->getString(3);
        product.price = rs->getString(4); // 保留为字符串
        product.count = rs->getInt(5);
        product.description = rs->getString(6);
        product.path = rs->getString(7);

        int quantity = std::stoi(fromHt.count);
        conn->terminateStatement(stmt);

        if (quantity <= 0 || product.ID.empty()) {
            std::cerr << "Invalid quantity or product." << std::endl;
            return false;
        }

        // 2. 获取订单ID
        auto* seq_stmt = conn->createStatement("SELECT order_seq.NEXTVAL FROM dual");
        auto* seq_rs = seq_stmt->executeQuery();
        int new_order_id = 0;
        if (seq_rs->next()) {
            new_order_id = seq_rs->getInt(1);
        }
        conn->terminateStatement(seq_stmt);

        // 3. 插入 Orders 表
        std::string insert_order_sql = R"(
            INSERT INTO Orders(OrderID, CustomerID, OrderTime, TotalAmount, Status)
            VALUES(:1, :2, SYSTIMESTAMP, :3, '未处理')
        )";
        auto* order_stmt = conn->createStatement(insert_order_sql);

        double unit_price = std::stod(product.price);
        double total_price = unit_price * quantity;

        order_stmt->setInt(1, new_order_id);
        order_stmt->setString(2, fromHt.customerID);
        order_stmt->setDouble(3, total_price);
        order_stmt->executeUpdate();
        conn->terminateStatement(order_stmt);

        // 4. 插入 OrderItem 表
        std::string insert_item_sql = R"(
            INSERT INTO OrderItem(OrderID, ProductID, Quantity, Price)
            VALUES(:1, :2, :3, :4)
        )";
        auto* item_stmt = conn->createStatement(insert_item_sql);
        item_stmt->setInt(1, new_order_id);
        item_stmt->setInt(2, std::stoi(product.ID));
        item_stmt->setInt(3, quantity);
        item_stmt->setDouble(4, total_price);
        item_stmt->executeUpdate();
        conn->terminateStatement(item_stmt);

        // 5. 提交事务
        conn->commit();
        return true;
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "Create_Order_From_Html SQL Error: " << e.getMessage() << std::endl;
        return false;
    }
}
bool SqlTools::CustormerLogin(const std::string& email, const std::string& password) {
    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();

        std::string sql = R"(
            SELECT COUNT(*) FROM Customer 
            WHERE CustomeID = :1 AND Password = :2
        )";

        auto* stmt = conn->createStatement(sql);
        stmt->setString(1, email);
        stmt->setString(2, password);

        oracle::occi::ResultSet* rs = stmt->executeQuery();

        bool loginSuccess = false;
        if (rs->next() && rs->getInt(1) > 0) {
            loginSuccess = true;
        }

        stmt->closeResultSet(rs);
        conn->terminateStatement(stmt);

        return loginSuccess;
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "CustomerLogin SQL Error: " << e.getMessage() << std::endl;
        return false;
    }
}
std::pair<std::vector<ProductDetail>, int> SearchProductTable_BySearchStruct(const search& s) {
    std::vector<ProductDetail> results;
    int total_count = 0;

    try {
        oracle::occi::Connection* conn = OracleDB::getInstance().getConnection();

        std::string base_sql = "SELECT * FROM Product WHERE 1=1";
        std::string count_sql = "SELECT COUNT(*) FROM Product WHERE 1=1";

        // 构造条件
        if (!s.category.empty() && s.category != "all") {
            base_sql += " AND Category = :category";
            count_sql += " AND Category = :category";
        }

        if (!s.prise.empty()) {
            if (s.prise == "小于10元") {
                base_sql += " AND Price < 10";
                count_sql += " AND Price < 10";
            }
            else if (s.prise == "10元到50元") {
                base_sql += " AND Price >= 10 AND Price <= 50";
                count_sql += " AND Price >= 10 AND Price <= 50";
            }
            else if (s.prise == "50元到100元") {
                base_sql += " AND Price > 50 AND Price <= 100";
                count_sql += " AND Price > 50 AND Price <= 100";
            }
            else if (s.prise == "大于100元") {
                base_sql += " AND Price > 100";
                count_sql += " AND Price > 100";
            }
        }

        if (!s.name.empty()) {
            base_sql += " AND LOWER(Name) LIKE :name";
            count_sql += " AND LOWER(Name) LIKE :name";
        }

        // 排序逻辑
        base_sql += s.re ? " ORDER BY Price DESC" : " ORDER BY Price ASC";

        // 查询总数
        auto* count_stmt = conn->createStatement(count_sql);
        int bind_index = 1;
        if (!s.category.empty() && s.category != "all") {
            count_stmt->setString(bind_index++, s.category);
        }
        if (!s.name.empty()) {
            count_stmt->setString(bind_index++, "%" + toLower(s.name) + "%");
        }
        auto* count_rs = count_stmt->executeQuery();
        if (count_rs->next()) {
            total_count = count_rs->getInt(1);
        }
        count_stmt->closeResultSet(count_rs);
        conn->terminateStatement(count_stmt);

        // 查询数据
        auto* stmt = conn->createStatement(base_sql);
        bind_index = 1;
        if (!s.category.empty() && s.category != "all") {
            stmt->setString(bind_index++, s.category);
        }
        if (!s.name.empty()) {
            stmt->setString(bind_index++, "%" + toLower(s.name) + "%");
        }

        auto* rs = stmt->executeQuery();
        while (rs->next()) {
            ProductDetail pd;
            pd.ID = std::to_string(rs->getInt(1));
            pd.name = rs->getString(2);
            pd.category = rs->getString(3);
            pd.price = std::to_string(rs->getFloat(4));
            pd.count = std::to_string(rs->getInt(5));
            pd.description = rs->getString(6);
            pd.path = rs->getString(7);
            results.emplace_back(std::move(pd));
        }

        conn->terminateStatement(stmt);
    }
    catch (oracle::occi::SQLException& e) {
        std::cerr << "SearchProductTable_BySearchStruct SQL Error: " << e.getMessage() << std::endl;
    }

    return { results, total_count };
}







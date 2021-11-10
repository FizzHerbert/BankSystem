#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include "dialog.h"
#include "ui_dialog.h"

#include <QTabWidget>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <iostream>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QSplitter>
#include <QGroupBox>
#include <QComboBox>
#include <QDateEdit>



Dialog::Dialog(QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::Dialog)
{
	ui->setupUi(this);
	tab_widget_ = new QTabWidget();
 
	//连接数据库
	if (connect_database())
	{
		std::cout << "Connect database successfully!" << std::endl;
	}

	//向QTabWidget中添加客户管理页面
	client_init();

	//向QTabWidget中添加账户管理页面
	account_init();
	
	//向QTabWidget中添加贷款管理页面
	loan_init();

	//向QTabWidget中添加业务统计页面
	statistics_init();
	
	//设置tabwidget布局
	tab_layout_ = new QHBoxLayout();
	tab_layout_->addWidget(tab_widget_);
	this->setLayout(tab_layout_);
	this->resize(1500, 800);
	this->setWindowTitle("Bank System");	
}


Dialog::~Dialog()
{
    delete ui;
}


void Dialog::client_init()
{
	widget_client_ = new QWidget();
	//client_spliter_ = new QSplitter(Qt::Vertical, widget_client_);
	client_table_ = new QTableView();
	client_group_1 = new QGroupBox();
	client_group_2 = new QGroupBox("客户信息");
	//client_model_ = new QStandardItemModel();
	client_model_ = new QSqlQueryModel();

	client_id_ = new QLabel(tr("客户身份证号:"));
	client_name_ = new QLabel(tr("客户姓名:"));
	client_phone_ = new QLabel(tr("客户电话号码:"));
	client_address_ = new QLabel(tr("客户居住地址:"));
	client_contact_name_ = new QLabel(tr("联系人姓名:"));
	client_contact_phone_ = new QLabel(tr("联系人电话:"));
	client_contact_email_ = new QLabel(tr("联系人Email:"));
	client_contact_relationship_ = new QLabel(tr("客户与联系人关系:"));
	client_charge_id_ = new QLabel(tr("银行负责人身份证号:"));
	client_charge_type_ = new QLabel(tr("负责业务:"));

	client_id_line_ = new QLineEdit();
	client_name_line_ = new QLineEdit();
	client_phone_line_ = new QLineEdit();
	client_address_line_ = new QLineEdit();
	client_contact_name_line_ = new QLineEdit();
	client_contact_phone_line_ = new QLineEdit();
	client_contact_email_line_ = new QLineEdit();
	client_contact_relationship_line_ = new QLineEdit();
	client_charge_id_line_ = new QLineEdit();

	client_charge_type_box_ = new QComboBox();
	client_charge_type_box_->clear();
	client_charge_type_box_->addItem("贷款负责人");
	client_charge_type_box_->addItem("账户负责人");

	client_add_ = new QPushButton("增加");
	client_delete_ = new QPushButton("删除");
	client_update_ = new QPushButton("修改");
	client_query_ = new QPushButton("查询");
	client_clear_ = new QPushButton("清空");

	connect(client_clear_, SIGNAL(clicked()), this, SLOT(client_clear_button_click()));
	connect(client_query_, SIGNAL(clicked()), this, SLOT(client_query_button_click()));
	connect(client_add_, SIGNAL(clicked()), this, SLOT(client_add_button_click()));
	connect(client_delete_, SIGNAL(clicked()), this, SLOT(client_delete_button_click()));
	connect(client_update_, SIGNAL(clicked()), this, SLOT(client_update_button_click()));

	client_group_layout_ = new QGridLayout(client_group_1);
	client_layout_ = new QVBoxLayout;
	client_layout_1 = new QVBoxLayout(client_group_2);

	//client_spliter_->addWidget(client_group_);
	//client_spliter_->addWidget(client_table_);

	client_model_->setQuery("select * from customer");
	client_model_->setHeaderData(0, Qt::Horizontal, tr("客户身份证号"));
	client_model_->setHeaderData(1, Qt::Horizontal, tr("客户姓名"));
	client_model_->setHeaderData(2, Qt::Horizontal, tr("客户电话号码"));
	client_model_->setHeaderData(3, Qt::Horizontal, tr("客户居住地址"));
	client_model_->setHeaderData(4, Qt::Horizontal, tr("联系人姓名"));
	client_model_->setHeaderData(5, Qt::Horizontal, tr("联系人电话"));
	client_model_->setHeaderData(6, Qt::Horizontal, tr("联系人Email"));
	client_model_->setHeaderData(7, Qt::Horizontal, tr("客户与联系人关系"));
	client_table_->setModel(client_model_);
	client_table_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//ResizeToContents

	client_group_layout_->addWidget(client_id_, 0, 0, 1, 1);
	client_group_layout_->addWidget(client_name_, 1, 0, 1, 1);
	client_group_layout_->addWidget(client_phone_, 2, 0, 1, 1);
	client_group_layout_->addWidget(client_address_, 3, 0, 1, 1);
	client_group_layout_->addWidget(client_charge_id_, 4, 0, 1, 1);
	client_group_layout_->addWidget(client_id_line_, 0, 1, 1, 1);
	client_group_layout_->addWidget(client_name_line_, 1, 1, 1, 1);
	client_group_layout_->addWidget(client_phone_line_, 2, 1, 1, 1);
	client_group_layout_->addWidget(client_address_line_, 3, 1, 1, 1);
	client_group_layout_->addWidget(client_charge_id_line_, 4, 1, 1, 1);
	client_group_layout_->addWidget(client_contact_relationship_, 0, 2, 1, 1);
	client_group_layout_->addWidget(client_contact_name_, 1, 2, 1, 1);
	client_group_layout_->addWidget(client_contact_phone_, 2, 2, 1, 1);
	client_group_layout_->addWidget(client_contact_email_, 3, 2, 1, 1);
	client_group_layout_->addWidget(client_charge_type_, 4, 2, 1, 1);
	client_group_layout_->addWidget(client_contact_relationship_line_, 0, 3, 1, 1);
	client_group_layout_->addWidget(client_contact_name_line_, 1, 3, 1, 1);
	client_group_layout_->addWidget(client_contact_phone_line_, 2, 3, 1, 1);
	client_group_layout_->addWidget(client_contact_email_line_, 3, 3, 1, 1);
	client_group_layout_->addWidget(client_charge_type_box_, 4, 3, 1, 1);
	client_group_layout_->addWidget(client_add_, 0, 4, 1, 1);
	client_group_layout_->addWidget(client_delete_, 1, 4, 1, 1);
	client_group_layout_->addWidget(client_update_, 2, 4, 1, 1);
	client_group_layout_->addWidget(client_query_, 3, 4, 1, 1);
	client_group_layout_->addWidget(client_clear_, 4, 4, 1, 1);

	client_layout_1->addWidget(client_table_);

	client_layout_->addWidget(client_group_1);
	client_layout_->addWidget(client_group_2);

	widget_client_->setLayout(client_layout_);
	client_table_->show();
	tab_widget_->addTab(widget_client_, "客户管理");
}


void Dialog::account_init()
{
	widget_account_ = new QWidget();

	account_table_left_ = new QTableView();
	account_table_right_ = new QTableView();

	account_group_1 = new QGroupBox();
	account_group_2 = new QGroupBox("储蓄账户");
	account_group_3 = new QGroupBox("支票账户");
	/*account_model_left_ = new QStandardItemModel();
	account_model_right_ = new QStandardItemModel();*/
	account_model_left_ = new QSqlQueryModel();
	account_model_right_ = new QSqlQueryModel();

	account_id_ = new QLabel(tr("账户号:"));
	account_balance_ = new QLabel(tr("账户余额:"));
	account_opendate_ = new QLabel(tr("开户日期:"));
	account_type_ = new QLabel(tr("账户类型:"));
	account_rate_ = new QLabel(tr("利率:"));
	account_currency_type_ = new QLabel(tr("货币类型:"));
	account_overdraft_ = new QLabel(tr("透支额:"));
	account_client_id_ = new QLabel(tr("开户客户身份证号:"));
	account_open_branch_ = new QLabel(tr("开户支行:"));

	account_id_line_ = new QLineEdit();
	account_balance_line_ = new QLineEdit();
	account_rate_line_ = new QLineEdit();
	account_currency_type_line_ = new QLineEdit();
	account_overdraft_line_ = new QLineEdit();
	account_client_id_line_ = new QLineEdit();
	account_open_branch_line_ = new QLineEdit();

	account_type_box_ = new QComboBox();

	account_opendate_line_ = new QDateEdit(QDate::currentDate());

	account_add_ = new QPushButton("开户");
	account_delete_ = new QPushButton("销户");
	account_update_ = new QPushButton("修改");
	account_query_ = new QPushButton("查询");
	account_clear_ = new QPushButton("清空");

	connect(account_clear_, SIGNAL(clicked()), this, SLOT(account_clear_button_click()));
	connect(account_query_, SIGNAL(clicked()), this, SLOT(account_query_button_click()));
	connect(account_add_, SIGNAL(clicked()), this, SLOT(account_add_button_click()));
	connect(account_delete_, SIGNAL(clicked()), this, SLOT(account_delete_button_click()));
	connect(account_update_, SIGNAL(clicked()), this, SLOT(account_update_button_click()));

	account_group_layout_ = new QGridLayout(account_group_1);
	account_box_layout_ = new QGridLayout();
	account_layout_1 = new QVBoxLayout(account_group_2);
	account_layout_2 = new QVBoxLayout(account_group_3);

	account_type_box_->clear();
	account_type_box_->addItem("储蓄");
	account_type_box_->addItem("支票");

	account_opendate_line_->setCalendarPopup(true);

	//account_model_left_->setHorizontalHeaderLabels({ "账户号", "账户余额", "开户日期", "货币类型", "利率" });
	account_model_left_->setQuery("select * from Savings_Account");
	account_model_left_->setHeaderData(0, Qt::Horizontal, tr("账户号"));
	account_model_left_->setHeaderData(1, Qt::Horizontal, tr("账户余额"));
	account_model_left_->setHeaderData(2, Qt::Horizontal, tr("开户日期"));
	account_model_left_->setHeaderData(3, Qt::Horizontal, tr("开户支行"));
	account_model_left_->setHeaderData(4, Qt::Horizontal, tr("利率"));
	account_model_left_->setHeaderData(5, Qt::Horizontal, tr("货币类型"));
	account_table_left_->setModel(account_model_left_);
	account_table_left_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	//account_model_right_->setHorizontalHeaderLabels({ "账户号", "账户余额", "开户日期", "透支额" });
	account_model_right_->setQuery("select * from Check_Account");
	account_model_right_->setHeaderData(0, Qt::Horizontal, tr("账户号"));
	account_model_right_->setHeaderData(1, Qt::Horizontal, tr("账户余额"));
	account_model_right_->setHeaderData(2, Qt::Horizontal, tr("开户日期"));
	account_model_right_->setHeaderData(3, Qt::Horizontal, tr("开户支行"));
	account_model_right_->setHeaderData(4, Qt::Horizontal, tr("透支额"));
	account_table_right_->setModel(account_model_right_);
	account_table_right_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	account_group_layout_->addWidget(account_id_, 0, 0, 1, 1);
	account_group_layout_->addWidget(account_balance_, 1, 0, 1, 1);
	account_group_layout_->addWidget(account_opendate_, 2, 0, 1, 1);
	account_group_layout_->addWidget(account_type_, 3, 0, 1, 1);
	account_group_layout_->addWidget(account_client_id_, 4, 0, 1, 1);
	account_group_layout_->addWidget(account_id_line_, 0, 1, 1, 1);
	account_group_layout_->addWidget(account_balance_line_, 1, 1, 1, 1);
	account_group_layout_->addWidget(account_opendate_line_, 2, 1, 1, 1);
	account_group_layout_->addWidget(account_type_box_, 3, 1, 1, 1);
	account_group_layout_->addWidget(account_client_id_line_, 4, 1, 1, 1);
	account_group_layout_->addWidget(account_currency_type_, 0, 2, 1, 1);
	account_group_layout_->addWidget(account_rate_, 1, 2, 1, 1);
	account_group_layout_->addWidget(account_overdraft_, 2, 2, 1, 1);
	account_group_layout_->addWidget(account_open_branch_, 3, 2, 1, 1);
	account_group_layout_->addWidget(account_currency_type_line_, 0, 3, 1, 1);
	account_group_layout_->addWidget(account_rate_line_, 1, 3, 1, 1);
	account_group_layout_->addWidget(account_overdraft_line_, 2, 3, 1, 1);
	account_group_layout_->addWidget(account_open_branch_line_, 3, 3, 1, 1);
	account_group_layout_->addWidget(account_add_, 0, 4, 1, 1);
	account_group_layout_->addWidget(account_delete_, 1, 4, 1, 1);
	account_group_layout_->addWidget(account_update_, 2, 4, 1, 1);
	account_group_layout_->addWidget(account_query_, 3, 4, 1, 1);
	account_group_layout_->addWidget(account_clear_, 4, 4, 1, 1);

	account_layout_1->addWidget(account_table_left_);
	account_layout_2->addWidget(account_table_right_);

	account_box_layout_->addWidget(account_group_1, 0, 0, 1, 2);
	account_box_layout_->addWidget(account_group_2, 1, 0, 1, 1);
	account_box_layout_->addWidget(account_group_3, 1, 1, 1, 1);

	widget_account_->setLayout(account_box_layout_);
	account_table_left_->show();
	account_table_right_->show();


	tab_widget_->addTab(widget_account_, "账户管理");
}


void Dialog::loan_init()
{
	widget_loan_ = new QWidget();
	
	loan_table_left_ = new QTableView();
	loan_table_right_ = new QTableView();

	loan_group_1 = new QGroupBox();
	loan_group_2 = new QGroupBox("贷款情况");
	loan_group_3 = new QGroupBox("贷款发放情况");
	loan_model_left_ = new QSqlQueryModel();
	loan_model_right_ = new QSqlQueryModel();

	loan_id_ = new QLabel(tr("贷款号:"));
	loan_branch_ = new QLabel(tr("贷款支行:"));
	loan_amount_ = new QLabel(tr("贷款金额:"));
	loan_client_id_ = new QLabel(tr("贷款客户身份证号:"));
	loan_paydate_ = new QLabel(tr("贷款发放日期:"));
	loan_pay_amount_ = new QLabel(tr("贷款发放金额:"));

	loan_id_line_ = new QLineEdit();
	loan_branch_line_ = new QLineEdit();
	loan_amount_line_ = new QLineEdit();
	loan_client_id_line_ = new QLineEdit();
	loan_pay_amount_line_ = new QLineEdit();

	loan_paydate_line_ = new QDateEdit(QDate::currentDate());

	loan_add_ = new QPushButton("增加");
	loan_delete_ = new QPushButton("删除");
	loan_query_ = new QPushButton("查询");
	loan_clear_ = new QPushButton("清空");
	loan_pay_ = new QPushButton("贷款发放");

	connect(loan_clear_, SIGNAL(clicked()), this, SLOT(loan_clear_button_click()));
	connect(loan_query_, SIGNAL(clicked()), this, SLOT(loan_query_button_click()));
	connect(loan_add_, SIGNAL(clicked()), this, SLOT(loan_add_button_click()));
	connect(loan_pay_, SIGNAL(clicked()), this, SLOT(loan_pay_button_click()));
	connect(loan_delete_, SIGNAL(clicked()), this, SLOT(loan_delete_button_click()));

	loan_group_layout_ = new QGridLayout(loan_group_1);
	loan_box_layout_ = new QGridLayout();
	loan_layout_1 = new QVBoxLayout(loan_group_2);
	loan_layout_2 = new QVBoxLayout(loan_group_3);

	loan_paydate_line_->setCalendarPopup(true);

	//loan_model_left_->setHorizontalHeaderLabels({ "贷款号", "贷款支行", "贷款金额", "贷款客户" });
	loan_model_left_->setQuery("select Loan.loan_id, bank_name, loan_amount, Customer_Loan.customer_id, loan_status \
								from Loan, Customer_Loan where Loan.loan_id = Customer_Loan.loan_id");
	loan_model_left_->setHeaderData(0, Qt::Horizontal, tr("贷款号"));
	loan_model_left_->setHeaderData(1, Qt::Horizontal, tr("贷款支行"));
	loan_model_left_->setHeaderData(2, Qt::Horizontal, tr("贷款金额"));
	loan_model_left_->setHeaderData(3, Qt::Horizontal, tr("贷款客户身份证号"));
	loan_model_left_->setHeaderData(4, Qt::Horizontal, tr("贷款发放状态"));
	loan_table_left_->setModel(loan_model_left_);
	loan_table_left_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	//loan_model_right_->setHorizontalHeaderLabels({ "贷款号", "贷款发放状态", "发放日期", "发放金额" });
	loan_model_right_->setQuery("select * from Payment");
	loan_model_right_->setHeaderData(0, Qt::Horizontal, tr("贷款号"));
	loan_model_right_->setHeaderData(1, Qt::Horizontal, tr("发放日期"));
	loan_model_right_->setHeaderData(2, Qt::Horizontal, tr("发放金额"));
	loan_table_right_->setModel(loan_model_right_);
	loan_table_right_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	loan_group_layout_->addWidget(loan_id_, 0, 0, 1, 1);
	loan_group_layout_->addWidget(loan_branch_, 1, 0, 1, 1);
	loan_group_layout_->addWidget(loan_amount_, 2, 0, 1, 1);
	loan_group_layout_->addWidget(loan_id_line_, 0, 1, 1, 1);
	loan_group_layout_->addWidget(loan_branch_line_, 1, 1, 1, 1);
	loan_group_layout_->addWidget(loan_amount_line_, 2, 1, 1, 1);
	loan_group_layout_->addWidget(loan_client_id_, 0, 2, 1, 1);
	loan_group_layout_->addWidget(loan_paydate_, 1, 2, 1, 1);
	loan_group_layout_->addWidget(loan_pay_amount_, 2, 2, 1, 1);
	loan_group_layout_->addWidget(loan_client_id_line_, 0, 3, 1, 1);
	loan_group_layout_->addWidget(loan_paydate_line_, 1, 3, 1, 1);
	loan_group_layout_->addWidget(loan_pay_amount_line_, 2, 3, 1, 1);
	loan_group_layout_->addWidget(loan_add_, 0, 4, 1, 1);
	loan_group_layout_->addWidget(loan_delete_, 1, 4, 1, 1);
	loan_group_layout_->addWidget(loan_query_, 2, 4, 1, 1);
	loan_group_layout_->addWidget(loan_clear_, 3, 1, 1, 1);
	loan_group_layout_->addWidget(loan_pay_, 3, 3, 1, 1);


	loan_layout_1->addWidget(loan_table_left_);
	loan_layout_2->addWidget(loan_table_right_);

	loan_box_layout_->addWidget(loan_group_1, 0, 0, 1, 2);
	loan_box_layout_->addWidget(loan_group_2, 1, 0, 1, 1);
	loan_box_layout_->addWidget(loan_group_3, 1, 1, 1, 1);

	widget_loan_->setLayout(loan_box_layout_);
	loan_table_left_->show();
	loan_table_right_->show();

	tab_widget_->addTab(widget_loan_, "贷款管理");
}


void Dialog::statistics_init()
{
	widget_statistics_ = new QWidget();
	
	statistics_table_left_ = new QTableView();
	statistics_table_right_ = new QTableView();

	statistics_group_1 = new QGroupBox("储蓄");
	statistics_group_2 = new QGroupBox("贷款");
	statistics_group_3 = new QGroupBox("统计储蓄");
	statistics_group_4 = new QGroupBox("统计贷款");
	statistics_model_left_ = new QSqlQueryModel();
	statistics_model_right_ = new QSqlQueryModel();

	statistics_saving_start_ = new QLabel(tr("储蓄统计起始时间:"));;
	statistics_saving_end_ = new QLabel(tr("储蓄统计终止时间:"));;
	statistics_loan_start_ = new QLabel(tr("贷款统计起始时间:"));;
	statistics_loan_end_ = new QLabel(tr("贷款统计终止时间:"));;

	statistics_saving_start_line_ = new QDateEdit(QDate::currentDate());
	statistics_saving_end_line_ = new QDateEdit(QDate::currentDate());
	statistics_loan_start_line_ = new QDateEdit(QDate::currentDate());
	statistics_loan_end_line_ = new QDateEdit(QDate::currentDate());

	statistics_saving_clear_ = new QPushButton("清空");
	statistics_saving_ = new QPushButton("统计");
	statistics_loan_clear_ = new QPushButton("清空");
	statistics_loan_ = new QPushButton("统计");

	connect(statistics_saving_clear_, SIGNAL(clicked()), this, SLOT(statistics_saving_clear_button_click()));
	connect(statistics_loan_clear_, SIGNAL(clicked()), this, SLOT(statistics_loan_clear_button_click()));
	connect(statistics_saving_, SIGNAL(clicked()), this, SLOT(statistics_saving_get_button_click()));
	connect(statistics_loan_, SIGNAL(clicked()), this, SLOT(statistics_loan_get_button_click()));

	statistics_group_layout_1 = new QGridLayout(statistics_group_1);
	statistics_group_layout_2 = new QGridLayout(statistics_group_2);
	statistics_box_layout_ = new QGridLayout();
	statistics_layout_1 = new QVBoxLayout(statistics_group_3);
	statistics_layout_2 = new QVBoxLayout(statistics_group_4);

	statistics_saving_start_line_->setCalendarPopup(true);
	statistics_saving_end_line_->setCalendarPopup(true);
	statistics_loan_start_line_->setCalendarPopup(true);
	statistics_loan_end_line_->setCalendarPopup(true);

	statistics_model_left_->setQuery("select d.open_branch, e.sum_customer, d.sum_savings from\
										(select open_branch,  sum(account_balance) as sum_savings \
										from Account where account_balance >= 0 Group by open_branch) as d inner join \
										(select bank_name, count(*) as sum_customer from \
											(select a.customer_id, a.bank_name from \
											Customer_Savings_Account as a \
											left JOIN Customer_Check_Account as b \
											on a.customer_id=b.customer_id and a.bank_name=b.bank_name \
											UNION SELECT b.customer_id, b.bank_name from \
											Customer_Savings_Account as a RIGHT JOIN \
											Customer_Check_Account as b \
											on a.customer_id = b.customer_id and a.bank_name = b.bank_name) as c \
										group by c.bank_name) as e \
										on d.open_branch = e.bank_name");
	statistics_model_left_->setHeaderData(0, Qt::Horizontal, tr("支行名"));
	statistics_model_left_->setHeaderData(1, Qt::Horizontal, tr("用户数"));
	statistics_model_left_->setHeaderData(2, Qt::Horizontal, tr("储蓄总金额"));
	statistics_table_left_->setModel(statistics_model_left_);
	statistics_table_left_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	statistics_model_right_->setQuery("select j.bank_name, k.customer_num, j.sum_loan from \
										(select x.bank_name, sum(x.sum_amount) as sum_loan from \
											(select h.loan_id, h.bank_name, h.sum_amount, Customer_Loan.customer_id from \
											(select f.loan_id, Loan.bank_name, f.sum_amount from \
												(select loan_id, sum(payment_amount) as sum_amount from \
												Payment group by loan_id) as f \
												left join Loan on f.loan_id = Loan.loan_id) as h right join \
												Customer_Loan on h.loan_id = Customer_Loan.loan_id) \
											as x group by x.bank_name) as j inner join \
											(select i.bank_name, count(*) as customer_num from \
											(select DISTINCT y.bank_name, y.customer_id from \
											(select h.loan_id, h.bank_name, h.sum_amount, Customer_Loan.customer_id from \
												(select f.loan_id, Loan.bank_name, f.sum_amount from \
												(select loan_id, sum(payment_amount) as sum_amount from \
												Payment group by loan_id) as f \
											left join Loan on f.loan_id = Loan.loan_id) as h right join \
											Customer_Loan on h.loan_id = Customer_Loan.loan_id)\
										as y) as i group by i.bank_name) as k on j.bank_name = k.bank_name");
	statistics_model_right_->setHeaderData(0, Qt::Horizontal, tr("支行名"));
	statistics_model_right_->setHeaderData(1, Qt::Horizontal, tr("用户数"));
	statistics_model_right_->setHeaderData(2, Qt::Horizontal, tr("发放贷款总金额"));
	statistics_table_right_->setModel(statistics_model_right_);
	statistics_table_right_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	statistics_group_layout_1->addWidget(statistics_saving_start_, 0, 0, 1, 1);
	statistics_group_layout_1->addWidget(statistics_saving_clear_, 1, 0, 1, 2);
	statistics_group_layout_1->addWidget(statistics_saving_start_line_, 0, 1, 1, 1);
	statistics_group_layout_1->addWidget(statistics_saving_end_, 0, 2, 1, 1);
	statistics_group_layout_1->addWidget(statistics_saving_, 1, 2, 1, 2);
	statistics_group_layout_1->addWidget(statistics_saving_end_line_, 0, 3, 1, 1);
	statistics_group_layout_2->addWidget(statistics_loan_start_, 0, 0, 1, 1);
	statistics_group_layout_2->addWidget(statistics_loan_clear_, 1, 0, 1, 2);
	statistics_group_layout_2->addWidget(statistics_loan_start_line_, 0, 1, 1, 1);
	statistics_group_layout_2->addWidget(statistics_loan_end_, 0, 2, 1, 1);
	statistics_group_layout_2->addWidget(statistics_loan_, 1, 2, 1, 2);
	statistics_group_layout_2->addWidget(statistics_loan_end_line_, 0, 3, 1, 1);

	statistics_layout_1->addWidget(statistics_table_left_);
	statistics_layout_2->addWidget(statistics_table_right_);

	statistics_box_layout_->addWidget(statistics_group_1, 0, 0, 1, 1);
	statistics_box_layout_->addWidget(statistics_group_2, 0, 1, 1, 1);
	statistics_box_layout_->addWidget(statistics_group_3, 1, 0, 1, 1);
	statistics_box_layout_->addWidget(statistics_group_4, 1, 1, 1, 1);

	widget_statistics_->setLayout(statistics_box_layout_);
	statistics_table_left_->show();
	statistics_table_right_->show();

	tab_widget_->addTab(widget_statistics_, "业务统计");
}


bool Dialog::connect_database()
{
	database_ = QSqlDatabase::addDatabase("QMYSQL");					//数据库驱动类型为SQL Server
	database_.setHostName("127.0.0.1");									//选择本地主机，127.0.0.1
	database_.setPort(3306);
	database_.setUserName("Fizz");										//登录用户
	database_.setPassword("1223980961");								//密码
	database_.setDatabaseName("lab3");
	if (!database_.open())												//打开数据库
	{
		qDebug() << "error open database because" << database_.lastError().text();
		QMessageBox::critical(0, QObject::tr("Database error"), database_.lastError().text());
		return false;													//打开失败
	}
	return true;
}


void Dialog::client_clear_button_click()
{
	client_id_line_->clear();
	client_name_line_->clear();
	client_phone_line_->clear();
	client_address_line_->clear();
	client_contact_name_line_->clear();
	client_contact_phone_line_->clear();
	client_contact_email_line_->clear();
	client_contact_relationship_line_->clear();
	client_charge_id_line_->clear();
}

void Dialog::account_clear_button_click()
{
	account_id_line_->clear();
	account_balance_line_->clear();
	account_rate_line_->clear();
	account_currency_type_line_->clear();
	account_overdraft_line_->clear();
	account_opendate_line_->setDate(QDate::currentDate());
	account_client_id_line_->clear();
	account_open_branch_line_->clear();
}


void Dialog::loan_clear_button_click()
{
	loan_id_line_->clear();
	loan_branch_line_->clear();
	loan_amount_line_->clear();
	loan_client_id_line_->clear();
	loan_pay_amount_line_->clear();
	loan_paydate_line_->setDate(QDate::currentDate());
}


void Dialog::statistics_loan_clear_button_click()
{
	statistics_loan_start_line_->setDate(QDate::currentDate());
	statistics_loan_end_line_->setDate(QDate::currentDate());
}


void Dialog::statistics_saving_clear_button_click()
{
	statistics_saving_start_line_->setDate(QDate::currentDate());
	statistics_saving_end_line_->setDate(QDate::currentDate());
}


void Dialog::client_query_button_click()
{
	QString c_id = client_id_line_->text();
	QString c_name = client_name_line_->text();
	QString c_phone = client_phone_line_->text();
	QString c_address = client_address_line_->text();
	QString contact_name = client_contact_name_line_->text();
	QString contact_phone = client_contact_phone_line_->text();
	QString contact_email = client_contact_email_line_->text();
	QString relationship = client_contact_relationship_line_->text();
	QString query;

	query = "select * from customer where ";
	query = query + "customer_id LIKE '%" + c_id + "%' and ";
	query = query + "customer_name LIKE '%" + c_name + "%' and ";
	query = query + "customer_phone LIKE '%" + c_phone + "%' and ";
	query = query + "customer_address LIKE '%" + c_address + "%' and ";
	query = query + "contact_name LIKE '%" + contact_name + "%' and ";
	query = query + "contact_phone LIKE '%" + contact_phone + "%' and ";
	query = query + "contact_email LIKE '%" + contact_email + "%' and ";
	query = query + "relationship LIKE '%" + relationship + "%'";

	client_model_->setQuery(query);
	client_table_->show();
}


void Dialog::account_query_button_click()
{
	QString a_id = account_id_line_->text();
	QString a_balance = account_balance_line_->text();
	QString a_rate = account_rate_line_->text();
	QString a_currency = account_currency_type_line_->text();
	QString a_overdraft = account_overdraft_line_->text();
	QString a_opendate = account_opendate_line_->date().toString(Qt::ISODate);
	QString a_type = account_type_box_->currentText();
	QString a_client_id = account_client_id_line_->text();
	QString a_open_branch = account_open_branch_line_->text();
	QString query;

	if (a_type == "储蓄")
	{
		query = "select * from Savings_Account where ";
		query = query + "account_id LIKE '%" + a_id + "%' and ";
		if (a_balance.isEmpty() == false)
		{
			if (a_balance.toStdString()[0] == '.' || (a_balance.toStdString()[0] >= '0' && a_balance.toStdString()[0] <= '9'))
			{
				query = query + "account_balance=" + a_balance + " and ";
			}
			else
			{
				query = query + "account_balance" + a_balance + " and ";
			}
		}
		if (account_opendate_line_->date() < QDate::currentDate())
		{
			query = query + "opendate='" + a_opendate + "' and ";
			//std::cout << a_opendate.toStdString() << std::endl;
		}
		query = query + "open_branch LIKE '%" + a_open_branch + "%' and ";
		if (a_rate.isEmpty() == false)
		{
			if (a_rate.toStdString()[0] == '.' || (a_rate.toStdString()[0] >= '0' && a_rate.toStdString()[0] <= '9'))
			{
				query = query + "interest_rate=" + a_rate + " and ";
			}
			else
			{
				query = query + "interest_rate" + a_rate + " and ";
			}
		}
		query = query + "currency_type LIKE '%" + a_currency + "%'";

		account_model_left_->setQuery(query);
		account_table_left_->show();
	}
	else if (a_type == "支票")
	{
		query = "select * from Check_Account where ";
		query = query + "account_id LIKE '%" + a_id + "%' and ";
		if (a_balance.isEmpty() == false)
		{
			if (a_balance.toStdString()[0] == '.' || (a_balance.toStdString()[0] >= '0' && a_balance.toStdString()[0] <= '9'))
			{
				query = query + "account_balance=" + a_balance + " and ";
			}
			else
			{
				query = query + "account_balance" + a_balance + " and ";
			}
		}
		if (account_opendate_line_->date() < QDate::currentDate())
		{
			query = query + "opendate='" + a_opendate + "' and ";
		}
		query = query + "open_branch LIKE '%" + a_open_branch + "%' and ";
		if (a_overdraft.isEmpty() == false)
		{
			if (a_overdraft.toStdString()[0] == '.' || (a_overdraft.toStdString()[0] >= '0' && a_overdraft.toStdString()[0] <= '9'))
			{
				query = query + "overdraft=" + a_overdraft + " and ";
			}
			else
			{
				query = query + "overdraft" + a_overdraft + " and ";
			}
		}
		query.chop(5);

		account_model_right_->setQuery(query);
		account_table_right_->show();
	}
}


void Dialog::loan_query_button_click()
{
	QString l_id = loan_id_line_->text();
	QString l_branch = loan_branch_line_->text();
	QString l_amount = loan_amount_line_->text();
	QString l_client_id = loan_client_id_line_->text();
	QString l_pay_amount = loan_pay_amount_line_->text();
	QString l_paydate = loan_paydate_line_->date().toString(Qt::ISODate);
	QString query1, query2;

	query1 = "select a.loan_id, a.bank_name, a.loan_amount, a.customer_id, a.loan_status \
				from (select Loan.loan_id, bank_name, loan_amount, Customer_Loan.customer_id, loan_status \
						from Loan, Customer_Loan where Loan.loan_id = Customer_Loan.loan_id) as a where ";
	query1 = query1 + "a.loan_id LIKE '%" + l_id + "%' and ";
	query1 = query1 + "a.bank_name LIKE '%" + l_branch + "%' and ";
	if (l_amount.isEmpty() == false)
	{
		if (l_amount.toStdString()[0] == '.' || (l_amount.toStdString()[0] >= '0' && l_amount.toStdString()[0] <= '9'))
		{
			query1 = query1 + "a.loan_amount=" + l_amount + " and ";
		}
		else
		{
			query1 = query1 + "a.loan_amount" + l_amount + " and ";
		}
	}
	query1 = query1 + "a.customer_id LIKE '%" + l_client_id + "%'";
	loan_model_left_->setQuery(query1);
	loan_table_left_->show();

	query2 = "select b.loan_id, b.payment_date, b.payment_amount from Payment as b, (" + query1 + ") as c\
				where b.loan_id=c.loan_id and ";
	if (loan_paydate_line_->date() < QDate::currentDate())
	{
		query2 = query2 + "b.payment_date='" + l_paydate + "' and ";
	}
	if (l_pay_amount.isEmpty() == false)
	{
		if (l_pay_amount.toStdString()[0] == '.' || (l_pay_amount.toStdString()[0] >= '0' && l_pay_amount.toStdString()[0] <= '9'))
		{
			query2 = query2 + "b.payment_amount=" + l_pay_amount + " and ";
		}
		else
		{
			query2 = query2 + "b.payment_amount" + l_pay_amount + " and ";
		}
	}
	query2.chop(5);
	loan_model_right_->setQuery(query2);
	loan_table_right_->show();
}


void Dialog::client_add_button_click()
{
	QString c_id = client_id_line_->text();
	QString c_name = client_name_line_->text();
	QString c_phone = client_phone_line_->text();
	QString c_address = client_address_line_->text();
	QString contact_name = client_contact_name_line_->text();
	QString contact_phone = client_contact_phone_line_->text();
	QString contact_email = client_contact_email_line_->text();
	QString relationship = client_contact_relationship_line_->text();
	std::vector<QString> items_list = { c_id, c_name, c_phone , c_address, contact_name, contact_phone, contact_email, relationship };
	QString c_charge_id = client_charge_id_line_->text();
	QString c_charge_type = client_charge_type_box_->currentText();
	QSqlQuery query = client_model_->query();
	QString insert;

	if (c_id.isEmpty() || c_name.isEmpty())
	{
		QMessageBox::critical(this, "警告", "客户身份证号和客户姓名不能为空！");
		return;
	}
	for (size_t i = 0; i < items_list.size(); i++)
	{
		if (items_list[i].isEmpty() == false && items_list[i].toStdString().find('\'') != std::string::npos)
		{
			QMessageBox::critical(this, "警告", "输入条目中含有单引号！");
			return;
		}
	}

	insert = "insert into customer values (";
	for (size_t i = 0; i < items_list.size(); i++)
	{
		if (items_list[i].isEmpty())
		{
			insert = insert + "null, ";
		}
		else
		{
			insert = insert + "'" + items_list[i] + "', ";
		}
	}
	insert.chop(2);
	insert += ")";

	if (query.exec(insert) == true)
	{
		QMessageBox::information(this, "添加成功", "新增客户身份证号:"+ c_id, QMessageBox::Ok);
		client_model_->setQuery("select * from customer");
		client_table_->show();
	}
	else
	{
		QMessageBox::critical(this, "警告", "添加失败，请修改后重试！");
	}
	if (c_charge_id.isEmpty() == false)
	{
		insert = "insert into Customer_Employee_Contact values (";
		insert = insert + "'" + c_charge_id + "', ";
		insert = insert + "'" + c_id + "', ";
		insert = insert + "'" + c_charge_type + "')";
		query.exec(insert);
	}
}


void Dialog::account_add_button_click()
{
	QString a_id = account_id_line_->text();
	QString a_balance = account_balance_line_->text();
	QString a_rate = account_rate_line_->text();
	QString a_currency = account_currency_type_line_->text();
	QString a_overdraft = account_overdraft_line_->text();
	QString a_opendate = account_opendate_line_->date().toString(Qt::ISODate);
	QString a_type = account_type_box_->currentText();
	QString a_client_id = account_client_id_line_->text();
	QString a_open_branch = account_open_branch_line_->text();
	std::vector<QString> items_list = { a_id, a_balance, a_opendate, a_open_branch };
	QSqlQuery query;
	QString insert;

	if (a_id.isEmpty() || a_balance.isEmpty() || a_open_branch.isEmpty() || a_client_id.isEmpty())
	{
		QMessageBox::critical(this, "警告", "账户号、账户余额、开户支行和开户客户身份证号不能为空！");
		return;
	}
	for (size_t i = 0; i < items_list.size(); i++)
	{
		if (items_list[i].toStdString().find('\'') != std::string::npos)
		{
			QMessageBox::critical(this, "警告", "输入条目中含有单引号！");
			return;
		}
	}

	insert = "insert into Account values (";
	for (size_t i = 0; i < items_list.size(); i++)
	{
		if (i != 1)
		{
			insert = insert + "'" + items_list[i] + "', ";
		}
		else
		{
			insert = insert + items_list[i] + ", ";
		}
	}
	insert.chop(2);
	insert += ")";
	if (a_type == "储蓄")
	{
		query = account_model_left_->query();
		query.exec(insert);
		insert = "insert into Savings_Account values (";
		for (size_t i = 0; i < items_list.size(); i++)
		{
			if (i != 1)
			{
				insert = insert + "'" + items_list[i] + "', ";
			}
			else
			{
				insert = insert + items_list[i] + ", ";
			}
		}
		insert = insert + a_rate + ", ";
		insert = insert + "'" + a_currency + "')";
		query.exec(insert);

		insert = "insert into Customer_Savings_Account values (";
	}
	else if (a_type == "支票")
	{
		query = account_model_right_->query();
		query.exec(insert);
		insert = "insert into Check_Account values (";
		for (size_t i = 0; i < items_list.size(); i++)
		{
			if (i != 1)
			{
				insert = insert + "'" + items_list[i] + "', ";
			}
			else
			{
				insert = insert + items_list[i] + ", ";
			}
		}
		insert = insert + a_overdraft + ")";
		query.exec(insert);

		insert = "insert into Customer_Check_Account values (";
	}
	insert = insert + "'" + a_client_id + "', ";
	insert = insert + "'" + a_open_branch + "', ";
	insert = insert + "'" + a_id + "', ";
	insert = insert + "'" + a_opendate + "')";
	if (query.exec(insert) == true)
	{
		QMessageBox::information(this, "添加成功", "新增账户号:" + a_id, QMessageBox::Ok);
		if (a_type == "储蓄")
		{
			account_model_left_->setQuery("select * from Savings_Account");
			account_table_left_->show();
		}
		else if (a_type == "支票")
		{
			account_model_right_->setQuery("select * from Check_Account");
			account_table_right_->show();
		}
	}
	else
	{
		QMessageBox::critical(this, "警告", "添加失败，请修改后重试！");
	}
}


void Dialog::loan_add_button_click()
{
	QString l_id = loan_id_line_->text();
	QString l_branch = loan_branch_line_->text();
	QString l_amount = loan_amount_line_->text();
	QString l_status = "未开始发放";
	QString l_client_id = loan_client_id_line_->text();
	std::vector<QString> items_list = { l_id, l_branch, l_amount, l_status };
	QSqlQuery query = loan_model_left_->query();
	QString insert;

	if (l_id.isEmpty() || l_branch.isEmpty() || l_amount.isEmpty() || l_client_id.isEmpty())
	{
		QMessageBox::critical(this, "警告", "贷款号、贷款支行、贷款金额和贷款客户身份证号不能为空！");
		return;
	}
	for (size_t i = 0; i < items_list.size(); i++)
	{
		if (items_list[i].toStdString().find('\'') != std::string::npos)
		{
			QMessageBox::critical(this, "警告", "输入条目中含有单引号！");
			return;
		}
	}

	insert = "insert into Loan values (";
	for (size_t i = 0; i < items_list.size(); i++)
	{
		if (i != 2)
		{
			insert = insert + "'" + items_list[i] + "', ";
		}
		else
		{
			insert = insert + items_list[i] + ", ";
		}
	}
	insert.chop(2);
	insert += ")";
	query.exec(insert);

	insert = "insert into Customer_Loan values (";
	insert = insert + "'" + l_id + "', ";
	insert = insert + "'" + l_client_id + "')";

	if (query.exec(insert) == true)
	{
		QMessageBox::information(this, "添加成功", "新增贷款号:" + l_id, QMessageBox::Ok);
		loan_model_left_->setQuery("select Loan.loan_id, bank_name, loan_amount, Customer_Loan.customer_id, loan_status \
								from Loan, Customer_Loan where Loan.loan_id = Customer_Loan.loan_id");
		loan_table_left_->show();
	}
	else
	{
		QMessageBox::critical(this, "警告", "添加失败，请修改后重试！");
	}
}


void Dialog::loan_pay_button_click()
{
	QString l_id = loan_id_line_->text();
	QString l_pay_amount = loan_pay_amount_line_->text();
	QString l_paydate = loan_paydate_line_->date().toString(Qt::ISODate);
	QString l_status;
	QSqlQuery query("select a.sum_pay from (select loan_id, SUM(payment_amount) as sum_pay from Payment Group by loan_id) as a \
						where a.loan_id='" + l_id + "'");
	QSqlQuery query1("select loan_amount from Loan where loan_id='" + l_id + "'");
	QSqlQuery query2 = loan_model_right_->query();
	QString insert, update;

	if (l_id.isEmpty() || l_pay_amount.isEmpty())
	{
		QMessageBox::critical(this, "警告", "贷款号和发放金额不能为空！");
		return;
	}
	if (l_id.toStdString().find('\'') != std::string::npos || l_pay_amount.toStdString().find('\'') != std::string::npos)
	{
		QMessageBox::critical(this, "警告", "输入条目中含有单引号！");
		return;
	}
	query.next();
	query1.next();
	if ((query.size() == 0 && l_pay_amount.toFloat() > query1.record().value("loan_amount").toFloat()) || \
		(query.size() != 0 && (query.record().value("sum_pay").toFloat() + l_pay_amount.toFloat()) > \
								query1.record().value("loan_amount").toFloat()))
	{
		QMessageBox::critical(this, "警告", "发放总额超出贷款金额！");
		return;
	}
	else if ((query.size() == 0 && std::fabs(l_pay_amount.toFloat() - query1.record().value("loan_amount").toFloat()) <= 0.01) \
				|| (query.size() != 0 && std::fabs(query.record().value("sum_pay").toFloat() + l_pay_amount.toFloat() - \
				query1.record().value("loan_amount").toFloat()) <= 0.01))
	{
		l_status = "已全部发放";
	}
	else
	{
		l_status = "发放中";
	}

	insert = "insert into Payment values (";
	insert = insert + "'" + l_id + "', ";
	insert = insert + "'" + l_paydate + "', ";
	insert = insert + l_pay_amount + ")";

	if (query2.exec(insert) == true)
	{
		QMessageBox::information(this, "添加成功", "新发放贷款号:" + l_id + "\n发放金额:" + l_pay_amount, QMessageBox::Ok);
		loan_model_right_->setQuery("select * from Payment");
		loan_table_right_->show();
	}
	else
	{
		QMessageBox::critical(this, "警告", "添加失败，请修改后重试！");
	}

	update = "update Loan set loan_status='" + l_status + "' where loan_id='" + l_id + "'";
	loan_model_left_->query().exec(update);
	loan_model_left_->setQuery("select Loan.loan_id, bank_name, loan_amount, Customer_Loan.customer_id, loan_status \
								from Loan, Customer_Loan where Loan.loan_id = Customer_Loan.loan_id");
	loan_table_left_->show();
}


void Dialog::client_delete_button_click()
{
	QString c_id = client_id_line_->text();
	QSqlQuery query = client_model_->query();
	QSqlQuery query0("select * from customer where customer_id='" + c_id + "'");
	QSqlQuery query1("select * from Customer_Savings_Account where customer_id='" + c_id + "'");
	QSqlQuery query2("select * from Customer_Check_Account where customer_id='" + c_id + "'");
	QSqlQuery query3("select * from Customer_Loan where customer_id='" + c_id + "'");
	QString c_delete;

	if (c_id.isEmpty())
	{
		QMessageBox::critical(this, "警告", "用于删除的客户身份证号不能为空！");
		return;
	}
	if (c_id.toStdString().find('\'') != std::string::npos)
	{
		QMessageBox::critical(this, "警告", "输入条目中含有单引号！");
		return;
	}
	if (query0.size() == 0)
	{
		QMessageBox::critical(this, "警告", "待删除的客户不存在！");
		return;
	}
	if (query1.size() != 0 || query2.size() != 0 || query3.size() != 0)
	{
		QMessageBox::critical(this, "警告", "不能删除存在关联账户或贷款记录的客户！");
		return;
	}

	c_delete = "delete from customer where customer_id='" + c_id + "'";

	if (query.exec(c_delete) == true)
	{
		QMessageBox::information(this, "删除成功", "删除客户身份证号:" + c_id, QMessageBox::Ok);
		client_model_->setQuery("select * from customer");
		client_table_->show();
	}
	else
	{
		QMessageBox::critical(this, "警告", "删除失败，请修改后重试！");
	}

	c_delete = "delete from Customer_Employee_Contact where customer_id=‘" + c_id + "'";
	query.exec(c_delete);
}


void Dialog::account_delete_button_click()
{
	QString a_id = account_id_line_->text();
	QSqlQuery query;
	QSqlQuery query1("select * from Account where account_id='" + a_id + "'");
	QSqlQuery query2("select * from Savings_Account where account_id='" + a_id + "'");
	QSqlQuery query3("select * from Check_Account where account_id='" + a_id + "'");
	QString a_delete;

	if (a_id.isEmpty())
	{
		QMessageBox::critical(this, "警告", "用于删除的账户号不能为空！");
		return;
	}
	if (a_id.toStdString().find('\'') != std::string::npos)
	{
		QMessageBox::critical(this, "警告", "输入条目中含有单引号！");
		return;
	}
	if (query1.size() == 0)
	{
		QMessageBox::critical(this, "警告", "待删除的账户不存在！");
		return;
	}

	if (query2.size() != 0)
	{
		query = account_model_left_->query();
		a_delete = "delete from Customer_Savings_Account where account_id='" + a_id + "'";
		query.exec(a_delete);
		
		a_delete = "delete from Savings_Account where account_id='" + a_id + "'";
		query.exec(a_delete);
	}
	else if (query3.size() != 0)
	{
		query = account_model_right_->query();
		a_delete = "delete from Customer_Check_Account where account_id='" + a_id + "'";
		query.exec(a_delete);

		a_delete = "delete from Check_Account where account_id='" + a_id + "'";
		query.exec(a_delete);
	}
	a_delete = "delete from Account where account_id='" + a_id + "'";

	if (query.exec(a_delete) == true)
	{
		QMessageBox::information(this, "删除成功", "删除账户号:" + a_id, QMessageBox::Ok);
		if (query2.size() != 0)
		{
			account_model_left_->setQuery("select * from Savings_Account");
			account_table_left_->show();
		}
		else if (query3.size() != 0)
		{
			account_model_right_->setQuery("select * from Check_Account");
			account_table_right_->show();
		}
	}
	else
	{
		QMessageBox::critical(this, "警告", "删除失败，请修改后重试！");
	}
}


void Dialog::loan_delete_button_click()
{
	QString l_id = loan_id_line_->text();
	QSqlQuery query = loan_model_left_->query();
	QSqlQuery query1("select * from Loan where loan_id='" + l_id + "'");
	QString l_delete;

	if (l_id.isEmpty())
	{
		QMessageBox::critical(this, "警告", "用于删除的贷款号不能为空！");
		return;
	}
	if (l_id.toStdString().find('\'') != std::string::npos)
	{
		QMessageBox::critical(this, "警告", "输入条目中含有单引号！");
		return;
	}
	if (query1.size() == 0)
	{
		QMessageBox::critical(this, "警告", "待删除的贷款不存在！");
		return;
	}
	query1.next();
	if (query1.record().value("loan_status").toString() == "发放中")
	{
		QMessageBox::critical(this, "警告", "该贷款正在发放中！");
		return;
	}

	l_delete = "delete from Customer_Loan where loan_id='" + l_id + "'";
	query.exec(l_delete);

	l_delete = "delete from Payment where loan_id='" + l_id + "'";
	query.exec(l_delete);

	l_delete = "delete from Loan where loan_id='" + l_id + "'";

	if (query.exec(l_delete) == true)
	{
		QMessageBox::information(this, "删除成功", "删除贷款号:" + l_id, QMessageBox::Ok);
		loan_model_left_->setQuery("select Loan.loan_id, bank_name, loan_amount, Customer_Loan.customer_id, loan_status \
								from Loan, Customer_Loan where Loan.loan_id = Customer_Loan.loan_id");
		loan_table_left_->show();
		loan_model_right_->setQuery("select * from Payment");
		loan_table_right_->show();
	}
	else
	{
		QMessageBox::critical(this, "警告", "删除失败，请修改后重试！");
	}
}


void Dialog::client_update_button_click()
{
	QString c_id = client_id_line_->text();
	QString c_name = client_name_line_->text();
	QString c_phone = client_phone_line_->text();
	QString c_address = client_address_line_->text();
	QString contact_name = client_contact_name_line_->text();
	QString contact_phone = client_contact_phone_line_->text();
	QString contact_email = client_contact_email_line_->text();
	QString relationship = client_contact_relationship_line_->text();
	std::vector<QString> items_list = { c_id, c_name, c_phone , c_address, contact_name, contact_phone, contact_email, relationship };
	QString c_charge_id = client_charge_id_line_->text();
	QString c_charge_type = client_charge_type_box_->currentText();
	QSqlQuery query = client_model_->query();
	QSqlQuery query1("select * from customer where customer_id='" + c_id + "'");
	QString update;

	if (c_id.isEmpty())
	{
		QMessageBox::critical(this, "警告", "待修改的客户身份证号不能为空！");
		return;
	}
	for (size_t i = 0; i < items_list.size(); i++)
	{
		if (items_list[i].isEmpty() == false && items_list[i].toStdString().find('\'') != std::string::npos)
		{
			QMessageBox::critical(this, "警告", "输入条目中含有单引号！");
			return;
		}
	}
	if (query1.size() == 0)
	{
		QMessageBox::critical(this, "警告", "待修改的客户不存在！");
		return;
	}
	
	update = "update customer set ";
	if (c_name.isEmpty() == false)
	{
		update = update + "customer_name='" + c_name + "', ";
	}
	if (c_phone.isEmpty() == false)
	{
		update = update + "customer_phone='" + c_phone + "', ";
	}
	if (c_address.isEmpty() == false)
	{
		update = update + "customer_address='" + c_address + "', ";
	}
	if (contact_name.isEmpty() == false)
	{
		update = update + "contact_name='" + contact_name + "', ";
	}
	if (contact_phone.isEmpty() == false)
	{
		update = update + "contact_phone='" + contact_phone + "', ";
	}
	if (contact_email.isEmpty() == false)
	{
		update = update + "contact_email='" + contact_email + "', ";
	}
	if (relationship.isEmpty() == false)
	{
		update = update + "relationship='" + relationship + "', ";
	}

	update.chop(2);
	update = update + " where customer_id='" + c_id + "'";

	if (query.exec(update) == true)
	{
		QMessageBox::information(this, "修改成功", "被修改的客户身份证号:" + c_id, QMessageBox::Ok);
		client_model_->setQuery("select * from customer");
		client_table_->show();
	}
	else
	{
		QMessageBox::critical(this, "警告", "修改失败，请修改后重试！");
	}

	if (c_charge_id.isEmpty() == false)
	{
		update = "update Customer_Employee_Contact set charge_type='" + c_charge_type + "' where \
					employee_id='" + c_charge_id + "' and customer_id='" + c_id + "'";
		query.exec(update);
	}
}


void Dialog::account_update_button_click()
{
	QString a_id = account_id_line_->text();
	QString a_balance = account_balance_line_->text();
	QString a_rate = account_rate_line_->text();
	QString a_currency = account_currency_type_line_->text();
	QString a_overdraft = account_overdraft_line_->text();
	QString a_opendate = account_opendate_line_->date().toString(Qt::ISODate);
	QString a_type = account_type_box_->currentText();
	QString a_client_id = account_client_id_line_->text();
	QString a_open_branch = account_open_branch_line_->text();
	std::vector<QString> items_list = { a_id, a_balance, a_opendate, a_open_branch };
	QSqlQuery query;
	QSqlQuery query1("select * from Account where account_id='" + a_id + "'");
	QSqlQuery query2("select * from Savings_Account where account_id='" + a_id + "'");
	QSqlQuery query3("select * from Check_Account where account_id='" + a_id + "'");
	QString update;

	if (a_id.isEmpty() || a_client_id.isEmpty())
	{
		QMessageBox::critical(this, "警告", "待修改的账户号以及客户身份证号不能为空！");
		return;
	}
	for (size_t i = 0; i < items_list.size(); i++)
	{
		if (items_list[i].toStdString().find('\'') != std::string::npos)
		{
			QMessageBox::critical(this, "警告", "输入条目中含有单引号！");
			return;
		}
	}
	if (query1.size() == 0)
	{
		QMessageBox::critical(this, "警告", "待修改的账户不存在！");
		return;
	}

	update = "update Account set ";
	if (a_balance.isEmpty() == false)
	{
		update = update + "account_balance=" + a_balance + ", ";
	}
	if (account_opendate_line_->date() < QDate::currentDate())
	{
		update = update + "opendate='" + a_opendate + "', ";
	}
	if (a_open_branch.isEmpty() == false)
	{
		update = update + "open_branch='" + a_open_branch + "', ";
	}
	update.chop(2);
	update = update + " where account_id='" + a_id + "'";
	//std::cout << query1.size() << "   " << query2.size() << "   " << query3.size() << std::endl;
	if (query2.size() != 0)
	{
		query = account_model_left_->query();
		query.exec(update);
		
		update = "update Savings_Account set ";
		if (a_balance.isEmpty() == false)
		{
			update = update + "account_balance=" + a_balance + ", ";
		}
		if (account_opendate_line_->date() < QDate::currentDate())
		{
			update = update + "opendate='" + a_opendate + "', ";
		}
		if (a_open_branch.isEmpty() == false)
		{
			update = update + "open_branch='" + a_open_branch + "', ";
		}
		if (a_rate.isEmpty() == false)
		{
			update = update + "interest_rate=" + a_rate + ", ";
		}
		if (a_currency.isEmpty() == false)
		{
			update = update + "currency_type='" + a_currency + "', ";
		}
		update.chop(2);
		update = update + " where account_id='" + a_id + "'";
		query.exec(update);

		update = "update Customer_Savings_Account set last_visit_date_saving='";
	}
	else if (query3.size() != 0)
	{
		query = account_model_right_->query();
		query.exec(update);

		update = "update Check_Account set ";
		if (a_balance.isEmpty() == false)
		{
			update = update + "account_balance=" + a_balance + ", ";
		}
		if (account_opendate_line_->date() < QDate::currentDate())
		{
			update = update + "opendate='" + a_opendate + "', ";
		}
		if (a_open_branch.isEmpty() == false)
		{
			update = update + "open_branch='" + a_open_branch + "', ";
		}
		if (a_overdraft.isEmpty() == false)
		{
			update = update + "overdraft=" + a_overdraft + ", ";
		}
		update.chop(2);
		update = update + " where account_id='" + a_id + "'";
		query.exec(update);

		update = "update Customer_Check_Account set last_visit_date_check='";
	}

	update = update + a_opendate + "' where account_id='" + a_id + "' and customer_id='" + a_client_id + "'";

	if (query.exec(update) == true)
	{
		QMessageBox::information(this, "修改成功", "修改账户号:" + a_id, QMessageBox::Ok);
		if (query2.size() != 0)
		{
			account_model_left_->setQuery("select * from Savings_Account");
			account_table_left_->show();
		}
		else if (query3.size() != 0)
		{
			account_model_right_->setQuery("select * from Check_Account");
			account_table_right_->show();
		}
	}
	else
	{
		QMessageBox::critical(this, "警告", "修改失败，请修改后重试！");
	}
}


void Dialog::statistics_loan_get_button_click()
{
	QString s_start = statistics_loan_start_line_->date().toString(Qt::ISODate);
	QString s_end = statistics_loan_end_line_->date().toString(Qt::ISODate);
	QString query;

	if (s_start > s_end)
	{
		QMessageBox::critical(this, "警告", "统计开始时间不能比结束时间要晚！");
		return;
	}

	query = "select j.bank_name, k.customer_num, j.sum_loan from \
				(select x.bank_name, sum(x.sum_amount) as sum_loan from \
					(select h.loan_id, h.bank_name, h.sum_amount, Customer_Loan.customer_id from \
						(select f.loan_id, Loan.bank_name, f.sum_amount from \
							(select loan_id, sum(payment_amount) as sum_amount from \
								Payment where ";
	if (statistics_loan_start_line_->date() < QDate::currentDate())
	{
		query = query + "payment_date>='" + s_start + "' and ";
	}
	query = query + "payment_date<='" + s_end + "' group by loan_id) as f \
				left join Loan on f.loan_id = Loan.loan_id) as h right join \
				Customer_Loan on h.loan_id = Customer_Loan.loan_id) \
				as x group by x.bank_name) as j inner join \
			(select i.bank_name, count(*) as customer_num from \
				(select DISTINCT y.bank_name, y.customer_id from \
					(select h.loan_id, h.bank_name, h.sum_amount, Customer_Loan.customer_id from \
						(select f.loan_id, Loan.bank_name, f.sum_amount from \
							(select loan_id, sum(payment_amount) as sum_amount from \
							Payment where ";
	if (statistics_loan_start_line_->date() < QDate::currentDate())
	{
		query = query + "payment_date>='" + s_start + "' and ";
	}
	query = query + "payment_date<='" + s_end + "' group by loan_id) as f \
				left join Loan on f.loan_id = Loan.loan_id) as h right join \
				Customer_Loan on h.loan_id = Customer_Loan.loan_id) as y) as i \
			group by i.bank_name) as k on j.bank_name = k.bank_name";

	statistics_model_right_->setQuery(query);
	statistics_table_right_->show();
}


void Dialog::statistics_saving_get_button_click()
{
	QString s_start = statistics_saving_start_line_->date().toString(Qt::ISODate);
	QString s_end = statistics_saving_end_line_->date().toString(Qt::ISODate);
	QString query;

	if (s_start > s_end)
	{
		QMessageBox::critical(this, "警告", "统计开始时间不能比结束时间要晚！");
		return;
	}

	query = "select d.open_branch, e.sum_customer, d.sum_savings from \
				(select open_branch, sum(account_balance) as sum_savings from Account \
				where account_balance >= 0 and ";
	if (statistics_saving_start_line_->date() < QDate::currentDate())
	{
		query = query + "opendate>='" + s_start + "' and ";
	}
	query = query + "opendate<='" + s_end + "' Group by open_branch) as d \
				inner join (select bank_name, count(*) as sum_customer from \
					(SELECT a.customer_id, a.bank_name from \
						(select s.customer_id, s.bank_name from Customer_Savings_Account as s, Account as q \
						where ";
	if (statistics_saving_start_line_->date() < QDate::currentDate())
	{
		query = query + "q.opendate>='" + s_start + "' and ";
	}
	query = query + "q.opendate<='" + s_end + "' and s.account_id = q.account_id) as a \
			left JOIN (select r.customer_id, r.bank_name from Customer_Check_Account as r, Account as p \
				where ";
	if (statistics_saving_start_line_->date() < QDate::currentDate())
	{
		query = query + "p.opendate>='" + s_start + "' and ";
	}
	query = query + "p.opendate<='" + s_end + "' and r.account_id = p.account_id) as b \
			on a.customer_id = b.customer_id and a.bank_name = b.bank_name \
			UNION select b.customer_id, b.bank_name from \
				(select s.customer_id, s.bank_name from Customer_Savings_Account as s, Account as q \
				where ";
	if (statistics_saving_start_line_->date() < QDate::currentDate())
	{
		query = query + "q.opendate>='" + s_start + "' and ";
	}
	query = query + "q.opendate<='" + s_end + "' and s.account_id = q.account_id) as a \
			RIGHT JOIN (select r.customer_id, r.bank_name from Customer_Check_Account as r, Account as p \
			where ";
	if (statistics_saving_start_line_->date() < QDate::currentDate())
	{
		query = query + "p.opendate>='" + s_start + "' and ";
	}
	query = query + "p.opendate<='" + s_end + "' and r.account_id = p.account_id) as b \
			on a.customer_id = b.customer_id and a.bank_name = b.bank_name) as c \
			group by c.bank_name) as e on d.open_branch = e.bank_name";

	statistics_model_left_->setQuery(query);
	statistics_table_left_->show();
}


#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSqlDatabase>

class QTabWidget;
class QWidget;
class QSplitter;
class QLabel;
class QLineEdit;
class QTableView;
class QGroupBox;
class QStandardItemModel;
class QPushButton;
class QGridLayout;
class QVBoxLayout;
class QHBoxLayout;
class QComboBox;
class QDateEdit;
class QSqlDatabase;
class QSqlQueryModel;
QT_BEGIN_NAMESPACE
namespace Ui 
{ 
    class Dialog; 
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
	QSqlDatabase database_;

	QWidget* widget_client_;
	QWidget* widget_account_;
	QWidget* widget_loan_;
	QWidget* widget_statistics_;

	QTableView* client_table_;
	QTableView* account_table_left_;
	QTableView* account_table_right_;
	QTableView* loan_table_left_;
	QTableView* loan_table_right_;
	QTableView* statistics_table_left_;
	QTableView* statistics_table_right_;

	QGroupBox* client_group_1;
	QGroupBox* client_group_2;
	QGroupBox* account_group_1;
	QGroupBox* account_group_2;
	QGroupBox* account_group_3;
	QGroupBox* loan_group_1;
	QGroupBox* loan_group_2;
	QGroupBox* loan_group_3;
	QGroupBox* statistics_group_1;
	QGroupBox* statistics_group_2;
	QGroupBox* statistics_group_3;
	QGroupBox* statistics_group_4;

	//QStandardItemModel* client_model_;
	QSqlQueryModel* client_model_;
	QSqlQueryModel* account_model_left_;
	QSqlQueryModel* account_model_right_;
	QSqlQueryModel* loan_model_left_;
	QSqlQueryModel* loan_model_right_;
	QSqlQueryModel* statistics_model_left_;
	QSqlQueryModel* statistics_model_right_;
	//QStandardItemModel* account_model_left_;
	//QStandardItemModel* account_model_right_;
	//QStandardItemModel* loan_model_left_;
	//QStandardItemModel* loan_model_right_;
	//QStandardItemModel* statistics_model_left_;
	//QStandardItemModel* statistics_model_right_;

	QLabel* client_id_;
	QLabel* client_name_;
	QLabel* client_phone_;
	QLabel* client_address_;
	QLabel* client_contact_name_;
	QLabel* client_contact_phone_;
	QLabel* client_contact_email_;
	QLabel* client_contact_relationship_;
	QLabel* client_charge_id_;
	QLabel* client_charge_type_;
	QLabel* account_id_;
	QLabel* account_balance_;
	QLabel* account_opendate_;
	QLabel* account_type_;
	QLabel* account_rate_;
	QLabel* account_currency_type_;
	QLabel* account_overdraft_;
	QLabel* account_client_id_;
	QLabel* account_open_branch_;
	QLabel* loan_id_;
	QLabel* loan_branch_;
	QLabel* loan_amount_;
	QLabel* loan_client_id_;
	QLabel* loan_paydate_;
	QLabel* loan_pay_amount_;
	QLabel* statistics_saving_start_;
	QLabel* statistics_saving_end_;
	QLabel* statistics_loan_start_;
	QLabel* statistics_loan_end_;

	QLineEdit* client_id_line_;
	QLineEdit* client_name_line_;
	QLineEdit* client_phone_line_;
	QLineEdit* client_address_line_;
	QLineEdit* client_contact_name_line_;
	QLineEdit* client_contact_phone_line_;
	QLineEdit* client_contact_email_line_;
	QLineEdit* client_contact_relationship_line_;
	QLineEdit* client_charge_id_line_;
	QLineEdit* account_id_line_;
	QLineEdit* account_balance_line_;
	QLineEdit* account_rate_line_;
	QLineEdit* account_currency_type_line_;
	QLineEdit* account_overdraft_line_;
	QLineEdit* account_client_id_line_;
	QLineEdit* account_open_branch_line_;
	QLineEdit* loan_id_line_;
	QLineEdit* loan_branch_line_;
	QLineEdit* loan_amount_line_;
	QLineEdit* loan_client_id_line_;
	QLineEdit* loan_pay_amount_line_;

	QComboBox* account_type_box_;
	QComboBox* client_charge_type_box_;

	QDateEdit* account_opendate_line_;
	QDateEdit* loan_paydate_line_;
	QDateEdit* statistics_saving_start_line_;
	QDateEdit* statistics_saving_end_line_;
	QDateEdit* statistics_loan_start_line_;
	QDateEdit* statistics_loan_end_line_;

	QPushButton* client_add_;
	QPushButton* client_delete_;
	QPushButton* client_update_;
	QPushButton* client_query_;
	QPushButton* client_clear_;
	QPushButton* account_add_;
	QPushButton* account_delete_;
	QPushButton* account_update_;
	QPushButton* account_query_;
	QPushButton* account_clear_;
	QPushButton* loan_add_;
	QPushButton* loan_delete_;
	QPushButton* loan_query_;
	QPushButton* loan_clear_;
	QPushButton* loan_pay_;
	QPushButton* statistics_saving_clear_;
	QPushButton* statistics_saving_;
	QPushButton* statistics_loan_clear_;
	QPushButton* statistics_loan_;

	QHBoxLayout* tab_layout_;

	QGridLayout* client_group_layout_;
	QGridLayout* account_group_layout_;
	QGridLayout* account_box_layout_;
	QGridLayout* loan_group_layout_;
	QGridLayout* loan_box_layout_;
	QGridLayout* statistics_group_layout_1;
	QGridLayout* statistics_group_layout_2;
	QGridLayout* statistics_box_layout_;


	QVBoxLayout* client_layout_;
	QVBoxLayout* client_layout_1;
	QVBoxLayout* account_layout_1;
	QVBoxLayout* account_layout_2;
	QVBoxLayout* loan_layout_1;
	QVBoxLayout* loan_layout_2;
	QVBoxLayout* statistics_layout_1;
	QVBoxLayout* statistics_layout_2;

public slots:
	void client_clear_button_click();
	void account_clear_button_click();
	void loan_clear_button_click();
	void statistics_loan_clear_button_click();
	void statistics_saving_clear_button_click();

	void client_query_button_click();
	void account_query_button_click();
	void loan_query_button_click();

	void client_add_button_click();
	void account_add_button_click();
	void loan_add_button_click();

	void loan_pay_button_click();

	void client_delete_button_click();
	void account_delete_button_click();
	void loan_delete_button_click();

	void client_update_button_click();
	void account_update_button_click();

	void statistics_loan_get_button_click();
	void statistics_saving_get_button_click();

private:
    bool connect_database();
    void client_init();
    void account_init();
    void loan_init();
    void statistics_init();

private:
    Ui::Dialog *ui;
    QTabWidget *tab_widget_;
};
#endif // DIALOG_H
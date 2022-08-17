#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./../db/contacts.db");
    if (db.open()) {
        ui->statusbar->showMessage("Connected to DB: " + db.databaseName());

        model = new QSqlTableModel(this, db);
        model->setTable("CONTACTS");
        model->select();

        ui->tableView->setModel(model);

        for (int col=0; col<2; col++)   // set columns width
        {
           ui->tableView->setColumnWidth(col,160);
        }
    } else {
        ui->statusbar->showMessage("Error. " + db.lastError().databaseText());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnAdd_clicked()
{
    model->insertRow(model->rowCount());
}


void MainWindow::on_btnDel_clicked()
{
    model->removeRow(currentRow);
    model->select();
}


void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    currentRow = index.row();
}


void MainWindow::on_btnAdd_2_clicked()
{
        QString name = ui->nameField->text();
        QString phone = ui->phoneField->text();
        QSqlQuery query;
        bool ok = query.exec("INSERT INTO CONTACTS VALUES('" + name + "'," + phone + ")");
        if (ok) {
            ui->statusbar->showMessage("Contact added");
        } else {
            ui->statusbar->showMessage("Error!");
        }
        model->select();    // update table
}


void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (db.open()) {
        ui->statusbar->showMessage("Connected to DB: " + db.databaseName());
    } else {
        ui->statusbar->showMessage("Error. " + db.lastError().databaseText());
    }
}


void MainWindow::on_searchField_textChanged(const QString &arg1)
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QSqlQuery query;
    query.exec("SELECT * FROM CONTACTS WHERE Name LIKE '%" + arg1 + "%' OR Phone LIKE '%" + arg1 + "%'");
    model->setQuery(query);
    ui->tableView->setModel(model);
}


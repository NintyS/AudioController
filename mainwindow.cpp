#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    std::vector<QString> list = getDevices();
    for (auto x : list) {
        ui->comboBox->addItem(x);
    }

}

MainWindow::~MainWindow()
{   
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if(ui->textEdit->toPlainText() == "") {
        return;
    }

    QString content = ui->textEdit->toPlainText().remove("\n").remove(" ");

    qDebug() << content;

    QString command = "pactl load-module module-null-sink sink_name=" + content;

    qDebug() << command;

    QProcess process1;
    process1.startCommand(command);
    process1.waitForFinished();

    QString output1 = process1.readAllStandardOutput();

    output1 = output1.remove("\n");

    qDebug() << output1;

    if( output1.isEmpty() || output1.isNull() ) {
        qDebug() << "Tworzenie zlewu się wyjebało";
    }

    command = "pactl load-module module-loopback source=" + content + ".monitor sink=" + devices[comboBoxSelected].remove("\n");

    qDebug() << command;

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    QProcess process2;
    process2.startCommand(command);
    process2.waitForFinished();

    QString output2 = process2.readAllStandardOutput();

    output2 = output2.remove("\n");

    qDebug() << output2;

    if( output2.isEmpty() || output2.isNull() ) {
        qDebug() << "Tworzenie zlewu się wyjebało";
    }

    ui->listWidget->addItem(content + " " + output1 + " " + output2);

    ui->textEdit->clear();

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

}


void MainWindow::on_pushButton_2_clicked()
{

    if(selected.isEmpty() || selected.isNull()) {
        return;
    }

    QString command = "pactl unload-module " + selected.split(" ")[1];

    qDebug() << command;

    QProcess process;
    process.startCommand(command);
    process.waitForFinished();

    process.close();

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    command = "pactl unload-module " + selected.split(" ")[2];

    qDebug() << command;

    process.startCommand(command);
    process.waitForFinished();

    if( ui->listWidget->count() <= 0) {
        return;
    }

    QListWidgetItem *it = ui->listWidget->takeItem(ui->listWidget->currentRow());

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    delete it;

}


void MainWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(current == NULL) {
        return;
    }

    selected = current->text();
    qDebug() << selected;
}

QString MainWindow::substr(int pos, int count, QString str) {
    QString output;
    for(int i = pos+1; i <= count-1; i++) {
        output += str[i];
    }
    return output;
}

std::vector<QString> MainWindow::getDevices() {
    QString command = "pactl list sinks";

    qDebug() << command;

    QProcess process;
    process.startCommand(command);
    process.waitForFinished();

    QString output = process.readAllStandardOutput();

    process.close();

    QStringList line = output.replace('\t', "").replace('\"', "").split('\n');

    std::vector<QString> list;

    for (auto x : line) {
        if(x.contains("Sink #")) {
            qDebug() << substr(x.indexOf("#"), x.length(), x);
            devices.push_back(substr(x.indexOf("#"), x.length(), x));
        }

        if(x.contains("device.profile.description")) {
            qDebug() << substr(x.indexOf("=")+1, x.length(), x);
            list.push_back(substr(x.indexOf("=")+1, x.length(), x));
        }

        if(x.contains("device.alias")) {
            qDebug() << substr(x.indexOf("=")+1, x.length(), x);
            list.push_back(substr(x.indexOf("=")+1, x.length(), x));
        }
    }

    return list;
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    comboBoxSelected = index;
}


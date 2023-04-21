#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidgetItem>
#include <QMainWindow>
#include <QProcess>

#include <QObject>

#include <thread>
#include <chrono>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    QString substr(int pos = 0, int count = 0, QString str = "");

    std::vector<QString> getDevices();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

    QString selected;

    std::vector<QString> devices;

    int comboBoxSelected = 0;
};
#endif // MAINWINDOW_H

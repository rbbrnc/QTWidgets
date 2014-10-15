#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QMenu;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void onChangePage(int pageIndex);
        void on_backButton_clicked();

    private:
        Ui::MainWindow *ui;

        int m_prevPage;  // Previous GUI page
};

#endif

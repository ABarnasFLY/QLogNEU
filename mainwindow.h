#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#ifdef _WIN32               //seting default file path regarding OS
#define DEFAULT_DIR "C:/"
#elif
#define DEFAULT_DIR "/home/"
#endif
//During runtime default path is changed to recently used
#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>

#include "progresswindow.h"
#include "DataSetAnalysis.h"
#include "converterthread.h"

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
    void on_pb_browse_log_clicked();

    void on_pb_browse_pic_clicked();

    void on_pb_rin_rinex_clicked();

    void on_pb_rin_pos_clicked();

    void on_pb_rin_log_clicked();

    void on_pb_rin_pics_clicked();

    void on_pb_binToLog_open_clicked();

    void on_pb_binToLog_save_clicked();

    void on_pb_binToLog_convert_clicked();

    void on_pb_run_clicked();

    void on_pb_rin_run_clicked();

    void on_pb_reset_clicked();

    void on_pb_confirmExclusions_clicked();

    void on_table_result_cellClicked(int row, int column);

private:
    Ui::MainWindow *ui;
    ProgressWindow *m_progresWindow;
    DataAnalysis *m_analizer;
    void resizeEvent(QResizeEvent *event);
    void showResult();

    QVector<QPushButton*> m_skipCam;
    QVector<QPushButton*> m_skipPic;

    QString m_defaultDir;
    void refreshResult();
};

#endif // MAINWINDOW_H

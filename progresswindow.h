#ifndef PROGRESSWINDOW_H
#define PROGRESSWINDOW_H

#include <QDialog>
#include <QTime>
#include <QTimer>


namespace Ui {
class ProgressWindow;
}

class ProgressWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressWindow(QWidget *parent = 0);
    ~ProgressWindow();

public slots:
    void updateProgress(int value);
    void setProgresBarMaxValue(int value);
    void showMessage(QString value);
    void updateTimeLeft();
    void TimeLeft();

private:
    Ui::ProgressWindow *ui;
    QTimer *m_updateTimeLeftTimer;
    QTimer *m_timeLeftTimer;
    QTime m_timeLeft;
    int m_ticks;
};

#endif // PROGRESSWINDOW_H

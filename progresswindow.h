#ifndef PROGRESSWINDOW_H
#define PROGRESSWINDOW_H

#include <QDialog>

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

private:
    Ui::ProgressWindow *ui;
};

#endif // PROGRESSWINDOW_H

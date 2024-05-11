#ifndef INTERFACE_H
#define INTERFACE_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QMainWindow>
#include <QDialog>
#include "button.h"

class Interface: public QWidget {

    Q_OBJECT
public:
    Interface();
    QComboBox* dropdown = new QComboBox();
    bool isBuildingCorrectly(double val1, double val2, QString selected_text, QWidget& windowError);

private:

    QString    m_pending_operation;
    QLineEdit* m_first_display_up;
    QLineEdit* m_first_display_down;
    QLabel*    m_figure;
    QLabel*    m_first_label;
    QLabel*    m_second_label;
    Button*    m_create;


private slots:
    void contactingTheUser();
    void readingValues();

};

class ErrorWindow: public QDialog {
public:
    ErrorWindow(QWidget* parent);
};






#endif // INTERFACE_H

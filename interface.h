#ifndef INTERFACE_H
#define INTERFACE_H
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include "button.h"

class Interface: public QWidget {

    Q_OBJECT
public:
    Interface();
private:

    QString    m_pending_operation;
    QLineEdit* m_first_display_up;
    QLineEdit* m_first_display_down;
    QLineEdit* m_second_display_up;
    QLineEdit* m_second_display_down;
    QLabel*    m_figure;
    QLabel*    m_first_label;
    QLabel*    m_second_label;
    Button*    m_create;
    QComboBox* m_dropdown;
};

#endif // INTERFACE_H

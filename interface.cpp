#include "interface.h"
#include <QGridLayout>
#include "button.h"
#include "wall.h"
#include <QDebug>

Stack<Wall> stack;

Interface::Interface() {


    m_first_display_up    = new QLineEdit;
    m_first_display_down  = new QLineEdit;

    m_figure       = new QLabel;
    m_first_label  = new QLabel;
    m_second_label = new QLabel;


    m_first_display_up->setMaxLength(15);
    m_first_display_down->setMaxLength(15);
    QFont font = m_first_display_up->font();
    font.setPointSize(font.pointSize() + 8);
    m_first_display_up->setFont(font);
    m_first_display_down->setFont(font);

    QGridLayout* main_layout = new QGridLayout;
    setLayout(main_layout);
    main_layout->setSizeConstraint(QLayout::SetFixedSize);


    Button* create = new Button("Create");


    dropdown->addItem("Cylinder");
    dropdown->addItem("Disk");
    dropdown->setEditable(true);


    main_layout->addWidget(create,               3, 0, 1, 2);
    main_layout->addWidget(dropdown,             0, 1, 1, 1);
    main_layout->addWidget(m_figure,             0, 0, 1, 1);
    main_layout->addWidget(m_first_label,        1, 0, 1, 1);
    main_layout->addWidget(m_second_label,       2, 0, 1, 1);
    main_layout->addWidget(m_first_display_up,   1, 1, 1, 1);
    main_layout->addWidget(m_first_display_down, 2, 1, 1, 1);


    connect(dropdown, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
        QString selected_text = dropdown->itemText(index);
        if (selected_text == "Cylinder") {
            m_first_label->setText("Hight:");
            m_second_label->setText("Radius:");
        } else {
            m_first_label->setText("Outer diameter:");
            m_second_label->setText("Inner diameter:");
        }
    });
    m_figure->setText("Choose your figure:");
    m_first_label->setText("Hight:");
    m_second_label->setText("Radius:");

    setWindowTitle("Creating figures");

    Create3DModel create3dModel;
    connect(create, &QPushButton::clicked, this, &Interface::readingValues);
}

//данная функция считывает данные со строчек ввода и
//в зависимости от того какой "сurren index" выбирает куда записывать эти данные
//и экземпляр какого класса добавлять
void Interface::readingValues()
{
    int index = dropdown->currentIndex();
    bool ok1, ok2;
    int val1 = m_first_display_up->text().toInt(&ok1);
    int val2 = m_first_display_down->text().toInt(&ok2);
    if (!ok1 || !ok2) {
        qDebug() << "Ошибка ввода. Убедитесь что введены числовые значения.";
    }
    QString selected_text = dropdown->itemText(index);
    if (isBuildingCorrectly(val1, val2)) {
        if (selected_text == "Cylinder") {
            Сylinder * c1 = new Сylinder(val1, val2);
            stack.push(c1);
        } else {
            Disk * d1 = new Disk(val1, val2);
            stack.push(d1);
        }
    } else {
        qDebug() << "Некоректно построена фигура, проверьте чего вам не хватает";
        }
}

bool Interface::isBuildingCorrectly(int val1,int val2)
{
    return true;
}

void Interface::contactingTheUser()
{

}

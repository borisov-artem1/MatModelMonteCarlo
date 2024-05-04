#include "interface.h"
#include <QGridLayout>
#include "button.h"
#include "wall.h"
#include <QDebug>
#include "exception.h"

Stack<Wall> stack;
QVector<Wall*> vector;

Interface::Interface() {


    m_first_display_up   = new QLineEdit;
    m_first_display_down = new QLineEdit;

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


    Button* create = new Button("Create", this);


    dropdown->addItem("Cylinder");
    dropdown->addItem("Disk");
    dropdown->setEditable(false);


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
    connect(create, &QPushButton::clicked, this, &Interface::readingValues);
    m_figure->setText("Choose your figure:");
    m_first_label->setText("Hight:");
    m_second_label->setText("Radius:");

    setWindowTitle("Creating figures");


    connect(create, SIGNAL(clicked), this, SLOT(Interface::readingValues));
    connect(create, &QPushButton::clicked, this, &Interface::readingValues);

}

//данная функция считывает данные со строчек ввода и
//в зависимости от того какой "сurrent index" выбирает куда записывать эти данные
//и экземпляр какого класса добавлять
void Interface::readingValues()
{

    int index = dropdown->currentIndex();
    bool ok1, ok2;
    double val1 = m_first_display_up->text().toDouble(&ok1);
    double val2 = m_first_display_down->text().toDouble(&ok2);
    if (!ok1 || !ok2) {
        throw InvalidDisplayInput();
    }
    QString selected_text = dropdown->itemText(index);

    if (isBuildingCorrectly(val1, val2, selected_text)) {
        if (selected_text == "Cylinder") {
            Сylinder* cylinder = new Сylinder(val1, val2);
            stack.push(cylinder);
            vector.push_back(cylinder);
            qDebug() << "Cylinder" << Qt::endl;
        } else if (selected_text == "Disk") {
            Disk* disk = new Disk(val1, val2);
            stack.push(disk);
            vector.push_back(disk);
            qDebug() << "Disk" << Qt::endl;
        } else {
            throw IncorrectFigure();
        }
    }
}

bool Interface::isBuildingCorrectly(const double val1, const double val2, const QString selected_text) {
    if (val1 < 0 || val2 < 0) {
        return false;
    }
    QString name = stack.top()->name;
    if (name == "Cylinder") {
        if ((selected_text == "Cylinder" && val1 == stack.top()->radiusOutside) ||
                (selected_text == "Disk" && val1 == stack.top()->radiusOutside) ||
                (selected_text == "Disk" && val2 == stack.top()->radiusInside)) {
            return true;
        } else {
            return false;
        }
    } else if (name == "Disk") {
        if (selected_text == "Cylinder" && (val1 == stack.top()->radiusInside || val1 == stack.top()->radiusOutside)) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

void Interface::contactingTheUser() {

}



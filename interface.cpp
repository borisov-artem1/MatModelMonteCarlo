#include "interface.h"
#include <QGridLayout>
#include "button.h"
#include "wall.h"
#include <QDebug>
#include <QMessageBox>
#include "exception.h"

std::stack<Wall*> stack;
QVector<Wall*> vector;
Generator generator;


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


}

//ErrorWindow::ErrorWindow(QWidget* parent): QDialog(parent) {}


//данная функция считывает данные со строчек ввода и
//в зависимости от того какой "сurrent index" выбирает куда записывать эти данные
//и экземпляр какого класса добавлять
void Interface::readingValues()
{
    QWidget windowError;
    windowError.move(300, 300);
    int index = dropdown->currentIndex();
    bool ok1, ok2;
    int count = -1;
    double val1 = m_first_display_up->text().toDouble(&ok1);
    double val2 = m_first_display_down->text().toDouble(&ok2);
    if (!ok1 || !ok2) {
        QMessageBox::critical(&windowError, "Error", "Empty line edit");
        return;
    }
    QString selected_text = dropdown->itemText(index);

    if (isBuildingCorrectly(val1, val2, selected_text, windowError)) {
        if (selected_text == "Cylinder") {
            Сylinder* cylinder = new Сylinder(val2, val1);
            stack.push(cylinder);
            vector.push_back(cylinder);
            count++;
            qDebug() << "Cylinder" << Qt::endl;
        } else if (selected_text == "Disk") {
            Disk* disk = new Disk(val1, val2);
            stack.push(disk);
            vector.push_back(disk);
            count++;
            generator.DownOrUp(count);
            qDebug() << "Disk" << Qt::endl;
        } else {
            QMessageBox::critical(&windowError, "Error", selected_text + "is incorrect figure");
        }
    }
    generator.CreatingPortal();
}

bool Interface::isBuildingCorrectly(double val1, double val2, const QString selected_text, QWidget &windowError) {
    //QWidget windowError;
    if (val1 < 0 || val2 < 0) {
        QMessageBox::critical(&windowError, "Error", "Input value is negative");
        return false;
    }
    if (selected_text == "Disk" && val1 < val2) {
        QMessageBox::critical(&windowError, "Error", "Invalid input value");
        return false;
    }
    if (stack.size() == 0) {
        return true;
    }

    QString name = stack.top()->name;
    Wall* topWall = stack.top();
    if (name == "Cylinder") {
        Сylinder* cylinder = dynamic_cast<Сylinder*>(topWall);
        bool a1 = selected_text == "Cylinder";
        bool a2 = val2 == cylinder->radiusOutsideCylinder;
        if (a1 && a2) {
            return true;
        } else if ((selected_text == "Disk" && val1 == cylinder->radiusOutsideCylinder) ||
            (selected_text == "Disk" && val2 == cylinder->radiusOutsideCylinder)) {
               return true;
        } else {
               QMessageBox::critical(&windowError, "Error", "Invalid figure or parametr");
               return false;
        }
    } else if (name == "Disk") {
        Disk* disk = dynamic_cast<Disk*>(topWall);
        if (selected_text == "Cylinder" && (val1 == disk->radiusInsideDisk || val1 == disk->radiusOutsideDisk)) {
            return true;
        } else {
            QMessageBox::critical(&windowError, "Error", "Invalid figure or parametr");
            return false;
        }
    } else {
        QMessageBox::critical(&windowError, "Error", "Invalid figure");
        return false;
    }
}

void Interface::contactingTheUser() {

}

void Generator::CreatingPortal()
{
    if (vector[1]->name == "Cylinder")
    {
        Сylinder* cylinder = dynamic_cast<Сylinder *>(vector[1]);
        Disk* disk = new Disk(cylinder->radiusOutsideCylinder, 0);
        vector.push_front(disk);
    }
    if (vector.back()->name == "Cylinder")
    {
        Сylinder* cylinder = dynamic_cast<Сylinder *>(vector.back());
        Disk* disk = new Disk(cylinder->radiusOutsideCylinder, 0);
        vector.push_back(disk);
    }
}

void Generator::DownOrUp(int count)
{
    if (count == 0){
        Disk* disk = dynamic_cast<Disk *>(vector[count]);
        if (disk != nullptr) {
            disk->down = false;
        }
    }
    Сylinder* previousCylinder = dynamic_cast<Сylinder *>(vector[count-1]);
    Disk* disk = dynamic_cast<Disk*>(vector[count]);
    if (previousCylinder != nullptr) {
        if (disk->radiusInsideDisk == previousCylinder->radiusOutsideCylinder){
            disk->down = false;
        } else {
            disk->down = true;
        }
    }
}




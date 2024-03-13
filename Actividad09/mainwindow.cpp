#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "mainwindow.h"
#include <iostream>
#include <sstream>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QString>

void buscarNeuronaGlobal(const std::list<Neurona>& listaNeuronas, QTableWidget* tablaNeuronas, QLineEdit* textoBusqueda) {
    QString idBuscado = textoBusqueda->text();

    for (int row = 0; row < tablaNeuronas->rowCount(); row++) {
        QTableWidgetItem* item = tablaNeuronas->item(row, 0);
        if (item && item->text() == idBuscado) {
            tablaNeuronas->setRowHidden(row, false);
        } else {
            tablaNeuronas->setRowHidden(row, true);
        }
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // spinboxes
    id_spinBox = findChild<QSpinBox*>("id_spinBox");
    voltaje_spinBox = findChild<QDoubleSpinBox*>("voltaje_spinBox");
    posicionX_spinBox = findChild<QSpinBox*>("posicionX_spinBox");
    posicionY_spinBox = findChild<QSpinBox*>("posicionY_spinBox");
    red_spinBox = findChild<QSpinBox*>("red_spinBox");
    green_spinBox = findChild<QSpinBox*>("green_spinBox");
    blue_spinBox = findChild<QSpinBox*>("blue_spinBox");

    // otros elementos
    output = findChild<QTextEdit*>("output");
    id_search = findChild<QLineEdit*>("id_search");
    tablaNeuronas = findChild<QTableWidget*>("tablaNeuronas");
    lienzo = findChild<QGraphicsView*>("lienzo");

    // otras acciones
    connect(id_search, &QLineEdit::textChanged, this, &MainWindow::on_id_search_textChanged);
    scene = new QGraphicsScene(this);
    lienzo->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_id_search_textChanged(const QString &text)
{
    // Llama a la función global de búsqueda
    buscarNeuronaGlobal(lista.getLista(), tablaNeuronas, id_search);
}

Neurona::Neurona(int _id, float _voltaje, int _posicion_x, int _posicion_y, int _red, int _green, int _blue) {
    id = _id;
    voltaje = _voltaje;
    posicion_x = _posicion_x;
    posicion_y = _posicion_y;
    red = _red;
    green = _green;
    blue = _blue;
}

Neurona::Neurona() {}

Neurona::~Neurona() {}

void Neurona::print() {
    std::cout << "NEURONA " << id << std::endl << std::endl
              << "voltaje: " << voltaje << std::endl
              << "posicion x: " << posicion_x << std::endl
              << "posicion y" << posicion_y << std::endl
              << "red: " << red << std::endl
              << "green: " << green << std::endl
              << "blue: " << blue << std::endl
              << std::endl;
}

int Neurona::get_id() const { return id; }
float Neurona::get_voltaje() const { return voltaje; }
int Neurona::get_posicionX() const { return posicion_x; }
int Neurona::get_posicionY() const { return posicion_y; }
int Neurona::get_red() const { return red; }
int Neurona::get_green() const { return green; }
int Neurona::get_blue() const { return blue; }
void Neurona::set_id(int _id) { id = _id; }
void Neurona::set_voltaje(float _voltaje) { voltaje = _voltaje; }
void Neurona::set_posicionX(int _posicionX) { posicion_x = _posicionX; }
void Neurona::set_posicionY(int _posicionY) { posicion_y = _posicionY; }
void Neurona::set_red(int _red) { red = _red; }
void Neurona::set_green(int _green) { green = _green; }
void Neurona::set_blue(int _blue) { blue = _blue; }

void ListaNeuronas::insertar_inicio(const Neurona& neurona) {
    lista.push_front(neurona);
}

void ListaNeuronas::insertar_final(const Neurona& neurona) {
    lista.push_back(neurona);
}

std::string ListaNeuronas::mostrar() {
    std::stringstream ss; // Crear un stringstream para construir la cadena
    for (std::list<Neurona>::iterator it = lista.begin(); it != lista.end(); ++it) {
        int id_actual = it->get_id();
        float voltaje_actual = it->get_voltaje();
        int posicion_x_actual = it->get_posicionX();
        int posicion_y_actual = it->get_posicionY();
        int red_actual = it->get_red();
        int green_actual = it->get_green();
        int blue_actual = it->get_blue();

        // Agregar los elementos a la cadena
        ss << "Neurona " << id_actual << std::endl
           << "  voltaje: " << voltaje_actual << std::endl
           << "  posicion x: " << posicion_x_actual << std::endl
           << "  posicion y: " << posicion_y_actual << std::endl
           << "  red: " << red_actual << std::endl
           << "  green: " << green_actual << std::endl
           << "  blue: " << blue_actual << std::endl << std::endl;

    }
    return ss.str();
}


std::list<Neurona> ListaNeuronas::getLista() {
    return lista;
}

void MainWindow::on_submit_start_clicked()
{
    Neurona n;
    n.set_id(id_spinBox->value());
    n.set_voltaje(voltaje_spinBox->value());
    n.set_posicionX(posicionX_spinBox->value());
    n.set_posicionY(posicionY_spinBox->value());
    n.set_red(red_spinBox->value());
    n.set_green(green_spinBox->value());
    n.set_blue(blue_spinBox->value());

    lista.insertar_inicio(n);

     // Aqui se integra el codigo para agregar las neuronas a la tabla
    tablaNeuronas->clearContents();
    tablaNeuronas->setRowCount(0);

    tablaNeuronas->setColumnCount(7);
    tablaNeuronas->setHorizontalHeaderLabels({"ID", "Voltaje", "PosX", "PosY", "Red", "Green", "Blue"});

    for (const Neurona& neurona : lista.getLista()) {

        int row = tablaNeuronas->rowCount();
        tablaNeuronas->insertRow(row);

        tablaNeuronas->setItem(row, 0, new QTableWidgetItem(QString::number(neurona.get_id())));
        tablaNeuronas->setItem(row, 1, new QTableWidgetItem(QString::number(neurona.get_voltaje())));
        tablaNeuronas->setItem(row, 2, new QTableWidgetItem(QString::number(neurona.get_posicionX())));
        tablaNeuronas->setItem(row, 3, new QTableWidgetItem(QString::number(neurona.get_posicionY())));
        tablaNeuronas->setItem(row, 4, new QTableWidgetItem(QString::number(neurona.get_red())));
        tablaNeuronas->setItem(row, 5, new QTableWidgetItem(QString::number(neurona.get_green())));
        tablaNeuronas->setItem(row, 6, new QTableWidgetItem(QString::number(neurona.get_blue())));
    }

    tablaNeuronas->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

}


void MainWindow::on_submit_end_clicked()
{
    Neurona n;
    n.set_id(id_spinBox->value());
    n.set_voltaje(voltaje_spinBox->value());
    n.set_posicionX(posicionX_spinBox->value());
    n.set_posicionY(posicionY_spinBox->value());
    n.set_red(red_spinBox->value());
    n.set_green(green_spinBox->value());
    n.set_blue(blue_spinBox->value());

    lista.insertar_final(n);

    // Aqui se integra el codigo para agregar las neuronas a la tabla
    tablaNeuronas->clearContents();
    tablaNeuronas->setRowCount(0);

    tablaNeuronas->setColumnCount(7);
    tablaNeuronas->setHorizontalHeaderLabels({"ID", "Voltaje", "PosX", "PosY", "Red", "Green", "Blue"});

    for (const Neurona& neurona : lista.getLista()) {

        int row = tablaNeuronas->rowCount();
        tablaNeuronas->insertRow(row);

        tablaNeuronas->setItem(row, 0, new QTableWidgetItem(QString::number(neurona.get_id())));
        tablaNeuronas->setItem(row, 1, new QTableWidgetItem(QString::number(neurona.get_voltaje())));
        tablaNeuronas->setItem(row, 2, new QTableWidgetItem(QString::number(neurona.get_posicionX())));
        tablaNeuronas->setItem(row, 3, new QTableWidgetItem(QString::number(neurona.get_posicionY())));
        tablaNeuronas->setItem(row, 4, new QTableWidgetItem(QString::number(neurona.get_red())));
        tablaNeuronas->setItem(row, 5, new QTableWidgetItem(QString::number(neurona.get_green())));
        tablaNeuronas->setItem(row, 6, new QTableWidgetItem(QString::number(neurona.get_blue())));
    }

    tablaNeuronas->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

}


void MainWindow::on_show_neurons_clicked()
{
    output->clear();
    QString cadena_qstring = QString::fromStdString(lista.mostrar());
    output->insertPlainText(cadena_qstring);

     // Aqui se integra el codigo para agregar las neuronas a la tabla
    tablaNeuronas->clearContents();
    tablaNeuronas->setRowCount(0);

    tablaNeuronas->setColumnCount(7);
    tablaNeuronas->setHorizontalHeaderLabels({"ID", "Voltaje", "PosX", "PosY", "Red", "Green", "Blue"});

    for (const Neurona& neurona : lista.getLista()) {

        int row = tablaNeuronas->rowCount();
        tablaNeuronas->insertRow(row);

        tablaNeuronas->setItem(row, 0, new QTableWidgetItem(QString::number(neurona.get_id())));
        tablaNeuronas->setItem(row, 1, new QTableWidgetItem(QString::number(neurona.get_voltaje())));
        tablaNeuronas->setItem(row, 2, new QTableWidgetItem(QString::number(neurona.get_posicionX())));
        tablaNeuronas->setItem(row, 3, new QTableWidgetItem(QString::number(neurona.get_posicionY())));
        tablaNeuronas->setItem(row, 4, new QTableWidgetItem(QString::number(neurona.get_red())));
        tablaNeuronas->setItem(row, 5, new QTableWidgetItem(QString::number(neurona.get_green())));
        tablaNeuronas->setItem(row, 6, new QTableWidgetItem(QString::number(neurona.get_blue())));
    }

    tablaNeuronas->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}


void MainWindow::on_actionAbrir_triggered()
{

    QString nombreArchivo = QFileDialog::getOpenFileName(nullptr, "Abrir archivo", "", "Archivos de texto (*.txt)");
    if (nombreArchivo.isEmpty()) {
        // El usuario canceló la selección del archivo
        return;
    }

    QFile archivo(nombreArchivo);
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo para leer.";
        return;
    }

    QTextStream in(&archivo);
    // Ignoramos la primera línea, que contiene los encabezados
    in.readLine();

    while (!in.atEnd()) {
        QString linea = in.readLine();
        QStringList partes = linea.split(',');
        if (partes.size() == 7) {
            int id = partes[0].toInt();
            float voltaje = partes[1].toFloat();
            int posicionX = partes[2].toInt();
            int posicionY = partes[3].toInt();
            int red = partes[4].toInt();
            int green = partes[5].toInt();
            int blue = partes[6].toInt();
            Neurona neurona(id, voltaje, posicionX, posicionY, red, green, blue);
            this->lista.insertar_final(neurona);
        } else {
            qDebug() << "Error al leer la línea del archivo:" << linea;
        }
    }

    archivo.close();
    on_show_neurons_clicked();
}

void MainWindow::on_actionGuardar_triggered()
{
    ListaNeuronas& lista = this->lista;

    QString nombreArchivo = QFileDialog::getSaveFileName(nullptr, "Guardar archivo", "", "Archivos de texto (*.txt)");
    if (nombreArchivo.isEmpty()) {
        // El usuario canceló la selección del archivo
        return;
    }

    // Verificar si el nombre de archivo ya tiene la extensión .txt
    if (!nombreArchivo.endsWith(".txt", Qt::CaseInsensitive)) {
        // Agregar automáticamente la extensión .txt al nombre de archivo
        nombreArchivo += ".txt";
    }

    QFile archivo(nombreArchivo);
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el archivo para escribir.";
        return;
    }

    QTextStream out(&archivo);
    out << "id,voltaje,posicion_x,posicion_y,red,green,blue\n";

    for (const Neurona& neurona : lista.getLista()) {
        out << neurona.get_id() << ","
            << neurona.get_voltaje() << ","
            << neurona.get_posicionX() << ","
            << neurona.get_posicionY() << ","
            << neurona.get_red() << ","
            << neurona.get_green() << ","
            << neurona.get_blue() << "\n";
    }

    archivo.close();
}

void MainWindow::on_pushButton_clicked()
{
    ListaNeuronas& lista = this->lista;

    scene->clear();

    for (const Neurona& neurona : lista.getLista()) {
        QPen pen;
        QColor c(neurona.get_red(), neurona.get_green(), neurona.get_blue());
        pen.setColor(c);
        QBrush Bru(c, Qt::SolidPattern);

        scene->addEllipse(neurona.get_posicionX(), neurona.get_posicionY(), neurona.get_voltaje(), neurona.get_voltaje(), pen, Bru);
    }

    lienzo->show();
    std::cout << "Hola!" << std::endl;
}


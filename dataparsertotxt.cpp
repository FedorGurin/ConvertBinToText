#include "dataparsertotxt.h"
#include "ui_dataparsertotxt.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QLibrary>

#include "./mppm/libmppm.h"
#include "./mppm/node.h"
#include "./mppm/libmppmSpec.h"

typedef IEngineData* (*CreateEngine)();
typedef void (*CpyMemToTreeSerialLib)(IEngineData* e,Node* toNode, QByteArray *fromMem);
typedef void (*ConvertValueTreeToStrings)(Node* root, QStringList &list);
typedef void (*ConvertTitleTreeToStrings)(Node* root,QStringList &listName,QStringList &listMes);

CpyMemToTreeSerialLib cpyMemToTree;
ConvertValueTreeToStrings convValToStrings;
ConvertTitleTreeToStrings convTitleToStrings;

dataparsertotxt::dataparsertotxt(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::dataparsertotxt)
{
    ui->setupUi(this);

    setWindowTitle(QString("Конвертер данных..."));
    setWindowIcon(QIcon(":/paste"));

    ui->textEdit->setVisible(false);
    ui->pushButton_open_bin->setIcon(QIcon(":/file_open"));
    ui->pushButton_open_bin->setToolTip(QString("Открыть..."));
    ui->pushButton_open_xml->setIcon(QIcon(":/file_open"));
    ui->pushButton_open_xml->setToolTip(QString("Открыть..."));
    resize(493, 100);

    ui->pushButton_preview->setVisible(false);

    name_newFile = QString("result");
    ui->lineEdit_result->setText(name_newFile);

#ifdef QT_DEBUG
    QLibrary libMPPM("libmppmd");
#else
    QLibrary libMPPM("libmppm");
#endif

    CreateEngine func = reinterpret_cast<CreateEngine > (libMPPM.resolve("createEngine"));
    if(func == nullptr)
    {
        QMessageBox::warning(this, tr("Внимание!"),
                             tr("libMPPM: (CreateEngine)libMPPM.resolve(\"createEngine\") = 0. \n"
                                "Библиотека libMPPM не подключена/не загружена"),
                             QMessageBox::Ok);

    }
    engine = func();
    cpyMemToTree = reinterpret_cast<CpyMemToTreeSerialLib > (libMPPM.resolve("cpyMemToTreeSerialLib"));
    convValToStrings = reinterpret_cast<ConvertValueTreeToStrings > (libMPPM.resolve("convertValueTreeToStrings"));
    convTitleToStrings = reinterpret_cast<ConvertTitleTreeToStrings > (libMPPM.resolve("convertTitleTreeToStrings"));


//    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));


}

dataparsertotxt::~dataparsertotxt()
{
    delete ui;
}

void dataparsertotxt::on_pushButton_open_bin_clicked()
{

    QStringList files = QFileDialog::getOpenFileNames(
                            this,
                            "Выберите файл...",
                            QApplication::applicationDirPath(),
                            "Binary files (*.bin)");
    if(!files.isEmpty())
        ui->lineEdit_path_bin->setText(files.at(0));
}

void dataparsertotxt::on_pushButton_open_xml_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames(
                            this,
                            "Выберите файл...",
                            QApplication::applicationDirPath(),
                            "XML files (*.xml)");
    if(!files.isEmpty())
        ui->lineEdit_path_xml->setText(files.at(0));
}

void dataparsertotxt::on_pushButton_cancel_clicked()
{
    close();
}

void dataparsertotxt::on_pushButton_preview_clicked()
{
    if(!ui->lineEdit_path_xml->text().isEmpty() && !ui->lineEdit_path_bin->text().isEmpty())
    {
        ui->textEdit->setVisible(true);
    }
}

void dataparsertotxt::on_pushButton_save_to_file_clicked()
{
    if(ui->lineEdit_path_xml->text().isEmpty() || ui->lineEdit_path_bin->text().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Есть незаполненные поля ввода.");
        msgBox.exec();
    }
    else if(ui->lineEdit_result->text().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Введите название создаваемого файла.");
        msgBox.exec();
    }
    else
    {
        QFile file_xml(ui->lineEdit_path_xml->text());
        QFile binfile(ui->lineEdit_path_bin->text());
        if(!file_xml.exists() || !binfile.exists())
        {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            if(file_xml.exists())
                msgBox.setText("Указан некорректный файл данных.");
            else if(binfile.exists())
                msgBox.setText("Указан некорректный файл со структурой данных.");
            else
                msgBox.setText("Указаны некорректные файлы.");
            msgBox.exec();
        }
        else
        {
            //разбор .xml
            Node* node = loadFileXmlSerialLib(ui->lineEdit_path_xml->text());

            //Запись в текстовый файл
            QFile binfile(ui->lineEdit_path_bin->text());
            if(binfile.open(QIODevice::ReadOnly))
            {
                quint16 value;
                                  quint8 val8;
                                  //считывание заголовка
                                  QDataStream in(&binfile);
                                  in >> recordheader.id_module >> recordheader.id_object >> recordheader.id_struct
                                          >> recordheader.t0 >> recordheader.delta_t >> recordheader.sizeOfStruct >>value >>val8>>val8;
                                  in.readRawData(recordheader.idName,sizeof(recordheader.idName));


                if(!name_newFile.contains(".txt"))
                    name_newFile = name_newFile + ".txt";
                QFile txtfile(QString(QApplication::applicationDirPath()+"/"+name_newFile));
                txtfile.open(QIODevice::ReadWrite | QIODevice::Text);
                if(txtfile.isOpen()== false)
                {
                    QMessageBox msgBox;
                    msgBox.setText("Не удалось открыть файл");
                    msgBox.exec();
                }

                QTextStream out(&txtfile);
                out << QString("Time;");
                QStringList listTitle;
                QStringList listMes;
                convertTitleTreeToStrings(node, listTitle, listMes);
                for(int i = 0; i < listTitle.size(); i++)
                {
                    //считваем размерность
                    out << listTitle.at(i)
                        << QString("(") << recordheader.id_object << QString(")")
                        << QString("[") << listMes.at(i) << QString("]")
                        << QString(";");
                }
                out << QString('\n');

//               /* if(txtfile.open(QIODevice::Append))
//                {
                    //char* ch;
                    QByteArray ba;
                    ba.resize(recordheader.sizeOfStruct);
                    //считывания блока памяти
                    int inc = 0;
                    while (!binfile.atEnd())
                    {
                        in >> ba;
                        cpyMemToTreeSerialLib(node, &ba);
                        QStringList list = convertValueTreeToStrings(node);
                        out << recordheader.t0+inc*recordheader.delta_t << QString(";");
                        for(int i = 0; i < list.size(); i++)
                        {
                            out << list.at(i) << QString(";");
                        }
                        out << QString('\n');
                        out.flush();
                        inc++;
                    }
//                }
                binfile.close();
                txtfile.flush();
                txtfile.close();
                ui->statusBar->showMessage(QString("Создан файл "+txtfile.fileName()));
            }
        }
    }
}

void dataparsertotxt::on_lineEdit_result_textChanged(const QString &arg1)
{
    name_newFile = arg1;
}

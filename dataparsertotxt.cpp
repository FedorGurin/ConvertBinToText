#include "dataparsertotxt.h"
#include "ui_dataparsertotxt.h"
#include <QFileDialog>
#include <QFile>
//#include "parserMemDCS.h"
#include <QTextStream>
#include <QLibrary>

#include "./mppm/IEngineData.h"
#include "./mppm/libmppmSpec.h"

typedef IEngineData* (*CreateEngine)();
typedef Node* (*FindNode)(IEngineData* , QString idName);

FindNode fNode;

dataparsertotxt::dataparsertotxt(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::dataparsertotxt)
{
    ui->setupUi(this);

    setWindowTitle(QString("Конвертер данных..."));
    setWindowIcon(QIcon(":/paste"));

    ui->pushButton_open_bin->setIcon(QIcon(":/file_open"));
    ui->pushButton_open_bin->setToolTip(QString("Открыть..."));
    //ui->pushButton_open_xml->setIcon(QIcon(":/file_open"));
    //ui->pushButton_open_xml->setToolTip(QString("Открыть..."));
    ui->pushButton_path_result->setIcon(QIcon(":/file_open"));
    resize(493, 100);

    result_files.clear();

//    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));


#ifdef QT_DEBUG
    QLibrary libMPPM("./libmppmd");
#else
    QLibrary libMPPM("./libmppm");
#endif

    CreateEngine func = reinterpret_cast<CreateEngine > (libMPPM.resolve("createEngineSpec"));
    fNode = reinterpret_cast<FindNode > (libMPPM.resolve("findNode"));
    if(func == nullptr)
    {
        QMessageBox::warning(this, tr("Внимание!"),
                             tr("libMPPM: (CreateEngine)libMPPM.resolve(\"createEngine\") = 0. \n"
                                "Библиотека libMPPM не подключена/не загружена"),
                             QMessageBox::Ok);

    }
    engine = static_cast<IEngineData*> (func());

    //initXmlSerialLib();

    path_result = curr_dir_bin = curr_dir_xml = QString(QApplication::applicationDirPath());
    ui->lineEdit_path_result->setText(path_result);

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
                            curr_dir_bin,
                            "Binary files (*.bin)");
    if(!files.isEmpty())
    {        
        bin_files = files;
        result_files.clear();

        QString str;
        str.clear();
        for(int i = 0; i < files.at(0).lastIndexOf(QString("/")); i++)
        {
            str += files.at(0).at(i);
        }
        curr_dir_bin = str;
        for(int i = 0; i < files.size(); i++)
        {
            ui->lineEdit_path_bin->setText(ui->lineEdit_path_bin->text()+files.at(i)+"; ");
            str = "result_";
            for(int j = files.at(i).lastIndexOf(QString("/"))+1; j < bin_files.at(i).lastIndexOf(QString(".bin")); j++)
            {
                str += bin_files.at(i).at(j);
            }
            str +=".txt";
            result_files.append(str);
        }
    }
}



void dataparsertotxt::on_pushButton_save_to_file_clicked()
{
    if(ui->lineEdit_path_bin->text().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setWindowIcon(QIcon(":/paste"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Есть незаполненные поля ввода.");
        msgBox.exec();
    }
    else
    {


            for(int i = 0; i < result_files.size(); i++)
            {
                QFile binfile(bin_files.at(i));
                if(!binfile.exists())
                {
                    QMessageBox msgBox;
                    msgBox.setWindowIcon(QIcon(":/paste"));
                    msgBox.setIcon(QMessageBox::Warning);
                    msgBox.setText("Указан некорректный файл данных.");
                    msgBox.exec();
                }

                QFile txtfile(path_result + "/" + result_files.at(i));
                if(txtfile.exists())
                {
                    int j = 1;
                    QMessageBox msgBox;
                    msgBox.setWindowIcon(QIcon(":/paste"));
                    msgBox.setIcon(QMessageBox::Question);
                    msgBox.setText(QString("Файл c именем %1 уже существует.").arg(txtfile.fileName()));
                    msgBox.setInformativeText("Хотите заменить его?");
                    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    msgBox.setButtonText(QMessageBox::Yes, "Да");
                    msgBox.setButtonText(QMessageBox::No, "Нет");
                    msgBox.setDefaultButton(QMessageBox::Ok);
                    int ret = msgBox.exec();
                    switch (ret)
                    {
                    case QMessageBox::Yes:
                        break;
                    case QMessageBox::No:
                    default:
                        int pos = result_files.at(i).lastIndexOf(QString(".txt"));
                        QString str = result_files[i];
                        str.insert(pos, QString("(%1)").arg(j));
                        txtfile.setFileName(path_result + "/" + str);

                        while(txtfile.exists())
                        {
                            j++;
                            int pos = result_files.at(i).lastIndexOf(QString(".txt"));
                            QString str = result_files[i];
                            str.insert(pos, QString("(%1)").arg(j));
                            txtfile.setFileName(path_result + "/" + str);
                        }
                    }
                }

                txtfile.open(QIODevice::WriteOnly);
                txtfile.close();
                //разбор .xml
                //Node* node =fNode(engine,"tk");//loadFileXmlSerialLib(ui->lineEdit_path_xml->text());

                //Запись в текстовый файл
                if(binfile.open(QIODevice::ReadOnly))
                {
                    //считывание заголовка
                    QDataStream in(&binfile);
                    in >> recordheader.id_module >> recordheader.id_object >> recordheader.id_struct
                            >> recordheader.t0 >> recordheader.delta_t >> recordheader.sizeOfStruct ;
                    in.readRawData(recordheader.idName,sizeof(recordheader.idName));


                    QTextStream out(&txtfile);
                    out << QString("Time;");
                    QStringList listTitle;
                    QStringList listMes;
                    //convertTitleTreeToStrings(node, listTitle, listMes);
                    for(int i = 0; i < listTitle.size(); i++)
                    {
                        //считваем размерность
                        out << listTitle.at(i)
                            << QString("(") << recordheader.id_object << QString(")")
                            << QString("[") << listMes.at(i) << QString("]")
                            << QString(";");
                    }
                    out << QString('\n');

                    if(txtfile.open(QIODevice::Append))
                    {
                        QByteArray ba;
                        ba.resize(recordheader.sizeOfStruct);
                        //считывания блока памяти
                        int inc = 0;
                        while (!binfile.atEnd())
                        {
                            in >> ba;
                            //cpyMemToTreeSerialLib(node, &ba);
                            QStringList list;// = convertValueTreeToStrings(node);
                            out << recordheader.t0+inc*recordheader.delta_t << QString(";");
                            for(int i = 0; i < list.size(); i++)
                            {
                                out << list.at(i) << QString(";");
                            }
                            out << QString('\n');
                            inc++;
                        }
                    }
                    binfile.close();
                    txtfile.close();
                    ui->statusBar->showMessage(QString("Создан файл "+txtfile.fileName()));
                }
            }

    }
}

void dataparsertotxt::on_pushButton_path_result_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    path_result,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(!dir.isEmpty())
    {
        ui->lineEdit_path_result->setText(dir);
        path_result = dir;
    }
}

#ifndef DATAPARSERTOTXT_H
#define DATAPARSERTOTXT_H

#include <QMainWindow>
#include <stdint.h>

#include <QMessageBox>

namespace Ui {
class dataparsertotxt;
}



class dataparsertotxt : public QMainWindow
{
    Q_OBJECT

public:
    explicit dataparsertotxt(QWidget *parent = 0);
    ~dataparsertotxt();

    struct Trecordheader
    {
        uint32_t id_module;
        uint32_t id_object;
        uint32_t id_struct;
        uint32_t sizeOfStruct;
        float delta_t;
        double t0; //sec
    }recordheader;

    QString name_newFile;

private slots:
    void on_pushButton_open_bin_clicked();

    void on_pushButton_open_xml_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_preview_clicked();

    void on_pushButton_save_to_file_clicked();

    void on_lineEdit_result_textChanged(const QString &arg1);

private:
    Ui::dataparsertotxt *ui;
};

#endif // DATAPARSERTOTXT_H

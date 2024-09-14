#ifndef DIALOG_HPP
#define DIALOG_HPP

#include <QDialog>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    enum Schema {
        English,
        Cpp,
        CMake
    };
public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

public slots:
    void generateMacro();

private:
    QString schemaPrefix() const;

private:
    Ui::Dialog *ui;
    QString m_template;
    QRegularExpression m_replaceRe;
    QRegularExpression m_squeezeRe;
    QRegularExpression m_clearRe;
    QHash<QString,QString> m_map;
};
#endif // DIALOG_HPP

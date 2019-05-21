#ifndef SCRIPTNAMESETTING_H
#define SCRIPTNAMESETTING_H

#include <QDialog>

namespace Ui
{
class ScriptNameSetting;
}

class ScriptNameSetting : public QDialog
{
    Q_OBJECT

public:
    explicit ScriptNameSetting(QWidget *parent = 0);
    ~ScriptNameSetting();
    QString scriptName();

private:
    Ui::ScriptNameSetting *ui;
};

#endif // SCRIPTNAMESETTING_H

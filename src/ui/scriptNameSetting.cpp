#include "scriptNameSetting.h"
#include "ui_scriptNameSetting.h"

ScriptNameSetting::ScriptNameSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScriptNameSetting)
{
    ui->setupUi(this);
}

ScriptNameSetting::~ScriptNameSetting()
{
    delete ui;
}

QString ScriptNameSetting::scriptName()
{
    return ui->scriptNameEdit->text();
}

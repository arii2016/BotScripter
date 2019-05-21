#include "mainWindow.h"
#include "ui_mainWindow.h"
#include "scriptNameSetting.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Conversation Scripter");
    m_scene = new QGraphicsScene(this);
    m_view = new QGraphicsView(m_scene);
    ui->GraphicFrame->layout()->addWidget(m_view);


    dataInitialize();
    connect(ui->FindScriptButton, SIGNAL(clicked()), this, SLOT(findScript()));
    connect(ui->LoadscriptButton, SIGNAL(clicked()), this, SLOT(loadScript()));
    connect(ui->keywordEdit, SIGNAL(editingFinished()), this, SLOT(updateKeyword()));
    connect(ui->parseButton,  SIGNAL(clicked()), this, SLOT(parsing()));
    connect(ui->FinishEdit, SIGNAL(clicked()), this, SLOT(saveScript()));
}

MainWindow::~MainWindow()
{
    delete m_scene;
    delete m_view;
    delete m_dataMgr;
    delete ui;
}

int MainWindow::dataInitialize()
{
    m_dataMgr = new DataManager();
}

void MainWindow::findScript()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Find Script", "/", "scriptFile(*.coffee)");
    if(filePath == "") {
        QMessageBox::information(this, "info", "create script");
    }
    ui->ScriptPath->setText(filePath);
}

void MainWindow::loadScript()
{
    if(m_dataMgr == NULL) { return; }

    m_dataMgr->loadScript(ui->ScriptPath->text(), m_scene);
    ui->keywordEdit->setText(m_dataMgr->keyword());
}

void MainWindow::saveScript()
{
    if(m_dataMgr == NULL) { return; }

    QString path = ui->ScriptPath->text();
    if(QFileInfo(path).exists() == false) {
        ScriptNameSetting dialog;
        if(dialog.exec() == QDialog::Rejected) {
            QMessageBox::information(this, "info", "Cancel saving\n" + path);
            return;
        }
        path = QCoreApplication::applicationDirPath() + "/scripts/" + dialog.scriptName() + ".coffee";
    }

    if(m_dataMgr->saveScript(path) == SUCCESS) {
        QMessageBox::information(this, "info", "Success saving\n" + path);
    } else {
        QMessageBox::information(this, "info", "Failed saving\n" + path);
    }
}

void MainWindow::updateKeyword()
{
    QString keyStr = ui->keywordEdit->text();
    if(m_dataMgr != NULL)
    { m_dataMgr->setKeyword(keyStr); }
}

void MainWindow::parsing()
{
    if(m_dataMgr != NULL) {
        m_dataMgr->parseAll();
    }
}

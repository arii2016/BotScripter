#include "dataManager.h"

DataManager::DataManager(QObject *parent)
    : QObject(parent)
{
    m_firstEntity = NULL;
}

ScriptEntity* DataManager::translateScript(QByteArray data, QGraphicsScene* scene)
{
    if(data.isEmpty() == true) {
        ScriptEntity* topEntity = new ScriptEntity();
        topEntity->setToScene(scene);
        topEntity->setPos(0, 100);
        m_firstEntity = topEntity;
    } else {
        parseKeyword(data);
        parseTree(data, scene);
    }
}

int DataManager::parseKeyword(QByteArray data)
{
    QString dataStr(data);
    dataStr = dataStr.toUtf8();
    QRegExp r("robot.respond /.*/i");
    r.setMinimal(true);
    r.indexIn(dataStr);
    QString keyStr = r.cap().remove("robot.respond ");
    keyStr.remove(QRegExp("\/|\/i"));
    if(keyStr.isEmpty() == true) {
        return WARN_EMPTY_KEYWORD;
    }
    QStringList keys = keyStr.split("|", QString::SkipEmptyParts);
    foreach (QString key, keys) {
        if(key.contains("?=.*") == true) {
            key.remove(QRegExp("\\(|\\)"));
            m_keyword += key.split("?=.*", QString::SkipEmptyParts).join("&");
        } else {
            m_keyword += key;
        }
    }

    if(m_keyword.count() == 0) {
        return WARN_FAILED_PARSE_KEYWORD;
    }

    return SUCCESS;
}

int DataManager::parseTree(QByteArray data, QGraphicsScene* scene)
{
    QString dataStr(data);
    int pos = dataStr.indexOf(m_scriptName);
    if(pos <= 0) {
        return ERR_NOT_FOUND_SCRIPT;
    }
    dataStr = dataStr.mid(pos);

    QRegExp varRE("Q[0-9]+_.* ");
    varRE.setMinimal(true);
    varRE.indexIn(dataStr);
    QString variableName = varRE.cap().trimmed();

    QRegExp contextRE("'''.+'''");
    contextRE.setMinimal(true);
    pos = contextRE.indexIn(dataStr);
    QString context = contextRE.cap().remove(QRegExp("'''|\t")).trimmed();

    m_firstEntity = new ScriptEntity(context);
    m_firstEntity->setToScene(scene);
    m_firstEntity->parseContent();
    dataStr = dataStr.mid(pos);

    m_firstEntity->parseTree(variableName, dataStr);

    return SUCCESS;
}

void DataManager::deleteEntity()
{
    if(m_firstEntity != NULL) {
        m_firstEntity->deleteChild();
        delete m_firstEntity;
        m_firstEntity = NULL;
    }
}

ScriptEntity* DataManager::firstEntity()
{
    return m_firstEntity;
}

void DataManager::setScene(QGraphicsScene* scene)
{
    scene->addItem(m_firstEntity);
}

QString DataManager::keyword()
{
    QString keywords = m_keyword.join(",");
    keywords = keywords.remove(QRegExp("(/)|(/\*i)"));//
    return keywords;
}

void DataManager::setKeyword(QString keyStr)
{
    QStringList keyList = keyStr.remove(QRegExp("\S")).split(",", QString::SkipEmptyParts);
    m_keyword.clear();
    m_keyword.append(keyList);
}

void DataManager::parseAll()
{
    if(m_firstEntity != NULL) {
        m_firstEntity->parseContentAll();
    }
}

int DataManager::loadScript(QString scriptPath, QGraphicsScene* scene)
{
    m_keyword.clear();
    m_scriptName = "";

    QByteArray data;
    if(QFileInfo(scriptPath).exists() == true) {
        QFile file(scriptPath);
        if(file.open(QIODevice::ReadOnly) == true) {
            data = file.readAll();
            file.close();

            m_scriptName = QFileInfo(scriptPath).baseName();
        }
    }

    scene->clear();
    translateScript(data, scene);
}

int DataManager::saveScript(QString scriptPath)
{
    QString scriptName = QFileInfo(scriptPath).baseName();
    QString scriptContent;
    QString headerString = SCRIPT_CAL;
    scriptContent = headerString.replace("cal", scriptName);

    //ScriptEntity
    scriptContent += m_firstEntity->script();

    QString footerString = QString(SCRIPT_NEXT_VALUE) + QString(SCRIPT_RESPOND) + QString(SCRIPT_HEAR);
    scriptContent += footerString.replace("cal", scriptName);

    if(m_keyword.isEmpty() == false) {
        QString keyword = "/";
        foreach (QString key, m_keyword) {
            if(key.contains("&") == true) {
                key = "(?=.*" + key.replace("&", ")(?=.*") + ")";
            }
            keyword += QString("%1|").arg(key);
        }
        keyword.chop(1);
        keyword += "/i";
        scriptContent.replace("//i", keyword);
    }

    QFile file(scriptPath);
    if(file.open(QIODevice::WriteOnly) == false) {
        return ERR_FAILED_SAVE_SCRIPT;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    out.setGenerateByteOrderMark(false);
    out << scriptContent.toUtf8();
    return SUCCESS;

}

#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include "scriptEntity.h"

#define SCRIPT_CAL \
                    "module.exports = (robot) -> \n" \
                    "\tcal = -> \n"
#define SCRIPT_NEXT_VALUE  \
                    "\treactedUserName = '' \n"               \
                    "\tnext_value = (input) -> \n"            \
                    "\t\trobot.cal.next(input).value \n"
#define SCRIPT_RESPOND \
                    "\trobot.respond //i, (msg) -> \n"        \
                    "\t\trobot.ignore_input = true \n"        \
                    "\t\trobot.cal = cal() \n"                \
                    "\t\treactedUserName = msg.message.user.name \n" \
                    "\t\tmsg.send(next_value()) \n"
/** //iの中にキーワードを入れる  */
#define SCRIPT_HEAR \
                    "\trobot.hear /(.*)\s*$/i, (msg) -> \n"                     \
                    "\t\tif !('cal' of robot and robot.cal) \n"                 \
                    "\t\t\treturn \n"                                           \
                    "\t\tif 'ignore_input' of robot and robot.ignore_input \n"  \
                    "\t\t\tdelete robot.ignore_input \n"                        \
                    "\t\t\treturn \n"                                           \
                    "\t\tif reactedUserName != msg.message.user.name \n"        \
                    "\t\t\treturn \n"                                           \
                    "\t\tmsg.send(next_value msg.match[1]) \n"


#define SUCCESS                      0
#define WARN_EMPTY_KEYWORD          -1
#define WARN_FAILED_PARSE_KEYWORD   -2
#define ERR_NOT_FOUND_SCRIPT        -3
#define ERR_FAILED_PARSE_SCRIPT     -4

#define ERR_FAILED_SAVE_SCRIPT      -11


class DataManager : public QObject
{
    Q_OBJECT
public:
    explicit DataManager(QObject *parent = nullptr);

    ScriptEntity* firstEntity();
    void setScene(QGraphicsScene*);
    QString keyword();
    void setKeyword(QString);
    void parseAll();

private:
    ScriptEntity* translateScript(QByteArray data, QGraphicsScene* scene);
    int parseKeyword(QByteArray data);
    int parseTree(QByteArray data, QGraphicsScene* scene);
    void deleteEntity();

signals:

public slots:
    int loadScript(QString scriptPath, QGraphicsScene* scene);
    int saveScript(QString scriptPath);

private:
    ScriptEntity*   m_firstEntity;
    QString         m_scriptName;
    QStringList     m_keyword;
};

#endif // DATAMANAGER_H

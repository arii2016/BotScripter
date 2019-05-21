#include "scriptEntity.h"
#include "utility.h"
#include "dataManager.h"

ScriptEntity::ScriptEntity(QGraphicsItem* parent)
    : QGraphicsTextItem("context\n１・２・３", parent)
{
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsScenePositionChanges);
    setTextInteractionFlags(Qt::TextEditorInteraction);

    m_scene = NULL;
    m_parentLine.first = NULL;
    m_level = 1;
    m_childEntity.clear();
}

ScriptEntity::ScriptEntity(QString context, int level, QGraphicsItem* parent)
    : QGraphicsTextItem(context, parent)
{
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsScenePositionChanges);
    setTextInteractionFlags(Qt::TextEditorInteraction);

    m_scene = NULL;
    m_parentLine.first = NULL;
    m_level = level;
    m_childEntity.clear();
}

int ScriptEntity::setToScene(QGraphicsScene* scene)
{
    m_scene = scene;
    m_scene->addItem(this);

    foreach (ScriptEntity* child, m_childEntity) {
        child->setToScene(m_scene);
    }
}

int ScriptEntity::removeFromScene()
{
    if(m_scene == NULL) { return 1; }

    foreach (ScriptEntity* child, m_childEntity) {
        child->removeFromScene();
    }

    m_scene->removeItem(this);
}

QVariant ScriptEntity::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene()) {
        // value is the new position.
        QPointF newPos = value.toPointF();

        moveLineToEdge(newPos);
    }
    return QGraphicsItem::itemChange(change, value);
}

QString ScriptEntity::script()
{
    QString retScript;
    QString indent = "\t";
    for(int i = 0; i < level(); i++) { indent += "\t"; }

    QString variableName = QString("Q%1_%2").arg(level()).arg(getRandomString());
    retScript += indent + variableName + QString(" = yield '''\n");
    retScript += indent + QString("\t") + toPlainText() + QString("\n");
    retScript += indent + QString("\t") + QString("'''\n");

    if(m_childEntity.count() >= 2) {
        retScript += indent;
    }
    foreach (ScriptEntity* child, m_childEntity) {
        QString answer = child->toPlainText().split("\n", QString::SkipEmptyParts).first();
        retScript += QString("if %1 == '%2' \n").arg(variableName).arg(answer);
        retScript += child->script();
        retScript += indent + QString("else ");
    }
    if(m_childEntity.count() >= 2) {
        retScript += QString("\n") + indent + QString(" \t　'''選択肢がありません''' \n");
    }

    return retScript;
}

void ScriptEntity::addChildLine(QGraphicsLineItem *line)
{
    ConnectionLine child;
    child.first = line;
    child.second = false;
    m_childLine.append(child);
}

void ScriptEntity::addParentLine(QGraphicsLineItem *line)
{
    m_parentLine.first = line;
    m_parentLine.second = true;
}

void ScriptEntity::moveLineToEdge(QPointF newPos)
{
    if(m_parentLine.first != NULL) {
        QGraphicsLineItem* parentLine = m_parentLine.first;
        int xOffset = boundingRect().x();
        int yOffset = boundingRect().y() + boundingRect().height() / 2;
        QPointF newConnectionPos = QPointF(newPos.x() + xOffset, newPos.y() + yOffset);
        QPointF p1 = parentLine->line().p1();
        QPointF p2 = newConnectionPos;
        m_parentLine.first->setLine(QLineF(p1, p2));
    }

    foreach (ConnectionLine line, m_childLine) {
        QGraphicsLineItem* childLine = line.first;
        int xOffset = boundingRect().x() + boundingRect().width();
        int yOffset = boundingRect().y() + boundingRect().height() / 2;

        QPointF newConnectionPos = QPointF(newPos.x() + xOffset, newPos.y() + yOffset);

        QPointF p1 = newConnectionPos;
        QPointF p2 = childLine->line().p2();

        childLine->setLine(QLineF(p1, p2));
    }

}

int ScriptEntity::parseTree(QString variableName, QString& dataModified)
{
    for(int i = 0; i < m_childEntity.count(); i++) {
        QRegExp variableRE(variableName);
        variableRE.setMinimal(true);
        int pos = variableRE.indexIn(dataModified);
        if(pos  < 0) { break; }

        dataModified = dataModified.mid(pos + variableName.length());
        QRegExp varRE("Q[0-9]+_.* ");
        varRE.setMinimal(true);
        pos = varRE.indexIn(dataModified);
        if(pos  < 0) { break; }
        dataModified = dataModified.mid(pos);
        QString newVariableName = varRE.cap().trimmed();

        QRegExp contextRE("'''.+'''");
        contextRE.setMinimal(true);
        pos = contextRE.indexIn(dataModified);
        dataModified = dataModified.mid(pos + contextRE.cap().length());
        QString context = contextRE.cap().remove(QRegExp("'''|\t")).trimmed();

        ScriptEntity* entity = m_childEntity.at(i);
        entity->setPlainText(context);
        entity->parseContent();
        entity->parseTree(newVariableName, dataModified);
    }
}

void ScriptEntity::deleteChild()
{
    foreach (ScriptEntity* child, m_childEntity) {
        child->deleteChild();
        delete child;
    }
    m_childEntity.clear();
}

int ScriptEntity::parseContent()
{
    //初回のContext編集時のみ発動
    //子がすでに存在する場合は作り直さない
    if(m_childEntity.count() != 0) { return 1; }

    QString content = toPlainText();
    QStringList contentList = content.split("\n", QString::SkipEmptyParts);
    QString selectors = contentList.last();

    if(selectors.indexOf("・") != -1) {
        createChild(selectors.split("・", QString::SkipEmptyParts));
    }

    return SUCCESS;
}

int ScriptEntity::parseContentAll()
{
    parseContent();

    foreach (ScriptEntity* child, m_childEntity) {
        child->parseContentAll();
    }
    return SUCCESS;
}

int ScriptEntity::createChild(QStringList selectors)
{
    int selectorCount = selectors.count();
    QPointF currentPos = pos();
    double x = currentPos.x();
    double y = currentPos.y();
    double topY = y - ((selectorCount - 1) * 50);

    for (int i = 0; i < selectorCount; i++) {
        ScriptEntity* childEntity = new ScriptEntity(selectors.at(i), level() + 1);
        childEntity->setToScene(m_scene);
        QGraphicsLineItem *line = m_scene->addLine(QLineF(0, 0, 10, 10));
        addChildLine(line);
        childEntity->addParentLine(line);
        childEntity->setPos(x + boundingRect().width() + 30, topY + (100 * i));
        m_childEntity.append(childEntity);
    }
    setPos(x + 1, y + 1); //枝の位置更新
}

#ifndef SCRIPTENTITY_H
#define SCRIPTENTITY_H

#include <QObject>
#include <QtWidgets>

#define ConnectionLine QPair<QGraphicsLineItem*, bool>

/*----------------------------------------

Entityのフォーマット

提示：　パターン1
　XXXですね
　では次のことを確認してください
　１．～～～～～～
　２．～～～～～～
　３．～～～～～～
　解決しましたか？
　はい・いいえ（Y/N）

提示：　パターン２
　XXXですね
　以下の選択肢のうち最も近い内容を選んでください
　１．～～～～～～
　２．～～～～～～
　３．～～～～～～
　１・２・３

------------------------------------------*/
class ScriptEntity : public QGraphicsTextItem
{
public:
    ScriptEntity(QGraphicsItem* parent = 0);
    ScriptEntity(QString context, int level = 1, QGraphicsItem* parent = 0);
    int setToScene(QGraphicsScene* scene);
    int removeFromScene();

    enum { Type = UserType + 10 };
    int type() const { return Type; }

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    QString script();
    int level() {return m_level;}

    int parseTree(QString variableName, QString& data);
    void deleteChild();

public slots:
    int parseContent();
    int parseContentAll();

private:
    int createChild(QStringList selectors);

    void addChildLine(QGraphicsLineItem *line);
    void addParentLine(QGraphicsLineItem *line);
    void moveLineToEdge(QPointF newPos) ;

private:
    QList<ScriptEntity*> m_childEntity;
    QGraphicsScene* m_scene;
    int m_level;

    ConnectionLine m_parentLine;
    QList<ConnectionLine> m_childLine;
};

#endif // SCRIPTENTITY_H

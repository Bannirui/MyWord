//
// Created by dingrui on 2022/5/23.
//

# pragma once

#include <QTextEdit>

// 子窗口部件
class MyChild : public QTextEdit
{
Q_OBJECT

public:
    MyChild();

    void newFile(); // 新建文件
    bool loadFile(const QString &fileName);

    bool save();

    bool saveAs();

    bool saveFile(QString fileName);

    QString userFriendlyCurrentFile(); // 提取文件名
    QString currentFile()
    { return this->curFile; } // 当前文件路径
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format); // 格式字体设置
    void setAlign(int align); // 段落对齐设置
    void setStyle(int style); // 段落标号编号

protected:
    void closeEvent(QCloseEvent *event) override; // 关闭事件

private slots:

    void documentWasModified(); // 文档被修改时 窗口显示更改状态标识

private:
    bool maybeSave();

    void setCurrentFile(const QString &fileName);

    QString strippedName(const QString &fullFileName); // 获取较短的绝对路径

private:
    QString curFile; // 当前文件路径
    bool isUntitled; // 标识是否命名过
};



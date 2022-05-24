//
// Created by dingrui on 2022/5/23.
//

#ifndef MYWORD_MYWORD_H
#define MYWORD_MYWORD_H

#include <QMainWindow>

/* 前置声明 start */
class QMdiArea; // 多文档区域
class MyChild; // 子窗口部件
class QAction; // 动作
class QMenu; // 菜单
class QComboBox;

class QMdiSubWindow;

class QFontComboBox;

class QSignalMapper; // 信号映射器
/* 前置声明 end */


QT_BEGIN_NAMESPACE
namespace Ui
{
    class MyWord;
}
QT_END_NAMESPACE

class MyWord : public QMainWindow
{
Q_OBJECT

public:
    explicit MyWord(QWidget *parent = nullptr);

    ~MyWord() override;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:

    void fileNew(); // 文件-新建
    void fileOpen(); // 文件-打开
    void fileSave(); // 文件-保存
    void fileSaveAs(); // 文件-另存为
    void filePrint(); // 文件-打印
    void filePrintPreview(); // 文件-打印预览

    void undo(); // 编辑-撤销
    void redo(); // 编辑-重做
    void cut(); // 编辑-剪切
    void copy(); // 编辑-复制
    void paste(); // 编辑-粘贴

    void textBold(); // 格式-字体-加粗
    void textItalic(); // 格式-字体-倾斜
    void textUnderline(); // 格式-字体-下划线
    void textAlign(QAction *a); // 格式-段落

    void about(); // 帮助-关于

    void textStyle(int styleIndex);

    void textFamily(const QString &f);

    void textSize(const QString &p);

    void textColor();

    void updateMenus(); // 更新菜单
    void updateWindowMenu(); // 更新窗口菜单

    MyChild *createMyChild(); // 新建子窗口
    void setActiveSubWindow(QWidget *window);

private:
    void createAction(); // 生成指令
    void createMenu(); // 生成菜单
    void createToolBars(); // 生成工具栏
    void createStatusBar(); // 生成状态栏

    /**
     * [格式]菜单下的各个子菜单生效
     */
    void enabledText();

    void colorChanged(const QColor &c);

    MyChild *activeMyChild(); // 活动子窗口
    QMdiSubWindow *findMyChild(const QString &fileName); // 打开文件

private:
    // 多文档区域
    QMdiArea *mdiArea;

    /* 主菜单*/
    QMenu *fileMenu; // 文件
    QMenu *editMenu; // 编辑
    QMenu *formatMenu; // 格式
    QMenu *windowMenu; // 窗口
    QMenu *helpMenu; // 帮助

    /* 二级菜单 */
    QMenu *fontMenu; // 格式-字体
    QMenu *alignMenu; // 格式-段落

    /* 工具条 */
    QToolBar *fileToolBar; // 文件
    QToolBar *editToolBar; // 编辑
    QToolBar *formatToolBar; // 格式
    QToolBar *comboToolBar; // 组合选择栏

    /* 工具条控件 */
    QComboBox *comboStyle; // 标号与编号类型选择框
    QFontComboBox *comboFont; // 字体选择框
    QComboBox *comboSize; // 字号选择框

    /* 动作指令 */
    QAction *newAct; // 文件-新建
    QAction *openAct; // 文件-打开
    QAction *saveAct; // 文件-保存
    QAction *saveAsAct; // 文件-另存为
    QAction *printAct; // 文件-打印
    QAction *printPreviewAct; // 文件-打印预览
    QAction *exitAct; // 文件-退出

    QAction *undoAct; // 编辑-撤销
    QAction *redoAct; // 编辑-重做
    QAction *cutAct; // 编辑-剪切
    QAction *copyAct; // 编辑-复制
    QAction *pasteAct; // 编辑-粘贴

    QAction *boldAct; // 格式-字体-加粗
    QAction *italicAct; // 格式-字体-倾斜
    QAction *underlineAct; // 格式-字体-下划线
    QAction *leftAlignAct; // 格式-段落-左对齐
    QAction *centerAct; // 格式-段落-居中
    QAction *rightAlignAct; // 格式-段落-右对齐
    QAction *justifyAct; // 格式-段落-两端对齐
    QAction *colorAct; // 格式-颜色

    QAction *closeAct; // 窗口-关闭
    QAction *closeAllAct; // 窗口-关闭所有
    QAction *tileAct; // 窗口-平铺
    QAction *cascadeAct; // 窗口-层叠
    QAction *nextAct; // 窗口-下一个
    QAction *previousAct; // 窗口-前一个
    QAction *separatorAct;

    QAction *aboutAct; // 帮助-关于
    QAction *aboutQtAct; // 帮助-关于Qt

    QSignalMapper *windowMapper; // 信号映射器
};

#endif //MYWORD_MYWORD_H

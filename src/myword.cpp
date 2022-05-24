//
// Created by dingrui on 2022/5/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MyWord.h" resolved

#include "myword.h"
// #include "Forms/ui_MyWord.h"
#include "MyChild.h"
#include <QtWidgets>

const QString images = "../images";

MyWord::MyWord(QWidget *parent) :
        QMainWindow(parent)
{
    this->mdiArea = new QMdiArea;
    this->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(this->mdiArea);
    connect(this->mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow * )), this, SLOT(updateMenus()));
    this->windowMapper = new QSignalMapper(this); // 信号映射器
    connect(windowMapper, SIGNAL(mapped(QWidget * )), this,
            SLOT(setActiveSubWindow(QWidget * ))); // 信号映射器重新发送信号 根据信号设置活动窗口
    this->createAction(); // 生成指令集
    this->createMenu(); // 生成菜单
    this->createToolBars(); // 生成工具栏
    this->createStatusBar(); // 生成状态栏
    this->updateMenus();
    move(200, 150);
    resize(800, 500);
    setWindowTitle(tr("My Word"));
    setUnifiedTitleAndToolBarOnMac(true);
}

MyWord::~MyWord()
{
}

void MyWord::closeEvent(QCloseEvent *event)
{
    this->mdiArea->closeAllSubWindows();
    if (this->mdiArea->currentSubWindow())
    {
        event->ignore();
    } else
    {
        event->accept();
    }
}


void MyWord::fileNew()
{
    MyChild *child = this->createMyChild();
    child->newFile();
    child->show();
    this->enabledText();
}

void MyWord::fileOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开"), QString(),
                                                    tr("HTML 文档 (*.htm *.html);;所有文件 (*.*)"));
    if (fileName.isEmpty()) return;
    QMdiSubWindow *existing = this->findMyChild(fileName);
    if (existing)
    {
        this->mdiArea->setActiveSubWindow(existing);
        return;
    }
    MyChild *child = this->createMyChild();
    if (child->loadFile(fileName))
    {
        statusBar()->showMessage(tr("文件已载入"), 2000);
        child->show();
        enabledText();
    } else
    {
        child->close();
    }
}

void MyWord::fileSave()
{
    if (this->activeMyChild() && this->activeMyChild()->save())
        statusBar()->showMessage(tr("保存成功"), 2000);
}

void MyWord::fileSaveAs()
{
    if (this->activeMyChild() && this->activeMyChild()->saveAs())
        statusBar()->showMessage(tr("保存成功"), 2000);
}

void MyWord::filePrint()
{
    statusBar()->showMessage(tr("NOT SUPPORT"), 2000);
}

void MyWord::filePrintPreview()
{
    statusBar()->showMessage(tr("NOT SUPPORT"), 2000);
}

void MyWord::undo()
{
    if (this->activeMyChild())
        this->activeMyChild()->undo();
}

void MyWord::redo()
{
    if (this->activeMyChild())
        this->activeMyChild()->redo();
}

void MyWord::cut()
{
    if (this->activeMyChild())
        this->activeMyChild()->cut();
}

void MyWord::copy()
{
    if (this->activeMyChild())
        this->activeMyChild()->copy();
}

void MyWord::paste()
{
    if (this->activeMyChild())
        this->activeMyChild()->paste();
}

void MyWord::textBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(boldAct->isChecked() ? QFont::Bold : QFont::Normal);
    if (this->activeMyChild())
        this->activeMyChild()->mergeFormatOnWordOrSelection(fmt);
}

void MyWord::textItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(italicAct->isChecked());
    if (this->activeMyChild())
        this->activeMyChild()->mergeFormatOnWordOrSelection(fmt);
}

void MyWord::textUnderline()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(underlineAct->isChecked());
    if (this->activeMyChild())
        this->activeMyChild()->mergeFormatOnWordOrSelection(fmt);
}

void MyWord::textAlign(QAction *a)
{
    if (this->activeMyChild())
    {
        if (a == this->leftAlignAct)
            activeMyChild()->setAlign(1);
        else if (a == this->centerAct)
            activeMyChild()->setAlign(2);
        else if (a == this->rightAlignAct)
            activeMyChild()->setAlign(3);
        else if (a == this->justifyAct)
            activeMyChild()->setAlign(4);
    }
}

void MyWord::about()
{
    QMessageBox::about(this, tr("关于"), tr("这是基于Qt实现的word"));
}

void MyWord::textStyle(int styleIndex)
{
    if (this->activeMyChild())
        this->activeMyChild()->setStyle(styleIndex);
}

void MyWord::textFamily(const QString &f)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    if (this->activeMyChild())
        this->activeMyChild()->mergeFormatOnWordOrSelection(fmt);
}

void MyWord::textColor()
{
    if (this->activeMyChild())
    {
        QColor col = QColorDialog::getColor(activeMyChild()->textColor(), this);
        if (!col.isValid())
            return;
        QTextCharFormat fmt;
        fmt.setForeground(col);
        this->activeMyChild()->mergeFormatOnWordOrSelection(fmt);
        colorChanged(col);
    }
}

void MyWord::updateMenus()
{
    // 至少有一个子文档打开着的情况
    bool hasMyChild = (this->activeMyChild() != 0);
    this->saveAct->setEnabled(hasMyChild);
    this->saveAsAct->setEnabled(hasMyChild);
    this->printAct->setEnabled(hasMyChild);
    this->printPreviewAct->setEnabled(hasMyChild);
    this->pasteAct->setEnabled(hasMyChild);
    this->closeAct->setEnabled(hasMyChild);
    this->closeAllAct->setEnabled(hasMyChild);
    this->tileAct->setEnabled(hasMyChild);
    this->cascadeAct->setEnabled(hasMyChild);
    this->nextAct->setEnabled(hasMyChild);
    this->previousAct->setEnabled(hasMyChild);
    this->separatorAct->setVisible(hasMyChild);
    // 文档打开着并且其中有内容被选中的情况
    bool hasSelection = (this->activeMyChild() && this->activeMyChild()->textCursor().hasSelection());
    this->cutAct->setEnabled(hasSelection);
    this->copyAct->setEnabled(hasSelection);
    this->boldAct->setEnabled(hasSelection);
    this->italicAct->setEnabled(hasSelection);
    this->underlineAct->setEnabled(hasSelection);
    this->leftAlignAct->setEnabled(hasSelection);
    this->centerAct->setEnabled(hasSelection);
    this->rightAlignAct->setEnabled(hasSelection);
    this->justifyAct->setEnabled(hasSelection);
    this->colorAct->setEnabled(hasSelection);
}

void MyWord::updateWindowMenu()
{
    this->windowMenu->clear(); // 清空菜单
    this->windowMenu->addAction(this->closeAct);
    this->windowMenu->addAction(this->closeAllAct);
    this->windowMenu->addSeparator();
    this->windowMenu->addAction(this->tileAct);
    this->windowMenu->addAction(this->cascadeAct);
    this->windowMenu->addSeparator();
    this->windowMenu->addAction(this->nextAct);
    this->windowMenu->addAction(this->previousAct);
    this->windowMenu->addAction(this->separatorAct);

    QList<QMdiSubWindow *> windows = this->mdiArea->subWindowList();
    this->separatorAct->setVisible(!windows.isEmpty());
    //显示当前打开着的文档子窗口项
    for (int i = 0, size = windows.size(); i < size; ++i)
    {
        MyChild *child = qobject_cast<MyChild *>(windows.at(i)->widget());

        QString text;
        if (i < 9)
        {
            text = tr("&%1 %2").arg(i + 1).arg(child->userFriendlyCurrentFile());
        } else
        {
            text = tr("%1 %2").arg(i + 1).arg(child->userFriendlyCurrentFile());
        }
        QAction *action = windowMenu->addAction(text);
        action->setCheckable(true);
        action->setChecked(child == activeMyChild());
        connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
        windowMapper->setMapping(action, windows.at(i));
    }
    enabledText();
}

void MyWord::textSize(const QString &p)
{
    qreal pointSize = p.toFloat();
    if (p.toFloat() > 0)
    {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        if (this->activeMyChild())
            this->activeMyChild()->mergeFormatOnWordOrSelection(fmt);
    }
}

MyChild *MyWord::createMyChild()
{
    MyChild *child = new MyChild;
    this->mdiArea->addSubWindow(child);
    connect(child, SIGNAL(copyAvailable(bool)), this->cutAct, SLOT(setEnabled(bool)));
    connect(child, SIGNAL(copyAvailable(bool)), this->copyAct, SLOT(setEnabled(bool)));
    return child;
}

void MyWord::setActiveSubWindow(QWidget *window)
{
    if (!window)
        return;
    this->mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

void MyWord::createAction()
{
    /* 文件菜单动作集 */
    // 文件-新建
    this->newAct = new QAction(QIcon(images + "/filenew.png"), tr("新建(&N)"), this);
    this->newAct->setShortcut(QKeySequence::New);
    this->newAct->setToolTip("新建"); // 工具栏提示
    this->newAct->setStatusTip(tr("创建一个新文档")); // 状态栏提示
    connect(this->newAct, SIGNAL(triggered()), this, SLOT(fileNew()));
    // 文件-打开
    this->openAct = new QAction(QIcon(images + "/fileopen.png"), tr("打开(&O)"), this);
    this->openAct->setShortcut(QKeySequence::Open);
    this->openAct->setToolTip("打开");
    this->openAct->setStatusTip(tr("打开已存在的文档"));
    connect(this->openAct, SIGNAL(triggered()), this, SLOT(fileOpen()));
    // 文件-保存
    this->saveAct = new QAction(QIcon(images + "/filesave.png"), tr("保存(&S)"), this);
    this->saveAct->setShortcut(QKeySequence::Save);
    this->saveAct->setToolTip("保存");
    this->saveAct->setStatusTip(tr("将当前文件存盘"));
    connect(this->saveAct, SIGNAL(triggered()), this, SLOT(fileSave()));
    // 文件-另存为
    this->saveAsAct = new QAction(tr("另存为(&A)..."), this);
    this->saveAsAct->setShortcut(QKeySequence::SaveAs);
    this->saveAsAct->setToolTip("另存为");
    this->saveAsAct->setStatusTip(tr("以一个新的名字保存文档"));
    connect(this->saveAsAct, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
    // 文件-打印
    this->printAct = new QAction(QIcon(images + "/fileprint.png"), tr("打印(&P)..."), this);
    this->printAct->setShortcut(QKeySequence::Print);
    this->printAct->setToolTip("打印");
    this->printAct->setStatusTip(tr("打印文档"));
    connect(this->printAct, SIGNAL(triggered()), this, SLOT(filePrint()));
    // 文件-打印预览
    this->printPreviewAct = new QAction(tr("打印预览..."), this);
    this->printPreviewAct->setStatusTip(tr("预览打印效果"));
    connect(this->printPreviewAct, SIGNAL(triggered()), this, SLOT(filePrintPreview()));
    // 文件-退出
    this->exitAct = new QAction(tr("退出(&X)..."), this);
    this->exitAct->setShortcut(QKeySequence::Quit);
    this->exitAct->setStatusTip(tr("退出应用程序"));
    connect(this->exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    /* 编辑菜单动作集 */
    // 编辑-撤销
    this->undoAct = new QAction(QIcon(images + "/editundo.png"), tr("撤销(&U)"), this);
    this->undoAct->setShortcut(QKeySequence::Undo);
    this->undoAct->setToolTip("撤销");
    this->undoAct->setStatusTip(tr("撤销当前操作"));
    connect(this->undoAct, SIGNAL(triggered()), this, SLOT(undo()));
    // 编辑-重做
    this->redoAct = new QAction(QIcon(images + "/editredo.png"), tr("重做(&R)"), this);
    this->redoAct->setShortcut(QKeySequence::Redo);
    this->redoAct->setToolTip("重做");
    this->redoAct->setStatusTip(tr("恢复之前操作"));
    connect(this->redoAct, SIGNAL(triggered()), this, SLOT(redo()));
    // 编辑-剪切
    this->cutAct = new QAction(QIcon(images + "/editcut.png"), tr("剪切(&R)"), this);
    this->cutAct->setShortcut(QKeySequence::Cut);
    this->cutAct->setToolTip("剪切");
    this->cutAct->setStatusTip(tr("剪切内容放入粘贴板"));
    connect(this->cutAct, SIGNAL(triggered()), this, SLOT(cut()));
    // 编辑-复制
    this->copyAct = new QAction(QIcon(images + "/editcopy.png"), tr("复制(&C)"), this);
    this->copyAct->setShortcut(QKeySequence::Copy);
    this->copyAct->setToolTip("复制");
    this->copyAct->setStatusTip(tr("复制内容放入粘贴板"));
    connect(this->copyAct, SIGNAL(triggered()), this, SLOT(copy()));
    // 编辑-粘贴
    this->pasteAct = new QAction(QIcon(images + "/editpaste.png"), tr("粘贴(&P)"), this);
    this->pasteAct->setShortcut(QKeySequence::Paste);
    this->pasteAct->setToolTip("粘贴");
    this->pasteAct->setStatusTip(tr("将粘贴板内容粘贴到文档"));
    connect(this->pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

    /* 格式菜单动作集 */
    // 格式-字体-加粗
    this->boldAct = new QAction(QIcon(images + "/textbold.png"), tr("加粗(&B)"), this);
    this->boldAct->setCheckable(true);
    this->boldAct->setShortcut(Qt::CTRL + Qt::Key_B);
    this->boldAct->setToolTip("加粗");
    this->boldAct->setStatusTip(tr("将所选文字加粗"));
    QFont bold;
    bold.setBold(true);
    this->boldAct->setFont(bold);
    connect(this->boldAct, SIGNAL(triggered()), this, SLOT(textBold()));
    // 格式-字体-倾斜
    this->italicAct = new QAction(QIcon(images + "/textitalic.png"), tr("倾斜(&I)"), this);
    this->italicAct->setCheckable(true);
    this->italicAct->setShortcut(Qt::CTRL + Qt::Key_I);
    this->italicAct->setToolTip("倾斜");
    this->italicAct->setStatusTip(tr("将所选文字用斜体表示"));
    QFont italic;
    italic.setItalic(true);
    this->italicAct->setFont(italic);
    connect(this->italicAct, SIGNAL(triggered()), this, SLOT(textItalic()));
    // 格式-字体-下划线
    this->underlineAct = new QAction(QIcon(images + "/textunder.png"), tr("下划线(&U)"), this);
    this->underlineAct->setCheckable(true);
    this->underlineAct->setShortcut(Qt::CTRL + Qt::Key_U);
    this->underlineAct->setToolTip("下划线");
    this->underlineAct->setStatusTip(tr("将所选文字加上下划线"));
    QFont underline;
    underline.setUnderline(true);
    this->underlineAct->setFont(underline);
    connect(this->underlineAct, SIGNAL(triggered()), this, SLOT(textUnderline()));
    // 格式-段落 子菜单下的各项为同一个菜单项组 只能选取其中一项
    QActionGroup *grp = new QActionGroup(this);
    connect(grp, SIGNAL(triggered(QAction * )), this, SLOT(textAlign(QAction * )));
    // 格式-段落(左对齐 居中 右对齐)
    if (QApplication::isLeftToRight())
    {
        this->leftAlignAct = new QAction(QIcon(images + "/textleft.png"), tr("左对齐(&L)"), grp);
        this->centerAct = new QAction(QIcon(images + "/textcenter.png"), tr("居中(&E)"), grp);
        this->rightAlignAct = new QAction(QIcon(images + "/textright.png"), tr("右对齐(&R)"), grp);
    } else
    {
        this->rightAlignAct = new QAction(QIcon(images + "/textright.png"), tr("右对齐(&R)"), grp);
        this->centerAct = new QAction(QIcon(images + "/textcenter.png"), tr("居中(&E)"), grp);
        this->leftAlignAct = new QAction(QIcon(images + "/textleft.png"), tr("左对齐(&L)"), grp);
    }
    // 格式-段落-两端对齐
    this->justifyAct = new QAction(QIcon(images + "/textjustify.png"), tr("两端对齐(&J)"), grp);
    // 格式-段落-左对齐
    this->leftAlignAct->setShortcut(Qt::CTRL + Qt::Key_L);
    this->leftAlignAct->setCheckable(true);
    this->leftAlignAct->setToolTip("左对齐");
    this->leftAlignAct->setStatusTip(tr("将文字左对齐"));
    // 格式-段落-居中
    this->centerAct->setShortcut(Qt::CTRL + Qt::Key_E);
    this->centerAct->setCheckable(true);
    this->centerAct->setToolTip("居中");
    this->centerAct->setStatusTip(tr("将文字居中对齐"));
    // 格式-段落-右对齐
    this->rightAlignAct->setShortcut(Qt::CTRL + Qt::Key_R);
    this->rightAlignAct->setCheckable(true);
    this->rightAlignAct->setToolTip("右对齐");
    this->rightAlignAct->setStatusTip(tr("将文字右对齐"));
    // 格式-段落-两端对齐
    this->justifyAct->setShortcut(Qt::CTRL + Qt::Key_J);
    this->justifyAct->setCheckable(true);
    this->justifyAct->setToolTip("两端对齐");
    this->justifyAct->setStatusTip(tr("将文字两端对齐并根据需要增加字距"));
    // 格式-颜色
    QPixmap pix(16, 16);
    pix.fill(Qt::red);
    this->colorAct = new QAction(pix, tr("颜色(&C)..."), this);
    this->colorAct->setToolTip("颜色");
    this->colorAct->setStatusTip(tr("设置文字颜色"));
    connect(this->colorAct, SIGNAL(triggered()), this, SLOT(textColor()));

    /* 窗口菜单动作集 */
    // 窗口-关闭
    this->closeAct = new QAction(tr("关闭(&O)"), this);
    this->closeAct->setStatusTip(tr("关闭活动文件子窗口"));
    connect(this->closeAct, SIGNAL(triggered()), this->mdiArea, SLOT(closeActiveSubWindow())); // 不用自己实现
    // 窗口-关闭所有
    this->closeAllAct = new QAction(tr("关闭所有(&A)"), this);
    this->closeAllAct->setStatusTip(tr("关闭所有文件子窗口"));
    connect(this->closeAllAct, SIGNAL(triggered()), this->mdiArea, SLOT(closeAllSubWindows())); // 不用自己实现
    // 窗口-平铺
    this->tileAct = new QAction(tr("平铺(&T)"), this);
    this->tileAct->setStatusTip(tr("平铺子窗口"));
    connect(this->tileAct, SIGNAL(triggered()), this->mdiArea, SLOT(tileSubWindows())); // 不用实现
    // 窗口-层叠
    this->cascadeAct = new QAction(tr("层叠(&C)"), this);
    this->cascadeAct->setStatusTip(tr("层叠子窗口"));
    connect(this->cascadeAct, SIGNAL(triggered()), this->mdiArea, SLOT(cascadeSubWindows())); // 不用实现
    // 窗口-下一个
    this->nextAct = new QAction(tr("下一个(&X)"), this);
    this->nextAct->setShortcut(QKeySequence::NextChild);
    this->nextAct->setStatusTip(tr("移动焦点到下一个窗口"));
    connect(this->nextAct, SIGNAL(triggered()), this->mdiArea, SLOT(activateNextSubWindow())); // 不用实现
    // 窗口-前一个
    this->previousAct = new QAction(tr("前一个(&C)"), this);
    this->previousAct->setShortcut(QKeySequence::PreviousChild);
    this->previousAct->setStatusTip(tr("移动焦点到前一个子窗口"));
    connect(this->previousAct, SIGNAL(triggered()), this->mdiArea, SLOT(activatePreviousSubWindow())); // 不用实现
    this->separatorAct = new QAction(this);
    this->separatorAct->setSeparator(true);

    /* 帮助菜单动作集 */
    // 帮助-关于
    this->aboutAct = new QAction(tr("关于(&A)"), this);
    this->aboutAct->setStatusTip(tr("关于MyWord"));
    connect(this->aboutAct, SIGNAL(triggered()), this, SLOT(about()));
    // 帮助-关于qt
    this->aboutQtAct = new QAction(tr("关于Qt(&Q)"), this);
    this->aboutQtAct->setStatusTip(tr("关于Qt库"));
    connect(this->aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt())); // 不用实现
}

void MyWord::createMenu()
{
    /* 文件 */
    this->fileMenu = menuBar()->addMenu(tr("文件(&F)"));
    this->fileMenu->addAction(this->newAct); // 新增
    this->fileMenu->addAction(this->openAct); // 打开
    this->fileMenu->addSeparator(); // 分隔线
    this->fileMenu->addAction(this->saveAct); // 保存
    this->fileMenu->addAction(this->saveAsAct); // 另存为
    this->fileMenu->addSeparator(); // 分隔线
    this->fileMenu->addAction(this->printAct); // 打印
    this->fileMenu->addAction(this->printPreviewAct); // 打印预览
    this->fileMenu->addSeparator(); // 分隔线
    this->fileMenu->addAction(this->exitAct); // 退出

    /* 编辑*/
    this->editMenu = menuBar()->addMenu(tr("编辑(&E)"));
    this->editMenu->addAction(this->undoAct); // 编辑-撤销
    this->editMenu->addAction(this->redoAct); // 编辑-重做
    this->editMenu->addSeparator();
    this->editMenu->addAction(this->cutAct); // 编辑-剪切
    this->editMenu->addAction(this->copyAct); // 编辑-复制
    this->editMenu->addAction(this->pasteAct); // 编辑-粘贴

    /* 格式 */
    this->formatMenu = menuBar()->addMenu(tr("格式(&O)"));
    // 子菜单: 格式-字体
    this->fontMenu = this->formatMenu->addMenu(tr("字体(&D)"));
    this->fontMenu->addAction(this->boldAct);
    this->fontMenu->addAction(this->italicAct);
    this->fontMenu->addAction(this->underlineAct);
    // 子菜单: 格式-段落
    this->alignMenu = this->formatMenu->addMenu(tr("段落"));
    this->alignMenu->addAction(this->leftAlignAct);
    this->alignMenu->addAction(this->centerAct);
    this->alignMenu->addAction(this->rightAlignAct);
    this->alignMenu->addAction(this->justifyAct);
    // 颜色
    this->formatMenu->addAction(this->colorAct);

    /* 窗口 */
    this->windowMenu = menuBar()->addMenu(tr("窗口(&W)"));
    this->updateWindowMenu(); // 更新窗口菜单
    connect(this->windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu())); // 将窗口菜单要显示的信号关联到更新的菜单槽上

    this->menuBar()->addSeparator();

    /* 帮助 */
    this->helpMenu = menuBar()->addMenu(tr("帮助(&H)"));
    this->helpMenu->addAction(this->aboutAct); // 帮助-关于
    this->helpMenu->addSeparator();
    this->helpMenu->addAction(this->aboutQtAct); // 帮助-关于Qt
}

void MyWord::createToolBars()
{
    /* 文件 */
    this->fileToolBar = addToolBar(tr("文件"));
    this->fileToolBar->addAction(this->newAct);
    this->fileToolBar->addAction(this->openAct);
    this->fileToolBar->addAction(this->saveAct);
    this->fileToolBar->addSeparator();
    this->fileToolBar->addAction(this->printAct);

    /* 编辑 */
    this->editToolBar = addToolBar(tr("编辑"));
    this->editToolBar->addAction(this->undoAct);
    this->editToolBar->addAction(this->redoAct);
    this->editToolBar->addSeparator();
    this->editToolBar->addAction(this->cutAct);
    this->editToolBar->addAction(this->copyAct);
    this->editToolBar->addAction(this->pasteAct);

    /* 格式 */
    this->formatToolBar = addToolBar(tr("格式"));
    this->formatToolBar->addAction(this->boldAct);
    this->formatToolBar->addAction(this->italicAct);
    this->formatToolBar->addAction(this->underlineAct);
    this->formatToolBar->addSeparator();
    this->formatToolBar->addAction(this->leftAlignAct);
    this->formatToolBar->addAction(this->centerAct);
    this->formatToolBar->addAction(this->rightAlignAct);
    this->formatToolBar->addAction(this->justifyAct);
    this->formatToolBar->addSeparator();
    this->formatToolBar->addAction(this->colorAct);

    /* 组合工具栏 */
    addToolBarBreak(Qt::TopToolBarArea); // 使这个工具条在界面上另起一行显示
    this->comboToolBar = addToolBar(tr("组合选择"));
    this->comboStyle = new QComboBox();
    this->comboToolBar->addWidget(this->comboStyle);
    this->comboStyle->addItem("标准");
    this->comboStyle->addItem("项目符号 (●)");
    this->comboStyle->addItem("项目符号 (○)");
    this->comboStyle->addItem("项目符号 (■)");
    this->comboStyle->addItem("编号 (⒈⒉⒊)");
    this->comboStyle->addItem("编号 ( a.b.c.)");
    this->comboStyle->addItem("编号 ( A.B.C.)");
    this->comboStyle->addItem("编号 (ⅰ.ⅱ.ⅲ.)");
    this->comboStyle->addItem("编号 (Ⅰ.Ⅱ.Ⅲ.)");
    this->comboStyle->setStatusTip("段落加标号或编号");
    connect(this->comboStyle, SIGNAL(activated(int)), this, SLOT(textStyle(int)));

    this->comboFont = new QFontComboBox();
    this->comboToolBar->addWidget(this->comboFont);
    this->comboFont->setStatusTip("更改字体");
    connect(this->comboFont, SIGNAL(activated(QString)), this, SLOT(textFamily(QString)));

    this->comboSize = new QComboBox();
    this->comboToolBar->addWidget(this->comboSize);
    this->comboSize->setEditable(true);
    this->comboSize->setStatusTip("更改字号");

    QFontDatabase db;
            foreach(int size, db.standardSizes())this->comboSize->addItem(QString::number(size));

    connect(this->comboSize, SIGNAL(activated(QString)), this, SLOT(textSize(QString)));
    this->comboSize->setCurrentIndex(this->comboSize->findText(QString::number(QApplication::font().pointSize())));
    /* 工具条 end*/
}

void MyWord::createStatusBar()
{
    statusBar()->showMessage(tr("就绪"));
}

void MyWord::enabledText()
{
    this->boldAct->setEnabled(true);
    this->italicAct->setEnabled(true);
    this->underlineAct->setEnabled(true);
    this->leftAlignAct->setEnabled(true);
    this->centerAct->setEnabled(true);
    this->rightAlignAct->setEnabled(true);
    this->justifyAct->setEnabled(true);
    this->colorAct->setEnabled(true);
}

void MyWord::colorChanged(const QColor &c)
{
    QPixmap pix(16, 16);
    pix.fill(c);
    colorAct->setIcon(pix);
}

MyChild *MyWord::activeMyChild()
{
    if (QMdiSubWindow *activeSubWindow = this->mdiArea->activeSubWindow())
        return qobject_cast<MyChild *>(activeSubWindow->widget());
    return 0;
}

QMdiSubWindow *MyWord::findMyChild(const QString &fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

            foreach (QMdiSubWindow *window, mdiArea->subWindowList())
        {
            MyChild *myChild = qobject_cast<MyChild *>(window->widget());
            if (myChild->currentFile() == canonicalFilePath)
                return window;
        }
    return 0;
}
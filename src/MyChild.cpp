//
// Created by dingrui on 2022/5/23.
//

#include "MyChild.h"
#include <QtWidgets>

MyChild::MyChild() : isUntitled(true)
{
    setAttribute(Qt::WA_DeleteOnClose); // 子窗口关闭时销毁这个类的对象
}

void MyChild::newFile()
{
    static int sequenceNumber = 1; // 设置窗口编号 这个编号会一直被保存 静态变量
    this->isUntitled = true; // 新建的文件默认未命名
    this->curFile = tr("文档 %1").arg(sequenceNumber++); // 文件命名=文档+编号
    setWindowTitle(this->curFile + "[*]" + tr(" -MyWord")); // 设置窗口标题
}

bool MyChild::loadFile(const QString &fileName)
{
    if (fileName.isEmpty()) return false;
    if (!QFile::exists(fileName))
        return false;
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return false;

    QByteArray data = file.readAll();
    QTextCodec *codec = Qt::codecForHtml(data);
    QString str = codec->toUnicode(data);
    if (Qt::mightBeRichText(str))
    {
        this->setHtml(str);
    } else
    {
        str = QString::fromLocal8Bit(data);
        this->setPlainText(str);
    }
    setCurrentFile(fileName);
    connect(document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));
    return true;
}

bool MyChild::save()
{
    if (this->isUntitled)
    {
        return this->saveAs();
    } else
    {
        return this->saveFile(curFile);
    }
}

bool MyChild::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("另存为"), this->curFile,
                                                    tr("HTML 文档 (*.htm *.html);;所有文件 (*.*)"));
    if (fileName.isEmpty())
        return false;

    return this->saveFile(fileName);
}

bool MyChild::saveFile(QString fileName)
{
    if (!(fileName.endsWith(".htm", Qt::CaseInsensitive) || fileName.endsWith(".html", Qt::CaseInsensitive)))
        fileName += ".html"; // 默认保存为 HTML 文档
    QTextDocumentWriter writer(fileName);
    bool success = writer.write(this->document());
    if (success)
        this->setCurrentFile(fileName);
    return success;
}

void MyChild::documentWasModified()
{
    // 判断编辑器内容是否被更改
    setWindowModified(document()->isModified());
}

bool MyChild::maybeSave()
{
    if (!document()->isModified())
        return true;
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Myself Qt Word"), tr("文档'%1'已被修改,保存吗?").arg(userFriendlyCurrentFile()),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (ret == QMessageBox::Save)
        return this->save();
    else if (ret == QMessageBox::Cancel)
        return false;
    return true;
}

void MyChild::setCurrentFile(const QString &fileName)
{
    this->curFile = QFileInfo(fileName).canonicalFilePath();
    this->isUntitled = false;
    document()->setModified(false);
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

QString MyChild::userFriendlyCurrentFile()
{
    return this->strippedName(this->curFile);
}

void MyChild::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = this->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    this->mergeCurrentCharFormat(format);
}

void MyChild::setAlign(int align)
{
    if (align == 1)
        this->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
    else if (align == 2)
        this->setAlignment(Qt::AlignHCenter);
    else if (align == 3)
        this->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
    else if (align == 4)
        this->setAlignment(Qt::AlignJustify);
}

void MyChild::setStyle(int style)
{
    QTextCursor cursor = this->textCursor();
    if (style != 0)
    {
        QTextListFormat::Style styleName = QTextListFormat::ListDisc;
        switch (style)
        {
            default:
            case 1:
                styleName = QTextListFormat::ListDisc;
                break;
            case 2:
                styleName = QTextListFormat::ListCircle;
                break;
            case 3:
                styleName = QTextListFormat::ListSquare;
                break;
            case 4:
                styleName = QTextListFormat::ListDecimal;
                break;
            case 5:
                styleName = QTextListFormat::ListLowerAlpha;
                break;
            case 6:
                styleName = QTextListFormat::ListUpperAlpha;
                break;
            case 7:
                styleName = QTextListFormat::ListLowerRoman;
                break;
            case 8:
                styleName = QTextListFormat::ListUpperRoman;
                break;
        }
        cursor.beginEditBlock();
        QTextBlockFormat blockFmt = cursor.blockFormat();
        QTextListFormat listFmt;
        if (cursor.currentList())
        {
            listFmt = cursor.currentList()->format();
        } else
        {
            listFmt.setIndent(blockFmt.indent() + 1);
            blockFmt.setIndent(0);
            cursor.setBlockFormat(blockFmt);
        }
        listFmt.setStyle(styleName);
        cursor.createList(listFmt);
        cursor.endEditBlock();
    } else
    {
        QTextBlockFormat bfmt;
        bfmt.setObjectIndex(-1);
        cursor.mergeBlockFormat(bfmt);
    }
}

void MyChild::closeEvent(QCloseEvent *event)
{
    if (this->maybeSave()) event->accept();
    else event->ignore();
}

QString MyChild::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
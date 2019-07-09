#include "Interface.h"
#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include "CodeRefactor.h"
#include <iostream>

using std::cout;
using std::endl;
using std::string;
using std::exception;

void Interface::getInputFileData()
{
    QString content;
    QFile file(getInputPath());
    if (file.open(QIODevice::ReadOnly)) {
        content = file.readAll();
        file.close();
    }
    setLeftText(content);
}

void Interface::getRefactorData()
{
    try
    {
        CodeRefactor c;
        c.setFor_While(For_While);
        c.setWhile_For(While_For);
        c.setMCIf_SCMIf(MCIf_SCMIf);
        c.setMIf_Switch(MIf_Switch);
        c.setSCMIf_MCIf(SCMIf_MCIf);
        c.setSwitch_MIf(Switch_MIf);
        c.run(m_leftText);
        setRightText(QString::fromStdString(c.show()));
    } catch (exception &e)
    {
        emit parseError("语法错误: "+QString::fromStdString(e.what()));
    }
}

void Interface::refreshData()
{
    auto str1=getLeftText();
    auto str2=getRightText();

    auto compare1=str2.toStdString();
    auto list1=str1.split("<br>");
    string::size_type num=0;
    for(auto &i:list1){
        i.insert(i.size(), "<br>");
        auto ptr=compare1.find(i.toStdString(), num);
        if(ptr==compare1.npos){
            i.insert(0, "<font style=\"color:red;\">");
            i.insert(i.length(), "</font>");
        }
        else{
            num=ptr;
        }
    }

    auto compare2=str1.toStdString();
    auto list2=str2.split("<br>");
    num=0;
    for(auto &i:list2){
        i.insert(i.size(), "<br>");
        auto ptr=compare2.find(i.toStdString(), num);
        if(ptr==compare2.npos){
            i.insert(0, "<font style=\"color:red;\">");
            i.insert(i.length(), "</font>");
        }
        else{
            num=ptr;
        }
    }
    QString leftText="";
    for(auto i:list1){
        leftText+=i;
    }
    QString rightText="";
    for(auto i:list2){
        rightText+=i;
    }
    emit textRefreshData(leftText, rightText);
}

void Interface::writeOutputFileData()
{
    QFile file(getOutputPath());
    if (file.open(QIODevice::WriteOnly)) {
        file.write(m_rightText.data());
        file.close();
    }
}

void Interface::setMCIf_SCMIf(bool value)
{
    MCIf_SCMIf=value;
}

void Interface::setSCMIf_MCIf(bool value)
{
    SCMIf_MCIf=value;
}

void Interface::setMIf_Switch(bool value)
{
    MIf_Switch=value;
}

void Interface::setSwitch_MIf(bool value)
{
    Switch_MIf=value;
}

void Interface::setFor_While(bool value)
{
    For_While=value;
}

void Interface::setWhile_For(bool value)
{
    While_For=value;
}

Interface::Interface(QObject *parent)
    :QObject (parent)
{
    connect(this, &Interface::inputPathChanged, this, &Interface::getInputFileData);
}

QString Interface::getInputPath() const
{
    return m_inputPath;
}

void Interface::setInputPath(const QString &inputPath)
{
    auto path=inputPath;
    path.remove(QString("file://"), Qt::CaseInsensitive);
    if(!QFileInfo(path).isFile()){
        emit parseError("路径错误："+path+QString::fromStdString("不是一个文件"));
        return ;
    }
    m_inputPath = path;
    emit inputPathChanged();
}

QString Interface::getOutputPath() const
{
    return m_outputPath;
}

void Interface::setOutputPath(const QString &outputPath)
{
    m_outputPath = outputPath;
    m_outputPath.remove(QString("file://"), Qt::CaseInsensitive);
    emit outputPathChanged();
}

string Interface::richText_plainText(const QString &data)
{
    auto str=data;
    str.remove("<font style=\"color:red;\">");
    str.remove("</font>");

    str.replace("<br>", "\n");
    str.replace("&nbsp;&nbsp;", "\t");
    str.replace("&nbsp;", " ");
    str.replace("&gt;", ">");
    str.replace("&lt;", "<");
    str.replace("&quot;", "\"");
    str.replace("&amp;", "&");

    return str.toStdString();
}
QString Interface::plainText_richText(const string &data)
{
    auto str=QString(data.data());
    str.replace("&", "&amp;");
    str.replace("\"", "&quot;");
    str.replace("<", "&lt;");
    str.replace(">", "&gt;");
    str.replace(" ", "&nbsp;");
    str.replace("\t", "&nbsp;&nbsp;");
    str.replace("\n", "<br>");

    return str;
}

QString Interface::getRightText()
{
    return plainText_richText(m_rightText);
}

void Interface::setRightText(const QString &rightText)
{
    m_rightText = rightText.toStdString();
    emit rightTextChanged();
}

QString Interface::getLeftText()
{
    return plainText_richText(m_leftText);
}

void Interface::setLeftText(const QString &leftText)
{
    m_leftText = leftText.toStdString();
    emit leftTextChanged();
}

#include "Interface.h"
#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include "CodeRefactor.h"

QString Interface::getInputFileData()
{
    QString content;
    QFile file(getInputPath());
    if (file.open(QIODevice::ReadOnly)) {
        content = file.readAll();
        file.close();
    }
    return content;
}

QString Interface::getRefactorData(const QString &data)
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
		c.run(data.toStdString());
		return QString::fromStdString(c.show());
	} catch (std::exception &e)
	{
        emit parseError("语法错误: "+QString::fromStdString(e.what()));
		return "";
    }
}

void Interface::writeOutputFileData(QString data)
{
    QFile file(getOutputPath());
    if (file.open(QIODevice::WriteOnly)) {
        file.write(data.toLatin1().data());
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
    emit inputPathChanged(m_inputPath);
}

QString Interface::getOutputPath() const
{
    return m_outputPath;
}

void Interface::setOutputPath(const QString &outputPath)
{
    m_outputPath = outputPath;
    m_outputPath.remove(QString("file://"), Qt::CaseInsensitive);
    emit outputPathChanged(m_outputPath);
}

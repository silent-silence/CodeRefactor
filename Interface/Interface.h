#ifndef INTERFACE_INTERFACE_H
#define INTERFACE_INTERFACE_H

#include <QObject>
#include <QString>
#include "CodeRefactor.h"

class Interface : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString inputPath READ getInputPath WRITE setInputPath NOTIFY inputPathChanged)
    Q_PROPERTY(QString outputPath READ getOutputPath WRITE setOutputPath NOTIFY outputPathChanged)
public:
    Q_INVOKABLE QString getInputFileData();
    Q_INVOKABLE QString getRefactorData(const QString &data);
    Q_INVOKABLE void writeOutputFileData(QString data);
    Q_INVOKABLE void setMCIf_SCMIf(bool value);
    Q_INVOKABLE void setSCMIf_MCIf(bool value);
    Q_INVOKABLE void setMIf_Switch(bool value);
    Q_INVOKABLE void setSwitch_MIf(bool value);
    Q_INVOKABLE void setFor_While(bool value);
    Q_INVOKABLE void setWhile_For(bool value);

    explicit Interface(QObject *parent = nullptr);

    QString getInputPath() const;
    void setInputPath(const QString &inputPath);

    QString getOutputPath() const;
    void setOutputPath(const QString &outputPath);

signals:
    void inputPathChanged(const QString& source);
    void outputPathChanged(const QString& source);
    void parseError(const QString &e);
private:
    bool MCIf_SCMIf;
    bool SCMIf_MCIf;
    bool MIf_Switch;
    bool Switch_MIf;
    bool For_While;
    bool While_For;
    QString m_inputPath;
    QString m_outputPath;
};

#endif // INTERFACE_INTERFACE_H

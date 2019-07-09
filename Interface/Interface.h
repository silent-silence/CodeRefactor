#ifndef INTERFACE_INTERFACE_H
#define INTERFACE_INTERFACE_H

#include <QObject>
#include <QString>
#include "CodeRefactor.h"

class Interface : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString inputPath READ getInputPath WRITE setInputPath)
    Q_PROPERTY(QString outputPath READ getOutputPath WRITE setOutputPath)
    Q_PROPERTY(QString leftText READ getLeftText WRITE setLeftText NOTIFY leftTextChanged)
    Q_PROPERTY(QString rightText READ getRightText WRITE setRightText NOTIFY rightTextChanged)
public:
    Q_INVOKABLE void getRefactorData();
    Q_INVOKABLE void refreshData();
    Q_INVOKABLE void writeOutputFileData();

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

    QString getLeftText();
    void setLeftText(const QString &leftText);

    QString getRightText();
    void setRightText(const QString &rightText);

signals:
    void leftTextChanged();
    void rightTextChanged();
    void textRefreshData(const QString &leftText, const QString &rightText);
    void parseError(const QString &e);

    void inputPathChanged();
    void outputPathChanged();
private:
    void getInputFileData();
    std::string richText_plainText(const QString &data);
    QString plainText_richText(const std::string &data);

    bool MCIf_SCMIf;
    bool SCMIf_MCIf;
    bool MIf_Switch;
    bool Switch_MIf;
    bool For_While;
    bool While_For;

    std::string m_leftText;
    std::string m_rightText;
    QString m_inputPath;
    QString m_outputPath;
};

#endif // INTERFACE_INTERFACE_H

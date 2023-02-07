#ifndef GBEMULATOR_APPLICATIONDATA_HPP
#define GBEMULATOR_APPLICATIONDATA_HPP

#include <QtCore>

class ApplicationData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString romFile READ romFile)

  public:
    void setRomFile(const QString& file)
    {
        _romFile = file;
    }

    QString romFile() const
    {
        return _romFile;
    }

  private:
    QString _romFile = "";
};

#endif // GBEMULATOR_APPLICATIONDATA_HPP

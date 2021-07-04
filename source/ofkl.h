#ifndef OFKL_H
#define OFKL_H

#include <alias.h>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Ofkl; }
QT_END_NAMESPACE

class Ofkl : public QMainWindow
{
    Q_OBJECT

public:
    Ofkl(QWidget *parent = nullptr);
    ~Ofkl();

protected:
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;

private slots:

    void on_runBox_textEdited(const QString &arg1);

    void on_runBox_returnPressed();

private:
    Ui::Ofkl *ui;
    QString args;

    void createActions();
    void createTrayIcon();
    void createProcess(QString processName, QString args, QString verb);


    QAction *minimizeAction;
    QAction *showOptionsAction;
    QAction *restoreAction;
    QAction *quitAction;

    QMenu *trayMenu;
    QSystemTrayIcon *trayIcon;
    QString loadTextFile();
    Alias createAliasFromCsv(QString line);
    void parseCSVFile(QString filePath);

    QList<Alias> aliases;
    void createAliases();

    QStringList searchDirs;

    QListWidget searchList;
};
#endif // OFKL_H

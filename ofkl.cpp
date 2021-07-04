#include "ofkl.h"
#include "ui_ofkl.h"
#include "qt_windows.h"
#include "qwindowdefs_win.h"

#include <alias.h>

#include <shellapi.h>
#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QProcess>
#include <QDesktopServices>
#include <QInputDialog>
#include <QFile>
#include <options.h>
#include <QDir>
#include <QStandardItemModel>
#include <QCompleter>
#include <QStyleFactory>
#include <QDirIterator>
#include <QStandardPaths>

Ofkl::Ofkl(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Ofkl)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    createActions();
    createTrayIcon();
    trayIcon->show();
    ui->runBox->clear();

    move(QGuiApplication::primaryScreen()->size().width() / 2 - size().width() / 2, QGuiApplication::primaryScreen()->size().height() / 2 - size().height() / 2);

    searchList.setAttribute(Qt::WA_ShowWithoutActivating);
    searchList.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    searchList.resize(650, 83);
    searchList.move(pos().x() + 0, pos().y() + 93);

    if(!RegisterHotKey(HWND(winId()), 0, MOD_ALT, 0x20))
    {
        QMessageBox::warning(this, "Warning", "Unable to register hotkey");
    }

    searchDirs << "C:/Users/ogsew/Desktop" << "C:/Users/ogsew/AppData/Roaming/Microsoft/Windows/Start Menu/Programs" << "C:/ProgramData/Microsoft/Windows/Start Menu/Programs";

    createAliases();
}

Ofkl::~Ofkl()
{
    delete ui;
}

void Ofkl::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        hide();
    }
    if(event->key() == Qt::Key_Tab)
    {
        args = QInputDialog::getText(this, "Enter Program Parameters", "Parameters:", QLineEdit::Normal, args);
    }
}

void Ofkl::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}

void Ofkl::showEvent(QShowEvent *event)
{
    ui->runBox->clear();
}

void Ofkl::createActions()
{
    minimizeAction = new QAction("Minimize", this);
    connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);

    showOptionsAction = new QAction("Options", this);
    connect(showOptionsAction, &QAction::triggered, this, [=]()
    {
        Options *optionsWindow = new Options();
        optionsWindow->show();
    });

    restoreAction = new QAction("Restore", this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    quitAction = new QAction("Quit", this);
    connect(quitAction, &QAction::triggered, this,[=]()
    {
        searchList.close();
        QCoreApplication::quit();
    });
}

void Ofkl::createTrayIcon()
{
    trayMenu = new QMenu(this);
    trayMenu->addAction(showOptionsAction);
    trayMenu->addSeparator();
    trayMenu->addAction(minimizeAction);
    trayMenu->addAction(restoreAction);
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayMenu);
    trayIcon->setIcon(QIcon(":/images/trash.png"));
}

void Ofkl::createProcess(QString processName, QString args, QString verb)
{
    //    QProcess *newProcess = new QProcess();
    //    newProcess->startDetached("explorer.exe Shell:::{2559a1f3-21d7-11d4-bdaf-00c04f60b9f0}");

    SHELLEXECUTEINFO *exInfo = new SHELLEXECUTEINFO();

    exInfo->cbSize = sizeof(SHELLEXECUTEINFO);
    exInfo->fMask = NULL;
    exInfo->hwnd = NULL;
    exInfo->lpVerb = reinterpret_cast<const WCHAR*>(verb.utf16());
    exInfo->lpFile = reinterpret_cast<const WCHAR*>(processName.utf16());
    exInfo->lpParameters = reinterpret_cast<const WCHAR*>(args.utf16());
    exInfo->lpDirectory = NULL;
    exInfo->nShow = SW_NORMAL;
    exInfo->hInstApp = NULL;

    long result = (long)ShellExecuteEx(exInfo);



    //    QDesktopServices::openUrl(QUrl::fromLocalFile(processName));
}

bool Ofkl::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
{
    Q_UNUSED(eventType);
    Q_UNUSED(result);
    MSG* msg = static_cast<MSG*>(message);
    if(msg->message == WM_HOTKEY)
    {
        if(isVisible() == false)
        {
            showNormal();
            activateWindow();
        }
        else
        {
            searchList.hide();
            hide();
        }
        return true;
    }
    return false;
}

void Ofkl::parseCSVFile(QString filePath)
{
    QFile inputFile(filePath);
    inputFile.open(QIODevice::ReadOnly);

    QTextStream in(&inputFile);
    while(!in.atEnd())
    {
        QString line = in.readLine();
        Alias newAlias = createAliasFromCsv(line);
        aliases.append(newAlias);
    }
}

void Ofkl::createAliases()
{
    aliases.clear();

    //Load aliases from csv
    parseCSVFile(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/aliases.csv");

    //Load aliases from search folders
    for(int i=0; i < searchDirs.count(); i++)
    {
        QDirIterator it(searchDirs[i], QStringList() << "*", QDir::Files, QDirIterator::Subdirectories);
        while(it.hasNext())
        {
//            qInfo() << it.filePath() + "/" + it.fileName();
            Alias newAlias = Alias(it.fileInfo().baseName(), it.filePath(), "", it.fileInfo().suffix());

            aliases.append(newAlias);
            it.next();
        }
    }

    //runBox stuff
    ui->runBox->clear();

    QStringList aliasTextList;

    for(int i=1; i < aliases.count(); i++)
    {
        //        ui->runBox->addItem(aliases[i].alias);
        //        ui->listWidget->addItem(aliases[i].alias);
        aliasTextList.append(aliases[i].alias);

    }

    searchList.clear();
    searchList.addItems(aliasTextList);

    QCompleter *completer = new QCompleter(aliasTextList, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::InlineCompletion);
    completer->setFilterMode(Qt::MatchStartsWith);

    connect(completer, QOverload<const QModelIndex &>::of(&QCompleter::activated), [=](const QModelIndex &index)
    {
        qInfo() << ui->runBox->text();
    });

    ui->runBox->setCompleter(completer);
}

Alias Ofkl::createAliasFromCsv(QString line)
{
    QStringList lineSplit = line.split(",");
    qInfo() << lineSplit;

    Alias *newAlias = new Alias(lineSplit[0], lineSplit[1], lineSplit[2], "alias");
    return *newAlias;
}


void Ofkl::on_runBox_textEdited(const QString &runBoxText)
{
    //    QList<QListWidgetItem *> out = ui->listWidget->findItems(runBoxText, Qt::MatchContains);

    //    if(out.size() > 0)
    //    {
    //        ui->listWidget->setCurrentItem(out[0]);
    //    }

    //    searchList.addItem(ui->runBox->completer()->currentCompletion());
    if(ui->runBox->text() == "")
    {
        searchList.hide();
    }
    else
    {
        searchList.show();
        QList<QListWidgetItem *> out = searchList.findItems(runBoxText, Qt::MatchStartsWith);

        if(out.size() > 0)
        {
            searchList.setCurrentItem(out[0]);
        }
    }
}


void Ofkl::on_runBox_returnPressed()
{
    //    if(ui->runBox->completer()->completionCount())
    //    {
    //    ui->runBox->setText(ui->runBox->completer()->currentCompletion());
    QString _verb;
    if(Qt::KeyboardModifiers().testFlag(Qt::ShiftModifier))
    {
        _verb = "runas";
    }

    searchList.hide();
    hide();

    for(int i=1; i < aliases.count(); i++)
    {
        if(ui->runBox->text() == aliases[i].alias)
        {
            createProcess(aliases[i].fullPath, args, _verb);
            args = "";

            return;
        }
    }

    createProcess(ui->runBox->text(), args, _verb);

    args = "";
    //    }
    //    else
    //    {
    //        qInfo() << "  ";
    ////        ui->runBox->setText(ui->runBox->completer()->currentCompletion());
    //        ui->runBox->completer()->complete();
    //    }
}


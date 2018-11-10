/**
 ** Copyright (C) 2011-2017 Tycho Softworks.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "main.hpp"
#include "about.hpp"
#include "viewer.hpp"
#include "args.hpp"
#include "ui_main.h"

#include <QStandardPaths>
#include <QFileDialog>
#include <QComboBox>
#include <QGraphicsPixmapItem>
#include <QDesktopServices>
#include <QDebug>
#include <QTranslator>

static Ui::MainWindow ui;
static QString dirPrefix, fileFilter;
static Viewer *currentView = nullptr;

Main *Main::Instance = nullptr;
bool Main::Sensitive = false;

Main::Main(const QStringList& args, bool reset) :
settings(CONFIG_FROM)
{
    if(args.count() > 0)
        dirPrefix = args[0];

    if(args.count() > 1)
        fileFilter = args[1];

    Q_ASSERT(Instance == nullptr);

    if(reset)
        settings.clear();

#ifdef PROJECT_TESTDATA
    if(dirPrefix.isEmpty())
        dirPrefix = PROJECT_TESTDATA;
    if(fileFilter.isEmpty())
        fileFilter = ".txt";
#endif

    Instance = this;
    index = nullptr;
    options = nullptr;

    if(dirPrefix.isEmpty())
        QDir::setCurrent(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    else
        QDir::setCurrent(dirPrefix);

    dir.setPath(QDir::currentPath());
    history.append(dir.path());

    // ui setup and object initialization...

    setToolButtonStyle(Qt::ToolButtonIconOnly);
    setIconSize(QSize(16, 16));         // uniform icon size...

    ui.setupUi(static_cast<QMainWindow *>(this));
    ui.filterTypes->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui.searchName->setAttribute(Qt::WA_MacShowFocusRect, false);
    toolbar = new Toolbar(this, ui.toolBar);
    statusbar = new Statusbar(ui.centralwidget, ui.statusBar);
    statusbar->enableSettings();

    setContextMenuPolicy(Qt::CustomContextMenu);
    ui.indexView->setContextMenuPolicy(Qt::CustomContextMenu);

    // apply and initialize settings...

    qDebug() << "Dir Prefix" << dirPrefix << ", File Filter " << fileFilter;
    if(!fileFilter.isEmpty()) {
        ui.filterTypes->setText(fileFilter);
        ui.filterTypes->setDisabled(true);
    }
    else
        ui.filterTypes->setText(settings.value("filter", ".txt;.log").toString());

#ifdef PROJECT_TESTDATA
    Sensitive = false;
    Find::setSensitive(false);
#else
    Sensitive = settings.value("filename", false).toBool();
    Find::setSensitive(settings.value("textbody", false).toBool());
#endif

    if(dirPrefix.isEmpty()) {
        int paths = settings.beginReadArray("paths");
    //  qDebug() << "SIZE " << paths << endl;
        for(int path = 0; path < paths; ++path) {
            settings.setArrayIndex(path);
            QString temp = settings.value("path").toString();
    //      qDebug() << "STR " << path << " VALUE " << temp << endl;
            if(temp == dir.path())
                continue;
            history.append(temp);
        }
        settings.endArray();
    }

    resize(settings.value("size", QSize(760, 540)).toSize());
    sync();

    // app menus

#ifdef Q_OS_MAC
    auto appBar = new QMenuBar(this);
    auto appMenu = appBar->addMenu(tr("TextSeeker"));

    auto appQuit = new QAction(tr("&Quit"), appMenu);
    appQuit->setMenuRole(QAction::QuitRole);

    auto appAbout = new QAction(tr("&About"), appMenu);
    appAbout->setMenuRole(QAction::AboutRole);
    appAbout->setEnabled(true);

    auto appConfig = new QAction(tr("&Settings"), appMenu);
    appConfig->setMenuRole(QAction::PreferencesRole);
    appConfig->setEnabled(true);

    connect(appQuit, &QAction::triggered, qApp, &QApplication::quit);
    connect(appConfig, &QAction::triggered, this, &Main::changeSettings);
    connect(appAbout,  &QAction::triggered, this, &Main::about);

    appMenu->addAction(appAbout);
    appMenu->addAction(appConfig);
    appMenu->addAction(appQuit);
    appBar->show();
#endif

    // initialize connections and states

    connect(ui.actionAbout, &QAction::triggered, this, &Main::about);
    connect(ui.actionClear, &QAction::triggered, this, &Main::clear);
    connect(ui.actionOptions, &QAction::triggered, this, &Main::changeSettings);
    connect(ui.actionQuit, &QAction::triggered, qApp, &QCoreApplication::quit);

    connect(ui.pathButton, &QPushButton::pressed, this, &Main::changeDir);
    connect(ui.pathBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &Main::selectDir);
    connect(ui.actionSearch, &QAction::triggered, this, &Main::reloadMatches);
    connect(ui.searchName, &QLineEdit::returnPressed, this, &Main::reloadMatches);
    connect(ui.filterTypes, &QLineEdit::returnPressed, this, &Main::reloadMatches);

    connect(ui.tabs, &QTabWidget::tabCloseRequested, this, &Main::closeTab);
    connect(ui.tabs, &QTabWidget::currentChanged, this, &Main::changeTab);
    connect(ui.indexView, &QAbstractItemView::activated, this, &Main::openAt);
    connect(ui.actionView, &QAction::triggered, this, &Main::openViewer);
    connect(ui.actionOpen, &QAction::triggered, this, &Main::openDocument);

    connect(this, &QMainWindow::customContextMenuRequested, this, &Main::showContextMenu);
    connect(ui.indexView, &QTableView::customContextMenuRequested, this, &Main::openContext);

    // adding history triggers selectDir...
    ui.pathBox->addItems(history);

    clear();
    show();
}

Main::~Main()
{
    sync();
}

void Main::sync()
{
#ifndef PROJECT_TESTDATA
    if(fileFilter.isEmpty())
        settings.setValue("filter", ui.filterTypes->text());

    if(dirPrefix.isEmpty()) {
        int pos = 0;
        settings.beginWriteArray("paths");
        while(pos < history.size()) {
            settings.setArrayIndex(pos);
            settings.setValue("path", history[pos++]);
        }
        settings.endArray();
    }

    settings.setValue("filename", Sensitive);
    settings.setValue("textbody", Find::sensitive());
    settings.setValue("size", size());
    settings.sync();
#endif
}

void Main::status(const QString& text)
{
    ui.statusBar->setStyleSheet("color: black");
    ui.statusBar->showMessage(text);
    ui.statusBar->update();
    ui.statusBar->repaint();
    update();
}

void Main::error(const QString& text)
{
    ui.statusBar->setStyleSheet("color: red");
    ui.statusBar->showMessage(text);
    ui.statusBar->update();
    ui.statusBar->repaint();
    update();
}

void Main::about()
{
    About info(this);
    info.exec();
}

void Main::clear()
{
    ui.indexView->setModel(nullptr);
    ui.searchName->setText("*");
    toolbar->clearSearch();
    status(tr("ready"));

    if(index) {
        delete index;
        index = nullptr;
    }
}

void Main::reloadMatches()
{
    QString match = toolbar->searching();
    toolbar->disableSearch();
    statusbar->disableHome();
    status(tr("searching...please wait..."));
    this->update();

    ui.indexView->setModel(nullptr);
    ui.tabs->setCurrentIndex(0);
    if(index) {
        delete index;
        index = nullptr;
    }

    QString filters = ui.filterTypes->text();
    filters.replace(QChar(','), QChar(';'));
    filters.remove(QChar('*'));

    auto ind = new Index(ui.searchName->text(), filters.split(";"), match, Sensitive);
    connect(ind, &Index::updateIndex, this, &Main::updateIndex);
    ind->start();
    setEnabled(false);
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
}

void Main::searchHome()
{
    closeTab(0);
}

void Main::updateIndex(Index *ind)
{
    index = ind;
    ui.indexView->setModel(index);
    toolbar->enableSearch();
    statusbar->enableHome();
    setEnabled(true);
    status(tr("ready"));
    QApplication::restoreOverrideCursor();
}

void Main::openViewer()
{
    openAt(ui.indexView->currentIndex());
}

void Main::openDocument()
{
    QModelIndex pos = ui.indexView->currentIndex();
    QString name = index->name(pos.row());

    status(tr("opening ") + name);
    name = ui.pathBox->currentText() + QDir::separator() + name;

    QUrl url = QUrl::fromLocalFile(name);

    if(QDesktopServices::openUrl(url)) {
        status(tr("ready"));
        return;
    }

    error(tr("failed to open ") + name);
}

void Main::openAt(const QModelIndex& cursor)
{
    QString name = index->name(cursor.row());

    for(int tab = 1; tab < ui.tabs->count(); ++tab) {
        if(ui.tabs->tabText(tab) == name) {
            ui.tabs->setCurrentIndex(tab);
            return;
        }
    }

    status(tr("loading ") + name);
    new Viewer(ui.tabs, name);
    status(tr("loaded ") + name);
}

void Main::openContext(const QPoint& pos)
{
    QMenu m;
    m.addAction(ui.actionView);
    m.addAction(ui.actionOpen);
    m.exec(ui.indexView->mapToGlobal(pos));
}

void Main::selectDir(int selected)
{
    if(selected < 0)
        return;

    QString path = history.takeAt(selected);
    QDir::setCurrent(path);
    dir.setPath(QDir::currentPath());
//  qDebug() << "SELECT PATH " << path << " DIR " << dir.path() << endl;
    history.insert(0, dir.path());

    ui.indexView->setModel(nullptr);
    if(index) {
        delete index;
        index = nullptr;
    }
}

void Main::changeSettings()
{
    if(!options)
        options = new Options(ui.tabs);

    ui.tabs->setCurrentIndex(options->tabIndex());
    ui.tabs->setTabsClosable(true);
    statusbar->disableSettings();
}

void Main::showContextMenu(const QPoint& pos)
{
    QMenu m;
    m.addAction(ui.actionAbout);
    m.addAction(ui.actionOptions);

    m.addSeparator();
    m.addAction(ui.actionClear);
    m.addAction(ui.actionQuit);
    m.exec(mapToGlobal(pos));
}

void Main::changeDir()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Directory"), dir.path());

    if(!path.isNull()) {
        QDir::setCurrent(path);
        dir.setPath(QDir::currentPath());

        int pos = history.size();
        while(pos > 0) {
            --pos;

            if(dir.path() == history[pos])
                history.takeAt(pos);
            else if(!QDir(history[pos]).exists())
                history.takeAt(pos);
        }

        // no more than 10 elements...
        while(history.size() > 9)
            history.takeLast();

        history.insert(0, dir.path());

        // triggers select dir...
        ui.pathBox->clear();
        ui.pathBox->addItems(history);

        // rest in selectDir
    }
}

void Main::searchText()
{
    qDebug() << "F3 KEY";
    if(currentView)
        currentView->findNextText();
}

void Main::enableFind()
{
    qDebug() << "FIND KEY";
    if(currentView)
        currentView->activateFind();
}

void Main::changeTab(int tab)
{
    auto priorView = currentView;

    qDebug() << "CHANGED " << tab;
    if(options && options->tabIndex() == tab)
        statusbar->disableSettings();
    else
        statusbar->enableSettings();

    if(tab == 0 || (options && options->tabIndex() == tab)) {
        statusbar->disableSearch();
        currentView = nullptr;
    }
    else {
        statusbar->enableSearch();
        currentView = static_cast<Viewer *>(ui.tabs->widget(tab));
    }
    if(priorView && priorView != currentView)
        priorView->release();
}

void Main::closeCurrent()
{
    closeTab(ui.tabs->currentIndex());
}

void Main::closeTab(int tab)
{
    // close of index tab actually closes all other open manpages
    if(tab == 0) {
        int count = ui.tabs->count();
        for(tab = 1; tab < count; ++tab)
            closeTab(1);
        clear();
        toolbar->homeOnSearch();
        return;
    }

    if(options && options->tabIndex() == tab) {
        ui.tabs->removeTab(tab);
        options->deleteLater();
        options = nullptr;
        statusbar->enableSettings();
    }
    else {
        auto view = static_cast<Viewer *>(ui.tabs->widget(tab));
        if(!view)
            return;

        if(view == currentView)
            currentView = nullptr;

        ui.tabs->removeTab(tab);
        view->deleteLater();
    }

    if(ui.tabs->count() < 2)
        ui.tabs->setTabsClosable(false);
}

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationVersion(PROJECT_VERSION);
    QCoreApplication::setOrganizationName("Tycho Softworks");
    QCoreApplication::setOrganizationDomain("tychosoft.com");
    QCoreApplication::setApplicationName("TextSeeker");
    QTranslator localize;

    QApplication app(argc, argv);
    QCommandLineParser args;
    Q_INIT_RESOURCE(desktop);

#if defined(Q_OS_MAC)
    localize.load(QLocale::system().name(), "textseeker", "_", 
        Args::exePath("../Translations"));
#elif defined(Q_OS_WIN)
    localize.load(QLocale::system().name(), "textseeker", "_", 
        Args::exePath("./Translations"));
#else
    localize.load(QLocale::system().name(), "textseeker", "_", 
        Args::exePath("../share/translations"));
#endif
    if(!localize.isEmpty())
        QApplication::installTranslator(&localize);

#ifdef Q_OS_MAC
    QFile style(":/styles/macos.css");
#else
    QFile style(":/styles/desktop.css");
#endif

    if(style.exists()) {
        style.open(QFile::ReadOnly);
        QString css = QLatin1String(style.readAll());
        style.close();
        qApp->setStyleSheet(css);
    }
    else
        abort();

    args.setApplicationDescription("Simple Text Viewer and Search Tool");
    Args::add(args, {
        {Args::HelpArgument},
        {Args::VersionArgument},
        {{"reset"}, "Reset Config"},
        {{{"prefix"}, "Directory prefix to use."}},
        {{{"filter"}, "Filter by file types."}},
    });

    args.process(app);
    Main w(args.positionalArguments(), args.isSet("reset"));
    w.show();
    return QApplication::exec();
}



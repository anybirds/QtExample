#include <QtWidgets>

#include "painter.h"
#include "scribblearea.h"

Painter::Painter()
{
    scribbleArea = new ScribbleArea();
    setCentralWidget(scribbleArea);
    createActions();
    createMenus();
    setWindowTitle(tr("Scribble"));
    resize(500, 500);
}

void Painter::closeEvent(QCloseEvent *event) {
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void Painter::open() {
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open image"), QDir::currentPath());
        if (!fileName.isEmpty()) {
            scribbleArea->openImage(fileName);
        }
    }
}

void Painter::save() {
    QAction *action = qobject_cast<QAction*>(sender());
    QByteArray buf = action->data().toByteArray();
    saveFile(buf);
}

void Painter::penColor() {
    // creates a user prompt that shows color options. scribbleArea->penColor() is used as an initial color setting.
    QColor newColor = QColorDialog::getColor(scribbleArea->penColor(), this);
    if (newColor.isValid()) {
        scribbleArea->setPenColor(newColor);
    }
}

void Painter::penWidth() {
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Scribble"), tr("Select pen width: "), scribbleArea->penWidth(), 1, 50, 1, &ok);
    if (ok) {
        scribbleArea->setPenWidth(newWidth);
    }
}

void Painter::about() {
    QMessageBox::about(this, tr("About Scribble"), tr("<p>The <b>Scribble</b> example is awesome.</p>"));
}

void Painter::createActions() {
    // not so much differernt from creating ui into it ... I think it would be possible to recreate the whole thing without using scribblearea
    openAct = new QAction("&Open", this);
    openAct->setShortcut(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1...").arg(QString(format).toUpper());
        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, SIGNAL(triggered()), this, SLOT(save()));
        saveAsActs.append(action);
    }

    printAct = new QAction(tr("&Print..."), this);
    connect(printAct, SIGNAL(triggered()), scribbleArea, SLOT(print()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    penColorAct = new QAction(tr("&Pen Color..."), this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));

    penWidthAct = new QAction(tr("&Pen Width..."), this);
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

    clearScreenAct = new QAction(tr("&Clear screen..."), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, SIGNAL(triggered()), scribbleArea, SLOT(clearImage()));

    aboutAct = new QAction(tr("&About..."), this);
    connect(aboutAct, SIGNAL(triggered()), SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt..."), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void Painter::createMenus() {
    saveAsMenu = new QMenu(tr("&Save as"), this);
    foreach(QAction *action, saveAsActs) {
        saveAsMenu->addAction(action);
    }

    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    optionMenu = new QMenu("&Options", this);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addSeparator();
    optionMenu->addAction(clearScreenAct);
    helpMenu = new QMenu("&Help", this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(helpMenu);
}

bool Painter::maybeSave() {
    if (scribbleArea->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Scribble"),
                                   tr("The image has been modified\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) {
            saveFile("png");
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}


bool Painter::saveFile(const QByteArray &fileFormat) {
    QString initialPath = QDir::currentPath() + "/untitled." + QString(fileFormat);
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                   initialPath,
                                   tr("%1 Files (*.%2);; All Files(*)")
                                   .arg(QString::fromLatin1(fileFormat.toUpper()))
                                   .arg(QString::fromLatin1(fileFormat)));
    if (fileName.isEmpty()) {
        return false;
    }
    return scribbleArea->saveImage(fileName, fileFormat.constData());
}

Painter::~Painter()
{

}


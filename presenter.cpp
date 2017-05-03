#include "presenter.h"
Presenter* Presenter::INSTANCE = 0;

Presenter::Presenter(QObject *parent) : QObject(parent)
{
    mainDocument=nullptr;
}


bool Presenter::convertPPT(QString filePath, QString saveFileName)
{

    if(filePath.isEmpty())
        return  false;
    QFile myFile(filePath);
    if(!myFile.exists()||!myFile.open(QIODevice::ReadOnly))
        return false;
    myFile.close();
    QProcess myProcess;
    QTemporaryDir tempDirectory;
    QString tempDirPath="";
    if(tempDirectory.isValid())
        tempDirPath=tempDirectory.path();
    else
        return false;
    QString processCommand=QString("libreoffice --convert-to pdf  \"%0\" --outdir %1").arg(filePath,tempDirPath);
    int res=myProcess.execute(processCommand);

    qDebug()<<processCommand;
    QFileInfo fileInfo(filePath);
    QString convertedFilePath=tempDirectory.path()+"/"+fileInfo.baseName()+".pdf";

    QString newFile="pdfs/"+saveFileName;
    if (QFile::exists(newFile))
    {
        QFile::remove(newFile);
    }
    return QFile::rename(convertedFilePath,newFile);

}

int Presenter::loadPdf(QString filename)
{

    endPresent();

    if(filename.isEmpty()||!QFile::exists(filename))
        return -202;

    mainDocument=NULL;

    mainDocument = Poppler::Document::load(filename);

    if (!mainDocument || mainDocument->isLocked()) {
        // ... error message ....
        qDebug() << "Error1";
        delete mainDocument;
        mainDocument =NULL;

        return -203;
    }

    mainDocument->setRenderHint(Poppler::Document::Antialiasing);

    pr = new PageRenderer();

    auto count = pageCount();
    for(int i=0;i<count;i++)
    {
        pr->addPage(i,mainDocument->page(i));;
    }
    return setCurrentPage(1);
}


void Presenter::endPresent()
{
    delete mainDocument;
    delete this->pr;
}

int Presenter::setCurrentPage(int pageNumber)
{
    if(!mainDocument)
        return -201;
    if(mainDocument->numPages()<pageNumber||pageNumber<0)
        return -200;
//    Poppler::Page* pdfPage = mainDocument->page(pageNumber-1);  // Document starts at page 0
//    if (!pdfPage) {
//        return -204;
//    }

    // Generate a QImage of the rendered page

    emit setPage(pr->getImage(pageNumber-1));
//    delete pdfPage;

    return 0;
}

int Presenter::pageCount()
{
    return mainDocument->numPages();
}

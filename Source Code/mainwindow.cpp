#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPixmap>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pix(":/rec/login.jpg");
    int w = ui->label_pic ->width();
    int h = ui->label_pic ->height();
    ui->label_pic->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    ui->statusbar->addPermanentWidget(ui->label_3);
}

MainWindow::~MainWindow()
{
    delete ui;
}


// button to login operating system
void MainWindow::on_pushButton_Login_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();

    if(username == "test" && password == "test") {

        std::string serialise_path =  "";

        std::string root_path = ui->rootpath->text().toStdString();
        root_path = root_path +"/root";

        bool a = false;               // a: reconstructe, false -- from begining , true---reconstruct

        Directory* direct = fileSysInit(root_path, a);

        if (direct == NULL) {                       // for a = true          when the input root path is invalid

            QMessageBox::warning(this,"Invalid input root path", "invalid root path, please input again");
            return;
        }

        secDialog = new SecDialog(this, root_path ,serialise_path, !a, direct);    //second dialog show out for filesystem
        secDialog->show();
        ui->statusbar->showMessage("Username and password is correct",5000); //5秒

    } else {
        ui->statusbar->showMessage("Username and password is not correct"); //condition when user name and passport are incorrect
    }

}


//to initialize directory
#ifdef _WIN32
Directory* fileSysInit(std::string rootPath, bool &reconstruct)
{
    size_t convertedChars = 0;
    size_t pathsize = rootPath.length() + 1;
    wchar_t* wcstring = new wchar_t[pathsize - 1];
    mbstowcs_s(&convertedChars, wcstring, pathsize, rootPath.c_str(), _TRUNCATE);

    DWORD dwAttrib = GetFileAttributes(wcstring);

    bool exist = INVALID_FILE_ATTRIBUTES != dwAttrib && 0 != (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
    if (!exist)                                                   //if root path not exist
    {
        if (_access("_detail.txt", 0) == -1)
        {
            if (_mkdir(rootPath.c_str()) == 0)                    //if create the path seccessfully
            {
                reconstruct = false;                          //create new directory, do not reconstruct
                Directory* _direct = new Directory(rootPath);
                return _direct;                                // return directory pointer
            }
        }

    }
    else if (_access("_detail.txt", 0) != -1)                     // if the root path is already eaixt and we can find
    {

        reconstruct = true;                                   // file for reconstruction
        std::cout << "deserialise" << std::endl;
        Directory* _direct = new Directory(rootPath);         // do not need to _mkdir
        return _direct;                                       //return root directory for reconstruction
    }

    std::cout << "Invalid path" << std::endl;
    return NULL;
}
# else
Directory* fileSysInit(std::string rootPath,bool& reconstruct)
{

    int exist = access(rootPath.c_str(), F_OK);
    if (exist == -1)
    {
        if (access("_detail.txt", F_OK) == -1)
        {
            if (mkdir(rootPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0)
            {
                Directory* _direct = new Directory(rootPath);
                return _direct;
            }
        }
    }
    else if (access("_detail.txt", F_OK) != -1)
    {
        reconstruct = true;
        std::cout << "deserialise" << std::endl;
        Directory* _direct = new Directory(rootPath);
        return _direct;
    }

    std::cout << "Invalid path" << std::endl;
    return NULL;
}
# endif

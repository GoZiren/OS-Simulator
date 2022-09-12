#include "secdialog.h"
#include "ui_secdialog.h"

# include <iostream>
# include <fstream>

#include "QListWidgetItem"

#include <QDateTime>

#include <string>

#include "task1.h"
#include "task2.h"

#include <sstream>

# include <iostream>

# include <fstream>

#include <QDir>

#include <QMessageBox>

using namespace std;

#include <QProcess>


//全局变量directory
Directory* direct;

std::string serialise_path="";




string rootPath;


SecDialog::SecDialog(QWidget *parent, std::string root_Path, std::string serialise_Path, bool a, Directory* direct_):
    QDialog(parent),
    ui(new Ui::SecDialog)
{
    //global variable: serialisepath
    serialise_path = serialise_Path;
    rootPath = root_Path;

    //global variable: directory pointer
    direct = direct_;


    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(myfunction()));
    timer->start(1000); //每间隔1秒钟print "update.."


    //初始化路径 逆序列化过程

/*nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn*/

    bool test;
    if (a)
    {
        test = (*direct).Mkdir("test");
        test = (*direct).Mkdir("test1");
        test = (*direct).Mkdir("test1");
        test = (*direct).Mkdir("test2");
        test = (*direct).cd("test");
        test = (*direct).cd("..");
        (*direct).ls(true);
        test = (*direct).cd("test");
        test = (*direct).cd("/test2");
        test = (*direct).Mkdir("test");
        test = (*direct).cd("test");
        test = (*direct).Mkdir("test");
        test = (*direct).cd("test");
        std::cout << std::endl;
        test = (*direct).cd("../..");
        std::cout << std::endl;
        test = (*direct).cd("/test1");
        std::cout << std::endl;
        test = (*direct).cd("/test2/test/test");
        std::cout << std::endl;
        test = (*direct).cd("-");
        test = (*direct).cd("-");
        test = (*direct).cd("-");
        test = (*direct).cd("-");

        test = (*direct).cd("/test2");
        test = (*direct).find("test");

        test = (*direct).Mkdir("test3");
        test = (*direct).cd("test3");
        test = (*direct).Mkdir("test4");
        test = (*direct).Mkdir("test5");
        (*direct).ls();
        test = (*direct).cd("/test2");
        test = (*direct).mv("3", "a", 0);
        test = (*direct).mv("test3", "t", 0);
        (*direct).ls();
        test = (*direct).mv("t", "test3", 0);
        test = (*direct).mv("test3", "/", 0);
        (*direct).pwd();
        (*direct).ls();
        test = (*direct).cd("/");
        (*direct).ls();
        test = (*direct).cd("test3");
        (*direct).ls();
# ifdef _WIN32
        test = (*direct).mount("C:/a");
        test = (*direct).mount("C:/a/b");
        test = (*direct).mount("C:/a2");
# else
        test = (*direct).mount("/home/cuhksz_csc/Desktop/a");
# endif
        (*direct).pwd();

        test = (*direct).cd("/");
        (*direct).ls(true);


    }
    else
    {
        (*direct).deserialise("");
        test = (*direct).cd("/test3");
        (*direct).ls(true);
    }
    std::string str="";
    cout << "\nserialise" << endl;
    (*direct).serialise("");
    cout << "serialise" << endl << endl;

    test = (*direct).cd("/");
    (*direct).ls();
    (*direct).touch("c.txt");
    (*direct).ls();
    (*direct).open("c.txt",str);
     (*direct).repos(1, 0);
    (*direct).put("121212121", false, false);
    (*direct).close();

    (*direct).cat("c.txt",str);
    (*direct).open("c.txt",str);
    (*direct).lsOpen();
    (*direct).repos(1, 2);
    (*direct).put("added content", false, false);
    (*direct).cat("c.txt",str);
    (*direct).repos(0, 1);
    (*direct).get(5);
    (*direct).close();
    (*direct).lsOpen();
    cout<< test<< endl;
    //(*direct).rm("c.txt");
//	if (!a)
//	{
//		test = (*direct).Rmdir("test");
//		(*direct).ls();
//		test = (*direct).Rmdir("test1");
//		(*direct).ls();
//		test = (*direct).Rmdir("test2");
//		test = (*direct).Rmdir("test3");
//		test = (*direct).rm("c.txt");
//		(*direct).ls();
//	}


/*nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn*/

        QDir dir_root(QString::fromStdString(rootPath));

        int i = 0;
        foreach (QFileInfo var, dir_root.entryInfoList()) {
            i+=1;
            if (i>2) {

                /* present each item in listWidget according to
                 * root path and wording directory from root*/
                std::string path = var.absoluteFilePath().toStdString();
                std::string current  = path.substr(rootPath.size());

                if (var.isDir())
                    ui->listWidget->addItem("Dir |"+ QString::fromStdString(current));

                if(var.isFile())
                    ui->listWidget->addItem("File |" + QString::fromStdString(current));
        }
        }
        ui->current_direct->setText(QString::fromStdString((*direct).rwd()));
        (*direct).cd("/");



        //------------------------integrate process and memory------------

        //初始化整个os的memory
        Memory*m = new Memory;
        sysMemory = *m;
        calculator* w = new calculator;
        //window = *w;


        //初始化process
        ts1 = task1(m); //<-名字叫task1.cpp
        ts2 = task2(m); //<-名字叫task2.exe
        ts3 = task3(m,w); //名字叫task3.cpp

        //初始化process 的 vector
        psVector = new std::vector<Process>;

        psVector->push_back(ts1);
        psVector->push_back(ts2);
        psVector->push_back(ts3);

         ui->listWidget->addItem(QString::fromStdString(ts1.name));
         ui->listWidget->addItem(QString::fromStdString(ts2.name));
         ui->listWidget->addItem(QString::fromStdString(ts3.name));
         QString table = sysMemory.getTable();
         //sysMemory.showMemory();


         //打开第三个ui并更新
         thirdDialog = new ThirdDialog(table,this);
         thirdDialog->show();
}


/*to show current time*/
void SecDialog::myfunction()
{
    QTime time = QTime::currentTime();
    QString time_text = time.toString("hh : mm : ss");
    if ((time.second() % 2) == 0) {
        time_text[3] = ' ';
        time_text[8] = ' ';
    }
    ui->label_date_time->setText(time_text);
}

/*destructor*/
SecDialog::~SecDialog()
{
    (*direct).serialise(serialise_path);
    this->close();
    delete direct;
    delete ui;
}


/*
 * create new file in current directory
 */
void SecDialog::on_create_file_clicked() //create file
{

    string current_path = ui->current_direct->text().toStdString();

    string filename=ui->lineEdit_filename->text().toStdString();
    if ((filename.find("-") != std::string::npos)||
            (filename.find("~") != std::string::npos) ||
            (filename.find("/") != std::string::npos) ||
            (filename.find("\\") != std::string::npos) ||
             (filename.find("|") != std::string::npos) ||
             (filename.find("<") != std::string::npos) ||
             (filename.find(">") != std::string::npos) ||
             (filename.find("*") != std::string::npos) ||
             (filename.find("?") != std::string::npos) ||
            (filename.find("\"") != std::string::npos)
            )
    {
        QMessageBox::warning(this,"Invalid name","This name is invalid, please give a new name.");
        return;
    }

   bool test =  (*direct).touch(filename);
    if (test == false)
    {
        QMessageBox::warning(this,"Invalid name","Fail to create new folder");
        return;
    }
    string newfile;
    if (current_path=="/") {
        newfile = "File |" + current_path+filename;
    }
    else {
         newfile = "File |" + current_path+"/"+filename;
    }
     ui->listWidget->addItem(QString::fromStdString(newfile));
}




//directory back to upper directory
void SecDialog::on_pushButton_4_clicked()
{
    string path = ui->current_direct->text().toStdString();
    if (path == "/") {                                    // if the current working directory is already
        QMessageBox::warning(this,"Invalid button",
                             "Already root directory, cannot back further");                                                //root directory, "back" button cannot be pressed
        return;
    }

    ui->listWidget->clear();
    int pos = path.find_last_of("/");
    string sub = path.substr(0,pos);
    (*direct).cd("..");                                    // cd(..) represent for going bacl to upper directory

    if ((*direct).rwd()=="/"){
        ui->listWidget->addItem(QString::fromStdString(ts1.name));
        ui->listWidget->addItem(QString::fromStdString(ts2.name));
        ui->listWidget->addItem(QString::fromStdString(ts3.name));
    }

    std::string backPath = rootPath + sub;


    /* present folders and files in
     * back directory in uer interface*/
    QDir dir(QString::fromStdString(backPath));
    int i = 0;
    foreach (QFileInfo var, dir.entryInfoList()) {
        i+=1;
        if (i>2) {

            /* present each item in listWidget according to
             * root path and wording directory from root*/
            std::string path = var.absoluteFilePath().toStdString();
            std::string current  = path.substr(rootPath.size());

            if (var.isDir())
                ui->listWidget->addItem("Dir |"+ QString::fromStdString(current));

            if(var.isFile())
                ui->listWidget->addItem("File |" + QString::fromStdString(current));
}

        /*update current directory in ui*/
    ui->current_direct->setText(QString::fromStdString((*direct).rwd()));
}
}

//open and touch the file out to ui->pliantextEdit
void SecDialog::on_pushButton_open_clicked()
{

    string str = ui->listWidget->currentItem()->text().toStdString();
     int pos = str.find("|");
     int pos2 = str.find_last_of("/");
     string path = rootPath+str.substr(pos+1);
     if (str.substr(0,3)=="Fil") {
     cout<<path<<endl;
     std::string name = str.substr(pos2+1);
     //QFile file(QString::fromStdString(path));
     string result;
     bool test = (*direct).open(name,result);
     if (test == false)
     {
         QMessageBox::warning(this,"title","file not open, more information in terminal!");
         ui->plainTextEdit_details->appendPlainText(QString::fromStdString(result));
         return;
     }
     }


     if (str.substr(0,3)=="Dir") {
         std::string name = str.substr(pos2+1);
         (*direct).cd(name);
         ui->current_direct->setText(QString::fromStdString((*direct).rwd()));

         /* update user interface*/

         ui->listWidget->clear();
         QDir dir(QString::fromStdString(path));
         int i =0;
         foreach (QFileInfo var, dir.entryInfoList()) {
             i+=1;
             if (i>2) {

                 /* present each item in listWidget according to
                  * root path and wording directory from root*/

                 std::string path = var.absoluteFilePath().toStdString();
                 std::string current  = path.substr(rootPath.size());

                 if (var.isDir())
                     ui->listWidget->addItem("Dir |"+ QString::fromStdString(current));

                 if(var.isFile())
                     ui->listWidget->addItem("File |" + QString::fromStdString(current));
     }
     }
     }
}


/* write in a file you selected*/
void SecDialog::on_Button_write_clicked() {
    QMessageBox::StandardButton reply=QMessageBox::question(this,"Save edition",          //you will be asked to save it or not
                          "Do you want to save your edition?",
                          QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
    string str = ui->listWidget->currentItem()->text().toStdString();
    int pos = str.find("|");
    cout<<pos<<endl;
    string path = str.substr(pos+1);

    QFile file(QString::fromStdString(rootPath+path));

    if(!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this,"title","file not open");
    }
    QTextStream out(&file);
    QString text = ui->plainTextEdit->toPlainText();
    out << text;
    file.flush();
    file.close();
    QMessageBox::information(this,"successful save", "save the file successfully");
} else {
        QMessageBox::information(this,"fail save", "did not save the file");
    }
}




void SecDialog::on_Button_delete_clicked() //rmdir
{
    string str = ui->listWidget->currentItem()->text().toStdString();
    int pos2 = str.find_last_of("/");
    string name = str.substr(pos2+1);


    if (name == ts1.name || name == ts2.name || name ==ts3.name )
    {
        QMessageBox::warning(this,"permission error","You do not have permition to delete this file.");
    }

    if (str.substr(0,3)=="Fil")
    {                                                         // delete file from directory
        bool test = (*direct).rm(name);
        if (test == false)
        {
            QMessageBox::warning(this,"invalid action","Invalid action");
            return;
        }

    }

    if (str.substr(0,3)=="Dir")
    {
        bool test = (*direct).Rmdir(name);                               // clear and update items in listWight
        if (test == false)
        {
            QMessageBox::warning(this,"invalid action","Invalid action");
            return;
        }
    }

         ui->listWidget->clear();                             // delete folder from directory

        QString wd = QString::fromStdString(rootPath) + ui->current_direct->text();
        QDir dir(wd);
        int i =0;                                            // update
        foreach (QFileInfo var, dir.entryInfoList()) {
            i+=1;
            if (i>2) {

                /* present each item in listWidget according to
                 * root path and wording directory from root*/
                std::string path = var.absoluteFilePath().toStdString();
                std::string current  = path.substr(rootPath.size());

                if (var.isDir())
                    ui->listWidget->addItem("Dir |"+ QString::fromStdString(current));

                if(var.isFile())
                    ui->listWidget->addItem("File |" + QString::fromStdString(current));
    }
            }
        if (ui->current_direct->text().toStdString()=="/"){
            ui->listWidget->addItem(QString::fromStdString(ts1.name));
            ui->listWidget->addItem(QString::fromStdString(ts2.name));
            ui->listWidget->addItem(QString::fromStdString(ts3.name));

    }
        }



void SecDialog::on_pushButton_move_clicked()                          //remove folder/file from one directory to another directory
{
   // string source = ui->src_input->text().toStdString();
    //string destination = ui->dest_input->text().toStdString();
    //int dest_loc = ui->dest_loc->text().toInt();
    //string cps = ui->copy_input->text().toStdString();
    //bool cp;
    //std::istringstream(cps) >> std::boolalpha >> cp;                    //convert QString to bool
    //(*direct).mv(source, destination, dest_loc, cp);
   }


void SecDialog::on_create_folder_clicked()
{
    {
    string foldername = ui->lineEdit_foldername->text().toStdString();  //to prevent invalid input of folder name
    if (foldername.find("-") != std::string::npos||
            (foldername.find("~") != std::string::npos) ||
            (foldername.find("/") != std::string::npos) ||
            (foldername.find("\\") != std::string::npos) ||
             (foldername.find("|") != std::string::npos) ||
             (foldername.find("<") != std::string::npos) ||
             (foldername.find(">") != std::string::npos) ||
             (foldername.find("*") != std::string::npos) ||
             (foldername.find("?") != std::string::npos) ||
            (foldername.find("\"") != std::string::npos) ||
             (foldername.find(".") != std::string::npos)
            ) {
        QMessageBox::warning(this,"Invalid name","This name is invalid, please give a new name.");
        return;
    }
     bool test = (*direct).Mkdir(foldername);
     if (test == false)
     {
         QMessageBox::warning(this,"Invalid name","Fail to create new folder");
         return;
     }


    string current_path = ui->current_direct->text().toStdString();

  if (current_path == "/"){
      string newfolder = "Dir |" + current_path+foldername;
  }
    string newfolder = "Dir |" + current_path+"/"+foldername;
     ui->listWidget->addItem(QString::fromStdString(newfolder));
    }
}




/* close the program
   and */
void SecDialog::on_pushButton_clicked()
{

    (*direct).serialise(serialise_path);
    this->close();
    delete direct;
}

void SecDialog::on_pushButton_run_clicked()
{


    //读取选择的文件路径
    string str = ui->listWidget->currentItem()->text().toStdString();
    int pos = str.find("|");
    string path = str.substr(pos+1);

    //将文件路径转化为文件名
    int pos2 = str.find_last_of("/"); //标记“/”的位置,
    std::string filename = str.substr(pos2+1); // 然后 取pos2  之后的  substr

    //输出空的memory table
    //sysMemory.showMemory();

    //建立线程池
    struct threadpool *pool = threadpool_init(3, 20);

    if (filename==ts1.name){
        ts1.threadpool = pool;
        ts1.changeStatus(Ready);
        ts1.run();
    }else if (filename == ts2.name) {
        ts2.threadpool = pool;
        ts2.changeStatus(Ready);
        ts2.run();
    }else if (filename== ts3.name) {
        ts3.threadpool = pool;
        ts3.changeStatus(Ready);
        ts3.run();
    }


    //--------------------更新--------------//
    //获取memory使用情况
    QString table = sysMemory.getTable();
    //更新第三个ui
    thirdDialog->updateM(table);

    sysMemory.releaseMemory(&ts1);
    sysMemory.releaseMemory(&ts2);
    //sysMemory.releaseMemory(&ts3);
    sleep(2);

    table = sysMemory.getTable();
    thirdDialog->updateM(table);
    //sysMemory.showMemory();
}



/* present history of visiting directories*/
void SecDialog::on_pushButton_history_clicked()
{
    {
        (*direct).cd("-");    //go back history
        ui->current_direct->setText(QString::fromStdString((*direct).rwd()));

        string path = rootPath + (*direct).rwd();

        ui->listWidget->clear();
        if ((*direct).rwd() == "/"){
            ui->listWidget->addItem(QString::fromStdString(ts1.name));
            ui->listWidget->addItem(QString::fromStdString(ts2.name));
            ui->listWidget->addItem(QString::fromStdString(ts3.name));
        }
        QDir dir(QString::fromStdString(path));
        int i =0;
        foreach (QFileInfo var, dir.entryInfoList()) {
            i+=1;
            if (i>2) {


                /* present each item in listWidget according to
                 * root path and wording directory from root*/

                std::string path = var.absoluteFilePath().toStdString();
                std::string current  = path.substr(rootPath.size());

                if (var.isDir())
                    ui->listWidget->addItem("Dir |"+ QString::fromStdString(current));

                if(var.isFile())
                    ui->listWidget->addItem("File |" + QString::fromStdString(current));
}
}
    }
    }

void SecDialog::on_mount_clicked()
{
    string path = ui->mount_input->text().toStdString();
    bool flag = (*direct).mount(path);
    if (flag == false)
    {
        QMessageBox::warning(this,"Invalid input path",
                             "Invalid input path, please enter again!");
        return;
    }
    ui->mounts->addItem(QString::fromStdString(path));
}

void SecDialog::on_unmount_clicked()
{
    string path = ui->mounts->currentItem()->text().toStdString();
    int mId;
    string dest;
    (*direct).searchM(path,dest,mId);
    (*direct).umount(mId);
    int row = ui->mounts->currentRow();
    ui->mounts->takeItem(row);
}

void SecDialog::on_searchM_clicked()
{

    string path = ui->lineEdit_pathsearch->text().toStdString();
    int loc;
    string dest;
    bool flag = (*direct).searchM(path,dest,loc);
    if (flag == false )
    {
        QMessageBox::warning(this,"Search Error","No matching results.");
        return;
    }
    ui->mount_path->setText(QString::fromStdString(dest));
    string locs = to_string(loc);
    ui->loc->setText(QString::fromStdString(locs));
}

void SecDialog::on_move_clicked()
{
    //还需要判断不是int的情况、空的情况
    int src_loc = ui->lineEdit_src_loc->text().toInt();
    string src = ui->lineEdit_src->text().toStdString();
    int dest_loc = ui->dest_loc->text().toInt();
    string dest = ui->dest_path->text().toStdString();

    if (ui->lineEdit_src_loc->text().toStdString() =="" &&          //deal with the condition that user do not input anything  and push the button
            ui->dest_loc->text().toStdString() == "" &&
            src == "" &&
            dest == "")
    {
        QMessageBox::warning(this,"Empty input","You did not input all parameters that are needed!");
        return;
    }

   if (src_loc == 0)                                               // move from /root to other directories
   {
      bool flag = (*direct).mv(src, dest, dest_loc, false);
      if (flag == false)                                           // fail to mv
      {
          QMessageBox::warning(this,"Invalid input","Invalid input, please input again!");
          return;
      }
      cout<<"Success move from /root to " + dest <<endl;
   }

   else                                                             // move from mount to /root
   {
       bool loc_flag = (*direct).setMount(src_loc);                  // user may input invalid src_loc which does not exist
      cout<<"set mount"+to_string(loc_flag) <<endl;
       if (loc_flag == false)
       {
           QMessageBox::warning(this,"Invalid input","Invalid input of sorce_loc, please input again!");
           return;
       }

      bool result_flag = (*direct).mv(src,dest,dest_loc,false);

      if (result_flag == false)                                       // fail to move
      {
          QMessageBox::warning(this,"Invalid input","Invalid input, please input again!");
          return;
   }
       cout<<"Success move from /mount to " + dest <<endl;
}

    (*direct).cd("/");
   ui->listWidget->clear();
   if ((*direct).rwd() == "/"){
       ui->listWidget->addItem(QString::fromStdString(ts1.name));
       ui->listWidget->addItem(QString::fromStdString(ts2.name));
       ui->listWidget->addItem(QString::fromStdString(ts3.name));
   }

   QDir dir(QString::fromStdString(rootPath)+ ui->current_direct->text());
   int i =0;
   foreach (QFileInfo var, dir.entryInfoList()) {
       i+=1;
       if (i>2) {


           /* present each item in listWidget according to
            * root path and wording directory from root*/

           std::string path = var.absoluteFilePath().toStdString();
           std::string current  = path.substr(rootPath.size());

           if (var.isDir())
               ui->listWidget->addItem("Dir |"+ QString::fromStdString(current));

           if(var.isFile())
               ui->listWidget->addItem("File |" + QString::fromStdString(current));
}
}
}


void SecDialog::on_copy_clicked()
{
    int src_loc = ui->lineEdit_src_loc->text().toInt();
    string src = ui->lineEdit_src->text().toStdString();
    int dest_loc = ui->dest_loc->text().toInt();
    string dest = ui->dest_path->text().toStdString();

    if (ui->lineEdit_src_loc->text().toStdString() =="" &&          //deal with the condition that user do not input anything  and push the button
            ui->dest_loc->text().toStdString() == "" &&
            src == "" &&
            dest == "")
    {
        QMessageBox::warning(this,"Empty input","You did not input all parameters that are needed!");
        return;
    }

   if (src_loc == 0)                                               // move from /root to other directories
   {
      bool flag = (*direct).mv(src, dest, dest_loc, true);
      if (flag == false)                                           // fail to mv
      {
          QMessageBox::warning(this,"Invalid input","Invalid input, please input again!");
          return;
      }
      cout<<"Success move from /root to " + dest <<endl;
   }

   else                                                             // move from mount to /root
   {
       bool loc_flag = (*direct).setMount(src_loc);                  // user may input invalid src_loc which does not exist
       if (loc_flag == false)
       {
           QMessageBox::warning(this,"Invalid input","Invalid input of sorce_loc, please input again!");
           return;
       }

      bool result_flag = (*direct).mv(src,dest,dest_loc,true);

      if (result_flag == false)                                       // fail to move
      {
          QMessageBox::warning(this,"Invalid input","Invalid input, please input again!");
          return;
   }
       cout<<"Success move from /mount to " + dest <<endl;
}


  (*direct).cd("/");
   ui->listWidget->clear();
   if ((*direct).rwd() == "/"){
       ui->listWidget->addItem(QString::fromStdString(ts1.name));
       ui->listWidget->addItem(QString::fromStdString(ts2.name));
       ui->listWidget->addItem(QString::fromStdString(ts3.name));
   }

   QDir dir(QString::fromStdString(rootPath)+ ui->current_direct->text());
   int i =0;
   foreach (QFileInfo var, dir.entryInfoList()) {
       i+=1;
       if (i>2) {


           /* present each item in listWidget according to
            * root path and wording directory from root*/

           std::string path = var.absoluteFilePath().toStdString();
           std::string current  = path.substr(rootPath.size());

           if (var.isDir())
               ui->listWidget->addItem("Dir |"+ QString::fromStdString(current));

           if(var.isFile())
               ui->listWidget->addItem("File |" + QString::fromStdString(current));
}
}
}

//void SecDialog::on_pushButton_rename_clicked()
//{
//    string oldpath = ui->listWidget->currentItem()->text().toStdString();
//    int pos = oldpath.find_last_of("/");
//    string oldname = oldpath.substr(pos);
//    string newname = ui->lineEdit_rename->text().toStdString();
//    if (newname.find("-") != std::string::npos||
//            (newname.find("~") != std::string::npos) ||
//            (newname.find("/") != std::string::npos) ||
//            (newname.find("\\") != std::string::npos) ||
//             (newname.find("|") != std::string::npos) ||
//             (newname.find("<") != std::string::npos) ||
//             (newname.find(">") != std::string::npos) ||
//             (newname.find("*") != std::string::npos) ||
//             (newname.find("?") != std::string::npos) ||
//            (newname.find("\"") != std::string::npos) ||
//             (newname.find(".") != std::string::npos) ||
//            (newname == "")
//            ) {
//        QMessageBox::warning(this,"Invalid name","This name is invalid, please give a new name.");
//        return;
//    }

//    bool test = (*direct).mv(oldname,newname,0);
//    if (test == false)
//    {
//        QMessageBox::warning(this,"Invalid input name","Invalid input of input name, please input again!");
//        return;
//    }

//    QDir dir(QString::fromStdString(rootPath)+ ui->current_direct->text());
//    int i =0;
//    foreach (QFileInfo var, dir.entryInfoList()) {
//        i+=1;
//        if (i>2) {


//            /* present each item in listWidget according to
//             * root path and wording directory from root*/

//            std::string path = var.absoluteFilePath().toStdString();
//            std::string current  = path.substr(rootPath.size());

//            if (var.isDir())
//                ui->listWidget->addItem("Dir |"+ QString::fromStdString(current));

//            if(var.isFile())
//                ui->listWidget->addItem("File |" + QString::fromStdString(current));
// }
//}
//}



void SecDialog::on_ls_button_clicked()
{
    bool detail = true;
    string str = (*direct).ls_r(detail);
    ui->plainTextEdit_details->appendPlainText(QString::fromStdString(str));
}

void SecDialog::on_lsopen_button_clicked()
{
    string str = (*direct).lsOpen_r();
    ui->plainTextEdit_details->appendPlainText(QString::fromStdString(str));
}

void SecDialog::on_lsmounts_button_clicked()
{
    string str = (*direct).lsMount_r();
    ui->plainTextEdit_details->appendPlainText(QString::fromStdString(str));
}

void SecDialog::on_pushButton_defragment_clicked()
{
    string str = "Since this memory control process on disk in "
                  "real OS is protected from user, we do not show "
                  "it out. Please check terminal for more information!";
    ui->plainTextEdit_details->appendPlainText(QString::fromStdString(str));
    (*direct).defragment();
}

void SecDialog::on_pushButton_2_clicked() //close files
{
    bool test = (*direct).close();
    ui->plainTextEdit->clear();
    if (test == false)
    {
        QMessageBox::warning(this,"no file open","No file is open");
        return;
    }
}

void SecDialog::on_pushButton_3_clicked()
{
    string str = ui->listWidget->currentItem()->text().toStdString();

    int pos2 = str.find_last_of("/");
    if (str.substr(0,3)=="Dir")
    {
         QMessageBox::warning(this,"fail to show content of directory",
                              "Cannot show content of directory. More "
                              "information in terminal!");
    }

    if (str.substr(0,3)=="Fil") {
    std::string name = str.substr(pos2+1);
    string content = "";
    bool test2 = (*direct).cat(name,content);
    if (test2 == false)
    {
        QMessageBox::warning(this,"fail show content of this","fail show content of this, more information in terminal!");
        ui->plainTextEdit_details->appendPlainText(QString::fromStdString(content));
        return;
    }
    ui->plainTextEdit->setPlainText(QString::fromStdString(content));
    }
}

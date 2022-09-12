#ifndef SECDIALOG_H
#define SECDIALOG_H

#include <QDialog>
#include <QTimer>
#include "Directory.h"
#include <string>
#include "header.h"
#include "thirdDialog.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"


# ifdef _WIN32
//# include <Windows.h>
# include <fileapi.h>
# endif


using namespace std;


namespace Ui {
class SecDialog;
}

Directory* fileSysInit(std::string rootPath, bool &reconstruct);

class SecDialog : public QDialog
{
    Q_OBJECT

public:
    int memory(int argc, char *argv[]);
    explicit SecDialog(QWidget *parent= nullptr,std::string rootPath=nullptr,
                       std::string serialisePath=nullptr,bool a=false,
                       Directory* direct=nullptr);
    ~SecDialog();


    string name = "happy";
    //Directory dir_1(string name);

    void ini_file();


   //From Process
    void on_Button_write_clickedHelper2();
    //
private slots:

    /*
     * create new file in current directory
     */
    void on_create_file_clicked();


    void myfunction();


    /*open file or directory*/
    void on_pushButton_open_clicked();

    /*write in and save the
     * content in file*/
    void on_Button_write_clicked();

   /*directory back to upper directory*/
    void on_pushButton_4_clicked();

    /*delete folder or file in current directory*/
    void on_Button_delete_clicked();


    /*move the file or directory in current directory
     * or current tree to other places,either inside
     * operating system or outside operating system.
     * If the destination is out side operating system,
     * thisis the situation of download. */
    void on_pushButton_move_clicked();

    /*create a new file in current directory*/
    void on_create_folder_clicked();


/* close the program and record current
 * information of directory */
    void on_pushButton_clicked();

    /* used for run exacutable file*/
    void on_pushButton_run_clicked();


/* present history last directory of visiting*/
    void on_pushButton_history_clicked();

    void on_mount_clicked();

    void on_unmount_clicked();

    void on_searchM_clicked();

    void on_move_clicked();

    void on_copy_clicked();

    //void on_pushButton_rename_clicked();




    void on_ls_button_clicked();

    void on_lsopen_button_clicked();

    void on_lsmounts_button_clicked();

    void on_pushButton_defragment_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:


    Ui::SecDialog *ui;
    QTimer *timer;
    ThirdDialog *thirdDialog;


    //vector to record succesful process
    std::vector<Process>* psVector;
    Memory sysMemory;
    calculator window;
    task1 ts1 = task1(&sysMemory);
    task2 ts2 = task2(&sysMemory);
    task3 ts3 = task3(&sysMemory,&window);


};


void * on_Button_write_clickedHelper1(void* arg);

#endif // SECDIALOG_H

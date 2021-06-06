#include "file_system.h"

bool all_blank(string &str)
{
    for(int i=0;i<str.length();i++)
        if(str[i]!=' ') return false;
    return true;
}

int main()
{
    system("CLS");
    File_system *file_system = new File_system();

    while (true){
        system("pause");
        system("CLS");
        cout << "             My File System Help Message     " << endl;
        cout << "    ———————————————————————————————" << endl;
        cout << "     1.login <name> <pass>  -- 登录  " << endl;
        cout << "     2.register <name> <pass> -- 注册     " << endl;
        cout << "     3.remove <name> <pass> -- 移除用户  " << endl;
        cout << "     4.show           -- 显示用户信息      " << endl;
        cout << "     5.exit           -- 显示用户信息      " << endl;
        cout << "    ———————————————————————————————" << endl;
        string cmd, op, sname, dname;
        getline(cin, cmd, '\n');
        /* 如果只输入空格 */
        if(all_blank(cmd)) continue;
        /* 删除两端空格 */
        cmd = cmd.substr(cmd.find_first_not_of(' '),
                         cmd.find_last_not_of(' ') - cmd.find_first_not_of(' ') + 1);
        if(cmd.length() == 0) continue;

        /* 切分指令 */
        if(cmd.find(" ") == cmd.npos) op = cmd;
        else{
            op = cmd.substr(0, cmd.find(" "));
            dname = cmd.substr(cmd.rfind(" ") + 1);

            /* 判断是否为双输入指令 */
            sname = cmd.substr(cmd.find(" "), cmd.rfind(" ")-cmd.find(" ")+1);
            if(!all_blank(sname)){
                sname = sname.substr(sname.find_first_not_of(' '),
                                     sname.find_last_not_of(' ')-sname.find_first_not_of(' ')+1);
            }
        }


        if(op == "login"){
            if(file_system->log_in(sname, dname)!=USR_EXIST){
                cout << "登录失败！" << endl;
                continue;
            }
        }
        else if(op == "register"){ file_system->Register(sname, dname); continue;}
        else if(op == "remove"){ file_system->Remove(sname, dname); continue;}
        else if(op == "show"){ file_system->show_Usr(); continue;}
        else if(op == "exit"){ break;}
        else continue;
        file_system->show_help();
        file_system->dir_path = " ";
        while (true) {
            bool is_cp = false;
            cout << "lx@ubuntu:" << file_system->dir_path << "$";

            getline(cin, cmd, '\n');

            /* 如果只输入空格 */
            if(all_blank(cmd))
                continue;
            /* 删除两端空格 */
            cmd = cmd.substr(cmd.find_first_not_of(' '),
                             cmd.find_last_not_of(' ') - cmd.find_first_not_of(' ') + 1);
            if(cmd.length() == 0) continue;

            /* 切分指令 */
            if(cmd.find(" ") == cmd.npos) op = cmd;
            else{
                op = cmd.substr(0, cmd.find(" "));
                dname = cmd.substr(cmd.rfind(" ") + 1);

                /* 判断是否为cp指令 */
                sname = cmd.substr(cmd.find(" "), cmd.rfind(" ")-cmd.find(" ")+1);
                if(!all_blank(sname)){
                    is_cp = true;
                    sname = sname.substr(sname.find_first_not_of(' '),
                                         sname.find_last_not_of(' ')-sname.find_first_not_of(' ')+1);
                }
            }

            if(op == "touch")  file_system->touch(dname);
            if(op == "rm") file_system->delete_file(dname);
            if(op == "cp") file_system->copy_file(sname, dname);
            if(op == "cat") file_system->catch_file(dname);
            if(op == "gedit") file_system->gedit_file(dname);
            if(op == "mkdir") file_system->mkdir(dname);
            if(op == "rmdir") file_system->rmdir(dname);
            if(op == "ls") file_system->catch_dir();
            if(op == "cd") file_system->cd_dir(dname);
            if(op == "clear"){ system("cls"); file_system->show_help(); }
            if(op == "help") file_system->show_help();
            if(op == "exit") { file_system->log_out();break; }
            fflush(stdin);
        }
    }
    delete file_system;
    return 0;
}

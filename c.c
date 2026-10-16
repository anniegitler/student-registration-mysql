
#include "C:/Program Files/MySQL/MySQL Server 8.0/include/mysql.h"

#include <stdio.h>
#include <string.h>

#pragma comment(lib,"C:/Program Files/MySQL/MySQL Server 8.0/lib/libmysql.lib")

typedef unsigned __int64 u64;
#include <windows.h>

 

typedef struct STUDENT
{ 

char No[100];
char name[100];
char surname[100];
char year[100];
char major[100];
char gpa[100];
char age[100];


}STD;

STD student;

#define EDITBUTTON 50
#define DELATEBUTTON 30
#define SHOWLITSBUTTON 150
#define COMBOBOXX 1100
#define CLEARBUTTON 400


HWND enterButton;
HWND readButton;
HWND otherButton;
HWND delatebutton;
HWND editbutton;
HWND clearbutton;


HWND editgpa;
HWND editmajor;
HWND editname;
HWND editsurname;
HWND editage;
HWND edityear;
HWND combobox;
HWND staticlist;
HWND editButton;

char fname[100] = { "register.txt" };
int maxsize = 1000;

int num = 1;


MYSQL* conn;
MYSQL_RES* res;
MYSQL_ROW row;

char* server = "localhost";
char* user = "root";
char* password = "";
char* database = "students";

char cmd[1024] = "";




LRESULT CALLBACK wnd_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
        case WM_DESTROY:

            mysql_close(conn);
            PostQuitMessage(0);

		return 0;

        break;

	
   

        case WM_COMMAND:
        {

            switch((short)wParam)

            {


                case COMBOBOXX:
                { 

                    if (HIWORD(wParam) == CBN_SELCHANGE)
                    {
                        char buffer[500];

                        int index = SendMessageA(combobox, CB_GETCURSEL, 0, 0);

                        if (index == CB_ERR)
                            break;


                        SendMessageA(combobox, CB_GETLBTEXT, index, (LPARAM)buffer);

                        sscanf_s(buffer, "%s %s %s %s %s %s %s", student.No, 100, student.name, 100, student.surname, 100, student.year, 100, student.major, 100, student.gpa, 100,  student.age, 100);


                        SendMessageA(editname, WM_SETTEXT, 0, (LPARAM)student.name);
                        SendMessageA(editsurname, WM_SETTEXT, 0, (LPARAM)student.surname);
                        SendMessageA(edityear, WM_SETTEXT, 0, (LPARAM)student.year);
                        SendMessageA(editmajor, WM_SETTEXT, 0, (LPARAM)student.major);
                        SendMessageA(editgpa, WM_SETTEXT, 0, (LPARAM)student.gpa);
                        SendMessageA(editage, WM_SETTEXT, 0, (LPARAM)student.age);

                    }


                    

                    if (HIWORD(wParam) == CBN_EDITCHANGE)
                    {


                       
                        char cedit[100];

                        SendMessageA(combobox, WM_GETTEXT, 100, (LPARAM)cedit);

                        SendMessageA(combobox, CB_RESETCONTENT, 0, 0);

                        SendMessageA(combobox, WM_SETTEXT, 0, (LPARAM)cedit);
                        SendMessageA(combobox, CB_SETEDITSEL, 0, (LPARAM)((strlen(cedit) << 16) | strlen(cedit)));

                        sprintf_s(cmd, sizeof(cmd), "SELECT * FROM `student list` WHERE name LIKE '%s%%';",cedit);


                        if (mysql_query(conn, cmd))
                        {
                            fprintf(stderr, "%s\n", mysql_error(conn));
                            exit(1);
                        }

                        MYSQL_RES* res = mysql_store_result(conn);

                        if (res == NULL)
                        {
                            printf("%s\n", mysql_error(conn));
                            return;
                        }

                        MYSQL_ROW row;

                        while ((row = mysql_fetch_row(res)))
                        {
                            char buffer[500];

                            sprintf_s(buffer, sizeof(buffer),"%s %s %s %s %s %s %s",
                                row[0],  // No
                                row[1],  // name
                                row[2],  // surname
                                row[3],  // year
                                row[4],  // major
                                row[5],  // gpa
                                row[6]); // age

                            SendMessageA(combobox, CB_ADDSTRING, 0, (LPARAM)buffer);
                        }

                        mysql_free_result(res);


                        /*

                        FILE* fp = fopen("register.txt", "rb");

                        if (fp)
                        {
                            while (fread(&student, sizeof(student), 1, fp) == 1)
                            {
                                char buffer[500];

                                if (strlen(cedit) > 0 && strncmp(student.name, cedit, strlen(cedit)) == 0)
                                {
                                    sprintf_s(buffer, 500, "%s %s %s %s %s %s %s", student.No, student.name, student.surname, student.phone, student.id, student.gpa, student.age);

                                    SendMessageA(combobox, CB_ADDSTRING, 0, (LPARAM)buffer);

                                }
                            }
                        }
                        fclose(fp);

                        */







                    }


                }
                break;









                case 100:
                {


                    mysql_query(conn, "SELECT MAX(No) FROM `student list`");

                    MYSQL_RES* res = mysql_store_result(conn);
                    MYSQL_ROW row = mysql_fetch_row(res);

                    if (row[0] != NULL)
                        num = atoi(row[0]) + 1;
                    else
                        num = 1;

                    mysql_free_result(res);


                    sprintf_s(student.No, 100, "%d", num);

                    SendMessageA(editname, WM_GETTEXT, 100, (LPARAM)student.name);
                    SendMessageA(editsurname, WM_GETTEXT, 100, (LPARAM)student.surname);
                    SendMessageA(edityear, WM_GETTEXT, 100, (LPARAM)student.year);
                    SendMessageA(editmajor, WM_GETTEXT, 100, (LPARAM)student.major);
                    SendMessageA(editgpa, WM_GETTEXT, 100, (LPARAM)student.gpa);
                    SendMessageA(editage, WM_GETTEXT, 100, (LPARAM)student.age);

                    sprintf_s(cmd, 1024,
                        "INSERT INTO `student list` (No,name,surname,year,major,gpa,age) VALUES ('%s','%s','%s','%s','%s','%s','%s');",
                        student.No,
                        student.name,
                        student.surname,
                        student.year,
                        student.major,
                        student.gpa,
                        student.age
                    );

                    

                    if (mysql_query(conn, cmd))
                    {
                        fprintf(stderr, "%s\n", mysql_error(conn));
                        exit(1);
                    }

                    


                    SendMessageA(editname, WM_SETTEXT, 0, 0);
                    SendMessageA(editsurname, WM_SETTEXT, 0, 0);
                    SendMessageA(edityear, WM_SETTEXT, 0, 0);
                    SendMessageA(editmajor, WM_SETTEXT, 0, 0);
                    SendMessageA(editgpa, WM_SETTEXT, 0, 0);
                    SendMessageA(editage, WM_SETTEXT, 0, 0);

              

                    FILE* fp = fopen("register.txt", "ab");

                    fwrite(&student, sizeof(student), 1, fp);

                    fclose(fp);



                }
                break;

                case SHOWLITSBUTTON:
                { 

                    sprintf_s(cmd, sizeof(cmd), "SELECT * FROM `student list`;");

                    if (mysql_query(conn, cmd))
                    {
                        fprintf(stderr, "%s\n", mysql_error(conn));
                        break;
                    }


                    MYSQL_RES* res = mysql_store_result(conn);

                    if (res == NULL)
                    {
                        fprintf(stderr, "%s\n", mysql_error(conn));
                        break;
                    }


                    MYSQL_ROW row;

                    SendMessageA(combobox, CB_RESETCONTENT, 0, 0);

                    while ((row = mysql_fetch_row(res)))
                    {
                        char buffer[500];


                        sprintf_s(buffer, sizeof(buffer),
                            "%s %s %s %s %s %s %s",
                            row[0], // No
                            row[1], // name
                            row[2], // surname
                            row[3], // phone
                            row[4], // id
                            row[5], // gpa
                            row[6]  // age
                        );


                        SendMessageA(combobox, CB_ADDSTRING, 0, (LPARAM)buffer);
                    }


                    mysql_free_result(res);

                    
                    /*
                  
                  
                               FILE* fp = fopen("register.txt", "rb");
                    char buffer[500];

                    if (fp)
                    {
                        while (fread(&student, sizeof(student), 1, fp) == 1)
                        {

                            

                            sprintf_s(buffer, 500, "%s %s %s %s %s %s %s", student.No, student.name, student.surname, student.phone, student.id, student.gpa, student.age);

                            SendMessageA(combobox, CB_ADDSTRING, 0, (LPARAM)buffer);


                        }
                    }

                    fclose(fp);

                    
                  
                  
                  */
       
                    

                }
                break;



                case 170:
                {

                    char search[500];

                    SendMessageA(editname, WM_GETTEXT, 500, (LPARAM)search);

                    FILE* fp = fopen("register.txt", "rb");

                    if (fp)
                    {
                        char buffer[500];

                        while (fread(&student, sizeof(student), 1, fp) == 1)
                        {

                            if (strstr(student.name, search))
                            {

                                sprintf_s(buffer, 500, "% s% s% s% s% s% s% s", student.No, student.name, student.surname, student.year, student.major, student.gpa, student.age);//printf_s(buffer, 500, "No. %s |Name: %s | Surname: %s | Phone: %s | ID: %s | GPA: %s | Age: %s", student.No, student.name, student.surname, student.phone, student.id, student.gpa, student.age);

                                SendMessageA(combobox, CB_ADDSTRING, 0, (LPARAM)buffer);

                                
                            }  


                        }

                        fclose(fp);

                    }


                
                                         
                 }
                break;

                case EDITBUTTON:
                {

                  



                    SendMessageA(editname, WM_GETTEXT, 100, (LPARAM)student.name);
                    SendMessageA(editsurname, WM_GETTEXT, 100, (LPARAM)student.surname);
                    SendMessageA(edityear, WM_GETTEXT, 100, (LPARAM)student.year);
                    SendMessageA(editmajor, WM_GETTEXT, 100, (LPARAM)student.major);
                    SendMessageA(editgpa, WM_GETTEXT, 100, (LPARAM)student.gpa);
                    SendMessageA(editage, WM_GETTEXT, 100, (LPARAM)student.age);

                    sprintf_s(cmd, 1024, "UPDATE `student list` SET name='%s', surname='%s', year='%s', major='%s', gpa='%s', age='%s' WHERE No='%s';",
                        student.name,
                        student.surname,
                        student.year,
                        student.major,
                        student.gpa,
                        student.age,
                        student.No
                    );

                    if (mysql_query(conn, cmd))
                    {
                        fprintf(stderr, "%s\n", mysql_error(conn));
                    }

                 

                }
                break;
               






                case DELATEBUTTON:
                {
                   


                    int index = SendMessageA(combobox, CB_GETCURSEL, 0, 0);

                    if (index != CB_ERR)
                    {
                        char buffer[500];

                        SendMessageA(combobox, CB_GETLBTEXT, index, (LPARAM)buffer);



                        sscanf_s(buffer, "%s %s %s %s %s %s %s",
                            student.No, 100,
                            student.name, 100,
                            student.surname, 100,
                            student.year, 100,
                            student.major, 100,
                            student.gpa, 100,
                            student.age, 100);


                        sprintf_s(cmd, 1024,
                            "DELETE FROM `student list` WHERE No='%s';",
                            student.No
                        );


                        if (mysql_query(conn, cmd))
                        {
                            fprintf(stderr, "%s\n", mysql_error(conn));
                            exit(1);
                        }


                    }


                }
                break;



                case CLEARBUTTON:
                {

                    SendMessageA(editname, WM_SETTEXT, 0, 0);
                    SendMessageA(editsurname, WM_SETTEXT, 0, 0);
                    SendMessageA(edityear, WM_SETTEXT, 0, 0);
                    SendMessageA(editmajor, WM_SETTEXT, 0, 0);
                    SendMessageA(editgpa, WM_SETTEXT, 0, 0);
                    SendMessageA(editage, WM_SETTEXT, 0, 0);

                }
                break;
            }

           
        }
         break;
    }
         



	return DefWindowProcA(hwnd, message, wParam, lParam);
}









int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    char fnm[500] = {"son.bmp"};

    HBRUSH hbrush;

    HBITMAP hbmp;

    WNDCLASSEXA wc = { 0 };

    hbmp = LoadImageA(0,fnm,0,1440,810,16);
    hbrush = CreatePatternBrush(hbmp);

    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = wnd_proc;
    wc.lpszClassName = "M16";
    wc.hInstance = hInstance;
   
    wc.hbrBackground = hbrush;

    if (!RegisterClassExA(&wc))
    {
        ULONG er = GetLastError();
        printf("RegisterClassExA failed: %lu\n", er);
        return 1;
    }

    DWORD style = WS_VISIBLE | WS_OVERLAPPEDWINDOW;
    int x = 400, y = 100, w = 1440, h = 810;

    HWND hwnd = CreateWindowExA(0,wc.lpszClassName,"2324",style,x,y,w, h,NULL,NULL,wc.hInstance,NULL);


    conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
   

    if (!hwnd)
    {
        ULONG er = GetLastError();
        printf("CreateWindowExA failed: %lu\n", er);
        return 1;
    }

    enterButton      = CreateWindowExA (0, "BUTTON", "REGISTER", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 50, 630, 80, 40, hwnd, (HMENU)100, wc.hInstance, NULL);
    editButton       = CreateWindowExA (0, "BUTTON", "EDIT", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 260, 630, 80, 40, hwnd, (HMENU)EDITBUTTON, wc.hInstance, NULL);
   // otherButton      = CreateWindowExA (0, "BUTTON", "search", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,300, 330, 80, 40, hwnd, (HMENU)170, wc.hInstance, NULL);
    readButton       = CreateWindowExA (0, "BUTTON", "SHOW LIST", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 455, 380, 100, 40, hwnd, (HMENU)SHOWLITSBUTTON, wc.hInstance, NULL);
    delatebutton     = CreateWindowExA (0, "BUTTON", "REMOVE", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 600, 380, 100, 40, hwnd, (HMENU)DELATEBUTTON, wc.hInstance, NULL);
    //editbutton       = CreateWindowExA (0, "BUTTON", "Enter Name:", WS_VISIBLE | WS_CHILD | SS_SIMPLE, 50, 78, 200, 20, hwnd, (HMENU)300, wc.hInstance, NULL);
    clearbutton      = CreateWindowExA (0, "BUTTON", "CLEAR", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 155, 630, 80, 40, hwnd, (HMENU)CLEARBUTTON, wc.hInstance, NULL);
    editname         = CreateWindowExA (0, "edit", "", WS_VISIBLE | WS_CHILD | ES_LEFT, 59, 135, 264, 25, hwnd, (HMENU)700, wc.hInstance, NULL);
    editsurname      = CreateWindowExA (0, "edit", "", WS_VISIBLE | WS_CHILD | ES_LEFT, 59, 225, 265, 25, hwnd, (HMENU)800, wc.hInstance, NULL);
    edityear         = CreateWindowExA (0, "edit", "", WS_VISIBLE | WS_CHILD | ES_LEFT, 60, 308, 264, 25, hwnd, (HMENU)1000, wc.hInstance, NULL);
    editmajor         = CreateWindowExA (0, "edit", "", WS_VISIBLE | WS_CHILD | ES_LEFT, 61, 394, 264, 24, hwnd, (HMENU)600, wc.hInstance, NULL);
    editgpa          = CreateWindowExA (0, "edit", "", WS_VISIBLE | WS_CHILD | ES_LEFT, 61, 479, 264, 23, hwnd, (HMENU)500, wc.hInstance, NULL);
    editage          = CreateWindowExA (0, "edit", "", WS_VISIBLE | WS_CHILD | ES_LEFT, 61, 566, 264, 23, hwnd, (HMENU)900, wc.hInstance, NULL);
    combobox         = CreateWindowExA (0, "combobox", "", WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_HASSTRINGS ,  450,40, 890, 300, hwnd, (HMENU)COMBOBOXX, wc.hInstance, NULL);
    //staticlist     = CreateWindowExA (0, "static", "", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 50, 400, 200, 400, hwnd, (HMENU)1200, wc.hInstance, NULL);

    MSG msg;

    while (GetMessageA(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return 1;


  
}
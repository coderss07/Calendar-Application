#include <stdio.h>
#include <conio.h>
#include <windows.h>
struct Date
{
    int dd;
    int mm;
    int yy;
} date;
struct Remainder
{
    int dd;
    int mm;
    char note[50];
} R;
COORD xy = {0, 0};
void gotoxy(int x, int y)
{
    xy.X = x;
    xy.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy);
}
void SetColor(int ForgC)
{
    WORD wColor;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}
void ClearColor()
{
    SetColor(15);
}
void ClearConsoleToColors(int ForgC, int BackC)
{
    WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {0, 0};
    DWORD count;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SetConsoleTextAttribute(hStdOut, wColor);
    if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        FillConsoleOutputCharacter(hStdOut, (TCHAR)32, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
        FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
        SetConsoleCursorPosition(hStdOut, coord);
    }
    return;
}
void SetColorAndBackground(int ForgC, int BackC)
{
    WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
    ;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
    return;
}
int check_leapYear(int year)
{
    if (year % 400 == 0 || (year % 100 != 0 && year % 4 == 0))
        return 1;
    return 0;
}
void increase_month(int *mm, int *yy)
{
    ++*mm;
    if (*mm > 12)
    {
        ++*yy;
        *mm = *mm - 12;
    }
}
void decrease_month(int *mm, int *yy)
{
    --*mm;
    if (*mm < 1)
    {
        --*yy;
        if (*yy < 1600)
        {
            printf("No record available");
            return;
        }
        *mm = *mm + 12;
    }
}
int getNumberOfDays(int month, int year)
{
    switch (month)
    {
    case 1:
        return 31;
    case 2:
        if (check_leapYear(year) == 1)
            return 29;
        else
            return 28;
    case 3:
        return 31;
    case 4:
        return 30;
    case 5:
        return 31;
    case 6:
        return 30;
    case 7:
        return 31;
    case 8:
        return 31;
    case 9:
        return 30;
    case 10:
        return 31;
    case 11:
        return 30;
    case 12:
        return 31;
    default:
        return -1;
    }
}
char *getName(int day)
{
    switch (day)
    {
    case 0:
        return "Sunday";
    case 1:
        return "Monday";
    case 2:
        return "Tuesday";
    case 3:
        return "Wednesday";
    case 4:
        return "Thursday";
    case 5:
        return "Friday";
    case 6:
        return "Saturday";
    default:
        return "Invalid month";
    }
}
void print_month(int mm, int yy)
{
    printf("---------------------------\n");
    gotoxy(25, 6);
    switch (mm)
    {
    case 1:
        printf("January");
        break;
    case 2:
        printf("February");
        break;
    case 3:
        printf("March");
        break;
    case 4:
        printf("April");
        break;
    case 5:
        printf("May");
        break;
    case 6:
        printf("June");
        break;
    case 7:
        printf("July");
        break;
    case 8:
        printf("August");
        break;
    case 9:
        printf("September");
        break;
    case 10:
        printf("October");
        break;
    case 11:
        printf("November");
        break;
    case 12:
        printf("December");
        break;
    }
    printf(" , %d", yy);
    gotoxy(20, 7);
    printf("---------------------------");
}
int getDayNumber(int day, int mon, int year)
{
    int res = 0, t1, t2, y = year;
    year = year - 1600;
    while (year >= 100)
    {
        res = res + 5;
        year = year - 100;
    }
    res = (res % 7);
    t1 = ((year - 1) / 4);
    t2 = (year - 1) - t1;
    t1 = (t1 * 2) + t2;
    t1 = (t1 % 7);
    res = res + t1;
    res = res % 7;
    t2 = 0;
    for (t1 = 1; t1 < mon; t1++)
    {
        t2 += getNumberOfDays(t1, y);
    }
    t2 = t2 + day;
    t2 = t2 % 7;
    res = res + t2;
    res = res % 7;
    if (y > 2000)
        res = res + 1;
    res = res % 7;
    return res;
}
char *getDay(int dd, int mm, int yy)
{
    int day;
    if (mm < 1 || mm > 12)
    {
        return ("Invalid month value");
    }
    if (dd < 1 || dd > getNumberOfDays(mm, yy))
    {
        return ("Invalid date");
    }
    if (yy >= 1600)
    {
        day = getDayNumber(dd, mm, yy);
        day = day % 7;
        return (getName(day));
    }
    else
    {
        return ("Please give year more than 1600");
    }
}
int checkNote(int dd, int mm)
{
    FILE *fp;
    fp = fopen("cnote.dat", "rb");
    if (fp == NULL)
    {
        return 0;
    }
    while (fread(&R, sizeof(R), 1, fp) == 1)
    {
        if (R.dd == dd && R.mm == mm)
        {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}
void show_month(int mon, int year, int x, int y)
{
    int nod, day, cnt, d = 1, x1 = x, y1 = y, isNote = 0;
    if (mon < 1 || mon > 12)
    {
        printf("INVALID MONTH\nPress q to go back...");
        return;
    }
    if (year < 1600)
    {
        printf("INVALID YEAR\nPress q to go back...");
        return;
    }
    gotoxy(20, y);
    print_month(mon, year);
    y += 3;
    gotoxy(x, y);
    printf("S   M   T   W   T   F   S   ");
    y++;
    nod = getNumberOfDays(mon, year);
    day = getDayNumber(d, mon, year);
    switch (day)
    {
    case 0:
        x = x;
        cnt = 1;
        break;
    case 1:
        x = x + 4;
        cnt = 2;
        break;
    case 2:
        x = x + 8;
        cnt = 3;
        break;
    case 3:
        x = x + 12;
        cnt = 4;
        break;
    case 4:
        x = x + 16;
        cnt = 5;
        break;
    case 5:
        x = x + 20;
        cnt = 6;
        break;
    case 6:
        x = x + 24;
        cnt = 7;
        break;
    default:
        return;
    }
    gotoxy(x, y);
    if (cnt == 1)
    {
        SetColor(12);
    }
    if (checkNote(d, mon) == 1)
    {
        SetColorAndBackground(15, 12);
    }
    printf("%02d", d);
    SetColorAndBackground(15, 3);
    for (d = 2; d <= nod; d++)
    {
        if (cnt % 7 == 0)
        {
            y++;
            cnt = 0;
            x = x1 - 4;
        }
        x = x + 4;
        cnt++;
        gotoxy(x, y);
        if (cnt == 1)
        {
            SetColor(12);
        }
        else
        {
            ClearColor();
        }
        if (checkNote(d, mon) == 1)
        {
            SetColorAndBackground(15, 12);
        }
        printf("%02d", d);
        SetColorAndBackground(15, 3);
    }
    gotoxy(20, y + 2);
    SetColor(14);
    printf("Press:\n");
    gotoxy(20, y + 3);
    printf("N/n -> next month\n");
    gotoxy(20, y + 4);
    printf("P/p -> Previous month\n");
    gotoxy(20, y + 5);
    printf("S/s -> see note(indicated by red background)\n");
    gotoxy(20, y + 6);
    printf("Q/q -> Quit\n");
    ClearColor();
}
void AddNote()
{
    FILE *fp;
    fp = fopen("cnote.dat", "ab+");
    system("cls");
    gotoxy(5, 7);
    printf("Enter the date(DD/MM): ");
    scanf("%d%d", &R.dd, &R.mm);
    gotoxy(5, 8);
    printf("Enter the Note of Maximum 50 character : ");
    fflush(stdin);
    scanf("%[^\n]", R.note);
    if (fwrite(&R, sizeof(R), 1, fp))
    {
        gotoxy(5, 12);
        puts("Note is saved sucessfully");
        fclose(fp);
    }
    else
    {
        gotoxy(5, 12);
        SetColor(12);
        puts("\aFail to save!!\a");
        ClearColor();
    }
    gotoxy(5, 15);
    printf("Press any key............");
    getch();
    fclose(fp);
    return;
}
void showNote(int mm)
{
    FILE *fp;
    int i = 0, isFound = 0;
    system("cls");
    fp = fopen("cnote.dat", "rb");
    while (fread(&R, sizeof(R), 1, fp) == 1)
    {
        if (R.mm == mm)
        {
            gotoxy(10, 5 + i);
            printf("Note %d Day = %d: %s", i + 1, R.dd, R.note);
            isFound = 1;
            i++;
        }
    }
    fclose(fp);
    if (isFound == 0)
    {
        gotoxy(10, 5);
        printf("This Month contains no note");
        gotoxy(10, 8 + i);
        printf("Press 'b' key to go back.......");
    }
    else
    {
        gotoxy(10, 6 + i);
        SetColor(14);
        printf("Press:\n");
        gotoxy(10, 7 + i);
        printf("D/d to delete note\n");
        gotoxy(10, 8 + i);
        printf("B/b to go back\n");
        ClearColor();
    }
}
void delnote(int mm)
{
    FILE *fp1, *fp2;
    int n, i = 0, isFound = 0, l, del = 0;
    system("cls");
    fp1 = fopen("cnote.dat", "r");
    while (fread(&R, sizeof(R), 1, fp1) == 1)
    {
        if (R.mm == mm)
        {
            gotoxy(10, 5 + i);
            printf("Note %d Day = %d: %s", i + 1, R.dd, R.note);
            isFound = 1;
            i++;
        }
    }
    if (isFound == 0)
    {
        gotoxy(10, 5);
        printf("This Month contains no note");
        gotoxy(10, 8 + i);
        printf("Press 'b' key to go back.......");
        return;
    }
    rewind(fp1);
    fp2 = fopen("replica.dat", "wb+");
    printf("\nEnter note number to be deleted:");
    scanf("%d", &l);
    n = 1;
    while (fread(&R, sizeof(R), 1, fp1) == 1)
    {
        if (R.mm == mm)
        {
            if (n == l)
            {
                system("cls");
                SetColor(14);
                gotoxy(10, 10);
                printf("Note is deleted Successfully...\n");
                ClearColor();
                del = 1;
                gotoxy(10, 11);
                printf("Press any key to continue...\n");
                getch();
            }
            else
            {
                fwrite(&R, sizeof(R), 1, fp2);
            }
            n++;
        }
    }
    fclose(fp1);
    fclose(fp2);
    remove("cnote.dat");
    rename("replica.dat", "cnote.dat");
    if (del == 0)
    {
        system("cls");
        SetColor(12);
        gotoxy(10, 10);
        printf("%d has no note...!\n", l);
        ClearColor();
        gotoxy(10, 11);
        printf("Press any key to continue.....\n");
        getch();
    }
}
int main()
{
    ClearConsoleToColors(15, 3);
    int choice;
    char ch = 'a', c = 'a';
    printf("************************* Mini Project *************************\n\n");
    printf("\t\tCALENDER APPLICATION USING C LANGUAGE\n\n\t\t\t      By\n\n");
    printf("\t\t    Sarthak Sharma, 1961152\n");
    printf("\t\t  PROJECT MENTOR: Dr.Naveen Tewari\n\n");
    printf("\t\t  DEBUGGING CREDIT : Sarthak Sharma\n\n");
    printf("****************************************************************");
    printf("\n\t\tPress any key to continue....");
    getch();
    system("cls");
    while (1)
    {
        system("cls");
        printf("************************* CALENDER APPLICATION **************************\n\n");
        printf("\t\t\t1. Find Out the Day\n");
        printf("\t\t\t2. Print all the day of month\n");
        printf("\t\t\t3. Add Note\n");
        printf("\t\t\t4. EXIT\n");
        printf("\n*********************************************************************\n");
        printf("\t\t\tENTER YOUR CHOICE : ");
        scanf("%d", &choice);
        system("cls");
        switch (choice)
        {
        case 1:
            printf("Enter date (DD MM YYYY) : ");
            scanf("%d %d %d", &date.dd, &date.mm, &date.yy);
            printf("Day is : %s", getDay(date.dd, date.mm, date.yy));
            printf("\nPress any key to continue......");
            getch();
            break;
        case 2:
            printf("Enter month and year (MM YYYY) : ");
            scanf("%d %d", &date.mm, &date.yy);
            system("cls");
            do
            {
                show_month(date.mm, date.yy, 20, 5);
                ch = getch();
                if (ch == 'n' || ch == 'N')
                {
                    increase_month(&date.mm, &date.yy);
                    system("cls");
                    show_month(date.mm, date.yy, 20, 5);
                }
                else if (ch == 'p' || ch == 'P')
                {
                    decrease_month(&date.mm, &date.yy);
                    system("cls");
                    show_month(date.mm, date.yy, 20, 5);
                }
                else if (ch == 's' || ch == 'S')
                {
                    do
                    {
                        showNote(date.mm);
                        c = getch();
                        if (c == 'd' || c == 'D')
                        {
                            delnote(date.mm);
                            system("cls");
                        }
                    } while (c != 'b' && c != 'B');
                    system("cls");
                }
            } while (ch != 'q' && ch != 'Q');
            break;
        case 3:
            AddNote();
            break;
        case 4:
            exit(0);
        }
    }
    return 0;
}

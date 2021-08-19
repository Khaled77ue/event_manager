
#include "date.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assert.h"
#define INVALID_MONTH 0
#define MIN_DAY 1
#define MAX_DAY 30
#define MONTHS_NUM 12
#define DAYS_IN_YEAR 365
#define MONTH_STR_LEN 4
#define NEGATIVE -1
#define POSITIVE 1

struct Date_t{
    int day;
    char month[MONTH_STR_LEN];
    int year;
};

const char* const months[]={"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG",
                            "SEP","OCT","NOV","DEC"};
int monthToInt(char* month)
{
    assert(month[MONTH_STR_LEN-1] == '\0');
    for(int i=0;i<MONTHS_NUM;i++)
    {
        if(strcmp(month,months[i]) == 0)
        {
            return i+1;
        }
    }
    return INVALID_MONTH;
}

/**
* isDayValid: checks if the day is valid or not
*
* @param day - the day that we check the validity of
*
* @return
* 	true if it is valid
* 	otherwise false
*/
static bool isDayValid(int day)
{
    return day>= MIN_DAY && day<= MAX_DAY;
}

/**
* isMonthNumberValid: checks if the month is valid
*
* @param month - the month that we check the validity of
*
* @return
* 	true if it is valid
* 	otherwise false
*/
static bool isMonthNumberValid(int month)
{
    return month>=1 && month <=MONTHS_NUM;
}

/**
* dateToDays: return the date in days
*
* @param date - the date that we want to know how many days it contains
*
* @return
* 	number of days
*/
static int dateToDays(Date date)
{
    int month = monthToInt(date->month);
    return date->day + month*(MAX_DAY - MIN_DAY + 1)
           + DAYS_IN_YEAR * date->year;
}

Date dateCreate(int day, int month, int year)
{
    if(!isDayValid(day) || !isMonthNumberValid(month))
    {
        return NULL;
    }
    Date date = malloc(sizeof(*date));
    if(date == NULL)
    {
        return NULL;
    }
    date->day=day;
    strcpy(date->month,months[month-1]);
    date->year = year;
    return date;
}
void dateDestroy(Date date)
{
    free(date);
}
Date dateCopy(Date date)
{
    if(date == NULL)
    {
        return NULL;
    }
    return dateCreate(date->day,monthToInt(date->month),date->year);
}
bool dateGet(Date date, int* day, int* month, int* year)
{
    if(date == NULL || day == NULL || month == NULL || year == NULL)
    {
        return NULL;
    }
    *day=date->day;
    *month=monthToInt(date->month);
    *year=date->year;
    return true;
}
int dateCompare(Date date1, Date date2)
{

    if(date1 == NULL || date2 == NULL)
    {
        return 0;
    }
    int days1 = dateToDays(date1);
    int days2 = dateToDays(date2);
    if(days1 < days2)
    {
        return NEGATIVE;
    }
    if(days1 == days2)
    {
        return 0;
    }
    if(days1 > days2)
    {
        return POSITIVE;
    }
    return POSITIVE;
}

void dateTick(Date date)
{
    if(date == NULL)
    {
        return;
    }
    date->day+=1;
    if(date->day > MAX_DAY)
    {
        date->day = 1;
        int newmonth = monthToInt(date->month) + 1;
        if(newmonth > MONTHS_NUM)
        {
            newmonth = 1;
            date->year+=1;
            strcpy(date->month,months[newmonth - 1]);
        }
        else
        {
            strcpy(date->month,months[newmonth - 1]);
        }
    }
}

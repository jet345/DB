
#include <windows.h>
#include <iostream>
#include <mysql.h>

#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "rudal45"
#define DB_NAME "DB"
#define CHOP(x) x[strlen(x) - 1] = ' '

using namespace std;


MYSQL       *connection = NULL, conn;
MYSQL_RES   *sql_result;
MYSQL_ROW   sql_row;
MYSQL_FIELD *sql_field;
int query_stat;

char name[12];
char tableName[12];
char columnName[12];
char columns[100];
char address[80];
char tel[12];
char query[255];
char values[100];
int length[255];

int show_tables()
{

    query_stat = mysql_query(connection, "show tables");
   if (query_stat != 0)
   {
      fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
      return 1;
   }

    sql_result = mysql_store_result(connection);

    printf("*****SHOW TABLES*****\n");
   while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
   {
      printf("%s \n", sql_row[0]);
   }

   mysql_free_result(sql_result);

}

int select_table()
{
    /*******************************SELECT TABLES******************************************/
    printf("Select Table \n");

    printf("table name : ");
    scanf("%s", &tableName);

    printf("find column : ");
    scanf("%s", &columnName);

    printf("SELECT %s from TABLE %s\n", columnName, tableName);
    sprintf(query, "select %s from %s", columnName, tableName);

    query_stat = mysql_query(connection, query);
    if (query_stat != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
        return 1;
    }

    sql_result = mysql_store_result(connection);

    int i = 0;
    while ((sql_field = mysql_fetch_field(sql_result)) != NULL)
    {

        printf("| ");
        printf("%s\t", sql_field->name);
    }
    printf("|\n");


    //printf("%d", sql_field->max_length);
    int k = 1;
    while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
    {

        int i;
        for (i = 0; i < mysql_num_fields(sql_result); i++)
        {
            printf("| ");
            printf("%s\t", sql_row[i]);
        }
        k++;
        printf("|\n");
    }



    /**************************************************************************************/

}

int insert_values()
{
    /*********************************INSERT VALUES*****************************************/
    printf("Target table name : ");
    //fgets(tableName, 13, stdin);
    scanf("%s",&tableName);
    //CHOP(tableName);

    sprintf(query, "explain %s",tableName);
    query_stat = mysql_query(connection, query);

    if (query_stat != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
        return 1;
    }

    sql_result = mysql_store_result(connection);

    printf("-----------------Structure Of Target Table---------------------\n");
    printf("Field\tType\t\tNull\tKey\tDefault\tExtra\n");
    while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
    {
        printf("%s\t%s\t%s\t%s\t%s\t%s\t\n", sql_row[0],sql_row[1],sql_row[2],sql_row[3],sql_row[4],sql_row[5]);
    }
    mysql_free_result(sql_result);
    printf("\n");
    printf("insert into %s values('values')\n", tableName);
    printf("values for input : ");
    //fgets(values, 100, stdin);
    scanf("%s",&values);
    //CHOP(values);

    sprintf(query, "insert into %s values('%s')",tableName,values);

    query_stat = mysql_query(connection, query);
    if (query_stat != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
        return 1;
    }
    else
    {
        printf("Success\n");
    }

    /***************************************************************************************/

}

int create_table()
{
    /*******************************CREATE TABLES**************************************/
    printf("Insert Table \n");
    printf("CREATE TABLE 'table name'('columns')\n");

    printf("table name : ");
    //fgets(tableName, 12, stdin);

    scanf("%s",&tableName);
//printf("tablename  = %s\n",tableName);
    // CHOP(tableName);
    //printf("tablename  = %s\n",tableName);
    sprintf(query, "show tables like '%s'",tableName);

    query_stat = mysql_query(connection, query);

    if (query_stat != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
        return 1;
    }

    sql_result = mysql_store_result(connection);

    if(mysql_fetch_row(sql_result) != NULL)
    {
        printf("Table is already existed!!\n");
    }
    else
    {
        printf("columns : ");
        scanf("%s",&columns);
        printf("column name = %s\n",columns);
        sprintf(query, "create table %s(%s varchar(20))",tableName,columns);
        printf("CREATE TABLE %s(%s varchar(20))\n",tableName,columns);
        query_stat = mysql_query(connection, query);
        sql_result=mysql_store_result(connection);
    }
    mysql_free_result(sql_result);
    /***************************************************************************************/
}

int main()
{
    int flag=0;
    int user_select_number=0;
    /*********************************MYSQL CONNECTION************************************/
    mysql_init(&conn);

    connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char *)NULL, 0);

    if(connection == NULL)
    {
        fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
        return 1;
    }
    /*************************************************************************************/


    printf("*********************\n");
    printf("1. Show Tables\n");
    printf("2. Create Table\n");
    printf("3. Insert Values\n");
    printf("4. Select Table\n");
    printf("5. EXIT\n");
    printf("*********************\n");

    scanf("%d",&user_select_number);

    while(flag==0)
    {

        switch(user_select_number)
        {
        case 1 :
            show_tables();
            printf("*********************\n");
            break;
        case 2:
            create_table();
            printf("*********************\n");
            break;
        case 3:
            insert_values();
            printf("*********************\n");
            break;
        case 4:
            select_table();
            printf("*********************\n");
            break;

        case 5:
            printf("Mysql exit\n");
            printf("*********************\n");
            mysql_close(connection);
            flag=1;
            break;

        } // End of Switch
        if(flag==0)
        {
            scanf("%d",&user_select_number);
        }



    } //End of While

}

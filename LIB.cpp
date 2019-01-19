#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <winsock.h>
#include <mysql.h>
#include <stdlib.h>
#include <ctime>
//#include <boost/lexical_cast.hpp>
#include <sstream>

using namespace std;



class user
{
public:
	char uname[20];
	char fetched_uname[50];
	int roll;
	int sem;
	char stream[10];

	string int_to_str(int num)
	{
   		stringstream ss;
		ss << num;
		return ss.str();
	}

	int str_to_int(string str)
	{
    	stringstream ss(str);
		int num;
    	ss >> num;
		return num;
	}

	void user_input()
	{
		cout<<"PLEASE ENTER YOUR ROLL_NO\n";
		cin>>roll;
	}
};




//for the books
class books
{
public:
	char bname[20];
	char fetched_bname[50];
	int id;
	char rack_no[10];
	void book_input()
	{
		cout<<"PLEASE ENTER BOOK_NO\n";
		cin>>id;
	}
};




class date_time
{
public:
	string dat , tim;
	void get_date_time()
	{
		time_t rawtime;
 	 	struct tm * timeinfo;
   		char buffer[80];

  		time (&rawtime);
  		timeinfo = localtime(&rawtime);

  		strftime(buffer,sizeof(buffer),"%d-%m-%Y",timeinfo);
  		std::string dat1(buffer);

  		strftime(buffer,sizeof(buffer),"%I:%M:%S",timeinfo);
  		std::string tim1(buffer);

  		dat = dat1;
  		tim = tim1;
	}
};




//for the database connectivity
class sqldata: public user, public books
{
protected:
	MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;
    int qstate;

public:
	sqldata() //doing the connectivity using constructor
	{
		conn = mysql_init(0);
    	if(conn)
        	cout<<"connection OK"<<endl;
    	else
        	cout<<"connection PROBLEM!"<<mysql_error(conn)<<endl;

        conn = mysql_real_connect(conn, "localhost", "root", "", "library", 0,NULL,0);
	}

	//void while_search(string b_id)//fetching book info from database
	//{
	//	string query = "select * from "
	//}

};




class find_book: public sqldata//fetching book info from database and user side data fetching
{
	public:
	char b_name[50];
	char rack[10];
	char units[10];
	char slots[50];
	int unit_s;

	void finding()
	{
	    book_input();
	    string a = int_to_str(id);
		string query = "select * from book where id = '"+a+"'";
		const char* q = query.c_str();
		qstate = mysql_query(conn,q);
		res = mysql_store_result(conn);

		if(!qstate)
        {
			row= mysql_fetch_row(res);
			strcpy(b_name,row[1]);
			strcpy(rack,row[3]);
			strcpy(units,row[4]);
		}
		else
		{
           	cout<<"query execution PROBLEM!"<<mysql_error(conn)<<endl;
		}
		strcpy(fetched_bname, b_name);
		stringstream ss(units);
		//units >> unit_s;
	}

	void give_slot()
	{
		user_input();
		string query = "select * from times where status = 'V'";
		const char* q = query.c_str();
		qstate = mysql_query(conn,q);
		res = mysql_store_result(conn);

		if(!qstate)
        {
			row= mysql_fetch_row(res);
			strcpy(slots,row[0]);
			//strcpy(rack,row[3]);
			//strcpy(units,row[4]);
		}
		else
		{
           	cout<<"query execution PROBLEM!"<<mysql_error(conn)<<endl;
		}
	}

	void confirm_slot(int j)
	{
	    if (j==1)
        {
            string s2 = int_to_str(roll);
            string query = "update times set status = 'F' , alloted_to = '"+s2+"' where status = 'V'";
            const char* q = query.c_str();
            qstate = mysql_query(conn,q);
            res = mysql_store_result(conn);
        }
	}

};



class find_user: public sqldata//finding user data from DB
{
public:
	char u_name[50];
	char semester[10];
	char streams[10];

	void finds(string a)
	{
		string query = "select * from book where id = '"+a+"'";
		const char* q = query.c_str();
		qstate = mysql_query(conn,q);
		res = mysql_store_result(conn);

		if(!qstate)
        {
			row= mysql_fetch_row(res);
			strcpy(u_name,row[0]);
			strcpy(semester,row[1]);
			strcpy(streams,row[2]);
		}
		else
		{
           	cout<<"query execution PROBLEM!"<<mysql_error(conn)<<endl;
		}
        strcpy(fetched_uname, u_name);
	}
};




class return_book: public sqldata
{
	public:
	char date_i[50];
	void issue_date(string r, string bid)
	{
		string query= "select * from issued_data where  id='"+bid+"' and roll_no='"+r+"'";
		const char* q = query.c_str();
		qstate = mysql_query(conn,q);
		res = mysql_store_result(conn);
		if(!qstate)
       	{
			row= mysql_fetch_row(res);
			//strcpy(date,row[1]);
			strcpy(date_i,row[2]);
		}
		else
		{
          	cout<<"query execution PROBLEM!"<<mysql_error(conn)<<endl;
		}
	}
	int returning()
	{
		user_input();
		book_input();

		string s1 = int_to_str(id);
		string s2 = int_to_str(roll);

		//issue_date(s1,s2);

		string query="delete from issued_data where  id='"+s1+"' and roll_no='"+s2+"'";
		const char* q = query.c_str();
		qstate = mysql_query(conn,q);
		return qstate;

	}
};




class issued_book: public sqldata, public date_time
{
public:
    string s1,s2;
	int adding()
	{
		get_date_time();
		user_input();
		book_input();

		s1 = int_to_str(id);
		s2 = int_to_str(roll);

			string query="insert into issued_data values('"+s1+"','"+s2+"','"+dat+"')";
		const char* q = query.c_str();
		qstate = mysql_query(conn,q);
		return qstate;
	}
};

	/*int insert_data(string a, string b)
		{
			string query="insert into storage (username,password) values('"+a+"','"+b+"')";
			const char* q = query.c_str();
			qstate = mysql_query(conn,q);
			return qstate;

		}
		void fetch_data(string a)
		{
			string query="select * from storage where  username='"+a+"'";
			const char* q = query.c_str();
			qstate = mysql_query(conn,q);
			res = mysql_store_result(conn);

			if(!qstate)
        	{
				row= mysql_fetch_row(res);
				strcpy(u,row[1]);
				strcpy(p,row[2]);
			}
			else
			{
            	cout<<"query execution PROBLEM!"<<mysql_error(conn)<<endl;
			}
		}
		void del_data(string a)
		{
			string query="delete from storage where  username='"+a+"'";
			const char* q = query.c_str();
		}*/







int main()
{
	int a;
	cout<<"\t\t\t\tWELCOME\n\n\n\n";
	cout<<"ENTER 1 for LIBRARY SIDE and 2 FOR USER SIDE:";
	cin>>a;
	//find_book f1;
	//find_user f2;

	//LIBRARY INTERFACE
	if(a==1)
	{
		system("cls");
		cout<<"\t\t Welcome to the fast library services for managing books\n\n";
		int ch;
		cout<<"Enter 1 for issuing a book OR 2 for returning a book:";
		cin>>ch;
		if(ch==1)//issuing a book
		{
			issued_book a1;
			int ret = a1.adding();
			if(!ret)
			{
				//f1.finding(a1.s1);
				//f2.finds(a1.s2);
				system("cls");
				cout<<"\t\t\tTHANK YOU\n";
				//cout<<"The BOOK "<<f1.b_name<<" HAS BEEN ISSUED TO "<<f2.u_name<<" ON "<<a1.dat<<endl;
				cout<<"The BOOK HAS BEEN ISSUED \n\n"<<endl;
				cout<<"\t\tPLEASE return on time\n\n";
			}
		}
		else if(ch==2)//returning a book
		{
			return_book r1;
			int ret = r1.returning();
			if(!ret)
			{
				system("cls");
				cout<<"THE BOOK HAS BEEN RETURNED\n\n";
			}
			else
			{
				system("cls");
				cout<<"WRONG INPUT.........MAYBE BOOK DOESNT belong to UU";
			}
		}
		else
		{
			system("cls");
			cout<<"SORRY";
		}
	}
	//USER WALI SIDE
	else if(a==2)
	{
		system("cls");
		cout<<"\t\t Welcome to the fast library services for checking books and getting a slot\n\n";
		int ch;
		cout<<"Enter 1 for status of a book OR 2 for booking a slot:";
		cin>>ch;
		if(ch == 1 )
		{

		}
		else if (ch == 2)
		{
            find_book bb;
            bb.give_slot();
            cout<<"Avaliable slot is "<<(bb.slots)<<endl;
            cout<<"Enter 1 to confirm\n";
            int w;
            cin>>w;
            bb.confirm_slot(w);
            system("cls");
            cout<<"SLOT HAS BEEN BOOKED FOR "<<bb.roll<<" AT "<<bb.slots;
		}
		else
		{
			system("cls");
			cout<<"SORRY FOLKS!!!........YOU are OVERSMART to use our system\n\n";
		}
	}

	else
	{
		cout<<"\t\tSORRY YOU CANT ACCESS OUR LIBRARY\n\n";
	}

	return 0;
}


/*#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <winsock.h>
#include <mysql.h>
#include <stdlib.h>
using namespace std;

class user_pass
{
    //protected:
public:
    char username[50];
    char password[50];
    //public:
    void enter_user()
    {
        cout<<"\nPlease Enter your USERNAME : ";
        cin>>username;
    }
    void enter_pass()
    {
        cout<<"\nPlease Enter your PASSWORD : ";
        cin>>password;
    }
};

class encryption: public user_pass
{
//private:
    public:
    //char d[30];
    void encrypt(char* a)
    {
        //strcpy(d,a);
        //d=a;
        int l= strlen(a);
        for(int i=0;i<l;i++)
        {
            //if(d[i]=='z')
                //d[i] = d[i]-5;
            //else if(d[i]=='y')
             //   d[i] = d[i]-2;
            //else if(i%2 == 0)
             //   d[i]= d[i]+ 2;
            //else
                a[i]= a[i]+3;
        }
       // return d;
    }
};

class decryption: public user_pass
{
    //private:

    public:
    //char* d;
    void decrypt(char* a)
    {
        //strcpy(d,a);
        //d=a;
        int l= strlen(a);
        for(int i=0;i<l;i++)
        {
            //if(d[i]=='z')
              //  d[i] = d[i]+5;
            //else if(d[i]=='y')
             //   d[i] = d[i]+2;
           // else if(i%2 == 0)
            //    d[i]= d[i]-2;
            //else
                a[i]= a[i]-3;
        }
        //return d;
    }
};

class verify: public user_pass
{
private:
    int l,flag=0,f=0;
public:
    int verify_mail()
    {
        enter_user();
        int l= strlen(username);
        int flag=0,f=0;
        for(int i=0;i<l;i++)
        {
            if(username[i]=='@')
            {
                flag=1;
                break;
            }
        }
        if(flag==1)
            f=1;
        else
            f=2;

        return f;
    }
};

class sql_wali: public user_pass
{
	protected:
	MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;
    int qstate;

public:
	char u[50];
	char p[50];
    sql_wali()
    {
    	conn = mysql_init(0);
    	if(conn)
        	cout<<"connection OK"<<endl;
    	else
        	cout<<"connection PROBLEM!"<<mysql_error(conn)<<endl;

        conn = mysql_real_connect(conn, "localhost", "root", "", "user_pass", 0,NULL,0);
    }

		//string inserting, deleting, selecting;
		//inserting = "insert into storage ";
		//deleting = "delete from storage ";
		//selecting = "select * from storage ";

		int insert_data(string a, string b)
		{
			string query="insert into storage (username,password) values('"+a+"','"+b+"')";
			const char* q = query.c_str();
			qstate = mysql_query(conn,q);
			return qstate;

		}
		void fetch_data(string a)
		{
			string query="select * from storage where  username='"+a+"'";
			const char* q = query.c_str();
			qstate = mysql_query(conn,q);
			res = mysql_store_result(conn);

			if(!qstate)
        	{
				row= mysql_fetch_row(res);
				strcpy(u,row[1]);
				strcpy(p,row[2]);
			}
			else
			{
            	cout<<"query execution PROBLEM!"<<mysql_error(conn)<<endl;
			}
		}
		void del_data(string a)
		{
			string query="delete from storage where  username='"+a+"'";
			const char* q = query.c_str();
		}
};


int main()
{

		int n;
		sql_wali sab;


	cout<<"                     Welcome to OOPs PROJECT\n";
	cout<<"Enter::   1 for SignUP   ||  2 for LogIN  ||  3 for DELETE ACCOUNT\n";
	cin>>n;

	//SIGNUP WALA PAGE
	if (n==1)
	{
		system("cls");
		cout<<"PLEASE ENTER YOUR REQUIRED DETAILS TO SIGNUP\n";
		verify p1;
		int r = 0;
		while(r!=1)
		{
			r= p1.verify_mail();
			if(r==2)
			{
				cout<<"Please enter a valid Email ID\n";
			}
		}
		p1.enter_pass();
		//encryption val;
		//val.encrypt(p1.password);
        //val.encrypt(p1.username);
        string mn, nm;
        mn = p1.username;
        nm = p1.password;
		int o = sab.insert_data(mn , nm);
		if(!o){
			system("cls");
			cout<<"ACCOUNT CREATED successfully\n     NOW YOU CAN LOGIN";

		}
	}


	//LOGIN WALA PAGE
	else if(n==2)
	{
		system("cls");
		verify p1;
		int w = 0;
		while(w!=1)
		{
			w= p1.verify_mail();
			if(w==2)
			{
				cout<<"Please enter a valid Email ID\n";
			}
		}
		p1.enter_pass();
		//encryption val;
		//val.encrypt(p1.password);
		string opq;
		opq = p1.username;
		//sab.insert_data(p1.username, val.d);
		sab.fetch_data(opq);
		if(strcmp(p1.password,sab.p) == 0)
		{
			system("cls");
			cout<<"LOGGED IN successfully\n";
		}
		else
		{
			system("cls");
			cout<<"PASSWORD DOESNT MATCH\n";
		}
	}

	else if(n==3)
	{
        system("cls");
        verify p3;
	}
    return 0;
}


//int main()
//{
    //MYSQL* conn;
    //MYSQL_ROW row;
    //MYSQL_RES* res;
    //int qstate;

    //using array to store data


    //conn = mysql_init(0);
    //if(conn)
    //    cout<<"connection OK"<    <endl;
    //else
    //    cout<<"connection PROBLEM!"<<mysql_error(conn)<<endl;

    //conn = mysql_real_connect(conn, "localhost", "root", "", "user_pass", 0,NULL,0);

    //if(conn)

        //cout<<"connection DONE successful  "<<conn<<endl;
       // string uname, password,n;
        //string a[10], b[10],c[10];
        //int k=0;

        //for input of data to DB
        // cout<<"Enter S.No. , Username, Password\n";
        // cin>>n>>uname>>password;
        // string query="insert into user_data(S_No,UserName,PassWord) values('"+n+"','"+uname+"','"+password+"')";

        // const char* q = query.c_str();

        //cout<<"query is: "<<q<<endl;
        //qstate = mysql_query(conn,q);


        /*if(!qstate)
            cout<<"record inserted successfully..."<<endl<<endl<<endl;
        else
            cout<<"query problem: "<<mysql_error(conn)<<endl<<endl;*/





        //next code is for display
        // qstate = mysql_query(conn,"select * from user_data");
        // if(!qstate)
        // {
            // res = mysql_store_result(conn);
            // while(row= mysql_fetch_row(res))
            // {
                // cout<<"\nS.No.: "<<row[0]<<" Email: "<<row[1]<<" Password: "<<row[2];//getting database elements
            // }
            // for(int i=0;i<k;i++)
            // {
                // cout<<"\nS.No.: "<<a[k]<<" Email: "<<b[k]<<" Password: "<<c[k];//getting database elements
            // }
        // }
        // else
        // {
            // cout<<"query execution PROBLEM!"<<mysql_error(conn)<<endl;
        // }
    // }
    // else
    // {
        // cout<<"connection PROBLEM!"<<mysql_error(conn)<<endl;
    // }
//        return 0;
//}





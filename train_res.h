#include<iostream>
#include"train_reservation.h"
#include<cstring>
#include<sqlca.h>
#include<sqlda.h>
#include<sqlcpr.h>
using namespace std;
exec sql begin declare section;
                        int logid,input_op;
                        char passenger_name[15];
                        char pass[10];
                        char input_mail[20],check_mail[20];
                        int passenger=1;
                        int option=0;
                        int p_id,l,age;
                        long p_no,input_no,check_phone;
                        int tra_no,dur,seat,f_a,f_b,f_c,input_train,n,n1,input_class,total,up_seats;
                        int acc_total,input_acc;
                        float senior_cost,child_cost,cost,original_cost;
                        int total_cost=0;
                        char tr_name[10],sour[4],destination[4],depa[10],input_option,input_option1,input_pass[15],check_pass[15],input_accpass[15];
                        char name[15],mail[15],pa[15],add[20],gen[6];
                        char user[15]="tcs102@oracle";
                        char p[10]="tcs";
exec sql end declare section;
void reservation::admin()
{
        EXEC SQL connect:user identified by:p;
}
int reservation::register_user()
{
        cout<<"Passenger ID: ";
        cin>>p_id;
        cout<<"LOGIN_ID: ";
        cin>>l;
        cout<<"NAME: ";
        cin>>name;
        cout<<"AGE: ";
        cin>>age;
        cout<<"GENDER: ";
        cin>>gen;
        cout<<"EMAIL: ";
        cin>>mail;
        cout<<"Phone Number: ";
        cin>>p_no;
        cout<<"Address: ";
        cin>>add;
        cout<<"Password: ";
        cin>>pa;
        EXEC SQL connect:user identified by:p;
        exec sql insert into login values(:p_id,:l,:name,:age,:gen,:mail,:p_no,:add,:pa);
        cout<<"Registered sucessfully Login to Continue."<<endl;
cout<<"LOG IN:";
        cin>>logid;
        cout<<"Password:";
        cin>>pass;
        EXEC SQL select passenger_id,name into :passenger,:passenger_name from login where login_id=:logid and password=:pass;
        return passenger;
}
int reservation::login_user()
{
cout<<"LOG IN:";
cin>>logid;
cout<<"Password:";
cin>>pass;
cout<<logid<<endl;
cout<<pass<<endl;
EXEC SQL select passenger_id,name into :passenger,:passenger_name from login where login_id=:logid and password=:pass;
return passenger;
}
void reservation::booking_ticket()
{       int i;
       exec sql declare train_cursor cursor for select train_no,name,source,dest,departure,duration,seats,FEE_A,FEE_B,FEE_C from train;
       cout<<"TRAIN_no Train_name Source Destination DEaparture seats fee_A fee_b fee_c\n";
       EXEC SQL OPEN train_cursor;
        //exec sql whenever not found do break;
       for (i=0;i<6;i++)
       {
               exec sql fetch train_cursor into :tra_no,:tr_name,:sour,:destination,:depa,:dur,:seat,:f_a,:f_b,:f_c;
               cout<<tra_no<<"     " <<tr_name<<"    "<<sour<<"  "<<destination<<"        "<<depa<<"     "<<seat<<"    "<<f_a<<"  "<<f_b<<"  "<<f_c<<endl;
       }
        EXEC SQL CLOSE train_cursor;
        cout<<"Enter train Number:";
        cin>>input_train;
        //cout<<"Enter Password For Confirmation:";
        //cin>>input_pass;
        exec sql select password,email_id,contact_no into :check_pass,:check_mail,:check_phone from login where passenger_id=:passenger;
        cout<<"Enter Email ID:";
        cin>>input_mail;
        cout<<"Enter Contact Number:";
        cin>>input_no;
        //cout<<input_pass<<check_pass<<endl;
        //cout<<endl<<endl<<strcmp(check_mail,input_mail)<<"p="<<check_phone<<"p="<<input_no<<check_mail<<input_mail;
        if(strcmp(check_mail,input_mail)==32 && check_phone==input_no)
        {
                cout<<"Choose Class  1.class A \t 2.Class B \t Class C"<<endl;
                cin>>input_class;
                if (input_class==1)
                {
                exec sql select fee_A into :cost from train where train_no=:input_train;
                }
                else if (input_class==2)
                {
                exec sql select fee_B into :cost from train where train_no=:input_train;
                }
else if (input_class==3)
                {
                exec sql select fee_c into :cost from train where train_no=:input_train;
                }
                cout<<"Number of people travelling:"<<endl;
                cin>>total;
                total_cost=total*cost;
                cout<<"Senior citizen(if any) press y else press n:"<<endl;
                cin>>input_option;
                if(input_option=='y')
                {
                        cout<<"Number of People ,Senior Citizen:";
                        cin>>n;
                        original_cost=n*cost;
                        senior_cost=n*(cost-(cost*35/100));
                        senior_cost=original_cost-senior_cost;
                }
                else if(input_option!='y')
                {
                        senior_cost=0;
                }
                cout<<"People Below 18 years(if any),press y else n:";
                cin>>input_option1;
                if(input_option1=='y')
                {
                        cout<<"Number of People ,Below 18 years:";
                        cin>>n1;
                        original_cost=n1*cost;
                        child_cost=n1*(cost-(cost*30/100));
                        child_cost=original_cost-child_cost;
                }
                else if(input_option1!='y')
                {
                        child_cost=0;
                }
                total_cost=total_cost-senior_cost-child_cost;
                cout<<"Total_cost="<<total_cost<<endl;
                cout<<"Enter Account Number";
                cin>>input_acc;
                cout<<"Enter Password:";
                cin>>input_accpass;
                if(strcmp(input_accpass,check_pass)==-32)
                {
                        exec sql select balance into :acc_total from payment where acc_no=:input_acc and password=:input_accpass;
                        cout<<acc_total<<endl;
                        if(total_cost>acc_total)
                        {
                                cout<<"Insufficient Balance"<<endl;
                        }
                        else
                        {
                                cout<<total_cost<<" is debited from your accout"<<endl;
                                acc_total=acc_total-total_cost;
cout<<acc_total<<endl;
                                exec sql update payment set balance=:acc_total where acc_no=:input_acc;
                                exec sql select seats into :up_seats from train where train_no=:input_train;
                                up_seats=up_seats-total;
                                exec sql update train set seats=:up_seats where train_no=:input_train;
                                cout<<"Updated seats="<<up_seats<<endl;
                                exec sql insert into booking (passenger_id,train_no,no_seats,total_cost) values(:passenger,:input_train,:total,:total_cost);
                        }
                }
                else
                {
                cout<<"Acount Password Mismatch"<<endl;
                }
        }
        else
        {
        cout<<"Mail Id or contact_number Mismatch"<<endl;
        }
}
void reservation::cancel()
{
        cout<<"Enter Train Number to cancel the booking "<<endl;
        cin>>input_train;
        exec sql delete from booking where train_no=:input_train;
        up_seats=up_seats+total;
        exec sql update train set seats=:up_seats where train_no=:input_train;
        acc_total=acc_total+total_cost;
        cout<<total_cost<<" has been refunded "<<endl;
        exec sql update payment set balance=:acc_total where acc_no=:input_acc;
        cout<<"Balance in your Account="<<acc_total<<endl;
        cout<<"Updates Seats="<<up_seats<<endl;
        cout<<"Ticket Cancelled Sucesfully"<<endl;
}
void reservation::train_details()
{
        cout<<"TRAIN_no Train_name Source Destination DEaparture seats fee_A fee_b fee_c\n";
        EXEC SQL OPEN train_cursor;
        EXEC SQL WHENEVER NOT FOUND do break;
        for (;;)
        {
                exec sql fetch train_cursor into :tra_no,:tr_name,:sour,:destination,:depa,:dur,:seat,:f_a,:f_b,:f_c;
                cout<<tra_no<<"     " <<tr_name<<"    "<<sour<<"  "<<destination<<"        "<<depa<<"     "<<seat<<"    "<<f_a<<"  "<<f_b<<"  "<<f_c<<endl;
        }
        EXEC SQL CLOSE train_cursor;
}


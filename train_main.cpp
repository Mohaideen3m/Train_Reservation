#include<iostream>
#include"train_reservation.h"
using namespace std;
int main()
{

        reservation res_obj;
        res_obj.admin();
        int input_op,n;
        int option;
        cout<<"Enter Your option 1.Register 2.Login"<<endl;
        cin>>input_op;
        if(input_op==1)
        {
                n=res_obj.register_user();
        }
        else if(input_op==2)
        {
                n=res_obj.login_user();
        }
        if (n!=1)
        {
                cout<<"-----------------Login Sucessfull----------------\n";
                //cout<<"Welcome "<<res_obj.passenger_name<<endl;
                while(option!=4)
                {
                        cout<<"1.Reservation 2.Cancellation 3.Availability 4.Exit"<<endl;
                        cin>>option;
                        switch (option)
                        {
                                case 1:
                                        res_obj.booking_ticket();
                                        break;
                                case 2:
                                        res_obj.cancel();
                                        break;
                                case 3:
                                        res_obj.train_details();
                                        break;
                                case 4:
                                        break;
                                default:
                                        cout<<"Please enter Correct Option."    ;
                        }
                }
        }
        return 0;
}

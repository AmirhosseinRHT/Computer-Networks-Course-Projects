
#include"Reno.hpp"


Reno::Reno(/* args */)
{
}

Reno::~Reno()
{

}

void wait_time(int t){
    std::this_thread::sleep_for(std::chrono::microseconds(t));
}

void Reno::run(Client &client){
    client.handshake();
    int rec_ack[500];
    int pn = 1 , ans_num=-1;
    
    int speed_rev = first_speed;
    while (pn < 500) {

        for(int s =0 ; s< 3; s++){
            std::string message = std::to_string(pn);
            client.sendMessage(message , pn);
            std::thread w(&wait_time , speed_rev);
            auto ans = client.recieveMessage(speed_rev);
            w.join();
            

            if(ans == "TIMEOUT"){
                    printl("speed drop :" + std::to_string(speed_rev));
                    threshold = speed_rev * 2;
                    speed_rev = first_speed * 2;
                    break;
            }
            try{
                ans_num = std::stoi(ans);
            }catch(...){
                //send again a packet that droped
                ans_num = -1;
                int without_ack_num = std::stoi(ans.substr(1 , ans.size()-1));
                std::string message = std::to_string(without_ack_num);
                client.sendMessage(message , without_ack_num);
                auto ans = client.recieveMessage(20000 * 20000);//long time wait
                pn = without_ack_num +1;
                printl("Packet drop :" + std::to_string(speed_rev));
                threshold = speed_rev * 2;
                speed_rev = first_speed * 2;
                break;
            }
            rec_ack[ans_num]++;
            printl("ACK:" + ans + " " +std::to_string(pn));
            pn++;
            if(pn >= 1000)
                break;
            
        }
        if(speed_rev <= threshold){
                speed_rev -=512;
        }else
            speed_rev /=2;
    }
}
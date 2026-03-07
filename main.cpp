#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <random>
#define TRACK_LENGTH 3337
#define TRACKLAP_COUNT 78
using std::cout;
using std::string;
using std::thread;
std::mutex mux;
double G_fastlap=101.691;
string leader;


struct car{
  string team;
  double topspeed;
  double speed;
  int lapcount;
  double laptime;
  double bestlap;
  double mass;
  double fuelload;
  int position;
  bool crashed;

  car(string teamo,double topspeedo,double fuelo){
    lapcount=0;position=0;bestlap=9999;crashed=false;fuelload=fuelo;
    mass=800+fuelload;
    team=teamo;topspeed=topspeedo;
  }
};



int randi(int min, int max) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(min, max);
  return dist(gen);
}
void carrun(car& apx){
  for(int i=0;i<TRACKLAP_COUNT;i++){
    double speed_vary=randi(-50,50);
    apx.speed=apx.topspeed+speed_vary;
    apx.laptime=(TRACK_LENGTH/1000.0)/apx.speed*3.8;
    apx.lapcount++;
    
    if(apx.laptime<apx.bestlap){apx.bestlap=apx.laptime;}
    mux.lock();
    if(apx.bestlap<G_fastlap){G_fastlap=apx.bestlap;leader=apx.team;cout<<"The New Leader is "<<apx.team<<"\n";}
    mux.unlock();
    {std::lock_guard<std::mutex> lock(mux);
    cout<<apx.team<<"\tSpeed: "<<apx.speed<<"\t"<<"Lap Time: "<<apx.laptime<<"\t"<<"Lap Count: "<<apx.lapcount<<"\t\n";
    cout<<apx.team<<" Best Lap time: "<<apx.bestlap<<"\n";}

  }
}

int main(){

  cout<<"********************************\n";
  cout<<"\tWelcome to F1\n";
  cout<<"\t    Monaco\n";
  cout<<"********************************\n\n";
  car Mercedes={"Mercedes",340,50};
  car Ferrari={"Ferrari",340,50};
  car Redbull={"RedBull",340,50};
  thread merc(carrun,std::ref(Mercedes));
  thread ferr(carrun,std::ref(Ferrari));
  thread redb(carrun,std::ref(Redbull));
  merc.join();
  ferr.join();
  redb.join();
  cout<<"\n";
  cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
  cout<<"\tThe Race is Over!!!!\n";
  cout<<"\tThe Winner is "<<leader<<"\n";
  cout<<"\tFastest Lap: "<<G_fastlap<<"\n";
  cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
  


  return 0;
}
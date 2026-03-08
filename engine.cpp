#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>
#include <fstream>
#define TRACK_LENGTH 3337
#define TRACKLAP_COUNT 78
using std::cout;
using std::string;
using std::thread;
std::mutex mux;
double G_fastlap=101.691;
string leader;
std::ofstream lognow("race.csv");
int randi(int min, int max) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(min, max);
  return dist(gen);
}
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
  double tyre_wear;
  int pitlap;
  double tyre_thresh;

  car(string teamo,double topspeedo,double fuelo){
    lapcount=0;position=0;bestlap=9999;crashed=false;fuelload=fuelo;
    mass=800+fuelload;
    team=teamo;topspeed=topspeedo;
    tyre_wear=1.0;pitlap=randi(20,55);
    tyre_thresh=randi(16,20)/10.0;
  }
};




void pitstop(car& apx){
  
  if(apx.tyre_wear>apx.tyre_thresh){
    apx.laptime+=randi(2,5);
    apx.tyre_wear=1.0;
  }
}
void carrun(car& apx){
  for(int i=0;i<TRACKLAP_COUNT;i++){
    double speed_vary=randi(-5,5);
    double fuelburn=apx.fuelload/TRACKLAP_COUNT;
    apx.fuelload-=fuelburn;
    apx.mass=800+apx.fuelload;
    double mass_factor=1.0+(apx.mass-800)/10000.0;
    apx.speed=(apx.topspeed+speed_vary)/mass_factor;
    apx.laptime=(TRACK_LENGTH/1000.0)/apx.speed*3600.0;
    pitstop(apx);
    apx.lapcount++;
    apx.tyre_wear+=0.015;
    apx.laptime*=apx.tyre_wear;
    if(randi(1,5000)==1){apx.crashed=true;break;}
    std::this_thread::sleep_for(
      std::chrono::seconds(static_cast<int>(apx.laptime / 100))
    );
    if(apx.laptime<apx.bestlap){apx.bestlap=apx.laptime;}
    mux.lock();
    if(apx.bestlap<G_fastlap){G_fastlap=apx.bestlap;leader=apx.team;cout<<"The New Leader is "<<apx.team<<"\n";}
    mux.unlock();
    {std::lock_guard<std::mutex> lock(mux);
    cout<<apx.team<<"\tSpeed: "<<apx.speed<<"\t"<<"Lap Time: "<<apx.laptime<<"\t"<<"Lap Count: "<<apx.lapcount<<"\t\n";
    cout<<apx.team<<" Best Lap time: "<<apx.bestlap<<"\n";}

    {
      std::lock_guard<std::mutex> lock(mux);
  
      lognow << apx.lapcount << ","
           << apx.team << ","
           << apx.speed << ","
           << apx.laptime << ","
           << apx.bestlap << ","
           << apx.tyre_wear << "\n";
  }

  }
  if(apx.crashed==true){cout<<apx.team<<" is Crashed :(\n";apx.position=0;}
}

int main(){

  cout<<"********************************\n";
  cout<<"\tWelcome to F1\n";
  cout<<"\t    Monaco\n";
  cout<<"********************************\n\n";
  lognow << "lap,teams,speed,laptime,bestlap,tyre_wear\n";
  car Mercedes={"Mercedes",340,50};
  car Ferrari={"Ferrari",340,50};
  car Redbull={"RedBull",340,50};
  thread merc(carrun,std::ref(Mercedes));
  thread ferr(carrun,std::ref(Ferrari));
  thread redb(carrun,std::ref(Redbull));
  merc.join();
  ferr.join();
  redb.join();

  //std::vector<int> pole={Mercedes.position,Ferrari.position,Redbull.position};
  //std::ranges::sort(pole,std::ranges::greater());
  cout<<"\n";
  cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
  cout<<"\tThe Race is Over!!!!\n\n";
  //cout<<"Grid\n";
  //for(int i=0;i<pole.size();i++)cout<<"Position "<<pole[i];
  cout<<"\tThe Winner is "<<leader<<"\n";
  cout<<"\tFastest Lap: "<<G_fastlap<<"\n";
  cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
  


  return 0;
}
/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

class Person {
private:
    int state=0,n;
public:
    Person(int days_of_infection) : n(days_of_infection) {};
string status_string() {
    string sick = "sick";
    string susc = "susceptible";
    string recov = "recovered";
    string vacc = "vaccinated";
    char days_to_go = '0' + state;
    string status;
    if (state==0)
        status = susc;
    if (state!=0){
        if(state==-1)
            status = recov;
        else if(state==-2)
            status = vacc;
        else
            status = sick + " (" + days_to_go + " to go)";
    }
    return status;
}

void update() {
    if (state!=0 && state!=-1 && state!=-2){
        if (state==1)
            state=-1;
        else
        state = state-1;
    }
}
    
void infect(int n) {
    if (state==0)
        state = n;
}

bool is_stable() {
    if (state==-1)
        return true;
    else 
        return false;
}
bool no_infection() {
    if (state==0||state==-1||state==-2)
        return true;
    else 
         return false;
}

void vaccinate() {
    state=-2;
}

bool is_vaccinated() {
    if (state==-2)
        return true;
    else
        return false;
}

bool just_infected() {
    if (state==5)
        return true;
    else
        return false;
}

};



class Population {

private:
    int n,counter;
    vector<Person> myPopulation;
public:
    Population(int npeople) : n(npeople) {};

void create() {
    for (int i=0; i < n; i++) {
        Person newPerson(0);
        myPopulation.push_back(newPerson);
    }
}

void random_infection() {
    int infected_person = (int) rand() % n;
    while (myPopulation.at(infected_person).is_vaccinated()) {
        infected_person = (int) rand() % n;
    }
    myPopulation.at(infected_person).infect(5);
}

int count_infected() {
    counter=0;
    for (int j=0; j<n; j++){
        if (!myPopulation.at(j).no_infection())
            counter=counter+1;
    }
    return counter;
}

void set_probability_of_transfer(float p){
    float bad_luck_1 = (float) rand()/(float)RAND_MAX;
    float bad_luck_2 = (float) rand()/(float)RAND_MAX;
    for (int i=0;i<n;i++){
        if (!myPopulation.at(i).no_infection()){
            if (i!=0 && i!=myPopulation.size()-1){
                if (bad_luck_1<=p){
                   myPopulation.at(i-1).infect(5);}
                if (bad_luck_2<=p){
                   myPopulation.at(i+1).infect(5);}
            }
            if (i=0){
                if (bad_luck_1<=p){
                    myPopulation.at(i+1).infect(5);}
            }
            if (i=myPopulation.size()-1){
                if (bad_luck_1<=p){
                    myPopulation.at(i-1).infect(5);}
            }
        }           
    }
}

string display() {
    string popdisplay;
    string sick="+";
    string recovered="-";
    string susceptible="?";
    string vaccinated="@";
    for (int m=0;m<n;m++){
        if (!myPopulation.at(m).no_infection())
            popdisplay=popdisplay+sick;
        else if (myPopulation.at(m).is_stable())
            popdisplay=popdisplay+recovered;
        else if (myPopulation.at(m).is_vaccinated())
            popdisplay=popdisplay+vaccinated;
        else popdisplay=popdisplay+susceptible;
    }
    return popdisplay;
}

void updatePop() {
    for (int k=0;k<n;k++){
        myPopulation.at(k).update();
    }
}

void vaccinations(int percent_vaccinated) {
    int number_vaccinated = percent_vaccinated*0.01*n;
    vector<int> random_positions;
    random_positions.resize(number_vaccinated);
    for (int i=0;i<number_vaccinated;i++){
       random_positions.at(i)=rand() % n;
        for (int j=0;j<number_vaccinated;j++){
            if(random_positions.at(i)==random_positions.at(j) && i!=j)
            {
                i--;
                break;
           }
        }
    }
    for (int k=0;k<random_positions.size();k++){
        myPopulation.at(random_positions.at(k)).vaccinate();
    }
}

void SIR_model(int contacts, float p){
    vector<int> random_contacts;
    for (int k=0;k<n;k++){
        if(!myPopulation.at(k).no_infection() && !myPopulation.at(k).just_infected()){
            float bad_luck_3 = (float) rand()/(float)RAND_MAX;
            random_contacts.resize(contacts);
            for (int i=0;i<contacts;i++){
                random_contacts.at(i)=rand() % n;
                for (int j=0;j<contacts;j++){
                    if(random_contacts.at(i)==random_contacts.at(j) && i!=j)
                    {
                    i--;
                    break;
                    }
                }
            }
            for (int m=0; m<random_contacts.size();m++){
                if (bad_luck_3<=p){
                    myPopulation.at(random_contacts.at(m)).infect(5);
                }
            }
        }
    }
}


};

int main() {
srand(time(NULL));

int step = 1,npeople,percent_vaccinated,number_of_interactions;
float probability_of_transmission;

cout<<"Size of population? "; 
cin>>npeople;
cout<<"Probability of transmission? ";
cin>>probability_of_transmission;
cout<<"Percent vaccinated? ";
cin>>percent_vaccinated;
cout<<"Number of unique daily interactions per individual?";
cin>>number_of_interactions;

Population population(npeople);
population.create();
population.vaccinations(percent_vaccinated);
population.random_infection();

for ( ; ; step++) {
  cout << "In step " << step << " #sick: " << population.count_infected() << " : " << population.display() << endl;
  if (population.count_infected()==0){
       cout<<"Disease has ran its course by step " << step << endl;  
       break;
  }
  population.updatePop();
  population.SIR_model(number_of_interactions,probability_of_transmission);
  }
  
}
     
    
    
    
    



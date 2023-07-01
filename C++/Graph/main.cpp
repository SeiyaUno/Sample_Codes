// main.cpp
//
// ICS 46 Winter 2022
// Project #5: Rock and Roll Stops the Traffic
//
// This is the program's main() function, which is the entry point for your
// console user interface.
//#include "Digraph.hpp"
#include "RoadMapReader.hpp"
#include "TripReader.hpp"
#include "RoadMapWriter.hpp"

#include <iomanip>
#include <cmath>

void timeOutput(double, double);
void totalSec(double&, double, double);
void timeOutput(double);

int main()
{
    InputReader input(std::cin);
    RoadMapReader rmReader;
    TripReader tripR;
    RoadMap mp = rmReader.readRoadMap(input);   //mp is basically a Digraph

    //check if the map is strongly connected.
    if(!mp.isStronglyConnected()) {
        std::cout << "Disconnected Map" << std::endl;
        return 0;
    }

    std::vector<Trip> tripInMap = tripR.readTrips(input);

    //Process each trip
    for(int i = 0; i < tripInMap.size(); i++) {
        
        //stack for keeping the order of path
        std::stack<int> order;
        int current = tripInMap.at(i).endVertex;

        double total = 0;

        if(tripInMap.at(i).metric == TripMetric::Distance) {
            //find shortest distance
            std::map<int, int> pathMap = mp.findShortestPaths(tripInMap.at(i).startVertex, 
            [](RoadSegment rs)
            {
                return rs.miles;
            });

            while(current != tripInMap.at(i).startVertex) {
                order.push(current);
                current = pathMap.at(current);
            }

            std::cout << "Shortest distance from " << mp.vertexInfo(tripInMap.at(i).startVertex)
                    << " to " << mp.vertexInfo(tripInMap.at(i).endVertex) << "\n"; 
            
            std::cout << "  Begin at " << mp.vertexInfo(tripInMap.at(i).startVertex) << "\n";

            while(!order.empty()) {
                current = order.top();
                std::cout << "  Continue to " << mp.vertexInfo(current);
                std::cout << " (" << std::fixed << std::setprecision(1) << mp.edgeInfo(pathMap.at(current), current).miles
                     << " miles)\n";

                total += mp.edgeInfo(pathMap.at(current), current).miles;
                order.pop();
            }

            std::cout << "Total distance: " << total << " miles\n";
        }
        else {
            //find shortest time
            std::map<int, int> pathMap = mp.findShortestPaths(tripInMap.at(i).startVertex, 
            [](RoadSegment rs)
            {
                return rs.miles / rs.milesPerHour;
            });

            while(current != tripInMap.at(i).startVertex) {
                order.push(current);
                current = pathMap.at(current);
            }

            std::cout << "Shortest driving time from " << mp.vertexInfo(tripInMap.at(i).startVertex)
                    << " to " << mp.vertexInfo(tripInMap.at(i).endVertex) << "\n"; 
            std::cout << "  Begin at " << mp.vertexInfo(tripInMap.at(i).startVertex) << "\n";

            while(!order.empty()) {
                current = order.top();

                std::cout << "  Continue to " << mp.vertexInfo(current);
                std::cout << " (" << std::fixed << std::setprecision(1) 
                    << mp.edgeInfo(pathMap.at(current), current).miles << " miles @ ";
                std::cout << std::fixed << std::setprecision(1)
                    << mp.edgeInfo(pathMap.at(current), current).milesPerHour << "mph = ";
                timeOutput(mp.edgeInfo(pathMap.at(current), current).miles, mp.edgeInfo(pathMap.at(current), current).milesPerHour);
                std::cout << ")\n";

                totalSec(total, mp.edgeInfo(pathMap.at(current), current).miles, mp.edgeInfo(pathMap.at(current), current).milesPerHour);
                order.pop();
            }

            timeOutput(total);
        }

        std::cout << std::endl;
    }

    return 0;
}


//This function is for distance or time on an edge
void timeOutput(double miles, double mph) {

    int hours = 0, minutes = 0;
    double seconds = 0;

    totalSec(seconds, miles, mph);

    hours = (int)std::floor(seconds / 3600);

    if(hours > 0) {
        seconds = seconds - hours * 3600;
        minutes = (int)std::floor(seconds / 60);
        seconds = seconds - minutes * 60;

        std::cout << hours << " hrs " << minutes << " mins " 
            << std::fixed << std::setprecision(1) << seconds << " secs";        
    }
    else {
        //if hours is 0
        minutes = (int)std::floor(seconds / 60);
        seconds = seconds - minutes * 60;

        if(minutes > 0) {
            std::cout << minutes << " mins " 
                << std::fixed << std::setprecision(1) << seconds << " secs";
        }
        else {
            //if minutes is 0
            std::cout << minutes << std::fixed << std::setprecision(1) << seconds << " secs";
        }
    }
}

void totalSec(double& total, double miles, double mph) {
    //convert to mps and find how many seconds it takes
    mph = mph / 3600;
    total += miles / mph;
}

//This function is for total distance or time
void timeOutput(double seconds) {

    int hours = 0, minutes = 0;

    hours = (int)std::floor(seconds / 3600);

    std::cout << "Total time: ";    
    if(hours > 0) {
        seconds = seconds - hours * 3600;
        minutes = (int)std::floor(seconds / 60);
        seconds = seconds - minutes * 60;

        std::cout << hours << " hrs " << minutes << " mins " 
            << std::fixed << std::setprecision(1) << seconds << " secs\n";        
    }
    else {
        //if hours is 0
        minutes = (int)std::floor(seconds / 60);
        seconds = seconds - minutes * 60;

        if(minutes > 0) {
            std::cout << minutes << " mins " 
                << std::fixed << std::setprecision(1) << seconds << " secs\n";
        }
        else {
            //if minutes is 0
            std::cout << minutes << std::fixed << std::setprecision(1) << seconds << " secs\n";
        }
    }
}
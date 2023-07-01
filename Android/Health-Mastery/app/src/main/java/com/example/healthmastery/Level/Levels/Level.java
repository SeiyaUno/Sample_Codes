package com.example.healthmastery.Level.Levels;

public class Level {

    //private members
    int level;//the integer and numeric representation of level 1-5
    String name;//what is the name of the level
    double caloricBurn;//calorie percentage to be burned(based on weight)



    public Level(int level, String name, double caloricBurn)
    {
        this.level = level;
        this.name = name;
        this.caloricBurn = caloricBurn;
    }
}


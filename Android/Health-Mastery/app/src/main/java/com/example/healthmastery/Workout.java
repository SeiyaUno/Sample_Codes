package com.example.healthmastery;

public class Workout {

    //PRIVATE MEMBERS
    String name;//name of workout
    int level;//correlates to difficulty and tier system 1-5
    int calories;
    boolean flexibility;//correlates to if the workout can be done anywhere
    String description;//the desc or instruction of workout

    public Workout(String name, int level, int calories, boolean flexibility, String description)
    {
        this.name = name;
        this.level = level;
        this.calories = calories;
        this.flexibility = flexibility;
        this.description = description;

    }

    public String getName() {return name;}
    public int getCalories() {return calories;}

    public int getLevel() {
        return level;
    }

    public void setLevel(int level) {
        this.level = level;
    }

    public boolean getFlexibility() {
        return flexibility;
    }

    public void setFlexibility(boolean flexibility) {
        this.flexibility = flexibility;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }
}

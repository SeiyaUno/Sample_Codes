package com.example.healthmastery;

public class User {
    String username; //identifier for user in system
    String password; //password for login

    int startingLevel;//the level the started at or were recommended
    int level; //the current level they are at

    int age; //current age
    char gender; //Assigned at Birth (M or F)

    int height; //height by centimeters no double
    double startingWeight; //weight at sign up
    double currentWeight; //weight currently
    double BMI; //currentWeight/ Height^2 (CONVERT TO INCHES)

    //USED FOR PLACEMENT
    int estimatedCaloricIntake; //Daily
    int estimatedCaloricOutake; //Daily

    double[] WeightHistory;//each week update the weights [0] == week 0 [10] == week 10

}

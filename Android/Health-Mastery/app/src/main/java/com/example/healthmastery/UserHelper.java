package com.example.healthmastery;

public class UserHelper {

    String userName, name, email, age, weight, height, caloriesIntake, caloriesBurned, gender;
    String levels;

    public UserHelper()
    {

    }


    public UserHelper(String userName, String name, String email, String age, String weight, String height, String caloriesIntake, String caloriesburned, String gender, String levels)
    {
        this.userName = userName;
        this.name = name;
        this.email = email;
        this.age = age;
        this.weight = weight;
        this.height = height;
        this.caloriesIntake = caloriesIntake;
        this.caloriesBurned = caloriesburned;
        this.gender = gender;
        this.levels = levels;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public String getLevels() {
        return levels;
    }

    public void setLevels(String levels) {
        this.levels = levels;
    }

    public String getGender() {
        return gender;
    }

    public void setGender(String gender) {
        this.gender = gender;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getAge() {
        return age;
    }

    public void setAge(String age) {
        this.age = age;
    }

    public String getWeight() {
        return weight;
    }

    public void setWeight(String weight) {
        this.weight = weight;
    }

    public String getHeight() {
        return height;
    }

    public void setHeight(String height) {
        this.height = height;
    }

    public String getCaloriesIntake() {
        return caloriesIntake;
    }

    public void setCaloriesIntake(String caloriesIntake) {
        this.caloriesIntake = caloriesIntake;
    }

    public String getCaloriesBurned() {
        return caloriesBurned;
    }

    public void setCaloriesBurned(String caloriesBurned) {
        this.caloriesBurned = caloriesBurned;
    }
}

package com.example.healthmastery;

public class UserInformation {

    private String userRealName;
    private String userName;
    private String userLevel;
    private String weight;



    public UserInformation(String userRealName, String userName, String userLevel, String userCalories, String weight)
    {

        this.userRealName = userRealName;
        this.userName = userName;
        this.userLevel = userLevel;
        this.userCalories = userCalories;
        this.weight = weight;

    }

    public String getWeight() {
        return weight;
    }

    public void setWeight(String weight) {
        this.weight = weight;
    }

    public String getUserCalories() {
        return userCalories;
    }

    public void setUserCalories(String userCalories) {
        this.userCalories = userCalories;
    }

    public String getUserRealName() {
        return userRealName;
    }

    public void setUserRealName(String userRealName) {
        this.userRealName = userRealName;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public String getUserLevel() {
        return userLevel;
    }

    public void setUserLevel(String userLevel) {
        this.userLevel = userLevel;
    }

    private String userCalories;





}

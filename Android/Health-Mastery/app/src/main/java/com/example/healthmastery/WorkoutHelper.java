package com.example.healthmastery;

public class WorkoutHelper {

    private boolean flexibility;
    private String instructions;
    private int level;
    private String name;


    public WorkoutHelper() {
    }

    public WorkoutHelper(boolean flexibility, String instructions, int level, String name) {
        this.flexibility = flexibility;
        this.instructions = instructions;
        this.level = level;
        this.name = name;
    }

    public boolean isFlexibility() {
        return flexibility;
    }

    public String getInstructions() {
        return instructions;
    }

    public int getLevel() {
        return level;
    }

    public String getName() {
        return name;
    }
}
